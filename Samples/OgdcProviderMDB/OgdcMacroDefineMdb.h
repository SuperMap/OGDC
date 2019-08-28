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
#define		OG_REGISTER_TABLE		_U("OGRegister")			//数据库中唯一的注册表，记所有矢量数据集的信息	
#define		OG_IMGREGISTER_TABLE	_U("OGImgRegister")			//数据库中唯一的注册表，记所有栅格数据集的信息	
#define     OG_DATASOURCE_INFO		_U("OGDataSourceInfo")		//数据源信息表

//OGRegister表中的字段=======================================================================
//			宏名						字段名					        类型			说明	
//...........................................................................................
#define		OGRF_ID						_U("OGDatasetID")		//(dbLong)		//Dataset ID
#define		OGRF_DATASET_NAME			_U("OGDatasetName")		//(dbText)		//Dataset Name
#define		OGRF_PARENTDT_ID			_U("OGParentDTID")		//(dbInteger)	//Parent Dataset ID
#define		OGRF_DATASET_TYPE			_U("OGDatasetType")		//(dbInteger)	//Dataset's type
#define		OGRF_LEFT					_U("OGLeft")			//(dbLong)		//Dataset Bounds' Left
#define		OGRF_RIGHT					_U("OGRight")			//(dbLong)		//Dataset Bounds' Right
#define		OGRF_TOP					_U("OGTop")				//(dbLong)		//Dataset Bounds' Top
#define		OGRF_RECORDCOUNT			_U("OGRecordCount")		//(dbLong)		//Dataset recordCount
#define		OGRF_BOTTOM					_U("OGBottom")			//(dbLong)		//Dataset Bounds' Bottom
#define		OGRF_LASTUPDATE				_U("OGLastUpdate")		//dbtime

//OGImgRegister表中的字段=======================================================================
//			宏名				字段名							类型			说明	
//...........................................................................................
#define     OGIRF_PIXELFORMAT    _U("OGPixelFormat")     //int			//img pixel			
#define     OGIRF_WIDTH			_U("OGWidth")			//int			//img width
#define     OGIRF_HEIGHT		_U("OGHeight")			//int			//img height
#define     OGIRF_EBLOCKSIZE	_U("OGeBlockSize")		//int			//
#define     OGIRF_BLOCKSIZES	_U("OGBlockSizes")		//int			//img BlockSizes
#define     OGIRF_LEFT			_U("OGLeft")			//dbDouble		//img left
#define     OGIRF_TOP			_U("OGTop")				//dbDouble		//img top
#define     OGIRF_RIGHT			_U("OGRight")			//dbDouble		//img right
#define     OGIRF_BOTTOM		_U("OGBottom")			//dbDouble		//img bottom
#define     OGIRF_PALETTE		_U("OGPalette")         //Image         //img Palette
#define		OGIRF_PYRAMID		_U("OGPyramid")			//varchar TN_REGISTER_TABLE      //pyramid ID
#define		OGIRF_PYRAMIDLEVEL	_U("OGPyramidLevel")	//int			//pyramidLevel 
#define		OGIRF_MAXZ			_U("OGMaxZ")			//(dbDouble)	//Max Z Value of Dataset
#define		OGIRF_MINZ			_U("OGMinZ")			//(dbDouble)	//Min Z Value fo Dataset

//DatasetRaster表中的字段=======================================================================
//			宏名				字段名							类型			说明	
//...........................................................................................
#define		OG_DIRF_ROW				_U("OGRow")				//int			//Row 
#define		OG_DIRF_COLUMN			_U("OGColumn")			//int			//Column
#define		OG_DIRF_SIZE			_U("OGSize")			//int			//ImgBlock's Size
#define		OG_DIRF_RASTER			_U("OGRaster")			//image			//ImgBlock


//OGDataSourceInfo表中的字段=======================================================================
//			宏名				字段名							类型			说明	
//...........................................................................................
#define		OG_DF_VERSION			_U("OGVersion")			//dbLong		数据源版本号
#define		OG_DF_FLAG				_U("OGFlag")			//dbLong		数据源标志
#define		OG_DF_COORDSYS			_U("OGCoordSys")		//dbLong		坐标系

//DatasetVector表中的字段=======================================================================
//			宏名				字段名					类型				说明	
//...........................................................................................
#define		OG_OBJ_ID		_U("OGID")			// (dbLong)			内部编号	(All)
#define		OG_USER_ID		_U("OGUserID")		// (dbLong)			用户编号	(All)
#define		OG_GEOMETRY		_U("OGGeometry")	// (dbLongBinary)	几何数据	ltNetwork, ltLine, ltRegion, ltTin, ltText
#define		OG_SDRI_W		_U("OGSdriW")		// (dbDouble)		最小横坐标	ltNetwork, ltLine, ltRegion, ltTin, ltText
#define		OG_SDRI_N		_U("OGSdriN")		// (dbDouble)		最小纵坐标	ltNetwork, ltLine, ltRegion, ltTin, ltText
#define		OG_SDRI_E		_U("OGSdriE")		// (dbDouble)		最大横坐标	ltNetwork, ltLine, ltRegion, ltTin, ltText
#define		OG_SDRI_S		_U("OGSdriS")		// (dbDouble)		最大纵坐标	ltNetwork, ltLine, ltRegion, ltTin, ltText
#define		OG_X			_U("OGX")			// (dbDouble)		点的横坐标	ltPoint & ltPoint3D
#define		OG_Y			_U("OGY")			// (dbDouble)		点的纵坐标	ltPoint & ltPoint3D
#define		OG_Z			_U("OGZ")			// (dbDouble)		点的Z坐标	ltPoint3D
#define		OG_LENGTH		_U("OGLength")		// (dbDouble)		长度		ltLine& Network
#define		OG_AREA			_U("OGArea")		// (dbDouble)		面积		ltRegion
#define		OG_PERIMETER	_U("OGPerimeter")	// (dbDouble)		周长		ltRegion 
#define		OG_HASHCODE		_U("OGHashCode")	// (dbLong)			哈希值		ltTexture

#endif // !defined(AFX_MDB_MACRO_DEFINE_INCLUDED_)