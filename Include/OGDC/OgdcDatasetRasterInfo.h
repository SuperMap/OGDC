//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file OgdcDatasetRasterInfo.h
//!  \brief OgdcDatasetRasterInfo 的接口定义。
//!  \details Ogdc栅格数据集信息的接口定义。
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////
#ifndef OGDCDATASETRASTERINFO_H
#define OGDCDATASETRASTERINFO_H

#include "Base/OgdcColorset.h"
#include "OGDC/OgdcDataset.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace OGDC
{


class OGDC_API OgdcBandInfo
{
public:
	//! \brief 默认构造函数
	OgdcBandInfo();
	//! \brief 析构函数
	virtual ~OgdcBandInfo();

public:
	//! \brief 拷贝构造函数
	void operator=(const OgdcBandInfo& bInfo);
	//! \brief 波段数据集的波段序号，Base 0。
	OgdcInt m_nBandIndex;
	//! \brief 波段的名字
	OgdcString m_strName;
	//! \brief 波段所在的列名
	OgdcString m_strFieldName;
	//! \brief 波段的描述信息
	OgdcString m_strDescription;
	//! \brief 波段的像素格式
	PixelFormat m_nPixelFormat;
	//! \brief 波段的影像行字节数
	OgdcInt m_nWidthBytes;
	//! \brief 三维Bounds的最小值
	OgdcDouble m_dMinZ;
	//! \brief 三维Bounds的最大值
	OgdcDouble m_dMaxZ;
	//! \brief 调色板
	OgdcColorset m_colorset;
	//! \brief 无效值，如果某一点的值是m_NoValue,将认为这一点没有值
	OgdcDouble m_dNoValue;
};

//! \brief Ogdc栅格数据集描述信息类。
class OGDC_API OgdcDatasetRasterInfo  
{
public:
	//! \brief 栅格数据集单位块大小。
	enum IBSizeOption 
	{
		//! \brief 64*64
		IBS_64				= 64,
		//! \brief 128*128
		IBS_128				= 128,
		//! \brief 256*256
		IBS_256				= 256,
		//! \brief 512*512
		IBS_512				= 512,
		//! \brief 1024*1024
		IBS_1024			= 1024,
		//! \brief 2048*2048
		IBS_2048			= 2048,
		//! \brief 4096*4096
		IBS_4096			= 4096,
		//! \brief 8192*8192
		IBS_8192			= 8192
	};

public:
	//! \brief 默认构造函数
	OgdcDatasetRasterInfo();

	//! \brief 拷贝构造函数
	//! \param rInfo 拷贝栅格数据集描述对象[in]。
	OgdcDatasetRasterInfo(const OgdcDatasetRasterInfo& rasterInfo);	
	//! \brief 默认析构函数
	virtual ~OgdcDatasetRasterInfo();
public:
	//! \brief 赋值栅格数据集描述对象。
	//! \param rInfo 现有栅格数据集描述对象[in]。
	const OgdcDatasetRasterInfo& operator =(const OgdcDatasetRasterInfo& rasterInfo);	

	//! \brief 当前对象与目标对象进行比较是否完全相同。
	//! \param rInfo 目标栅格数据集描述对象[in]。
	//! \return 是否相同，相同返回TRUE，不同返回FALSE。
	OgdcBool operator ==(const OgdcDatasetRasterInfo& rasterInfo);	

	//! \brief 当前对象与目标对象进行比较是否不相同。
	//! \param rInfo 目标栅格数据集描述对象[in]。
	//! \return 是否不同，不同返回TRUE，相同返回FALSE。
	OgdcBool operator !=(const OgdcDatasetRasterInfo& rasterInfo);		

public:
	//! \brief 数据集名称。
	OgdcString m_strName;
	
	//! \brief 数据集类型。
	OgdcDataset::Type m_nType;
		
	//! \brief 像素格式。
	PixelFormat m_nPixelFormat;
	
	//! \brief 影像宽度，单位：像素值。
	OgdcInt m_nWidth;
	
	//! \brief 影像高度，单位：像素值。
	OgdcInt m_nHeight;
	
	//! \brief 影像每一行的字节数。
	OgdcInt m_nWidthBytes;
	
	//! \brief 单位影像块边长。
	OgdcDatasetRasterInfo::IBSizeOption m_nBlockSize;
	
	//! \brief 数据集的地理范围。
	OgdcRect2D m_rcBounds;	
	
	//! \brief 横向分块总数(包含余数)。
	OgdcInt m_nColBlocks;		
	
	//! \brief 纵向分块总数(包含余数)。
	OgdcInt m_nRowBlocks;	

	//! \brief 调色板
	OgdcColorset m_colorset;
	
	//! \brief 无效值，如果某一点的值是m_NoValue,将认为这一点没有值。
	OgdcDouble m_dNoValue;
	//! \brief 波段数
	OgdcInt m_nBandCount;

	//! \brief 波段数据集的波段序号，Base 0。
	OgdcInt m_nBandIndex;
	
	//! \brief 波段所在数据表的列名称。
	OgdcString m_strFieldName;

	//! \brief 对数据集的描述信息。
	OgdcString m_strDescription;

	//! \brief 最小高程值 
	OgdcDouble m_dMinZ;
	
	//! \brief 最大高程值
	OgdcDouble m_dMaxZ;

	//波段信息
	OgdcArray<OgdcBandInfo> m_arrBandInfo;
};

}
#endif // !defined(OGDCDATASETRASTERINFO_H)

