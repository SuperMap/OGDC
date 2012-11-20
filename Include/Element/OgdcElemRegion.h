// OgdcElemRegion.h: interface for the OgdcElemRegion class.
//
//////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)              
//                                                           
//!  \file OgdcElemRegion.h
//!  \brief 多边形
//!  \attention 
//!   Copyright(c) 2007-2012  SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCELEMREGION_H__9897F4E4_DA45_4B79_A486_97DEFCA99415__INCLUDED_)
#define AFX_OGDCELEMREGION_H__9897F4E4_DA45_4B79_A486_97DEFCA99415__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OgdcElement.h"

namespace OGDC {
//!  \brief 多边形对象类
class ELEMENT_API OgdcElemRegion : public OgdcElement  
{
public:
	//!  \brief 多边形对象默认构造函数
	OgdcElemRegion();
	//!  \brief 多边形对象默认析构函数
	virtual ~OgdcElemRegion();

public:
	//! \brief  得到多边形对象类型。
	virtual OgdcInt GetType() const;

	//! \brief  得到对象风格维数，2维。
	virtual OgdcInt GetStyleDimension() const;

	//! \brief  创建多边形对象
	OgdcBool Make(const OgdcElemRegion* pElement);
		
	//! \brief  创建多边形对象
	//! \param  points 点数组。
	//! \param  polyCounts 子对象点个数信息。
	OgdcBool Make(const OgdcArray<OgdcPoint2D>& points, 
				const OgdcArray<OgdcInt>& polyCounts);			
	
public:
	//! \brief 内部用点数组来存储多个点坐标。
	OgdcArray<OgdcPoint2D> m_points;
	//! \brief 所有子对象点个数信息
	OgdcArray<OgdcInt> m_polyCounts;			

};

}

#endif // !defined(AFX_OGDCELEMREGION_H__9897F4E4_DA45_4B79_A486_97DEFCA99415__INCLUDED_)

