// OgdcElemText.h: interface for the OgdcElemText class.
//
//////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file OgdcElemText.h
//!  \brief 文本
//!  \attention 
//!   Copyright(c) 2007-2012  SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCELEMTEXT_H__77E3E80E_0DC6_4BDD_A383_953B585BC8BA__INCLUDED_)
#define AFX_OGDCELEMTEXT_H__77E3E80E_0DC6_4BDD_A383_953B585BC8BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OgdcElement.h"
#include "Base/OgdcTextStyle.h"

namespace OGDC {
//!  \brief 文本对象类
class ELEMENT_API OgdcElemText : public OgdcElement  
{
public:
	//!  \brief 文本对象默认构造函数
	OgdcElemText();
	//!  \brief 文本对象默认析构函数
	virtual ~OgdcElemText();

public:
	//! \brief  得到文本对象类型
	virtual OgdcInt GetType() const;

	//! \brief  得到对象风格维数，-1维。
	virtual OgdcInt GetStyleDimension() const;	

	//! \brief  创建文本对象
	OgdcBool Make(const OgdcElemText* pElement);
		
	//! \brief  创建文本对象
	//! \param  points 子文本点坐标数组。
	//! \param  strTexts 子文本数组。
	//! \param  angles 子对象旋转角度数组
	//! \param  pTextStyle 文本风格
	OgdcBool Make(const OgdcArray<OgdcPoint2D>& points,
				const OgdcStringArray& strTexts,
				const OgdcArray<OgdcDouble>& angles,
				const OgdcTextStyle& textStyle);			
	
public:
	//! \brief 子文本点坐标数组。
	OgdcArray<OgdcPoint2D> m_points;
	//! \brief 子文本数组。
	OgdcStringArray m_strTexts;
	//! \brief 子对象旋转角度数组
	OgdcArray<OgdcDouble> m_angles;	
	//! \brief 文本风格
	OgdcTextStyle m_textStyle;					

};

}

#endif // !defined(AFX_OGDCELEMTEXT_H__77E3E80E_0DC6_4BDD_A383_953B585BC8BA__INCLUDED_)

