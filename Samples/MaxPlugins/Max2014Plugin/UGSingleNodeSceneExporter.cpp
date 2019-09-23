#include "max.h"
#include"UGSingleNodeSceneExporter.h"
#include "UGMaxLogFile.h"
#include "decomp.h"
#include "Base3D/UGQuaternion4d.h"

UGSingleNodeSceneExporter::UGSingleNodeSceneExporter(UGMaxModelProcess *pInModelProcess,UGC::UGString strTexDest,UGC::UGString strTexSrc, UGC::UGbool bExportAll, UGC::UGbool bCopyTex, UGC::UGbool bIsBIM)
{
	m_bExportAll = bExportAll;
	m_pModelProcess=pInModelProcess;
	m_strTexDest=strTexDest;
	if (strTexSrc.GetAt(strTexSrc.GetLength()-1)!='\\')
	{
		strTexSrc+='\\';
	}
	m_strTexSrc=strTexSrc;	
	m_bCopyTexture=bCopyTex;
	m_bIsBIM = bIsBIM;
	if(m_bCopyTexture)
		UGC::UGFile::MkDir(m_strTexDest+_U("Textures\\"));//   Modify by lina 2014-3-4 字符转换
	m_pMaxProcessBar = NULL;
	myExpInterface.InitScene();
}

void UGSingleNodeSceneExporter::InitParams(UGMaxModelProcess *pInModelProcess,UGC::UGString strTexDest,UGC::UGString strTexSrc, UGC::UGbool bExportAll, UGC::UGbool bCopyTex, UGC::UGbool bIsBIM)
{
	m_bExportAll = bExportAll;
	m_pModelProcess=pInModelProcess;
	m_strTexDest=strTexDest;
	if (strTexSrc.GetAt(strTexSrc.GetLength()-1)!='\\')
	{
		strTexSrc+='\\';
	}
	m_strTexSrc=strTexSrc;
	m_bCopyTexture=bCopyTex;
	m_bIsBIM = bIsBIM;
	if(m_bCopyTexture)
		UGC::UGFile::MkDir(m_strTexDest+_U("Textures\\"));//   Modify by lina 2014-3-4 字符转换
	m_pMaxProcessBar = NULL;
	myExpInterface.InitScene();
}

int UGSingleNodeSceneExporter::callback( INode *pNode )
{
	if (!m_bExportAll &&  (pNode->IsHidden() || pNode->IsObjectHidden()))
	{
		return TREE_CONTINUE;
	}

	if (m_pMaxProcessBar!=NULL && !m_bIsBIM)
	{
		m_pMaxProcessBar->Update();
	}

	//将不导出地形为模型
	UGC::UGString strMeshName(pNode->GetName());
	if (strMeshName.GetLength() >= 16)
	{
		strMeshName =strMeshName.Left(16);
		if (!strMeshName.Compare(_U("SuperMap_Terrain")))//   Modify by lina 2014-3-4 字符转换
		{
			return TREE_CONTINUE;
		}
	}
	ILayer* aLayer = (ILayer*) pNode->GetReference(NODE_LAYER_REF);
	UGC::UGString strLayer = aLayer->GetName();
	UGC::UGbool bIsBillBoard = FALSE;
	UGC::UGbool bIsRegionZ =FALSE;
	if(strLayer.CompareNoCase(BILLBOARDLAYER) == 0)
	{
		bIsBillBoard = TRUE;
	}
	else if (strLayer.CompareNoCase(REGION3DLAYER) == 0)
	{
		bIsRegionZ =TRUE;
	}
	//如果是BIM，则只需遍历实例化对象，不在此导出模型
	if (m_bIsBIM)
	{
		m_pModelProcess->AddInstanceObject(pNode);
		UGC::UGString strNodeName = m_pModelProcess->GetCurrentNodeName();
		if(bIsBillBoard)
		{
			UGMaxToolkit::m_setBillboardList.insert(strNodeName);
		}
		else if(bIsRegionZ)
		{
			UGMaxToolkit::m_setRegionZ.insert(strNodeName);
		}
	}
	else
	{
		bool bProcess = m_pModelProcess->ProcessNode(pNode);
		if(bProcess)
		{
			if(m_bCopyTexture)
				CopyTexture(pNode);
			INode *pNewNode=m_pModelProcess->ProcessNodeByStep();
			while(pNewNode)
			{
				UGC::UGString strNodeName = m_pModelProcess->GetCurrentNodeName();
				UGC::UGString modelPath = m_strTexDest + strNodeName;
				myExpInterface.ResetIScene(pNewNode);
				if(bIsBillBoard)
				{
					UGMaxToolkit::m_setBillboardList.insert(modelPath);
				}
				else if(bIsRegionZ)
				{
					UGMaxToolkit::m_setRegionZ.insert(modelPath);
				}

				m_pModelProcess->ExportModel(modelPath.Cstr(),&myExpInterface);
				pNewNode=m_pModelProcess->ProcessNodeByStep();
			}
			m_pModelProcess->RecoveryNode();
		}
	}
	return TREE_CONTINUE;
}

void UGSingleNodeSceneExporter::StartExport
	()
{
	IScene* pScene=GetCOREInterface7()->GetScene(); 
	if (m_pMaxProcessBar!=NULL && !m_bIsBIM)
	{
		CGetNodeNum CgetNum;
		pScene->EnumTree(&CgetNum);
		m_pMaxProcessBar->SetNumInWeight(CgetNum.GetNumNode());
	}
	UGMaxToolkit::m_setBillboardList.clear();
	UGMaxToolkit::m_setRegionZ.clear();
	int x= pScene->EnumTree(this);
}

void UGSingleNodeSceneExporter::ExportModelDataset(ExportParameter para)
{
	if (m_pModelProcess)
	{
		if (m_pModelProcess->m_mapInstanceNode.size() == 0)
		{
			StartExport();
		}
		if (m_pMaxProcessBar != NULL)
		{
			m_pMaxProcessBar->SetNumInWeight(m_pModelProcess->m_mapInstanceNode.size());
		}

		int nNodeCount = 0;
		for (std::map<UGC::UGString, INode*>::iterator iter = m_pModelProcess->m_mapInstanceNode.begin(); iter != m_pModelProcess->m_mapInstanceNode.end(); iter++)
		{
			//更新进度条
			if (m_pMaxProcessBar!=NULL)
			{
				m_pMaxProcessBar->SetWeight(nNodeCount++);
				m_pMaxProcessBar->Update();
			}
			INode* node = iter->second;
			UGC::UGString strNodeName = iter->first;
			UGC::UGString modelPath = m_strTexDest + strNodeName;
			m_pModelProcess->SetModelInfo(strNodeName);
			if(m_bCopyTexture)
				CopyTexture(node);
			m_pModelProcess->ExportModel(modelPath.Cstr(),&myExpInterface);
		}
		m_pModelProcess->ExportModelDataset(para);
		UGC::UGFile::RmDirEx(m_strTexDest + _U("Textures\\"));
	}
}

void UGSingleNodeSceneExporter::ExportS3MB()
{
	if (m_pModelProcess)
	{
		if (m_pModelProcess->m_mapInstanceNode.size() == 0)
		{
			StartExport();
		}
		if (m_pMaxProcessBar != NULL)
		{
			m_pMaxProcessBar->SetNumInWeight(m_pModelProcess->m_mapInstanceNode.size());
		}

		int nNodeCount = 0;
		//UGC::UGString strS3MBPath = m_strTexDest + _U("S3MB/");
		////先清空文件夹
		//if(UGC::UGFile::IsExist(strS3MBPath))
		//{
		//	UGC::UGFile::RmDirEx(strS3MBPath);
		//}
		//UGC::UGFile::MkDirEx(strS3MBPath);
		for (std::map<UGC::UGString, INode*>::iterator iter = m_pModelProcess->m_mapInstanceNode.begin(); iter != m_pModelProcess->m_mapInstanceNode.end(); iter++)
		{
			//更新进度条
			if (m_pMaxProcessBar!=NULL)
			{
				m_pMaxProcessBar->SetWeight(nNodeCount++);
				m_pMaxProcessBar->Update();
			}
			INode* node = iter->second;
			UGC::UGString strNodeName = iter->first;
			UGC::UGString modelPath = m_strTexDest + strNodeName;
			m_pModelProcess->SetModelInfo(strNodeName);
			myExpInterface.ResetIScene(node);
			if(m_bCopyTexture)
			{
				CopyTexture(node);
			}

			m_pModelProcess->ExportModel(modelPath.Cstr(),&myExpInterface);
			m_pModelProcess->SaveS3MB(modelPath);
		}
		UGC::UGFile::RmDirEx(m_strTexDest + _U("Textures\\"));
	}
}

void UGSingleNodeSceneExporter::ProcessTexture(Mtl* mtl,UGC::UGString strTexSrc,UGC::UGString strDest)
{		
	int iMap = 0 ;
	for(;iMap< 12; iMap++)
	{
		Texmap *tx = mtl->GetSubTexmap(iMap);
		if (tx)
		{
			if(tx->ClassID() == Class_ID(BMTEX_CLASS_ID, 0))
			{
				BitmapTex* bmt = NULL;
				bmt = (BitmapTex*)tx;
				UGC::UGString strTexPath(bmt->GetMapName());
				UGC::UGString strTexName = UGC::UGFile::GetName(strTexPath);
				UGC::UGString strDestTexName = UGMaxLogFile::GetMaxLogFile()->GetTexPerfix()+ strTexName;
				UGMaxToolkit::CopyTexture(strTexSrc + strTexName,strDest + _U("Textures\\") + strDestTexName);
			}
			else
			{
				UGC::UGString strTexName;
				UGC::UGString strTexPath(tx->GetFullName());
				UGC::UGint nIndex = strTexPath.Find(_U("("));
				nIndex = strTexPath.GetLength() - nIndex - 1;
				if (nIndex > 0)
				{
					strTexPath = strTexPath.Right(nIndex);
					strTexName = strTexPath.TrimRightString(_U(")"));
					if (strTexName.CompareNoCase(_U("Autodesk Bitmap")) == 0)
					{
						continue;
					}
				}
				UGC::UGString strDestTexName = UGMaxLogFile::GetMaxLogFile()->GetTexPerfix()+ strTexName;
				UGC::UGString strDestTGAName = UGC::UGFile::ChangeExt(strDestTexName,_U("jpg"));
				//如果临时文件中该纹理不存在才进行拷贝
				if (!UGC::UGFile::IsExist(strDest + _U("Textures\\") + strDestTexName) && !UGC::UGFile::IsExist(strDest + _U("Textures\\") + strDestTGAName))
				{
					if (UGC::UGFile::IsExist(strTexSrc + strTexName))
					{
						UGMaxToolkit::CopyTexture(strTexSrc + strTexName,strDest + _U("Textures\\") + strDestTexName);//   Modify by lina 2014-3-4 字符转换
					}
					else
					{
						UGC::UGString name = strDest + _U("Textures\\") + strDestTGAName;

						BitmapInfo bi;
						bi.SetType(BMM_TRUE_32);
						bi.SetWidth(256);
						bi.SetHeight(256);
						//bi.SetFlags(MAP_HAS_ALPHA);
						Bitmap* bmap = TheManager->Create(&bi);
						tx->RenderBitmap(0, bmap);	
						BMMRES status;
						// Set name on bitmap info.
						bi.SetName(name);
						// Write the bitmap to disk.
						status = bmap->OpenOutput(&bi);

						if(status!=BMMRES_SUCCESS)
						{
							continue;
						}
						status = bmap->Write(&bi);
						if(status!=BMMRES_SUCCESS)
						{
							continue;
						}
						bmap->Close(&bi);
					}
				}
			}
		}

	}
}

void UGSingleNodeSceneExporter::ReallyCopyTexture(Mtl* mtl)
{
	if(!mtl)
	{
		return;
	}
	if(mtl->ClassID() == Class_ID(MULTI_CLASS_ID,0))
	{
		for (int i=0; i<mtl->NumSubMtls(); i++)
		{
			Mtl *sub = mtl->GetSubMtl(i);
			ReallyCopyTexture(sub);
		}
	}
	else
	{
		ProcessTexture(mtl,m_strTexSrc,m_strTexDest);
	}
}

//! \ 拷贝纹理
void UGSingleNodeSceneExporter::CopyTexture(INode *pNode )
{

	Mtl* mtl=pNode->GetMtl();
	if(!mtl)return;
	if(mtl->ClassID()==Class_ID(MULTI_CLASS_ID,0))
	{
		for (int i=0; i<mtl->NumSubMtls(); i++)
		{
			Mtl *sub = mtl->GetSubMtl(i);
			ReallyCopyTexture(sub);
		}
	}
	else if (mtl->ClassID()==Class_ID(BAKE_SHELL_CLASS_ID,0))
	{
		ReallyCopyTexture(mtl->GetSubMtl(1));
		ReallyCopyTexture(mtl->GetSubMtl(0));
	}
	else
	{
		ReallyCopyTexture(mtl);
	}	
}