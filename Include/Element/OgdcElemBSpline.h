// OgdcElemBSpline.h: interface for the OgdcElemBSpline class.
//
//////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file OgdcElemBSpline.h
//!  \brief 三次B样条
//!  \details 
//!  \attention 
//!   Copyright(c) 2007-2012  SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCELEMBSPLINE_H__5ACC78F4_E7F6_4E1C_881B_CB02BFC714A9__INCLUDED_)
#define AFX_OGDCELEMBSPLINE_H__5ACC78F4_E7F6_4E1C_881B_CB02BFC714A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OgdcElement.h"

namespace OGDC {
//!  \brief 三次B样条曲线类，存储了构成B样条曲线的坐标点串。
class ELEMENT_API OgdcElemBSpline : public OgdcElement  
{
public:
	//! \brief 默认构造函数，生成B样条曲线对象。
	OgdcElemBSpline();
	//! \brief 默认析构函数
	virtual ~OgdcElemBSpline();

public:
	//! \brief  得到ElemBSpline类型。
	virtual OgdcInt GetType() const;

	//! \brief  得到对象风格维数，1维。
	virtual OgdcInt GetStyleDimension() const;	

	//! \brief  创建OgdcElemBSpline对象
	OgdcBool Make(const OgdcElemBSpline* pElement);
		
	//! \brief  创建OgdcElemBSpline对象。
	//! \param  points 点数组。
	OgdcBool Make(const OgdcArray<OgdcPoint2D>& points);			
	
public:
	//! \brief 用点数组来存储多个点坐标。
	OgdcArray<OgdcPoint2D> m_points;			

};

}

#endif // !defined(AFX_OGDCELEMBSPLINE_H__5ACC78F4_E7F6_4E1C_881B_CB02BFC714A9__INCLUDED_)

