// OgdcElemEllipticArc.h: interface for the OgdcElemEllipticArc class.
//
//////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file OgdcElemEllipticArc.h
//!  \brief 椭圆弧
//!  \attention 
//!   Copyright(c) 2007-2012  SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCELEMELLIPTICARC_H__F46DB9AC_143F_4A42_8753_E2B97A830BE3__INCLUDED_)
#define AFX_OGDCELEMELLIPTICARC_H__F46DB9AC_143F_4A42_8753_E2B97A830BE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OgdcElement.h"

namespace OGDC {
//!  \brief 椭圆弧对象类
class ELEMENT_API OgdcElemEllipticArc : public OgdcElement  
{
public:
	//! \brief 椭圆弧默认构造函数
	OgdcElemEllipticArc();
	//! \brief 椭圆弧默认构造函数
	virtual ~OgdcElemEllipticArc();

public:
	//! \brief  得到椭圆弧对象类型。
	virtual OgdcInt GetType() const;

	//! \brief  得到对象风格维数，1维。
	virtual OgdcInt GetStyleDimension() const;	

	//! \brief  创建 椭圆弧对象
	OgdcBool Make(const OgdcElemEllipticArc* pElement);
		
	//! \brief  创建 椭圆弧对象
	//! \param  pntCenter 圆心
	//! \param  dSemiMajorAxis 长半轴
	//! \param  dSemiMinorAxis 短半轴
	//! \param  dAngle 旋转角度
	//! \param  dStartAngle 起角
	//! \param  dEndAngle 终角
	OgdcBool Make(const OgdcPoint2D& pntCenter,			
					OgdcDouble dSemiMajorAxis,		
					OgdcDouble dSemiMinorAxis,		
					OgdcDouble dAngle,		
					OgdcDouble dStartAngle,			
					OgdcDouble dEndAngle);		

public:
	//! \brief 圆心
	OgdcPoint2D m_pntCenter;
	//! \brief 长半轴
	OgdcDouble m_dSemiMajorAxis;
	//! \brief 短半轴
	OgdcDouble m_dSemiMinorAxis;
	//! \brief 旋转角度
	OgdcDouble m_dAngle;
	//! \brief 起角			
	OgdcDouble m_dStartAngle;
	//! \brief 终角
	OgdcDouble m_dEndAngle;				

};

}

#endif // !defined(AFX_OGDCELEMELLIPTICARC_H__F46DB9AC_143F_4A42_8753_E2B97A830BE3__INCLUDED_)

