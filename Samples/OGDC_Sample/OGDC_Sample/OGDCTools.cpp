#include "OGDCTools.h"

OgdcDataSource* OGDCTools::CreateAndOpenDataSource(OgdcString sever)
{
	OgdcString UDBSever = sever;
	OGDC::OgdcDataSource* pUDBDataSource;
	//创建UDB数据引擎
	pUDBDataSource = OgdcProviderManager::CreateOgdcDataSource(OGDC::oeFile);
	pUDBDataSource->m_nEngineClass = 2;
	pUDBDataSource->m_connection.m_strServer = UDBSever;
	pUDBDataSource->m_connection.m_nType = OGDC::oeFile;
	bool result = pUDBDataSource->Create();
	if (!((OGDC::OgdcDataSource*)pUDBDataSource)->Open())
	{
		delete pUDBDataSource;
		return NULL;
	}
	return pUDBDataSource;
}

OgdcDatasetVector* OGDCTools::CreateOrOpenDataSetVector(OgdcDataSource*& pUDBDataSource,OgdcString DatasetName)
{
	if (pUDBDataSource == NULL)
	{
		return NULL;
	}
	OGDC::OgdcDataset *pDataset = pUDBDataSource->GetDataset(DatasetName);

	OGDC::OgdcDatasetVector* pDatasetVector = (OGDC::OgdcDatasetVector*)pDataset;
	if (NULL == pDataset)
	{
		OGDC::OgdcDatasetVectorInfo tempDatasetVectorInfo;
		tempDatasetVectorInfo.m_strName = DatasetName;
		tempDatasetVectorInfo.m_nType = OGDC::OgdcDataset::Model;

		pDatasetVector = pUDBDataSource->CreateDatasetVector(tempDatasetVectorInfo);
		//是否导出为平面
		bool IsPlanar = false;
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
		//创建字段（即属性信息）
		OgdcString m_strModelName = L"ModelName";
		OgdcString m_strId = L"ID";

		OGDC::OgdcFieldInfos fieldInfos;

		m_strModelName = pDatasetVector->GetUnoccupiedFieldName(m_strModelName);
		bool result = fieldInfos.AddField(m_strModelName, OgdcFieldInfo::FieldType::Text, 255);
		m_strId = pDatasetVector->GetUnoccupiedFieldName(m_strId);
		result = fieldInfos.AddField(m_strId, OgdcFieldInfo::FieldType::INT32, 4);
		pDatasetVector->CreateFields(fieldInfos);
	}
	return pDatasetVector;
}

OGDC::OgdcRecordset* OGDCTools::CreateRecordset(OgdcDatasetVector*& pDatasetVector)
{
	if (pDatasetVector == NULL)
	{
		return NULL;
	}
	OgdcQueryDef queryDef;
	pDatasetVector->Open();
	queryDef.m_nType = OgdcQueryDef::General;
	queryDef.m_nOptions = OgdcQueryDef::Both;
	queryDef.m_nCursorType = OgdcQueryDef::OpenDynamic;
	OGDC::OgdcRecordset* pRecordset = pDatasetVector->Query(queryDef);
	if (NULL == pRecordset)
	{
		return NULL;
	}
	return pRecordset;
}

void OGDCTools::UpdateRecordset(OgdcRecordset*& pRecordset, UGGeoModelPro*& pGeometry)
{
	//记录集更新（写入模型数据）
	pRecordset->AddNew(pGeometry);
	OgdcString modelName = L"模型A";
	//将属性写入属性表
	pRecordset->SetFieldValue(L"ModelName", modelName);
	int id = 1;
	UGVariant varId(id);
	pRecordset->SetFieldValue(L"ID", varId);
	bool result = pRecordset->Update();
}
