// OgdcElemCurve.h: interface for the OgdcElemCurve class.
//
//////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)               
//                                                           
//!  \file OgdcElemCurve.h
//!  \brief DGN Curve 曲线
//!  \attention 
//!   Copyright(c) 2007-2012  SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCELEMCURVE_H__B449596F_FA0F_4319_AF84_9E7B57FF0D0F__INCLUDED_)
#define AFX_OGDCELEMCURVE_H__B449596F_FA0F_4319_AF84_9E7B57FF0D0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OgdcElement.h"

namespace OGDC {
//!  \brief DGN Curve 曲线类
class ELEMENT_API OgdcElemCurve : public OgdcElement  
{
public:
	//! \brief Curve曲线默认构造函数
	OgdcElemCurve();
	//! \brief Curve曲线默认析构函数
	virtual ~OgdcElemCurve();

public:
	//! \brief  得到曲线对象类型。
	virtual OgdcInt GetType() const;

	//! \brief  得到对象风格维数，1维。
	virtual OgdcInt GetStyleDimension() const;	

	//! \brief  创建曲线对象
	OgdcBool Make(const OgdcElemCurve* pElement);
		
	//! \brief  创建曲线对象
	//! \param  points 点数组。
	OgdcBool Make(const OgdcArray<OgdcPoint2D>& points);			
	
public:
	//! \brief 用点数组来存储多个点坐标。
	OgdcArray<OgdcPoint2D> m_points;			

};

}


#endif // !defined(AFX_OGDCELEMCURVE_H__B449596F_FA0F_4319_AF84_9E7B57FF0D0F__INCLUDED_)

