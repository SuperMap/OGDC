//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)              
//                                                           
//!  \file OgdcMacroDefineMdb.h
//!  \brief OgdcProviderMdb的宏定义。
//!  \details OgdcProviderMdb需要用到的宏定义。
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Limited Company.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MDB_MACRO_DEFINE_INCLUDED_)
#define AFX_MDB_MACRO_DEFINE_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//说明：定义一些在SDX引擎中需要使用的表名，字段名

//===============================================================================
//			宏名					名称						        说明		
//................................................................................
#define		OG_REGISTER_TABLE		OGDCPCTSTR("OGRegister")			//数据库中唯一的注册表，记所有矢量数据集的信息	
#define		OG_IMGREGISTER_TABLE	OGDCPCTSTR("OGImgRegister")			//数据库中唯一的注册表，记所有栅格数据集的信息	
#define     OG_DATASOURCE_INFO		OGDCPCTSTR("OGDataSourceInfo")		//数据源信息表

//OGRegister表中的字段=======================================================================
//			宏名						字段名					        类型			说明	
//...........................................................................................
#define		OGRF_ID						OGDCPCTSTR("OGDatasetID")		//(dbLong)		//Dataset ID
#define		OGRF_DATASET_NAME				OGDCPCTSTR("OGDatasetName")		//(dbText)		//Dataset Name
#define		OGRF_DATASET_TYPE				OGDCPCTSTR("OGDatasetType")		//(dbInteger)	//Dataset's type
#define		OGRF_LEFT						OGDCPCTSTR("OGLeft")			//(dbLong)		//Dataset Bounds' Left
#define		OGRF_RIGHT					OGDCPCTSTR("OGRight")			//(dbLong)		//Dataset Bounds' Right
#define		OGRF_TOP						OGDCPCTSTR("OGTop")				//(dbLong)		//Dataset Bounds' Top
#define		OGRF_RECORDCOUNT				OGDCPCTSTR("OGRecordCount")		//(dbLong)		//Dataset recordCount
#define		OGRF_BOTTOM					OGDCPCTSTR("OGBottom")			//(dbLong)		//Dataset Bounds' Bottom
#define		OGRF_LASTUPDATE				OGDCPCTSTR("OGLastUpdate")		//dbtime

//OGImgRegister表中的字段=======================================================================
//			宏名				字段名							类型			说明	
//...........................................................................................
#define     OGIRF_PIXELFORMAT     OGDCPCTSTR("OGPixelFormat")     //int			//img pixel			
#define     OGIRF_WIDTH			OGDCPCTSTR("OGWidth")			//int			//img width
#define     OGIRF_HEIGHT			OGDCPCTSTR("OGHeight")			//int			//img height
#define     OGIRF_EBLOCKSIZE		OGDCPCTSTR("OGeBlockSize")		//int			//
#define     OGIRF_BLOCKSIZES		OGDCPCTSTR("OGBlockSizes")		//int			//img BlockSizes
#define     OGIRF_LEFT			OGDCPCTSTR("OGLeft")			//dbDouble		//img left
#define     OGIRF_TOP				OGDCPCTSTR("OGTop")				//dbDouble		//img top
#define     OGIRF_RIGHT			OGDCPCTSTR("OGRight")			//dbDouble		//img right
#define     OGIRF_BOTTOM			OGDCPCTSTR("OGBottom")			//dbDouble		//img bottom
#define     OGIRF_PALETTE			OGDCPCTSTR("OGPalette")         //Image         //img Palette
#define		OGIRF_PYRAMID			OGDCPCTSTR("OGPyramid")			//varchar TN_REGISTER_TABLE      //pyramid ID
#define		OGIRF_PYRAMIDLEVEL	OGDCPCTSTR("OGPyramidLevel")	//int			//pyramidLevel 
#define		OGIRF_MAXZ			OGDCPCTSTR("OGMaxZ")			//(dbDouble)	//Max Z Value of Dataset
#define		OGIRF_MINZ			OGDCPCTSTR("OGMinZ")			//(dbDouble)	//Min Z Value fo Dataset

//DatasetRaster表中的字段=======================================================================
//			宏名				字段名							类型			说明	
//...........................................................................................
#define		OG_DIRF_ROW				OGDCPCTSTR("OGRow")				//int			//Row 
#define		OG_DIRF_COLUMN			OGDCPCTSTR("OGColumn")			//int			//Column
#define		OG_DIRF_SIZE			OGDCPCTSTR("OGSize")			//int			//ImgBlock's Size
#define		OG_DIRF_RASTER			OGDCPCTSTR("OGRaster")			//image			//ImgBlock


//OGDataSourceInfo表中的字段=======================================================================
//			宏名				字段名							类型			说明	
//...........................................................................................
#define		OG_DF_VERSION			OGDCPCTSTR("OGVersion")			//dbLong		数据源版本号
#define		OG_DF_FLAG				OGDCPCTSTR("OGFlag")			//dbLong		数据源标志
#define		OG_DF_COORDSYS			OGDCPCTSTR("OGCoordSys")		//dbLong		坐标系

//DatasetVector表中的字段=======================================================================
//			宏名				字段名					类型				说明	
//...........................................................................................
#define		OG_OBJ_ID		OGDCPCTSTR("OGID")			// (dbLong)			内部编号	(All)
#define		OG_USER_ID		OGDCPCTSTR("OGUserID")		// (dbLong)			用户编号	(All)
#define		OG_GEOMETRY		OGDCPCTSTR("OGGeometry")	// (dbLongBinary)	几何数据	ltNetwork, ltLine, ltRegion, ltTin, ltText
#define		OG_SDRI_W		OGDCPCTSTR("OGSdriW")		// (dbDouble)		最小横坐标	ltNetwork, ltLine, ltRegion, ltTin, ltText
#define		OG_SDRI_N		OGDCPCTSTR("OGSdriN")		// (dbDouble)		最小纵坐标	ltNetwork, ltLine, ltRegion, ltTin, ltText
#define		OG_SDRI_E		OGDCPCTSTR("OGSdriE")		// (dbDouble)		最大横坐标	ltNetwork, ltLine, ltRegion, ltTin, ltText
#define		OG_SDRI_S		OGDCPCTSTR("OGSdriS")		// (dbDouble)		最大纵坐标	ltNetwork, ltLine, ltRegion, ltTin, ltText
#define		OG_X			OGDCPCTSTR("OGX")			// (dbDouble)		点横坐标的	ltPoint & ltPoint3D
#define		OG_Y			OGDCPCTSTR("OGY")			// (dbDouble)		点的纵坐标	ltPoint & ltPoint3D
#define		OG_LENGTH		OGDCPCTSTR("OGLength")		// (dbDouble)		长度		ltLine& Network
#define		OG_AREA			OGDCPCTSTR("OGArea")		// (dbDouble)		面积		ltRegion
#define		OG_PERIMETER	OGDCPCTSTR("OGPerimeter")	// (dbDouble)		周长		ltRegion 

#endif // !defined(AFX_MDB_MACRO_DEFINE_INCLUDED_)