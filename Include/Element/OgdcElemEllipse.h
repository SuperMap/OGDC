// OgdcElemEllipse.h: interface for the OgdcElemEllipse class.
//
//////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file OgdcElemEllipse.h
//!  \brief 椭圆
//!  \attention 
//!   Copyright(c) 2007-2012  SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCELEMELLIPSE_H__DDD92B30_88E8_46F8_8A23_6D1ED8B7CD9E__INCLUDED_)
#define AFX_OGDCELEMELLIPSE_H__DDD92B30_88E8_46F8_8A23_6D1ED8B7CD9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OgdcElement.h"

namespace OGDC {
//!  \brief 椭圆对象类
class ELEMENT_API OgdcElemEllipse : public OgdcElement  
{
public:
	//! \brief 椭圆默认构造函数
	OgdcElemEllipse();
	//! \brief 椭圆默认析构函数
	virtual ~OgdcElemEllipse();

public:
	//! \brief  得到椭圆对象类型。
	virtual OgdcInt GetType() const;

	//! \brief  得到对象风格维数，2维。
	virtual OgdcInt GetStyleDimension() const;	

	//! \brief  创建椭圆对象
	OgdcBool Make(const OgdcElemEllipse* pElement);
		
	//! \brief  创建椭圆对象
	//! \param  pntCenter 圆心
	//! \param  dSemiMajorAxis 长半轴
	//! \param  dSemiMinorAxis 短半轴
	//! \param  dAngle 角度
	OgdcBool Make(const OgdcPoint2D& pntCenter,			
					OgdcDouble dSemiMajorAxis,		
					OgdcDouble dSemiMinorAxis,		
					OgdcDouble dAngle);		

public:
	//! \brief 圆心
	OgdcPoint2D m_pntCenter;
	//! \brief 长半轴
	OgdcDouble m_dSemiMajorAxis;
	//! \brief 短半轴
	OgdcDouble m_dSemiMinorAxis;
	//! \brief 角度
	OgdcDouble m_dAngle;				

};

}

#endif // !defined(AFX_OGDCELEMELLIPSE_H__DDD92B30_88E8_46F8_8A23_6D1ED8B7CD9E__INCLUDED_)

