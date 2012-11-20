// OgdcElemLineM.h: interface for the OgdcElemLineM class.
//
//////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)               
//                                                           
//!  \file OgdcElemLineM.h
//!  \brief 刻度线
//!  \attention 
//!   Copyright(c) 2007-2012  SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCELEMLINEM_H__5B4E67E5_EF29_4803_9D6B_975F03262FF9__INCLUDED_)
#define AFX_OGDCELEMLINEM_H__5B4E67E5_EF29_4803_9D6B_975F03262FF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OgdcElement.h"

namespace OGDC {
//!  \brief 刻度线对象
class ELEMENT_API OgdcElemLineM : public OgdcElement  
{
public:
	//!  \brief 刻度线对象默认构造函数
	OgdcElemLineM();
	//!  \brief 刻度线对象默认析构函数
	virtual ~OgdcElemLineM();

public:
	//! \brief  得到刻度线对象类型。
	virtual OgdcInt GetType() const;

	//! \brief  得到对象风格维数，1维。
	virtual OgdcInt GetStyleDimension() const;	

	//! \brief  创建刻度线对象
	OgdcBool Make(const OgdcElemLineM* pElement);
		
	//! \brief  创建刻度线对象
	//! \param  points 点数组。
	//! \param  measures 点坐标的刻度。
	//! \param  polyCounts 子对象点个数信息。
	OgdcBool Make(const OgdcArray<OgdcPoint2D>& points, 
				const OgdcArray<OgdcDouble>& measures, 
				const OgdcArray<OgdcInt>& polyCounts);			
	
public:
	//! \brief 内部用点数组来存储多个点坐标。
	OgdcArray<OgdcPoint2D> m_points;
	//! \brief 所点坐标的刻度。
	OgdcArray<OgdcDouble> m_measures;
	//! \brief 所有子对象点个数信息
	OgdcArray<OgdcInt> m_polyCounts;			

};

}

#endif // !defined(AFX_OGDCELEMLINEM_H__5B4E67E5_EF29_4803_9D6B_975F03262FF9__INCLUDED_)

