// OgdcElemChord.h: interface for the OgdcElemChord class.
//
//////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)               
//                                                           
//!  \file OgdcElemChord.h
//!  \brief 弓形
//!  \details 
//!  \attention 
//!   Copyright(c) 2007-2012  SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCELEMCHORD_H__EFFF9CB0_D7D2_4654_A4B8_E623CEDC8B8F__INCLUDED_)
#define AFX_OGDCELEMCHORD_H__EFFF9CB0_D7D2_4654_A4B8_E623CEDC8B8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OgdcElement.h"

namespace OGDC {
//!  \brief 弓形对象类
class ELEMENT_API OgdcElemChord : public OgdcElement  
{
public:
	//! \brief 默认构造函数
	OgdcElemChord();
	//! \brief 默认析构函数
	virtual ~OgdcElemChord();

public:
	//! \brief  得到ElemChord类型。
	virtual OgdcInt GetType() const;

	//! \brief  得到对象风格维数，2维。
	virtual OgdcInt GetStyleDimension() const;

	//! \brief  创建弓形对象
	OgdcBool Make(const OgdcElemChord* pElement);
		
	//! \brief  创建弓形对象。
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

#endif // !defined(AFX_OGDCELEMCHORD_H__EFFF9CB0_D7D2_4654_A4B8_E623CEDC8B8F__INCLUDED_)

