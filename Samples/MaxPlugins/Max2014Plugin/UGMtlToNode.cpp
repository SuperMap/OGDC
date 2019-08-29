#include "max.h"
#include "UGMtlToNode.h"

using namespace UGC;
MtlToNode::~MtlToNode()
{
	std::map<Mtl*,std::list<NodeMultiTex*>>::const_iterator map_it = mtlToNode.begin();
	std::set<INode*>deletedNodes;
	while(map_it != mtlToNode.end())
	{
		std::list<NodeMultiTex*> deleteList = map_it->second;
		std::list<NodeMultiTex*>::const_iterator deleteList_it = deleteList.begin();
		while(deleteList_it != deleteList.end())
		{	
			INode *pNode = (*deleteList_it)->pNode;
			if(deletedNodes.count(pNode) == 0)
			{
				if((*deleteList_it)->bDeleteTriObj)
				{
					(*deleteList_it)->pTriObj->DeleteMe();
				}
				deletedNodes.insert(pNode);
			}
			delete *deleteList_it;
			deleteList_it++;
		}
		map_it++;
	}
}

int MtlToNode::callback( INode *pNode )
{
	if(pNode == NULL)
	{
		return TREE_CONTINUE;
	}

	if(pNode->GetName()==NULL)
	{
		return TREE_CONTINUE;
	}	

	Object *pOriginObj = pNode->EvalWorldState(m_nTimeStart).obj;
	if (pOriginObj == NULL)
	{
		return TREE_CONTINUE;
	}
	if(!pOriginObj->CanConvertToType(triObjectClassID))
	{
		return TREE_CONTINUE;
	}

	TriObject *pObj = (TriObject *)pOriginObj->ConvertToType(m_nTimeStart, triObjectClassID);
	if (pObj == NULL)
	{
		return TREE_CONTINUE;
	}

	Mesh mesh = pObj->GetMesh();
	if(mesh.numVerts <= 0)
	{
		if(pOriginObj != pObj)
		{
			pObj->DeleteMe();
		}
		return TREE_CONTINUE;
	}

	UGString strNewName = m_translateFileName.GetName(pNode->GetName());
	m_nodeToNameMap[pNode] = strNewName;

	if(pNode->GetParentNode()->IsRootNode())
	{
		m_NoFatherNodes.push_back(pNode);
	}

	//生成INODE与唯一名字对应的关系，并生成动画列表，以及动画个数		
	Control* pCpos = pNode->GetTMController()->GetPositionController();
	Control* pCrot = pNode->GetTMController()->GetRotationController();
	Control* pCscl = pNode->GetTMController()->GetScaleController();
	if(pCpos->IsAnimated() || pCrot->IsAnimated()|| pCscl->IsAnimated())
	{
		m_animationList.push_back(pNode);
	}		

	UGbool bDeleteTriObj = FALSE;
	if(pOriginObj != pObj)
	{
		bDeleteTriObj = TRUE;
	}
	Mtl* mtl = pNode->GetMtl();
	return ProcessMtl(mtl, pNode, pObj, bDeleteTriObj);
}

int MtlToNode::ProcessMtl(Mtl* mtl, INode *pNode, TriObject *pObj, UGbool bDeleteTriObj)
{
	if (mtl == NULL)
	{
		NodeMultiTex* pNodeTex = new NodeMultiTex;
		pNodeTex->pNode = pNode;
		pNodeTex->nIndexMtl = -1;
		pNodeTex->pTriObj = pObj;
		pNodeTex->bDeleteTriObj = bDeleteTriObj;
		m_nMeshNum++;
		mtlToNode[mtl].push_back(pNodeTex);
		return TREE_CONTINUE;
	}
	if (mtl->ClassID() == Class_ID(MULTI_CLASS_ID,0))
	{
		for (UGint i = 0; i < mtl->NumSubMtls();i++)
		{
			Mtl* pSub = mtl->GetSubMtl(i);
			NodeMultiTex* pNodeTex = new NodeMultiTex;
			pNodeTex->pNode = pNode;
			pNodeTex->nIndexMtl = i;
			pNodeTex->pTriObj = pObj;
			pNodeTex->bDeleteTriObj = bDeleteTriObj;
			m_nMeshNum++;
			mtlToNode[pSub].push_back(pNodeTex);
		}
	}
	else if(mtl->ClassID() == Class_ID(BAKE_SHELL_CLASS_ID,0))
	{
		Mtl* pShaked = mtl->GetSubMtl(1);
		Mtl* pOrigin = mtl->GetSubMtl(0);
		
		if(pShaked != NULL)
		{
			ProcessMtl(mtl->GetSubMtl(1), pNode, pObj, bDeleteTriObj);
		}
		else if(pOrigin != NULL)
		{
			ProcessMtl(mtl->GetSubMtl(0), pNode, pObj, bDeleteTriObj);
		}		
	}
	else
	{
		NodeMultiTex* pNodeTex = new NodeMultiTex;
		pNodeTex->pNode = pNode;
		pNodeTex->nIndexMtl = -1;
		pNodeTex->pTriObj = pObj;
		pNodeTex->bDeleteTriObj = bDeleteTriObj;
		m_nMeshNum++;
		mtlToNode[mtl].push_back(pNodeTex);
	}
	return TREE_CONTINUE;	
}