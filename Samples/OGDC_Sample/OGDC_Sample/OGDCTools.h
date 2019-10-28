#pragma once
#include "OGDC/OgdcDataSource.h"
#include "OGDC/OgdcProviderManager.h"
#include "ModelTools.h"
using namespace UGC;
class OGDCTools
{
public:
	//创建并打开数据源
	static OgdcDataSource* CreateAndOpenDataSource(OgdcString sever);
	//创建或获取已存在数据集
	static OgdcDatasetVector* CreateOrOpenDataSetVector(OgdcDataSource*& pUDBDataSource,OgdcString DatasetName,bool isLonLat);
	//创建记录集
	static OgdcRecordset* CreateRecordset(OgdcDatasetVector*& pDatasetVector);
	//记录集更新
	static void UpdateRecordset(OgdcRecordset*& pRecordset,UGGeoModelPro*& pGeometry);
};