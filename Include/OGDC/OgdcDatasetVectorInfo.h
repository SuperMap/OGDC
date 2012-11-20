//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file OgdcDatasetVectorInfo.h
//!  \brief OgdcDatasetVectorInfo 的接口定义。
//!  \details Ogdc矢量数据集信息的接口定义。
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////
#ifndef OGDCDATASETVECTORINFO_H
#define OGDCDATASETVECTORINFO_H

#include "OGDC/OgdcDataset.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace OGDC
{

//! \brief Ogdc矢量数据集描述信息类。
class OGDC_API OgdcDatasetVectorInfo  
{
public:
	//! \brief 默认构造函数
	OgdcDatasetVectorInfo();
	//! \brief 拷贝构造函数
	OgdcDatasetVectorInfo(const OgdcDatasetVectorInfo& vectorInfo);
	//! \brief 默认析构函数
	virtual ~OgdcDatasetVectorInfo();

public:
	//! \brief 重载操作符 =
	const OgdcDatasetVectorInfo& operator =(const OgdcDatasetVectorInfo& vectorInfo);
	//! \brief 重载操作符 ==
	OgdcBool operator ==(const OgdcDatasetVectorInfo& vectorInfo);
	//! \brief 重载操作符 !=
	OgdcBool operator !=(const OgdcDatasetVectorInfo& vectorInfo);

public:
	//! \brief 数据集名称。
	OgdcString m_strName;
	
	//! \brief 数据集类型。
	OgdcDataset::Type m_nType;		

	//! \brief 数据集的地理范围。
	OgdcRect2D m_rcBounds;	
	
	//! \brief 对数据集的描述信息。
	OgdcString m_strDescription;

	OgdcInt m_nSpatialIndexType;	
};

}
#endif // !defined(OGDCDATASETVECTORINFO_H)

