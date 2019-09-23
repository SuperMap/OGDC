
#include "UGBatchMaxImporter.h"
#include "UGMaxLogFile.h"


void UGBatchMaxImporter::DoImport()
{
	m_dictPosToMaxFile.RemoveAll();
	m_dictAllMaxFiles.RemoveAll();

	//载入所有的文件路径列表（全路径）
	RecurAllFiles(m_strFolderBasePath);
	//获取所有的文件位置信息（文件title与vector3d的dict）
	//GetAllPositionInfo();

	//遍历文件并输出
	OGDC::OgdcDict<UGC::UGString,UGMaxFileNameHongPei>::POSITION startPosition =m_dictAllMaxFiles.GetStartPosition();
	UGC::UGString strFirst;
	UGMaxFileNameHongPei hongpeiSecond;

	while(!m_dictAllMaxFiles.IsEOF(startPosition))
	{
		m_dictAllMaxFiles.GetNextAssoc(startPosition,strFirst,hongpeiSecond);

		//文件title
		UGC::UGString strFileTitle =strFirst;
		UGC::UGVector3d v3dPos(exportPara.X, exportPara.Y, exportPara.Z);

		//如果找到了位置信息
		//if (m_dictPosToMaxFile.Lookup(strFileTitle,v3dPos))
		{
			ExportMaxFileToDataset(hongpeiSecond.m_strRealName,v3dPos);
		}
	}
}

void UGBatchMaxImporter::RecurAllFiles(const UGC::UGString& strBasePath)
{
	UGArray<UGC::UGString> strFileArray;
	UGC::UGFile::ListFiles(strFileArray, strBasePath);
	UGC::UGint nFileCount = strFileArray.GetSize();

	for(UGC::UGint i = 0; i < nFileCount; i++)
	{
		UGC::UGString strFileName = strFileArray[i];
		UGC::UGint nPos =strFileName.Find(_U("."));

		//没有找到“.”认为是文件夹
		if (-1 ==nPos)
		{
			RecurAllFiles(strBasePath+_U("\\")+strFileName);
		}
		else
		{
			if (strFileName.Right(4).CompareNoCase(_U(".max")) == 0)
			{
				//烘培后但是没有更改属性表的文件
				UGC::UGString strBaseName;
				UGC::UGint nValue =-1;
				//通过文件名获取文件基本名
				strFileName.MakeLower();
				GetRealNameByH(strFileName,strBaseName,nValue);

				//如果没有。。首次添加的时候
				UGMaxFileNameHongPei hongpeiFind;
				if (!m_dictAllMaxFiles.Lookup(strBaseName,hongpeiFind))
				{
					UGMaxFileNameHongPei hongpei;
					hongpei.m_strRealName =strBasePath + _U("\\") +strFileName;
					hongpei.nValue =nValue;

					m_dictAllMaxFiles.SetAt(strBaseName,hongpei);
				}
				else
				{
					//如果找到了，但是它比现在的老，要替换之
					if (hongpeiFind.nValue <nValue)
					{
						UGMaxFileNameHongPei hongpei;
						hongpei.m_strRealName =strBasePath + _U("\\") +strFileName;
						hongpei.nValue =nValue;
						
						m_dictAllMaxFiles.SetAt(strBaseName,hongpei);
					}
				} //				if (!m_dictAllMaxFiles.LookUp(strBaseName,hongpeiFind))
			}//			if (!strFileName.Right(4).CompareNoCase(_U(".max")))
		}//		if (-1 !=nPos)
	}	//for(UGC::UGint i = 0; i < nFileCount; i++)
}

/*void UGBatchMaxImporter::GetAllPositionInfo()
{
	//任何参数为空即返回
	if (m_strLonFieldName.IsEmpty() || m_strLatFieldName.IsEmpty() || m_strFileFieldName.IsEmpty())
	{
		return;
	}

	//根路径不存在即返回
	if (!UGFile::IsExist(m_strFolderBasePath))
	{
		return;
	}

	//获取数据集，外部传入
	UGDataset *pDataset =m_pImportInfoDatasetPtr;
	if (NULL ==pDataset)
	{
		return;
	}

	//打开数据集
	if (!pDataset->IsOpen())
	{
		pDataset->Open();
	}
	
	UGQueryDef querydef;
	UGRecordset* pRecordSet = static_cast<UGDatasetVector *>(pDataset)->Query(querydef);
	if(pRecordSet == NULL || pRecordSet->IsEmpty())
	{
		return;
	}
	pRecordSet->MoveFirst();

	while(!pRecordSet->IsEOF())//循环所有的几何对象
	{
		UGdouble dLon;
		UGdouble dLat;
		UGString strFileName;
		UGVariant Var;

		//获取经度信息
		pRecordSet->GetFieldValue(m_strLonFieldName,Var);
		dLon =Var.ToDouble();

		//获取纬度信息
		pRecordSet->GetFieldValue(m_strLatFieldName,Var);
		dLat =Var.ToDouble();

		//获取模型文件名称信息
		pRecordSet->GetFieldValue(m_strFileFieldName,Var);
		strFileName =Var.ToString();
		strFileName.MakeLower();
		
		//添加到字典中
		UGVector3d v3d(dLon,dLat,10.0);
		m_dictPosToMaxFile.SetAt(strFileName,v3d);

		//记录下移
		pRecordSet->MoveNext();
	}
}*/

void UGBatchMaxImporter::ExportMaxFileToDataset(const UGC::UGString& strFileName,const UGC::UGVector3d& vecPos)
{


	//以下是导出到m_pExportDatasetPtr数据集
// 	if (pDataset != NULL)
// 	{
		//将模型导入到场景
		GetCOREInterface()->LoadFromFile(strFileName.Cstr());
		//UGERROR2(-100,strFileName);	

		UGC::UGString strTextPath =UGC::UGFile::GetDir(strFileName);

		UGMaxModelProcess modelProcessor(_U("udb"));
		modelProcessor.SetIsExportMaterialColor(!exportPara.IgnoreMaterialColor);
		modelProcessor.SetIsExportNormal(!exportPara.WithoutNormal);
		modelProcessor.SetIsBIM(TRUE);

		UGC::UGString strModelDir = UGC::UGFile::GetDir(exportPara.DatasourceName);
		UGC::UGFile::MkDir(strModelDir);
		modelProcessor.SetTexPath(strModelDir);
		
		
		UGSingleNodeSceneExporter exporter(&modelProcessor,strModelDir,strTextPath, m_bExportAll,true,true);
		exporter.SetModelRelative(0.0,0.0,0); //先设置为10

		UGMaxProcessBar maxProcessBar(hInstance,GetCOREInterface()->GetMAXHWnd());
		maxProcessBar.Start();
		maxProcessBar.SetWeight(0.1f);
		exporter.SetProcessBar(&maxProcessBar);
		exporter.ExportModelDataset(exportPara);
}

void UGBatchMaxImporter::GetRealNameByH(const UGC::UGString& strRealName,UGC::UGString &strBaseName,UGC::UGint &nValue)
{
	//基本文件名首先设置为去掉扩展名的原文件名
	strBaseName =UGC::UGFile::GetTitle(strRealName);
	
	UGC::UGint nPos =strRealName.Find(_U("_h"));

	if (nPos ==-1)
	{
		nValue =-1;
		return;
	}

	UGC::UGString strTitle =UGC::UGFile::GetTitle(strRealName);

	//恰好为_h，不带数字的情况
	UGC::UGString strExt =strTitle.Right(strTitle.GetLength() -nPos -2);
	if (strExt.IsEmpty())
	{
		strBaseName =strRealName.Left(nPos);
		nValue =0;
		return;
	}
	//Modify by lina,2014-3-4 U/非U下类型转换
	//UGint nNum =atof(strExt.Cstr());
	UGC::UGint nNum =strExt.ToInt();

	if (nNum >=0 && nNum <100)
	{
		strBaseName =strRealName.Left(nPos);
		nValue =nNum;
	}
	else
	{
		nValue =-1;
	}
}

void UGBatchMaxImporter::SetParams(UGC::UGdouble dLon,UGC::UGdouble dLat,UGC::UGdouble dAlt,const UGC::UGString& strBaseFolderPath,UGC::UGString strDataSourceName,UGC::UGString strDataSetName
								   ,UGC::UGbool bPlanar,UGC::UGbool m_bIsIngore, UGC::UGbool m_bIsRemoveDupVertex, UGC::UGbool m_bIsExportNormal,UGC::UGbool m_bExportAll)
{
	
	exportPara.X = dLon;
	exportPara.Y = dLat;
	exportPara.Z = dAlt;
	m_strFolderBasePath =strBaseFolderPath;
	exportPara.DatasourceName = strDataSourceName;
	exportPara.DatasetName =strDataSetName;
	exportPara.IsPlanar = bPlanar;
	exportPara.IgnoreMaterialColor = m_bIsIngore;
	exportPara.RemoveDupVertex = m_bIsRemoveDupVertex;
	exportPara.WithoutNormal = !m_bIsExportNormal;
	this->m_bExportAll = m_bExportAll;
}

UGBatchMaxImporter::UGBatchMaxImporter()
{
	exportPara.X = 0;
	exportPara.Y = 0;
	exportPara.Z = 0;
	m_strFolderBasePath = L"";
	exportPara.DatasourceName = L"";
	exportPara.DatasetName = L"";
	exportPara.IsPlanar = FALSE;
	exportPara.IgnoreMaterialColor = TRUE;
	exportPara.RemoveDupVertex = FALSE;
	exportPara.WithoutNormal = TRUE;
}

UGBatchMaxImporter::~UGBatchMaxImporter()
{
}