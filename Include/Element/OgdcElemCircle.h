// OgdcElemCircle.h: interface for the OgdcElemCircle class.
//
//////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file OgdcElemCircle.h
//!  \brief 圆
//!  \attention 
//!   Copyright(c) 2007-2012  SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCELEMCIRCLE_H__13731CE5_B3E4_4595_A353_4B4164F8A434__INCLUDED_)
#define AFX_OGDCELEMCIRCLE_H__13731CE5_B3E4_4595_A353_4B4164F8A434__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OgdcElement.h"

namespace OGDC {
//!  \brief 圆对象类
class ELEMENT_API OgdcElemCircle : public OgdcElement  
{
public:
	//! \brief 默认构造函数
	OgdcElemCircle();
	//! \brief 默认析构函数
	virtual ~OgdcElemCircle();

public:
	//! \brief  得到圆对象类型。
	virtual OgdcInt GetType() const;

	//! \brief  得到对象风格维数，2维。
	virtual OgdcInt GetStyleDimension() const;	

	//! \brief  创建圆对象
	OgdcBool Make(const OgdcElemCircle* pElement);
		
	//! \brief  创建圆对象
	//! \param  pntCenter 圆心
	//! \param  dRadius 半径
	OgdcBool Make(const OgdcPoint2D& pntCenter,	OgdcDouble dRadius);		

public:
	//! \brief 圆心
	OgdcPoint2D m_pntCenter;
	//! \brief 半径
	OgdcDouble m_dRadius;				

};

}


#endif // !defined(AFX_OGDCELEMCIRCLE_H__13731CE5_B3E4_4595_A353_4B4164F8A434__INCLUDED_)

