// UGDataSetExporter.h   : plugin class for 3DMAX.
////////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file headerfile.h
//!  \brief Class 导出生成模型数据集。
//!  \details 导出生成模型数据集。
//!  \author sunye。
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
#ifndef UGBATCHMAXIMPORTER_H
#define UGBATCHMAXIMPORTER_H

#include "max.h"
//#include "MaxPlugin/UGExporter.h"
//#include "MaxPlugin/UGDataSetExporter.h"
#include "UGSingleNodeSceneExporter.h"
//using namespace UGC;

struct UGMaxFileNameHongPei 
{
	UGC::UGString m_strRealName;
	UGC::UGint nValue;
};
class UGBatchMaxImporter
{
public:
	UGBatchMaxImporter();
	~UGBatchMaxImporter();
	
	void DoImport();
	void RecurAllFiles(const UGC::UGString& strBasePath);
	//void GetAllPositionInfo();
	void ExportMaxFileToDataset(const UGC::UGString& strFileName,const UGC::UGVector3d& vecPos);
	void GetRealNameByH(const UGC::UGString& strRealName,UGC::UGString &strBaseName,UGC::UGint &nValue);
	void SetParams(UGC::UGdouble dLon,UGC::UGdouble dLat,UGC::UGdouble dAlt,const UGC::UGString& strBaseFolderPath, UGC::UGString strDataSourceName,
		UGC::UGString strDataSetName,UGC::UGbool bPlanar,UGC::UGbool m_bIsIngore, UGC::UGbool m_bIsRemoveDupVertex, UGC::UGbool m_bIsExportNormal,UGC::UGbool m_bExportAll);
private:	
	ExportParameter exportPara;
	UGC::UGString m_strFolderBasePath;
	UGC::UGbool m_bExportAll;

	UGDict<UGC::UGString,UGC::UGVector3d> m_dictPosToMaxFile;
	UGDict<UGC::UGString,UGMaxFileNameHongPei> m_dictAllMaxFiles;

};
#endif
