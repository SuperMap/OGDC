// OgdcElemRectRound.h: interface for the OgdcElemRectRound class.
//
//////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)              
//                                                           
//!  \file OgdcElemRectRound.h
//!  \brief 圆角矩形
//!  \attention 
//!   Copyright(c) 2007-2012  SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCELEMRECTROUND_H__D1E033A1_CC4B_4107_948D_07C77C85D8F0__INCLUDED_)
#define AFX_OGDCELEMRECTROUND_H__D1E033A1_CC4B_4107_948D_07C77C85D8F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OgdcElement.h"

namespace OGDC {
//!  \brief 圆角矩形对象类
class ELEMENT_API OgdcElemRectRound : public OgdcElement  
{
public:
	//!  \brief 圆角矩形对象默认构造函数
	OgdcElemRectRound();
	//!  \brief 圆角矩形对象默认析构函数
	virtual ~OgdcElemRectRound();

public:
	//! \brief  得到圆角矩形对象类型。
	virtual OgdcInt GetType() const;

	//! \brief  得到对象风格维数，2维。
	virtual OgdcInt GetStyleDimension() const;

	//! \brief  创建圆角矩形对象
	OgdcBool Make(const OgdcElemRectRound* pElement);
		
	//! \brief  创建圆角矩形对象
	//! \param  pntCenter 圆心
	//! \param  dWidth 宽度
	//! \param  dHeight 高度
	//! \param  dAngle 角度
	//! \param  dRadiusX 圆角长半轴
	//! \param  dRadiusY 圆角短半轴
	OgdcBool Make(const OgdcPoint2D& pntCenter,			
					OgdcDouble dWidth,		
					OgdcDouble dHeight,		
					OgdcDouble dAngle,		
					OgdcDouble dRadiusX,		
					OgdcDouble dRadiusY);		

public:
	//! \brief 中心点
	OgdcPoint2D m_pntCenter;
	//! \brief 宽度
	OgdcDouble m_dWidth;
	//! \brief 高度			
	OgdcDouble m_dHeight;
	//! \brief 旋转角度			
	OgdcDouble m_dAngle;
	//! \brief 圆角长半轴
	OgdcDouble m_dRadiusX;
	//! \brief 圆角短半轴			
	OgdcDouble m_dRadiusY;				


};

}

#endif // !defined(AFX_OGDCELEMRECTROUND_H__D1E033A1_CC4B_4107_948D_07C77C85D8F0__INCLUDED_)

