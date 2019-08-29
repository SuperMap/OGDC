#pragma once
#include"UGMaxModelProcess.h"
#include "Stream/ugdefs.h"

class TriObject;
class Mtl;
class CtranslateFileName;
//class UGMaxModelProcess;
namespace UGC{
	struct meshIndex
	{
		UGint meshNum;
		std::list<UGString> meshIndexs;
	};
	struct NodeMultiTex
	{
		INode* pNode;
		//该NODE使用了该value对应的材质，这是材质在pNode中所在的索引位置，-1表示pNode使用的是非多层材质
		UGint nIndexMtl;
		//pNode所包含的三角面片数据，deleteTriObj说明triObj是否需要删除
		TriObject* pTriObj;
		UGbool bDeleteTriObj;
	};
	class MtlToNode:public ITreeEnumProc
	{
		friend class UGSGMExporter;
		friend class UGBIMExporter;
	public:

		MtlToNode(UGint timeStart)
		{
			m_nMeshNum = 0;
			m_nTimeStart = timeStart;
		}
		~MtlToNode();
		virtual int callback( INode *pNode );
		std::map<Mtl*,std::list<NodeMultiTex*>>& GetMtlToNode()
		{
			return mtlToNode;
		}
		int ProcessMtl(Mtl* mtl, INode *pNode, TriObject *pObj, UGbool bDeleteTriObj = FALSE);
	private:
		std::map<Mtl*,std::list<NodeMultiTex*>> mtlToNode;
		UGint m_nMeshNum;
		std::list<INode*> m_NoFatherNodes;
		std::list<INode*> m_animationList;
		/*UGint m_nAnimationNum;*/
		//写动画列表，并消除场景中名称相同的问题。
		std::map<INode*,UGString> m_nodeToNameMap;

		CtranslateFileName m_translateFileName;
		UGint m_nTimeStart;
	};
}
