#include "UGBIMExporter.h"

#include "Base3D/UGMesh.h"
#include "Base3D/UGMaterial.h"
#include "Base3D/UGMatrix4d.h"
#include "Base3D/UGAnimationCurve.h"

#include <set>
#include "decomp.h"
#include "contextids.h "
#include"ISTDPLUG.H"
#include <fstream>
#include<iostream>

#include "MaxPluginres.h"

#include "UGMaxToolkit.h"
#include "UGMaxLogFile.h"
#include "Base3D/UGMathEngine.h"
#include "Base3D/UGSimplification.h"
#include "FileParser/UGFileParseManager.h"
#include "FileParser/UGExchangeParamsManager.h"
#include "FileParser/UGFileParseToolkit.h"
#include "Toolkit/UGImageOperator.h"
#include"UGSingleNodeSceneExporter.h"
#include "FileParser/UGFileParseModel.h"
#include "Base3D/UGModelNodeEntities.h"
#include "Geometry3D/UGGeoModelPro.h"
#include "Base3D/UGModelNodeTools.h"

#include "OGDC/OgdcDataSource.h"
#include "OGDC/OgdcProviderManager.h"
#define TIMEPERMAXTICK 0.00020833333333333
namespace UGC
{
	/*-----------------------------------------------------------------------------------------------------------------------
	此类在遍历场景中每个节点所需要进行的操作，重要的方法是callback，ITreeEnumProc是Max中的接口，这里获取后面导出操作所必须的数据:
	1.得出以(mtl*,std::list<NodeMultiTex*>)，因为UGMESH是由mtl来组织的，后面是对应一系列的max中node，后面操作就是把所有的NODE中的顶点数据组织起来
	2.获取动画节点
	-----------------------------------------------------------------------------------------------------------------------*/
	//struct vectorLess
	//{
	//	bool operator()(const UGVector4d v1, const UGVector4d v2) const
	//	{
	//		if (v1.x < v2.x) return true;
	//		if (v1.x == v2.x && v1.y < v2.y) return true;
	//		if (v1.x == v2.x && v1.y == v2.y && v1.z < v2.z) return true;

	//		return false;
	//	}
	//}
	int	UGBIMExporter::ExtCount()
	{
		return 1;
	}
	const TCHAR * UGBIMExporter::Ext(int n)
	{
		if(0==n)
			return _U("udb");
		return _U("");
	}
	const TCHAR * UGBIMExporter::LongDesc()
	{
		return _U("SuperMap 3D Studio File");
	}
	const TCHAR * UGBIMExporter::ShortDesc()
	{
		return _U("SuperMap 3D Studio File");
	}
	const TCHAR * UGBIMExporter::AuthorName()
	{
		return _U("SuperMap");
	}
	const TCHAR * UGBIMExporter::CopyrightMessage()
	{
		return _U("");
	}
	const TCHAR * UGBIMExporter::OtherMessage1()
	{
		return _U("");
	}
	const TCHAR * UGBIMExporter::OtherMessage2()
	{
		return _U("");
	}
	unsigned int UGBIMExporter::Version()
	{
		return 200;
	}
	void UGBIMExporter::ShowAbout(HWND hWnd)
	{

	}
	UGBIMExporter::~UGBIMExporter()
	{
		m_LodInfo.m_arrModelLODInfo.RemoveAll();
	}

	UGBIMExporter::UGBIMExporter()
	{
		m_bExportAnimation = FALSE;
		m_nCurrentMeshID = 0;
		timeStart = 0;
		m_bShowPrompts = FALSE;
		m_mtlIDNow = 0;
		m_bIsExportVertexNormal =FALSE;
		m_bIsExportMaterialColor = TRUE;
		m_bIsExportNormal = TRUE;
	}
	BOOL UGBIMExporter::SupportsOptions(int ext, DWORD options)
	{
		assert(ext == 0);	// We only support one extension
		return(options == SCENE_EXPORT_SELECTED) ? TRUE : FALSE;
	}

	void UGBIMExporter::BeforeExport(Interface *i)
	{
		m_nCurrentMeshID = 0;
		m_NodeIndexMap.clear();
		timeStart = i->GetAnimRange().Start();
	}
	int	 UGBIMExporter::DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
	{
		if (UGMaxToolkit::m_setRegionZ.count(UGString(name)) ==1)
		{
			//如果正在导出的是面数据
			m_bIsExportVertexNormal =TRUE;
		}
		else
		{
			m_bIsExportVertexNormal =FALSE;
		}
		m_strDestTexPath = name;
		m_strDestTexPath = UGFile::GetDir(m_strDestTexPath) + _U("Textures/");
		m_bExportAnimation=FALSE;

		BeforeExport(i);
		if (!UGFile::GetExt(name).CompareNoCase(_U(".S3MB")))
		{
			return ExportS3MB(name, ei, i);
		}
		else if(!UGFile::GetExt(name).CompareNoCase(_U(".UDB")))
		{
			return ExportUDB(name, ei, i);
		}
		return IMPEXP_FAIL;
	}

	int UGBIMExporter::ExportS3MB(const TCHAR *name, ExpInterface *ei, Interface *i)
	{
		m_Geode = new UGModelGeode();
		ReadModelData(ei, i, m_Geode);
		return IMPEXP_SUCCESS;
	}

	int UGBIMExporter::ExportUDB(const TCHAR *name, ExpInterface *ei, Interface *i)
	{
		m_Geode = new UGModelGeode();
		ReadModelData(ei, i, m_Geode);
		ProcessRefernceNode(m_Geode);
		return IMPEXP_SUCCESS; 
	}

	UGbool UGBIMExporter::ReadModelData(ExpInterface *ei, Interface *inf, UGC::UGModelGeode* pGeode)
	{
		if(pGeode == NULL)
		{
			return FALSE;
		}

		//读取node信息
		MtlToNode mtlToNode(timeStart);
		//矩阵归一化
		Matrix3 OldMatrix=m_LodInfo.m_arrModelLODInfo[0].node->GetNodeTM(0);
		Matrix3 matrix = OldMatrix;
		matrix.IdentityMatrix();
		m_LodInfo.m_arrModelLODInfo[0].node->SetNodeTM(0, matrix);

		pGeode->m_strName = m_LodInfo.m_arrModelLODInfo[0].node->GetName();//获取模型名字

		((SingleNodeExpInterface*)ei)->ResetIScene(m_LodInfo.m_arrModelLODInfo[0].node);
		ei->theScene->EnumTree(&mtlToNode);
		std::map<Mtl*,std::list<NodeMultiTex*>> mtlToNodeMap=mtlToNode.GetMtlToNode();
		if(mtlToNodeMap.empty())return IMPEXP_SUCCESS;

		int mtlID=0;
		m_mtlIDNow = 0;
		m_mtlIDToChannels.clear();
		std::map<Mtl*,std::list<NodeMultiTex*>>::const_iterator map_it=mtlToNodeMap.begin();
		vector<UGint> isDDS;//存储需要翻转V的通道序号
		for(map_it=mtlToNodeMap.begin();map_it!=mtlToNodeMap.end();map_it++)
		{
			if(!map_it->first)
			{
				std::list<NodeMultiTex*>::const_iterator list_it=(map_it->second).begin();
				for(;list_it!=map_it->second.end();list_it++)
				{
					MeshParamInfos meshInfo = WriteMesh(*list_it,-1,mtlToNode);
					UGModelSkeleton*  pSkeleton= BuildSkeleton(meshInfo,isDDS);

					if (pSkeleton)
					{
						//添加默认材质
						UGModelMaterial* pMaterial= BuildMaterial(NULL,meshInfo.bTwoSided,isDDS);
						pSkeleton->SetMaterialName(pMaterial->m_strName);
						pGeode->AddEntity(pSkeleton);
						pGeode->AddEntity(pMaterial);
					}
				}
				pGeode->ComputeBoundingBox();
			}
			else
			{
				std::list<NodeMultiTex*>::const_iterator list_it=(map_it->second).begin();
				UGbool bUsingMtl = FALSE;
				UGbool bIsTwoSide = FALSE;
				UGModelMaterial* pMaterial;
				UGbool bIsFirst=TRUE;
				for(;list_it!=map_it->second.end();list_it++)
				{
					MeshParamInfos meshInfo = WriteMesh(*list_it,mtlID,mtlToNode,map_it->first);
					bUsingMtl = TRUE;
					bIsTwoSide = meshInfo.bTwoSided;
					if(bIsFirst)
					{
						pMaterial = BuildMaterial(map_it->first, bIsTwoSide,isDDS);
					    pGeode->AddEntity(pMaterial);
						bIsFirst=FALSE;
					}
					UGModelSkeleton*  pSkeleton= BuildSkeleton(meshInfo,isDDS);
					pSkeleton->SetMaterialName(pMaterial->m_strName);
					pGeode->AddEntity(pSkeleton);
				}
				if(bUsingMtl)
				{
					mtlID++;
				}
				pGeode->ComputeBoundingBox();
			}
			isDDS.clear();
		}

		//设置矩阵
		m_LodInfo.m_arrModelLODInfo[0].node->SetNodeTM(0, OldMatrix);
		MRow* m = OldMatrix.GetAddr();
		UGMatrix4d mat(m[0][0], m[0][1], m[0][2], 0,
			m[1][0], m[1][1], m[1][2], 0,
			m[2][0], m[2][1], m[2][2], 0,
			m[3][0], m[3][1], m[3][2], 1);	

		pGeode->SetPosition(mat);
		m_ModelGroup.push_back(pGeode);
		m_ModelInstanceGroup.push_back(pGeode);
		return TRUE;
	}


	void UGBIMExporter::ProcessRefernceNode(UGModelGeode* pGeode)
	{
		for (int i = 1; i < m_vecNode.size(); i++)
		{
			//处理精细层
			INode* pNode = m_vecNode[i];
			Matrix3 transM = pNode->GetNodeTM(0);
			MRow* m = transM.GetAddr();
			UGMatrix4d mat(m[0][0], m[0][1], m[0][2], 0,
				m[1][0], m[1][1], m[1][2], 0,
				m[2][0], m[2][1], m[2][2], 0,
				m[3][0], m[3][1], m[3][2], 1);	

			if(pGeode != NULL)
			{
				UGModelGeode* pRefernceGeode = new UGModelGeode();
				pRefernceGeode->m_strName = pNode->GetName();
				pRefernceGeode->SetPosition(mat);
				pRefernceGeode->ComputeBoundingBox();

				for (UGint nSkeleton = 0; nSkeleton < pGeode->GetEntityNames(UGModelEntity::METype::etSkeleton).size(); nSkeleton++)
				{
					//指向同一引用
					pRefernceGeode->AddSkeleton(pGeode->GetSkeleton(pGeode->GetEntityNames(UGModelEntity::METype::etSkeleton)[nSkeleton]));
				}
				for (UGint nTexture = 0; nTexture < pGeode->GetEntityNames(UGModelEntity::METype::etTexture).size(); nTexture++)
				{
					//指向同一引用
					pRefernceGeode->AddTexture(pGeode->GetTexture(pGeode->GetEntityNames(UGModelEntity::METype::etTexture)[nTexture]));
				}
				for (UGint nMaterial = 0; nMaterial < pGeode->GetEntityNames(UGModelEntity::METype::etMaterial).size(); nMaterial++)
				{
					//指向同一引用
					pRefernceGeode->AddMaterial(pGeode->GetMaterial(pGeode->GetEntityNames(UGModelEntity::METype::etMaterial)[nMaterial]));
				}
				m_ModelGroup.push_back(pRefernceGeode);
			}
		}
	}

	void UGBIMExporter::WriteNodes(UGNode& rootNode ,MtlToNode& mtlToNode)
	{
		std::list<INode*> rootNodes = mtlToNode.m_NoFatherNodes;
		std::list<INode*>::const_iterator rootNodeIter = rootNodes.begin();

		rootNode.SetName(_U("Scene Root"));
		UGVector3d vecScale(1.0,1.0,1.0);
		rootNode.SetScale(vecScale);
		UGQuaternion4d vecOrientation(0.0,0.0,0.0,0.0);
		rootNode.SetOrientation(vecOrientation);
		UGVector3d vecPostion(0.0,0.0,0.0);
		rootNode.SetPosition(vecPostion);
		rootNode.SetMeshName(_U("Scene Mesh"));
		while(rootNodeIter!=rootNodes.end())
		{
			UGNode* pMapNode = new UGNode;
			rootNode.AddChild(pMapNode);
			BuildNodeTree(pMapNode,*rootNodeIter,mtlToNode.m_nodeToNameMap);
			rootNodeIter++;
		}
		// 		
		// 	}

	}
	void UGBIMExporter::BuildNodeTree(UGNode* pMapNode,INode* pNode,std::map<INode*,UGString>& nodeToString)
	{
		std::list<INode*> needBuildSonNode;
		std::list<UGNode*> needBuildSonMapNode;
		needBuildSonNode.push_back(pNode);
		needBuildSonMapNode.push_back(pMapNode);
		Interval valid = FOREVER;
		while(!needBuildSonNode.empty())
		{
			INode* pInNode = needBuildSonNode.front();
			needBuildSonNode.pop_front();
			UGNode* pInMapNode =needBuildSonMapNode.front();
			needBuildSonMapNode.pop_front();

			//填充非树信息数据
			UGString strNodeName = nodeToString[pInNode];
			pInMapNode->SetName(strNodeName);
			Control* cpos = pInNode->GetTMController()->GetPositionController();
			Point3 pkey;
			cpos->GetValue(timeStart, &pkey, valid);
			UGVector3d vecPostion;	
			vecPostion.x = pkey.x;
			vecPostion.y = pkey.y;
			vecPostion.z = pkey.z;
			pInMapNode->SetPosition(vecPostion);

			Control* crot = pInNode->GetTMController()->GetRotationController();
			UGQuaternion4d vecOrientation;
			Quat rkey;
			crot->GetValue(timeStart, &rkey, valid);
			/*	Quat rkey=pInNode->GetObjOffsetRot();*/
			vecOrientation.x = rkey.x;
			vecOrientation.y = rkey.y;
			vecOrientation.z = rkey.z;
			vecOrientation.w = rkey.w;
			pInMapNode->SetOrientation(vecOrientation);

			Control* cscl = pInNode->GetTMController()->GetScaleController();
			ScaleValue skey;
			cscl->GetValue(timeStart, &skey, valid);
			UGVector3d vecScale;
			vecScale.x=skey.s.x;
			vecScale.y=skey.s.y;
			vecScale.z=skey.s.z;
			pInMapNode->SetScale(vecScale);

			UGString strName = m_NodeIndexMap[pInNode];
			pInMapNode->SetMeshName( strName );

			//树关系
			UGint numChildren = pInNode->NumberOfChildren();
			UGint nChild;
			for(nChild = 0; nChild < numChildren;nChild++)
			{
				INode* pInsertNode = pInNode->GetChildNode(nChild);
				needBuildSonNode.push_back(pInsertNode);
				UGNode* pInsertMapNode = new UGNode;
				pInMapNode->AddChild(pInsertMapNode);
				needBuildSonMapNode.push_back(pInsertMapNode);
			}
		}

	}
	void UGBIMExporter::WriteAnimation(std::list<INode*>& animationNodeInScene,UGFileStream& fStream,std::map<INode*,UGString>& nodeToString,Interface *ip)
	{
		UGint numAnimation = animationNodeInScene.size();
		fStream<<numAnimation;
		std::list<INode*>::const_iterator nodeIterator=animationNodeInScene.begin();
		UGint tpf = GetTicksPerFrame();
		UGint s = ip->GetAnimRange().Start(), e = ip->GetAnimRange().End();
		UGint frames = (e - s)/GetTicksPerFrame();
		frames++;
		Interval valid = FOREVER;
		while(nodeIterator!=animationNodeInScene.end())
		{
			INode* pNode = *nodeIterator;
			UGString animationNodeName = nodeToString[pNode];
			fStream<<animationNodeName;
			UGint i,t;
			UGAnimationCurve animation;
			for (i = 0, t = s; i < frames; i++, t += GetTicksPerFrame()) 
			{

				Control* cpos = pNode->GetTMController()->GetPositionController();
				Point3 pkey;
				cpos->GetValue(t, &pkey, valid);
				UGVector3d vecPostion;	
				vecPostion.x = pkey.x;
				vecPostion.y = pkey.y;
				vecPostion.z = pkey.z;

				Control* crot = pNode->GetTMController()->GetRotationController();
				UGQuaternion4d vecOrientation;
				Quat rkey;
				crot->GetValue(t, &rkey, valid);
				vecOrientation.x = rkey.x;
				vecOrientation.y = rkey.y;
				vecOrientation.z = rkey.z;
				vecOrientation.w = rkey.w;
				Control* cscl = pNode->GetTMController()->GetScaleController();
				ScaleValue skey;
				cscl->GetValue(t, &skey, valid);
				UGVector3d vecScale;
				vecScale.x=skey.s.x;
				vecScale.y=skey.s.y;
				vecScale.z=skey.s.z;
				UGAnimationKey* pKey = animation.AddKey();
				pKey->SetTranslate(vecPostion);
				pKey->SetRotation(vecOrientation);
				pKey->SetScale(vecScale);
				UGdouble timePos = t*TIMEPERMAXTICK;
				pKey->SetTime(timePos);
			}
			animation.Save(fStream);
			nodeIterator++;
		}


	}
	void UGBIMExporter::WriteMtl(Mtl *mtl,UGFileStream& fStream)
	{
		if(mtl==NULL)return;
		UGMaterial ugMaterial;
		UGString strMtlName(mtl->GetName());
		ugMaterial.SetName(strMtlName);

		UGbool bHasTex = FALSE;

		UGString strTexPath(_U(""));
		Mtl* pBakedMtl = NULL;
		if(mtl->ClassID()==Class_ID(BAKE_SHELL_CLASS_ID,0))
		{
			pBakedMtl = mtl->GetSubMtl(1);
			mtl = mtl->GetSubMtl(0);
		}
		BitmapTex* bmt=NULL;
		Texmap *tx;
		std::map<int,UGString> idToTexName;
		int mapIndices = 0;
		for(;mapIndices<12;mapIndices++)
		{
			StdMat* std = (StdMat *)mtl;
			tx=std->GetSubTexmap(mapIndices);
			if (tx)
			{
				bHasTex = TRUE;

				int maxChannleID = tx->GetMapChannel();
				//如果通道中已经有纹理了就不再添加了
				if (idToTexName.find(maxChannleID) != idToTexName.end())
				{
					continue;
				}
				bmt=(BitmapTex*)tx;
				UGString strFileName;
				strTexPath = bmt->GetFullName();
				UGint nIndex = strTexPath.Find(_U("("));
				nIndex = strTexPath.GetLength() - nIndex - 1;
				if (nIndex > 0)
				{
					strTexPath = strTexPath.Right(nIndex);
					strFileName = strTexPath.TrimRightString(_U(")"));
					if (strFileName.CompareNoCase(_U("Autodesk Bitmap")) == 0)
					{
						continue;
					}
				}

				UGString strDestTexName = UGMaxLogFile::GetMaxLogFile()->GetTexPerfix() + strFileName;
				if (!UGFile::IsExist(m_strDestTexPath + strDestTexName) && !m_bShowPrompts)
				{
					strDestTexName = UGFile::ChangeExt(strDestTexName,_U(".jpg"));
				}
				idToTexName[maxChannleID] = strDestTexName;
			}
		}
		if(pBakedMtl!=NULL)
		{
			BitmapTex* bmt=NULL;
			Texmap *tx;
			StdMat* std = (StdMat *)pBakedMtl;
			mapIndices = 0;
			for(;mapIndices<12;mapIndices++)
			{
				tx=std->GetSubTexmap(mapIndices);
				if (tx&&tx->ClassID()==Class_ID(BMTEX_CLASS_ID,0))
				{
					bHasTex = TRUE;

					int maxChannleID = tx->GetMapChannel();
					//如果通道中已经有纹理了就不再添加了
					if (idToTexName.find(maxChannleID) != idToTexName.end())
					{
						continue;
					}
					bmt=(BitmapTex*)tx;
					UGString strFileName;
					strTexPath = bmt->GetFullName();
					UGint nIndex = strTexPath.Find(_U("("));
					nIndex = strTexPath.GetLength() - nIndex - 1;
					if (nIndex > 0)
					{
						strTexPath = strTexPath.Right(nIndex);
						strFileName = strTexPath.TrimRightString(_U(")"));
						if (strFileName.CompareNoCase(_U("Autodesk Bitmap")) == 0)
						{
							continue;
						}
					}		
					UGString strDestTexName = UGMaxLogFile::GetMaxLogFile()->GetTexPerfix() + strFileName;
					if (!UGFile::IsExist(m_strDestTexPath + strDestTexName))
					{
						strDestTexName = UGFile::ChangeExt(strDestTexName,_U(".jpg"));
					}
					idToTexName[maxChannleID] = strDestTexName;
				}
			}
		}

		std::list<int> channelIDs = m_mtlIDToChannels[m_mtlIDNow];
		std::list<int>::iterator itorChannel = channelIDs.begin();
		for(;itorChannel!=channelIDs.end();itorChannel++)
		{
			if (idToTexName.find(*itorChannel) == idToTexName.end())
			{
				continue;
			}

			ugMaterial.AddTexturePath(idToTexName[*itorChannel]);
			if(m_bShowPrompts)
			{
				UGMaxToolkit::CopyTexture(m_strTexPath+idToTexName[*itorChannel],m_strDestTexPath+idToTexName[*itorChannel]);
			}
		}

		// 当没有纹理的时候一定要存材质的颜色
		if(m_bIsExportMaterialColor || !bHasTex)
		{
			UGfloat fParency = mtl->GetXParency();
			Color diffuseColor=mtl->GetDiffuse();
			ugMaterial.SetDiffuse(diffuseColor.r,diffuseColor.g,diffuseColor.b,1.0 - fParency);
			Color ambientColor=mtl->GetAmbient();
			ugMaterial.SetAmbient(ambientColor.r,ambientColor.g,ambientColor.b,1.0 - fParency);
			Color specularColor=mtl->GetSpecular();
			ugMaterial.SetSpecular(specularColor.r,specularColor.g,specularColor.b,1.0 - fParency);
			Color emmissiveColor=mtl->GetSelfIllumColor();
			ugMaterial.SetEmmissive(emmissiveColor.r,emmissiveColor.g,emmissiveColor.b,1.0 - fParency);
			ugMaterial.SetShininess(mtl->GetShininess());
		}

		ugMaterial.Save(fStream);
		m_mtlIDNow++;
	}

	UGModelTexture*  BuildTextureData(UGString& texture)
	{
		UGModelTexture* pTexture = new UGModelTexture();
		if (pTexture->m_strName.IsEmpty())
		{
			pTexture->m_strName = UGFile::GetTitle(texture);
		}
		UGFileParseToolkit::GetTextureData(texture, pTexture->m_pTextureData);
		return pTexture;
	}

	UGModelSkeleton* UGBIMExporter::BuildSkeleton(MeshParamInfos& meshInfo,vector<UGint> &isDDS)
	{
		UGModelSkeleton* pSkeleton=new UGModelSkeleton();
		//骨架名称
		UGString strName;
		strName.Format(_U("_%d"), meshInfo.mtlID);
		pSkeleton->m_strName= meshInfo.strID + strName;

		//顶点数据
		UGVertexDataPackage* m_pVertexPackage=new UGVertexDataPackage();
		m_pVertexPackage->SetVertexNum(meshInfo.verts.size()/3);
		m_pVertexPackage->m_nVertexDimension= 3;
		for (int i = 0; i < meshInfo.verts.size(); i++)
		{
			m_pVertexPackage->m_pVertices[i]=(UGdouble)meshInfo.verts[i];
		}
		//顶点索引数据
		UGIndexPackage* m_pIndexPackage=new UGIndexPackage();
		m_pIndexPackage->m_enIndexType=meshInfo.indexs.size()<65536 ? IT_16BIT: IT_32BIT;
		m_pIndexPackage->SetIndexNum(meshInfo.indexs.size());
		
		if(m_pIndexPackage->m_enIndexType==IT_16BIT)
		{
		   for (int i = 0; i < meshInfo.indexs.size(); i++)
		   {
			   m_pIndexPackage->m_pIndexes[i] = (UGushort)meshInfo.indexs[i];
		   }
		}
		else
		{
			UGuint* pUintIndex = (UGuint*)m_pIndexPackage->m_pIndexes;
			for (int i = 0; i < meshInfo.indexs.size(); i++)
		    {
			   pUintIndex[i] = meshInfo.indexs[i];
		    }
		}
		if (m_bIsExportNormal)
		{
			if(meshInfo.indexs.size()<65536)
			{
				m_pVertexPackage->m_pNormals = UGMathEngine::ComputeVertexNormals(
					m_pVertexPackage->m_pVertices,//顶点
					m_pIndexPackage->m_pIndexes,//索引
					m_pVertexPackage->m_nVerticesCount,//顶点个数
					m_pIndexPackage->m_nIndexesCount//索引个数
					);
				 m_pVertexPackage->m_nNormalCount=m_pVertexPackage->m_nVerticesCount;
			}
			else if(meshInfo.indexs.size() > 65536)
			{
				UGuint* pIndexs = (UGuint*)m_pIndexPackage->m_pIndexes;
				m_pVertexPackage->m_pNormals = UGMathEngine::ComputeVertexNormals(
					m_pVertexPackage->m_pVertices,//顶点
					pIndexs,//索引
					m_pVertexPackage->m_nVerticesCount,//顶点个数
					m_pIndexPackage->m_nIndexesCount//索引个数
					);
				 m_pVertexPackage->m_nNormalCount=m_pVertexPackage->m_nVerticesCount;//法向量个数等于顶点个数
			}
		}
		//多重纹理
		UGuint nLevels = 0;
		std::list<int> nChannelsTemp = m_mtlIDToChannels[meshInfo.mtlID];
		std::list<int>::iterator itorChannel = nChannelsTemp.begin();	
		for(; itorChannel != nChannelsTemp.end(); itorChannel++)
		{
			m_pVertexPackage->m_TexCoordCount[nLevels] = meshInfo.verts.size()/3;
			m_pVertexPackage->m_pTexCoords[nLevels] = new UGfloat[meshInfo.verts.size()/3 * 2];
			UGfloat* pTexCoords = m_pVertexPackage->m_pTexCoords[nLevels];
			std::vector<UGfloat> aChannel = m_multiTexList[*itorChannel];
			std::vector<UGfloat>::iterator valueChannel = aChannel.begin();
			//这里有问题
			int count=0;
			int nCount = std::count(isDDS.begin(),isDDS.end(),*itorChannel);
			for( ; valueChannel != aChannel.end(); valueChannel++,pTexCoords++)
			{
				count++;
				if (nCount>0 && count%2==0)//如果为dds纹理则V翻转
				{
					*pTexCoords = - *valueChannel;
				}
				else
				{
					*pTexCoords = *valueChannel;
				}
			}
			nLevels++;
		}

		pSkeleton->SetDataPackRef(m_pVertexPackage,m_pIndexPackage);
		pSkeleton->ComputerBoundingBox();
		return pSkeleton;
	}
	
	UGModelMaterial* UGBIMExporter::BuildMaterial(Mtl *mtl, UGbool bIsTwoSide,std::vector<UGint> &isDDS)
	{
		UGModelMaterial* pMaterial3D = new UGModelMaterial;
		pMaterial3D->MakeDefault();
		if (pMaterial3D->getNumTechniques() == 0)
		{
			pMaterial3D->createTechnique();
		}
		UGTechnique* pTech = pMaterial3D->getTechnique(0);
		if (pTech->getNumPasses() == 0)
		{
			UGPass* pPassCreated = pTech->createPass();
			pPassCreated->m_strName = pMaterial3D->m_strName;
		}
		UGPass* pPass = pTech->getPass(0);
		
		if(mtl==NULL)
		{
			pMaterial3D->m_strName=_U("default");
			return pMaterial3D;
		}
		UGString strTexPath(_U(""));

		UGbool bHasTex = FALSE;

		Mtl* pBakedMtl = NULL;
		if(mtl->ClassID()==Class_ID(BAKE_SHELL_CLASS_ID,0))
		{
			pBakedMtl = mtl->GetSubMtl(1);
			mtl = mtl->GetSubMtl(0);
		}
		BitmapTex* bmt=NULL;
		Texmap *tx;
		std::map<int,UGString> idToTexName;
		int mapIndices = 0;
		for(;mapIndices<12;mapIndices++)
		{
			StdMat* std = (StdMat *)mtl;
			tx=std->GetSubTexmap(mapIndices);
			if (tx)
			{
				bHasTex = TRUE;

				int maxChannleID = tx->GetMapChannel();
				//如果通道中已经有纹理了就不再添加了
				if (idToTexName.find(maxChannleID) != idToTexName.end())
				{
					continue;
				}
				bmt=(BitmapTex*)tx;
				UGString strFileName;
				strTexPath = bmt->GetFullName();
				UGint nIndex = strTexPath.ReverseFind(_U("("));
				nIndex = strTexPath.GetLength() - nIndex - 1;
				if (nIndex > 0)
				{
					strTexPath = strTexPath.Right(nIndex);
					strFileName = strTexPath.TrimRightString(_U(")"));
					if (strFileName.CompareNoCase(_U("Autodesk Bitmap")) == 0)
					{
						continue;
					}
				}	
				UGString strDestTexName = UGMaxLogFile::GetMaxLogFile()->GetTexPerfix() + strFileName;
				if (!UGFile::IsExist(m_strDestTexPath + strDestTexName))
				{
					strDestTexName = UGFile::ChangeExt(strDestTexName,_U(".jpg"));
				}
				idToTexName[maxChannleID] = strDestTexName;
			}
		}
		if(pBakedMtl!=NULL)
		{
			BitmapTex* bmt=NULL;
			Texmap *tx;
			StdMat* std = (StdMat *)pBakedMtl;
			mapIndices = 0;
			for(;mapIndices<12;mapIndices++)
			{
				tx=std->GetSubTexmap(mapIndices);
				if (tx&&tx->ClassID()==Class_ID(BMTEX_CLASS_ID,0))
				{
					bHasTex = TRUE;

					int maxChannleID = tx->GetMapChannel();
					//如果通道中已经有纹理了就不再添加了
					if (idToTexName.find(maxChannleID) != idToTexName.end())
					{
						continue;
					}
					bmt=(BitmapTex*)tx;
					UGString strFileName;
					strTexPath = bmt->GetFullName();
					UGint nIndex = strTexPath.Find(_U("("));
					nIndex = strTexPath.GetLength() - nIndex - 1;
					if (nIndex > 0)
					{
						strTexPath = strTexPath.Right(nIndex);
						strFileName = strTexPath.TrimRightString(_U(")"));
						if (strFileName.CompareNoCase(_U("Autodesk Bitmap")) == 0)
						{
							continue;
						}
					}
					UGString strDestTexName = UGMaxLogFile::GetMaxLogFile()->GetTexPerfix() + strFileName;
					if (!UGFile::IsExist(m_strDestTexPath + strDestTexName))
					{
						strDestTexName = UGFile::ChangeExt(strDestTexName,_U(".jpg"));
					}
					idToTexName[maxChannleID] = strDestTexName;
				}
			}
		}

		UGString strMtlName = _U("");
		pPass->SetPFFMode((PolygonFrontFace)PFF_NONE);

		std::list<int> channelIDs = m_mtlIDToChannels[m_mtlIDNow];
		std::list<int>::iterator itorChannel = channelIDs.begin();
		std::vector<UGString> vecTexNames;
		for(;itorChannel!=channelIDs.end();itorChannel++)
		{
			if (idToTexName.find(*itorChannel) == idToTexName.end())
			{
				continue;
			}

			if (!UGFile::IsExist(m_strDestTexPath + idToTexName[*itorChannel]))
			{
				continue;
			}

			strMtlName += m_strDestTexPath + idToTexName[*itorChannel];

			UGTextureUnitState* pTexUnit = pPass->CreateTextureUnitState();
			pTexUnit->m_strTextureName = m_strDestTexPath + idToTexName[*itorChannel];

			//创建TextureData
			if (!m_Geode->GetTexture(m_strDestTexPath + idToTexName[*itorChannel]).IsNull())
			{
				continue;
			}
			UGModelTexture* pModelTexture = new UGModelTexture;
			UGString strExt = UGC::UGFile::GetExt(m_strDestTexPath + idToTexName[*itorChannel]);
			UGFileType::EmType eType = UGFileType::ExtNameToFileType(strExt);

			if(strExt.CompareNoCase(_U(".dds")) == 0)
			{
				isDDS.push_back(*itorChannel);//添加需要翻转的纹理通道值
				UGRenderOperationNode::TextureDataInfo* pTextureDataInfo = NULL;
				pTextureDataInfo = UGMaxToolkit::ImageTOTextureDataInfo(m_strDestTexPath + idToTexName[*itorChannel]);
				UGTextureData* pTData = pTextureDataInfo->m_pTextureData;
				pModelTexture->m_pTextureData=pTData;
				pModelTexture->m_strName=m_strDestTexPath + idToTexName[*itorChannel];
				pModelTexture->m_bMipmap = false;
				if(pModelTexture == NULL)
				{
					continue;
				}
			}
			else
			{
				UGTextureData* pTextureData = GetTextureData(m_strDestTexPath + idToTexName[*itorChannel]);	
				if (!pTextureData)
				{
					continue;
				}

				pModelTexture->m_pTextureData = pTextureData;
				pModelTexture->m_strName=m_strDestTexPath + idToTexName[*itorChannel];
				if(eType == UGFileType::GIF)
				{
					pModelTexture->m_bMipmap = FALSE;
				}
				else
				{
					pModelTexture->m_bMipmap = TRUE;
				}
			}
			m_Geode->AddEntity(pModelTexture);
			vecTexNames.push_back(pModelTexture->m_strName);
		}
		if(vecTexNames.size()>0)
		{
			pMaterial3D->SetTextureName(vecTexNames);
		}
		
		//是否带材质色，如果没贴图必须带上
		if(!bHasTex)
		{
			UGfloat fParency = mtl->GetXParency();
			Color diffuseColor=mtl->GetDiffuse();
			pPass->m_Diffuse.SetValue(diffuseColor.r,diffuseColor.g,diffuseColor.b,1.0 - fParency);

			Color ambientColor=mtl->GetAmbient();

			pPass->m_Ambient.SetValue(ambientColor.r,ambientColor.g,ambientColor.b,1.0 - fParency);

			Color specularColor=mtl->GetSpecular();
			pPass->m_Specular.SetValue(specularColor.r,specularColor.g,specularColor.b,1.0 - fParency);

			Color emmissiveColor=mtl->GetSelfIllumColor();
			pPass->m_SelfIllumination.SetValue(emmissiveColor.r,emmissiveColor.g,emmissiveColor.b,1.0 - fParency);

			pPass->m_Shininess = mtl->GetShininess();
		}
		

		m_mtlIDNow++;
		if (bIsTwoSide)
		{
			pPass->SetPFFMode(PFF_NONE);
		}
		else 
		{
			pPass->SetPFFMode(PFF_CCW);
		}
		if (pPass->GetTextureUnitStatesSize() == 0)
		{
			strMtlName = MaterialHashFunc(pPass);
		}
		else
		{
			strMtlName = strMtlName + MaterialHashFunc(pPass);
		}
		pMaterial3D->m_strName = strMtlName;
		pPass ->m_strName = strMtlName;
		return pMaterial3D;
	}


	UGString UGBIMExporter::MaterialHashFunc(UGPass* pPass)
	{
		UGString strColor, strTemp;
		strTemp.Format(_U("%x_"), pPass->m_Ambient.GetValue());
		strColor += strTemp;
		strTemp.Format(_U("%x_"), pPass->m_Diffuse.GetValue());
		strColor += strTemp;
		strTemp.Format(_U("%x_"), pPass->m_Specular.GetValue());
		strColor += strTemp;
		strTemp.Format(_U("%6f_"), pPass->m_Shininess);
		strColor += strTemp;

		return strColor;
	}

	void UGBIMExporter::SetIsExportMaterialColor(UGbool bIsExport)
	{
		m_bIsExportMaterialColor = bIsExport;
	}

	void UGBIMExporter::SetIsExportNormal(UGbool bIsExport)
	{
		m_bIsExportNormal = bIsExport;
	}
	void UGBIMExporter::SetMaxProgressBar(UGMaxProcessBar* pMaxProcessBar)
	{
		m_pMaxProcessBar = pMaxProcessBar;
	}

	UGTextureData* UGBIMExporter::GetTextureData(UGString& strTexPath)
	{
		UGString strExt = UGC::UGFile::GetExt(strTexPath);
		UGFileType::EmType eType = UGFileType::ExtNameToFileType(strExt);
		if(eType == UGFileType::GIF)
		{
			UGTextureData* pTextureData = new UGTextureData;
			//读取文件内容
			UGint ihandler = UGopen(strTexPath, O_RDONLY | UGBINARY, 0644);
			if(ihandler==-1)
			{
				return NULL;
			}
			UGseek(ihandler, 0, SEEK_END);
			UGint filelen = UGtell(ihandler);
			//开始读文件
			UGseek(ihandler, 0, SEEK_SET);
			pTextureData->m_CompressType = UGDataCodec::enrGIF;
			pTextureData->m_nSize = filelen;
			pTextureData->m_pBuffer = new UGbyte[filelen];
			pTextureData->m_enFormat = PF_BYTE_RGB;
			UGread(ihandler, (void*)pTextureData->m_pBuffer, filelen);
			UGclose(ihandler);
			return pTextureData;
		}
		else
		{
			UGImportParams* pParms = UGExchangeParamsManager::MakeImportParams(eType);
			if(pParms == NULL)
			{
				return NULL;
			}	
			pParms->SetFilePathName(strTexPath);
			UGArray<UGImageData*> aryImageData;
			UGFileParseToolkit::GetImageData(pParms, aryImageData, TRUE);
			if (eType == UGFileType::BMP)
			{
				//翻转
					UGbyte* pdata =	(UGbyte*)aryImageData[0]->pBits;
					int nWidthBytes = aryImageData[0]->nWidthBytes;
					UGbyte* ptmp = new UGbyte[nWidthBytes];
					int iLine = 0;
					UGint nHeight = aryImageData[0]->nHeight;
					for (;iLine<nHeight/2;iLine++)
					{
						memcpy(ptmp, pdata+iLine*nWidthBytes, nWidthBytes);
						memcpy(pdata+iLine*nWidthBytes, pdata+(nHeight-iLine-1)*nWidthBytes, nWidthBytes);
						memcpy(pdata+(nHeight-iLine-1)*nWidthBytes, ptmp, nWidthBytes);
					}
					delete[] ptmp;
					ptmp = NULL;
			}

			if (aryImageData.GetSize() > 0)
			{
				//下面是创建mipmap纹理
				UGTextureData* pTextureData = new UGTextureData;
				UGuint nWidth = aryImageData[0]->nWidth;
				UGuint nHeight = aryImageData[0]->nHeight;
				UGuint nBitPixel = aryImageData[0]->btBitsPixel;
				UGuint comp;

				switch (nBitPixel)
				{
				case 24:
					pTextureData->m_enFormat=PF_DXT5;
					break;
				case 32:
					pTextureData->m_enFormat=PF_DXT5;
					break;
				case 8:
					comp=1;
					pTextureData->m_enFormat=PF_L8;
					break;
				default:
					break;
				}
			

 				// 调整像素格式排列成RGBA
			
				if(nBitPixel == 24 || nBitPixel == 32)
				{
					// 把图片缩放到最合适的范围内
					comp = aryImageData[0]->btBitsPixel/8;
					UGuint nAdjustW = UGMathEngine::NextP2(nWidth);
					UGuint nAdjustH = UGMathEngine::NextP2(nHeight);
					UGuchar* pDest = new UGuchar[nAdjustW * nAdjustH * comp];
					UGint nsizedatain = aryImageData[0]->nWidthBytes * nHeight;
					UGMathEngine::ScaleImageInternal(comp, nWidth, nHeight, (UGuchar*)aryImageData[0]->pBits, nAdjustW, nAdjustH, pDest, nsizedatain);
					delete[] aryImageData[0]->pBits;

					UGuchar* pRGBAData = new UGuchar[nAdjustW*nAdjustH*4];
					int i=0;
					int j=0;
					int k=0;
					if (comp == 3)
					{
						for (i=0;i<nAdjustW*nAdjustH;i++,j+=4,k+=3)
						{
							pRGBAData[j]   = pDest[k+2]; //r
							pRGBAData[j+1] = pDest[k+1]; //g
							pRGBAData[j+2] = pDest[k];	//b
							pRGBAData[j+3] = 255;
						}
					}
					else
					{
						for (i=0;i<nAdjustW*nAdjustH;i++,j+=4,k+=4)
						{
							pRGBAData[j]   = pDest[k+2];	//r
							pRGBAData[j+1] = pDest[k+1];	//g
							pRGBAData[j+2] = pDest[k];	//b
							pRGBAData[j+3] = pDest[k+3]; //a
						}
					}
					UGuchar* pOut = NULL;
					UGuint nSize =UGImageOperator::Encode(4, nAdjustW, nAdjustH, pRGBAData, &pOut, UGDataCodec::enrS3TCDXTN,TRUE);

					pTextureData->m_enFormat=PF_BYTE_RGBA;
					pTextureData->m_nSize = nSize;
					pTextureData->m_pBuffer = pOut;
					pTextureData->m_nWidth = nAdjustW;
					pTextureData->m_nHeight = nAdjustH;
					pTextureData->m_CompressType = UGDataCodec::enrS3TCDXTN;
					delete[] pRGBAData;
					delete[] pDest;
				}
				else if(nBitPixel == 8)
				{
					UGuint comp = 4;
					pTextureData->m_enFormat = PF_BYTE_RGBA;
					pTextureData->m_nHeight = aryImageData[0]->nHeight;
					pTextureData->m_nWidth = aryImageData[0]->nWidth;
					if(pTextureData->m_pBuffer != NULL)
					{
						delete pTextureData->m_pBuffer;
						pTextureData->m_pBuffer = NULL;
					}

					UGint nSize = aryImageData[0]->nHeight*aryImageData[0]->nWidth*comp;
					pTextureData->m_pBuffer = new UGuchar[nSize];
					UGuchar* pDataDes = pTextureData->m_pBuffer;

					UGbool bPalette = aryImageData[0]->btPlanes > 0;

					if(aryImageData[0]->btBitsPixel == 8)
					{
						UGint nSizeOrg = aryImageData[0]->nHeight*aryImageData[0]->nWidth;
						UGuchar* pDataSrc = (UGuchar*)aryImageData[0]->pBits;
						for (UGint i=0; i<nSizeOrg; i++)
						{
							UGint nValue = pDataSrc[i];
							if(bPalette && nValue >=0 && 
								aryImageData[0]->palette.GetSize() > nValue)
							{	
								pDataDes[4*i+0] = aryImageData[0]->palette.GetAt(nValue).peRed;
								pDataDes[4*i+1] = aryImageData[0]->palette.GetAt(nValue).peGreen;
								pDataDes[4*i+2] = aryImageData[0]->palette.GetAt(nValue).peBlue;
								pDataDes[4*i+3] = 255;
							}
							else
							{
								pDataDes[4*i+0] = nValue;
								pDataDes[4*i+1] = nValue;
								pDataDes[4*i+2] = nValue;
								pDataDes[4*i+3] = 255;
							}
						}
						UGuchar* pOut = NULL;
						UGuint nSize =UGImageOperator::Encode(4, pTextureData->m_nWidth, pTextureData->m_nHeight, pDataDes, &pOut, UGDataCodec::enrS3TCDXTN,TRUE);

						pTextureData->m_nSize = nSize;
						pTextureData->m_pBuffer = pOut;
						pTextureData->m_nWidth = pTextureData->m_nWidth;
						pTextureData->m_nHeight = pTextureData->m_nHeight;
						pTextureData->m_CompressType = UGDataCodec::enrS3TCDXTN;

						delete [] pDataDes;
					}
				}
				else if(nBitPixel == 16)
				{
					UGuint comp = 4;
					pTextureData->m_enFormat = PF_BYTE_RGBA;
					pTextureData->m_nHeight = aryImageData[0]->nHeight;
					pTextureData->m_nWidth = aryImageData[0]->nWidth;
					if(pTextureData->m_pBuffer != NULL)
					{
						delete pTextureData->m_pBuffer;
						pTextureData->m_pBuffer = NULL;
					}

					UGint nSize = aryImageData[0]->nHeight*aryImageData[0]->nWidth*comp;
					pTextureData->m_pBuffer = new UGuchar[nSize];
					UGuchar* pDataDes = pTextureData->m_pBuffer;

					UGbool bPalette = aryImageData[0]->btPlanes > 0;

					if(aryImageData[0]->btBitsPixel == 16)
					{
						UGint nSizeOrg = aryImageData[0]->nHeight*aryImageData[0]->nWidth;
						UGuchar* pDataSrc = (UGuchar*)aryImageData[0]->pBits;
						for (UGint i=0; i<nSizeOrg; i+=2)
						{
							UGint nValue = pDataSrc[i];
							UGint nValueA = pDataSrc[i+1];
							if(bPalette && nValue >=0 && 
								aryImageData[0]->palette.GetSize() > nValue)
							{	
								pDataDes[4*i/2+0] = aryImageData[0]->palette.GetAt(nValue).peRed;
								pDataDes[4*i/2+1] = aryImageData[0]->palette.GetAt(nValue).peGreen;
								pDataDes[4*i/2+2] = aryImageData[0]->palette.GetAt(nValue).peBlue;
								pDataDes[4*i/2+3] = nValueA;
							}
							else
							{
								pDataDes[4*i/2+0] = nValue;
								pDataDes[4*i/2+1] = nValue;
								pDataDes[4*i/2+2] = nValue;
								pDataDes[4*i/2+3] = nValueA;
							}
						}
						UGuchar* pOut = NULL;
						UGuint nSize =UGImageOperator::Encode(4, pTextureData->m_nWidth, pTextureData->m_nHeight, pDataDes, &pOut, UGDataCodec::enrS3TCDXTN,TRUE);

						pTextureData->m_nSize = nSize;
						pTextureData->m_pBuffer = pOut;
						pTextureData->m_nWidth = pTextureData->m_nWidth;
						pTextureData->m_nHeight = pTextureData->m_nHeight;
						pTextureData->m_CompressType = UGDataCodec::enrS3TCDXTN;

						delete [] pDataDes;
					}
				}
				else if(nBitPixel == 160)
				{
					UGuint comp = 4;
					pTextureData->m_enFormat = PF_BYTE_RGBA;
					pTextureData->m_nHeight = aryImageData[0]->nHeight;
					pTextureData->m_nWidth = aryImageData[0]->nWidth;
					if(pTextureData->m_pBuffer != NULL)
					{
						delete pTextureData->m_pBuffer;
						pTextureData->m_pBuffer = NULL;
					}

					UGint nSize = aryImageData[0]->nHeight*aryImageData[0]->nWidth*comp;
					pTextureData->m_pBuffer = new UGuchar[nSize];
					UGuchar* pDataDes = pTextureData->m_pBuffer;

					UGbool bPalette = aryImageData[0]->btPlanes > 0;

					if(aryImageData[0]->btBitsPixel == 160)
					{
						UGint nSizeOrg = aryImageData[0]->nHeight*aryImageData[0]->nWidth;
						UGushort* pDataSrc = (UGushort*)aryImageData[0]->pBits;
						for (UGint i=0; i<nSizeOrg; i++)
						{
							UGuchar nValueR = pDataSrc[i] / 256;
							{
								pDataDes[4 * i + 0] = nValueR;
								pDataDes[4 * i + 1] = nValueR;
								pDataDes[4 * i + 2] = nValueR;
								pDataDes[4 * i + 3] = 255 - nValueR;
							}
						}


						UGuchar* pOut = NULL;
						UGuint nSize =UGImageOperator::Encode(4, pTextureData->m_nWidth, pTextureData->m_nHeight, pDataDes, &pOut, UGDataCodec::enrS3TCDXTN,TRUE);

						pTextureData->m_nSize = nSize;
						pTextureData->m_pBuffer = pOut;
						pTextureData->m_nWidth = pTextureData->m_nWidth;
						pTextureData->m_nHeight = pTextureData->m_nHeight;
						pTextureData->m_CompressType = UGDataCodec::enrS3TCDXTN;
						delete [] pDataDes;
					}
				}
				else//如果进这里，说明图片类型目前还不支持，会崩溃
				{
					delete aryImageData[0];
					bool result = UGFileParseToolkit::GetTextureData(strTexPath ,pTextureData,TRUE);
					if (!result)
					{
						//ofstream ofs("C:\\Users\\lvjz\\Desktop\\testData\\test.txt", ios::out || ios::app);
						//ofs << strTexPath;
						//ofs << "\n";
						//ofs.close();
						return NULL;
					}
				}

				return pTextureData;
			}
		}
		return NULL;
	}

	MeshParamInfos UGBIMExporter::WriteMesh(NodeMultiTex*  nodeTex,UGint mtlID,MtlToNode& mtlToNode,Mtl* mtl)
	{
		std::map<INode*,UGString> nodeToNameMap = mtlToNode.m_nodeToNameMap;
		std::vector<UGuint> indexs;
		std::vector<UGfloat> verts;
		std::vector<UGfloat> colors;
		std::vector<UGfloat> texCoords;
		MeshParamInfos meshInfo;

		std::map<UGint,UGint> oldToNewPos;
		std::map<UGint,UGint> oldToNewTex;
		std::map<UGint,UGint> oldToNewMultiTex;
		oldToNewPos.clear();
		oldToNewTex.clear();
		oldToNewMultiTex.clear();
		verts.clear();
		indexs.clear();
		texCoords.clear();
		INode* pNode = nodeTex->pNode;
		TriObject *obj = nodeTex->pTriObj;
		Mesh& mesh = obj->GetMesh();
		std::list<int> mapIDList;
		UGint mapID = 1;
		for(; mapID < 100;mapID++)
		{
			if(mesh.mapSupport(mapID))
			{
				MeshMap* pMeshMap = &(mesh.Map(mapID));
				UGC::UGbool bMapIsUsed = pMeshMap->IsUsed();
				if(bMapIsUsed)
				{
					Texmap* tx = NULL;
					for(UGint mapIndexs=0; mapIndexs<12; mapIndexs++)
					{
						if(mtl != NULL)
						{
							tx = mtl->GetSubTexmap(mapIndexs);
							if(tx)
							{
								UGint nChannel = tx->GetMapChannel();
								if(nChannel == mapID)
								{
									mapIDList.push_back(mapID);	
									break;
								}
							}
						}

					}

				}
			}
		}

		//获取第二层纹理的ID值
		UGint nSecondID = -1;

		std::list<int>::iterator itorFindSecondMultiID = mapIDList.begin();
		if(itorFindSecondMultiID != mapIDList.end())
		{
			itorFindSecondMultiID++;
		}
		if(itorFindSecondMultiID != mapIDList.end())
		{
			nSecondID = *itorFindSecondMultiID;
		}

		m_mtlIDToChannels[mtlID] = mapIDList;
		m_multiTexList.clear();
		if(mesh.numVerts <= 0)
		{
			return meshInfo;
		}
		UGint faceNum = mesh.getNumFaces();

		if(nodeTex->nIndexMtl == -1)
		{
			//如果要正常的导出Mesh的各个顶点，即不改变顶点顺序，用于面数据集
			//wangxinhe 20120606
			if (m_bIsExportVertexNormal)
			{
				UGint nVertexNum =mesh.getNumVerts();
				UGint i;
				for (i =0;i < nVertexNum;++i)
				{
					//索引
					indexs.push_back(i);

					Matrix3 mat = pNode->GetObjectTM(timeStart);
					Point3 vert = mesh.verts[i];
					Point3 vertChange = vert*mat;
					if(m_bExportAnimation)
					{
						Matrix3 mat2 = pNode->GetNodeTM(timeStart);
						mat2 = mat*Inverse(mat2);
						vert = vert*mat2;
					}
					else
					{
						vert  = vert*mat;
					}
					verts.push_back(vert.x);
					verts.push_back(vert.y);
					verts.push_back(vert.z);

					if(vertChange.x>vecMax.x)vecMax.x=vertChange.x;
					if(vertChange.y>vecMax.y)vecMax.y=vertChange.y;
					if(vertChange.z>vecMax.z)vecMax.z=vertChange.z;
					if(vertChange.x<vecMin.x)vecMin.x=vertChange.x;
					if(vertChange.y<vecMin.y)vecMin.y=vertChange.y;
					if(vertChange.z<vecMin.z)vecMin.z=vertChange.z;
				}
			}
			else //if (!m_bIsExportVertexNormal) 如果不是面数据集
			{
				for(UGint i = 0;i < faceNum;i++)
				{
					UGint vertIndex=0;
					for(;vertIndex < 3;vertIndex++)
					{
						UGint size=verts.size();
						UGint indexVert=NeedAddVertAttribute(i,vertIndex,mesh,oldToNewPos,oldToNewTex,oldToNewMultiTex,nSecondID,size);
						if(indexVert!=-1)
						{
							indexs.push_back(indexVert);
						}
						else
						{
							WriteIndex(indexs,verts,colors,texCoords,i,vertIndex,mesh,pNode,mapIDList,mtl);
						}
					}
				}
			}
		}
		else
		{
			for(UGint j=0;j<faceNum;j++)
			{
				UGint faceIndex = mesh.getFaceMtlIndex(j);
				if(faceIndex==nodeTex->nIndexMtl)
				{
					UGint vertIndex=0;
					for(;vertIndex<3;vertIndex++)
					{
						UGint size=verts.size();
						UGint indexVert=NeedAddVertAttribute(j,vertIndex,mesh,oldToNewPos,oldToNewTex,oldToNewMultiTex,nSecondID,size);
						if(indexVert != -1)
						{
							indexs.push_back(indexVert);
						}
						else
						{
							WriteIndex(indexs,verts,colors,texCoords,j,vertIndex,mesh,pNode,mapIDList,mtl);
						}
					}
				}

			}
		}
		if (verts.size() <= 0)
		{
			return meshInfo;
		}
		int nVertNum = verts.size()/3;
		UGString strID = nodeToNameMap[pNode];
		m_NodeIndexMap[pNode]= strID;		
		UGbool bTwoSided = FALSE;
		if (mtl != NULL && mtl->IsSubClassOf( Class_ID(DMTL_CLASS_ID,0)))
		{
			StdMat* stdMat = (StdMat*)(mtl);
			bTwoSided = stdMat->GetTwoSided();
		}
		else if(mtl != NULL && mtl->ClassID() == Class_ID(BAKE_SHELL_CLASS_ID, 0)){
			Mtl* bakedMtl = mtl->GetSubMtl(1);
			if(bakedMtl && bakedMtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0)){
				StdMat* stdMtl = (StdMat *)bakedMtl;
				bTwoSided = stdMtl->GetTwoSided();
			}
		} 
		meshInfo.strID = strID;
		meshInfo.mtlID = mtlID;
		meshInfo.indexs = indexs;
		meshInfo.verts = verts;
		meshInfo.colors = colors;
		meshInfo.texCoords = texCoords;
		meshInfo.bTwoSided = bTwoSided;
		return meshInfo;
	}

	UGint UGBIMExporter::NeedAddVertAttribute(UGint faceIndex,UGint vertIndex,Mesh& mesh,std::map<UGint,UGint>&oldToNewPos,std::map<UGint,UGint>&oldToNewTex,std::map<UGint,UGint>&oldToNewMultiTex,int nSecondID,UGint size)
	{
		Face face = mesh.faces[faceIndex];
		UGint indexInFace = face.v[vertIndex];
		if(mesh.numTVerts>0)
		{
			TVFace tvFace = mesh.tvFace[faceIndex];
			UGint indexInTvFace = tvFace.t[vertIndex];
			if(nSecondID==-1)
			{
				if(oldToNewPos[indexInFace]==0||oldToNewTex[indexInTvFace]==0)
				{
					oldToNewPos[indexInFace]=size/3+1;
					oldToNewTex[indexInTvFace]=size/3+1;
					return -1;

				}
				else
				{
					if(oldToNewPos[indexInFace]==oldToNewTex[indexInTvFace])
					{
						return oldToNewPos[indexInFace]-1;
					}
					else
					{
						oldToNewPos[indexInFace]=size/3+1;
						oldToNewTex[indexInTvFace]=size/3+1;
						return -1;
					}
				}
			}
			else
			{
				MeshMap* pMeshMap = &(mesh.maps[nSecondID]);
				TVFace tvFaceMulti = (pMeshMap->tf)[faceIndex];
				UGint indexInTvFaceMulti = tvFaceMulti.t[vertIndex];

				if(oldToNewPos[indexInFace]==0||oldToNewTex[indexInTvFace]==0||oldToNewMultiTex[indexInTvFaceMulti]==0)
				{
					oldToNewPos[indexInFace]=size/3+1;
					oldToNewTex[indexInTvFace]=size/3+1;
					oldToNewMultiTex[indexInTvFaceMulti]=size/3+1;
					return -1;

				}
				else
				{
					if(oldToNewPos[indexInFace]==oldToNewTex[indexInTvFace]&&oldToNewTex[indexInTvFace]==oldToNewMultiTex[indexInTvFaceMulti])
					{
						return oldToNewPos[indexInFace]-1;
					}
					else
					{
						oldToNewPos[indexInFace]=size/3+1;
						oldToNewTex[indexInTvFace]=size/3+1;
						oldToNewMultiTex[indexInTvFaceMulti] = size/3+1;
						return -1;
					}
				}
			}

		}
		else
		{
			if(oldToNewPos[indexInFace]==0)
			{
				oldToNewPos[indexInFace]=size/3+1;
				return -1;
			}
			else
				return oldToNewPos[indexInFace]-1;
		}

	}


	void UGBIMExporter::WriteIndex(std::vector<UGuint>&indexs,std::vector<UGfloat>&verts,std::vector<UGfloat>&colors,std::vector<UGfloat>&texCoords,UGint faceIndex,UGint vertIndex,Mesh &mesh,INode* pNode,std::list<int>& mapIDList,Mtl* std)
	{
		Face   face   = mesh.faces[faceIndex];

		//索引
		indexs.push_back(verts.size()/3);

		Matrix3 mat = pNode->GetObjectTM(timeStart);
		Point3 vert = mesh.verts[face.v[vertIndex]];;
		Point3 vertChange = vert*mat;
		if(m_bExportAnimation)
		{
			Matrix3 mat2 = pNode->GetNodeTM(timeStart);
			mat2 = mat*Inverse(mat2);
			vert = vert*mat2;

		}
		else
		{
			vert  = vert*mat;
		}
		verts.push_back(vert.x);
		verts.push_back(vert.y);
		verts.push_back(vert.z);

		if(vertChange.x>vecMax.x)vecMax.x=vertChange.x;
		if(vertChange.y>vecMax.y)vecMax.y=vertChange.y;
		if(vertChange.z>vecMax.z)vecMax.z=vertChange.z;
		if(vertChange.x<vecMin.x)vecMin.x=vertChange.x;
		if(vertChange.y<vecMin.y)vecMin.y=vertChange.y;
		if(vertChange.z<vecMin.z)vecMin.z=vertChange.z;

		//纹理
		if(mesh.numTVerts>0)
		{
			BitmapTex* bmt = NULL;
			Texmap *tx;
			Matrix3 mat3;
			mat3.IdentityMatrix();
			if (std != NULL)
			{
				for(int mapIndices = 0;mapIndices<12;mapIndices++)
				{
					tx = std->GetSubTexmap(mapIndices);
					if (tx)
					{
						bmt=(BitmapTex*)tx;
						bmt->GetUVTransform(mat3);
					}
				}
			}

			std::list<int>::iterator itorMapID = mapIDList.begin();
			for(;itorMapID!=mapIDList.end();itorMapID++)
			{
				int nMapID = *itorMapID;
				MeshMap* pMeshMap = &(mesh.maps[nMapID]);
				TVFace* pTVFace = pMeshMap->tf;
				UVVert* ptVert = pMeshMap->tv;
				UVVert tVert = ptVert[pTVFace[faceIndex].t[vertIndex]];
				tVert = tVert*mat3;
				m_multiTexList[nMapID].push_back(tVert.x);
				m_multiTexList[nMapID].push_back(tVert.y);
			}
		}

		if (mesh.numCVerts>0)
		{
			TVFace vcFace = mesh.vcFace[faceIndex];
			VertColor vcVert = mesh.vertCol[vcFace.t[vertIndex]];
			UGint r = (UGint)((vcVert.x)*255)%256;
			UGint g = (UGint)((vcVert.y)*255)%256;
			UGint b=  (UGint)((vcVert.z)*255)%256;
			UGint a=255;
			UGint nValue;
			if(UGC_ISBIGENDIAN)
				nValue =  (r<<24) | (g<<16) | (b<<8) | a;
			else
				nValue =   (a<<24) | (b<<16) | (g<<8) | r;
			colors.push_back(nValue);

		}

	}

	void UGBIMExporter::SetLodInfo(ModelLOD& lodInfo)
	{
		m_LodInfo = lodInfo;
	}

	void UGBIMExporter::SetRefernceNode(std::vector<INode *>& vecNode)
	{
		m_vecNode = vecNode;
	}

	UGbool UGBIMExporter::SaveS3MB(UGString strFilePath)
	{
		UGString strExt = UGFile::GetExt(strFilePath);
		UGExportParams params;
		params.SetFilePathName(strFilePath);

		UGFileParser* pFileParser;
		pFileParser = UGFileParseManager::CreateFileParser(UGFileType::S3MB);
		UGGeoModelPro *pGomodelPro=new UGGeoModelPro();
		UGModelNode *pModelNode= (UGModelNode *)pGomodelPro->GetModelNode();
		UGModelPagedPatch *pPatch= pModelNode->CreatePatch();
		pPatch->AddGeode(m_Geode);

		vector<UGString> Skes = m_Geode->GetEntityNames(UGModelEntity::etSkeleton);
		for (int i=0;i<Skes.size();i++)
		{
			UGModelNodeTools ::RemoveDuplicateVertex2((m_Geode->GetSkeleton(Skes[i])).Get());
		}

		if(!((UGFileParseModel*)pFileParser)->Save(params, pModelNode))
		{
			UGFileParseManager::DestroyFileParser(pFileParser);
			return FALSE;
		}
		UGFileParseManager::DestroyFileParser(pFileParser);
		delete pGomodelPro;
		return TRUE;
	}

	UGbool UGBIMExporter::ExportModelDataset(ExportParameter para,std::map<UGC::UGString, int> m_mapId)
	{
		if (m_ModelGroup.size() == 0)
		{
			return FALSE;
		}
		OGDC::OgdcDataSource* pUDBDataSource;
		if (!UGC::UGFile::GetExt(para.DatasourceName).CompareNoCase(_U(".udb")))
		{
			pUDBDataSource = OgdcProviderManager::CreateOgdcDataSource(OGDC::oeFile);
		}
		pUDBDataSource->m_nEngineClass = 2;
		pUDBDataSource->m_connection.m_strServer = para.DatasourceName;
		if(!((OGDC::OgdcDataSource*)pUDBDataSource)->Open())
		{
			delete pUDBDataSource;
			return FALSE;
		}
		UGString strModelName;
		//创建或获取模型数据集
		OGDC::OgdcDatasetVector* pDataset = GetModelDatasetVector(pUDBDataSource,para);
		OgdcQueryDef queryDef;
		pDataset->Open();
		queryDef.m_nType=OgdcQueryDef::General;
		queryDef.m_nOptions=OgdcQueryDef::Both;
		//queryDef.m_nMode=OgdcQueryDef::GeneralQuery;
		queryDef.m_nCursorType=OgdcQueryDef::OpenDynamic;
		//插入点
		UGPoint3D position;
		position.x=para.X;
		position.y=para.Y;
		position.z=para.Z;

		OGDC::OgdcRecordset* pRecordset=pDataset->Query(queryDef);
		if (NULL==pRecordset)
		{
			return FALSE;
		}
		for (UGint i = 0; i < m_ModelGroup.size(); i++)
		{
			UGGeoModelPro *pGomodelPro=new UGGeoModelPro();
			UGModelNode *pModelNode= (UGModelNode *)pGomodelPro->GetModelNode();
			UGModelPagedPatch *pPatch= pModelNode->CreatePatch();
			UGModelGeode* pModelGeode= m_ModelGroup[i];//要导入的模型列表
			if (para.RemoveDupVertex)
			{
				vector<UGString> Skes = pModelGeode->GetEntityNames(UGModelEntity::etSkeleton);
				for (int i=0;i<Skes.size();i++)
				{
					UGModelNodeTools ::RemoveDuplicateVertex2((pModelGeode->GetSkeleton(Skes[i])).Get());
				}
			}

			UGString modelName= pModelGeode->m_strName;
			pPatch->AddGeode(pModelGeode);

			if (pModelNode->GetSkeletonCount(-1)>0)
			{
				pGomodelPro->SetSpherePlaced(!para.IsPlanar);
				pGomodelPro->SetPosition(position);
				pRecordset->AddNew(pGomodelPro);
				pRecordset->SetFieldValue(m_strModelName,modelName);
				UGVariant varId(m_mapId[modelName]);
				pRecordset->SetFieldValue(m_strId,varId);
				bool result = pRecordset->Update();
			}
			vector<UGModelGeode*>::iterator iter = find(m_ModelInstanceGroup.begin(), m_ModelInstanceGroup.end(), m_ModelGroup[i]);
			if (iter!= m_ModelInstanceGroup.end())
			{
				delete pGomodelPro;
				pGomodelPro = NULL;
			}
		}
		pRecordset->Close();
		//pDataset->SaveInfo();
		pUDBDataSource->Close();
		return TRUE;
	}

	bool ExportOgdcDataset(UGGeoModelPro* pGomodelPro)
	{
		//创建并打开OGDC数据源
		UGString UDBSever = L" c://DataSource.udb";
		OGDC::OgdcDataSource* pUDBDataSource;
		pUDBDataSource = OgdcProviderManager::CreateOgdcDataSource(OGDC::oeFile);
		pUDBDataSource->m_nEngineClass = 2;
		pUDBDataSource->m_connection.m_strServer = UDBSever;
		pUDBDataSource->Create();
		if (!((OGDC::OgdcDataSource*)pUDBDataSource)->Open())
		{
			delete pUDBDataSource;
			return FALSE;
		}
		//创建并打开OGDC数据集
		UGString DatasetName = L"OgdcDataset";
		OGDC::OgdcDataset *pDataset = pUDBDataSource->GetDataset(DatasetName);
		OGDC::OgdcDatasetVector* pDatasetVector = (OGDC::OgdcDatasetVector*)pDataset;
		if (NULL == pDataset)
		{
			OGDC::OgdcDatasetVectorInfo tempDatasetVectorInfo;
			tempDatasetVectorInfo.m_strName = DatasetName;
			tempDatasetVectorInfo.m_nType = OGDC::OgdcDataset::Model;

			pDatasetVector = pUDBDataSource->CreateDatasetVector(tempDatasetVectorInfo);
			bool IsPlanar = false;//是否导出为平面
			if (!IsPlanar)
			{
				OGDC::OgdcCoordSys prjCoordSys;
				prjCoordSys.SetEarth(6378137, 0.00335281066474748, 10000);
				pDatasetVector->SetCoordSys(prjCoordSys);
			}
			else
			{
				OGDC::OgdcCoordSys prjCoordSys;
				pDatasetVector->SetCoordSys(prjCoordSys);
			}
			//创建字段
			UGString m_strModelName = _U("ModelName");
			UGString m_strId = _U("ID");

			OGDC::OgdcFieldInfos fieldInfos;

			m_strModelName = pDatasetVector->GetUnoccupiedFieldName(m_strModelName);
			bool result = fieldInfos.AddField(m_strModelName, OgdcFieldInfo::FieldType::Text, 255);
			m_strId = pDatasetVector->GetUnoccupiedFieldName(m_strId);
			result = fieldInfos.AddField(m_strId, OgdcFieldInfo::FieldType::INT32, 4);
			pDatasetVector->CreateFields(fieldInfos);
		}

		//创建OGDC记录集
		OgdcQueryDef queryDef;
		pDataset->Open();
		queryDef.m_nType = OgdcQueryDef::General;
		queryDef.m_nOptions = OgdcQueryDef::Both;
		queryDef.m_nCursorType = OgdcQueryDef::OpenDynamic;
		OGDC::OgdcRecordset* pRecordset = pDatasetVector->Query(queryDef);
		if (NULL == pRecordset)
		{
			return FALSE;
		}

		//记录集更新
		pRecordset->AddNew(pGomodelPro);
		UGString modelName = L"模型A";
		pRecordset->SetFieldValue(L"ModelName", modelName);
		int id = 1;
		UGVariant varId(id);
		pRecordset->SetFieldValue(L"ID", varId);
		bool result = pRecordset->Update();
		pRecordset->Close();
		//pDataset->SaveInfo();//这句话需要注释掉，因为目前会出现bounds异常。
		pUDBDataSource->Close();
		return TRUE;
	}
	/*UGbool UGBIMExporter::ExportCADDataSet(ExportParameter para)
	{
		OGDC::OgdcDataSource* pUDBDataSource;
		pUDBDataSource = OgdcProviderManager::CreateOgdcDataSource(OGDC::oeFile);
		pUDBDataSource->m_nEngineClass = 2;
		pUDBDataSource->m_connection.m_strServer = para.DatasourceName;
		if(!((OGDC::OgdcDataSource*)pUDBDataSource)->Open())
		{
			delete pUDBDataSource;
			return FALSE;
		}
		UGC::UGString strModelName = _U("ModelName");//   Modify by lina 2014-3-4 字符转换
		UGC::UGString strLongitudeColumn = _U("Longitude");
		UGC::UGString strLatitudeColumn= _U("Latitude");
		UGC::UGString strAltitudeColumn = _U("Altitude");
		OGDC::OgdcRecordset* pRecordset;
		OGDC::OgdcDataset* pDataset;
		OGDC::OgdcDatasetVector* pDatasetVector;
		OGDC::OgdcDatasetVectorInfo tempDatasetVectorInfo;
		tempDatasetVectorInfo.m_strName = para.DatasetName+_U("_CAD");
		tempDatasetVectorInfo.m_nType = OGDC::OgdcDataset::Compound;
		pDatasetVector =  pUDBDataSource->CreateDatasetVector(tempDatasetVectorInfo);

		OGDC::OgdcFieldInfos fieldInfos;
		fieldInfos.AddField(strModelName,UGC::UGFieldInfo::Text ,250);
		fieldInfos.AddField(strLongitudeColumn,UGC::UGFieldInfo::Double ,8);
		fieldInfos.AddField(strLatitudeColumn,UGC::UGFieldInfo::Double,8);
		fieldInfos.AddField(strAltitudeColumn,UGC::UGFieldInfo::Double,8);
		fieldInfos.AddField(_U("ID"),UGC::UGFieldInfo::INT64,8);
		pDatasetVector->CreateFields(fieldInfos);

		if (!para.IsPlanar)
		{
			OGDC::OgdcCoordSys prjCoordSys;
			prjCoordSys.SetEarth(6378137,0.00335281066474748,10000);
			pDatasetVector->SetCoordSys(prjCoordSys);
		}

		OGDC::OgdcQueryDef queryDef;
		pRecordset = pDatasetVector->Query(queryDef);
		if(pRecordset == NULL)
		{
			delete pUDBDataSource;
			return ;
		}

		for(int i=0;i<count;i++)
		{
			ModelProp modelProp = models.GetAt(i);
			UGC::UGString strPath = modelProp.strPath;
			if(UGMaxToolkit::m_setBillboardList.count(strPath) == 0)
			{
				UGC::UGFile::Delete(strPath);
				continue;
			}

			UGC::UGGeometry3D *pGeo3d = new UGC::UGGeoModelPro();
			UGC::UGStyle styleBillBoard;
			styleBillBoard.SetBillBoardMode(UGC::AUTO_Z_AXIAL);
			pGeo3d->SetStyle(&styleBillBoard);
		}
		return TRUE;
	}*/

	OGDC::OgdcDatasetVector* UGBIMExporter:: GetModelDatasetVector(OGDC::OgdcDataSource* pDatasource,ExportParameter para)
	{
		OGDC::OgdcDataset *pDataset=pDatasource->GetDataset(para.DatasetName);
		OGDC::OgdcDatasetVector* pDatasetVector = (OGDC::OgdcDatasetVector*)pDataset;
		if(NULL == pDataset)
		{
			OGDC::OgdcDatasetVectorInfo tempDatasetVectorInfo;
			tempDatasetVectorInfo.m_strName = para.DatasetName;
			tempDatasetVectorInfo.m_nType = OGDC::OgdcDataset::Model;

			pDatasetVector =  pDatasource->CreateDatasetVector(tempDatasetVectorInfo);

			if (!para.IsPlanar)
			{
				OGDC::OgdcCoordSys prjCoordSys;
				prjCoordSys.SetEarth(6378137,0.00335281066474748,10000);
				pDatasetVector->SetCoordSys(prjCoordSys);
			}
			else
			{
				OGDC::OgdcCoordSys prjCoordSys;
				pDatasetVector->SetCoordSys(prjCoordSys);
			}

		}

		//创建字段
		m_strModelName= _U("ModelName"); 
		m_strId=_U("ID");

		OGDC::OgdcFieldInfos fieldInfos;

		m_strModelName = pDatasetVector->GetUnoccupiedFieldName(m_strModelName);
		bool result=fieldInfos.AddField(m_strModelName,OgdcFieldInfo::FieldType::Text,255);
		m_strId = pDatasetVector->GetUnoccupiedFieldName(m_strId);
		result=fieldInfos.AddField(m_strId,OgdcFieldInfo::FieldType::INT32,4);
		pDatasetVector->CreateFields(fieldInfos);

		return pDatasetVector;
	}

}
