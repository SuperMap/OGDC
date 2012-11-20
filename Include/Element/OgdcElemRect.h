// OgdcElemRect.h: interface for the OgdcElemRect class.
//
//////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)               
//                                                           
//!  \file OgdcElemRect.h
//!  \brief 矩形
//!  \attention 
//!   Copyright(c) 2007-2012  SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCELEMRECT_H__FE530EC5_4893_49D7_B990_7DF785015E2B__INCLUDED_)
#define AFX_OGDCELEMRECT_H__FE530EC5_4893_49D7_B990_7DF785015E2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OgdcElement.h"

namespace OGDC {
//!  \brief 矩形对象类
class ELEMENT_API OgdcElemRect : public OgdcElement  
{
public:
	//!  \brief 矩形对象构造函数
	OgdcElemRect();
	//!  \brief 矩形对象析构函数
	virtual ~OgdcElemRect();

public:
	//! \brief  得到矩形对象类型。
	virtual OgdcInt GetType() const;

	//! \brief  得到对象风格维数，2维。
	virtual OgdcInt GetStyleDimension() const;	

	//! \brief  创建矩形对象
	OgdcBool Make(const OgdcElemRect* pElement);
		
	//! \brief  创建矩形对象
	//! \param  pntCenter 圆心
	//! \param  dWidth 宽度
	//! \param  dHeight 高度
	//! \param  dAngle 角度
	OgdcBool Make(const OgdcPoint2D& pntCenter,			
					OgdcDouble dWidth,		
					OgdcDouble dHeight,		
					OgdcDouble dAngle);		

public:
	//! \brief 中心点
	OgdcPoint2D m_pntCenter;
	//! \brief 宽度
	OgdcDouble m_dWidth;
	//! \brief 高度			
	OgdcDouble m_dHeight;
	//! \brief 旋转角度			
	OgdcDouble m_dAngle;				

};

}

#endif // !defined(AFX_OGDCELEMRECT_H__FE530EC5_4893_49D7_B990_7DF785015E2B__INCLUDED_)

