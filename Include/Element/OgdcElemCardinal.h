// OgdcElemCardinal.h: interface for the OgdcElemCardinal class.
//
//////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)               
//                                                           
//!  \file OgdcElemCardinal.h
//!  \brief Cardinal曲线
//!  \details 
//!  \attention 
//!   Copyright(c) 2007-2012  SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCELEMCARDINAL_H__A491E56C_2000_4A40_B48E_C765927B73BF__INCLUDED_)
#define AFX_OGDCELEMCARDINAL_H__A491E56C_2000_4A40_B48E_C765927B73BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OgdcElement.h"

namespace OGDC {
//!  \brief Cardinal曲线
class ELEMENT_API OgdcElemCardinal : public OgdcElement  
{
public:
	//! \brief 默认构造函数，生成Cardinal曲线对象。
	OgdcElemCardinal();
	//! \brief 默认析构函数
	virtual ~OgdcElemCardinal();

public:
	//! \brief  得到Cardinal曲线的类型。
	virtual OgdcInt GetType() const;

	//! \brief  得到对象风格维数，1维。
	virtual OgdcInt GetStyleDimension() const;	

	//! \brief  创建Cardinal曲线对象
	OgdcBool Make(const OgdcElemCardinal* pElement);
		
	//! \brief  创建Cardinal曲线对象。
	//! \param  points 点数组。
	OgdcBool Make(const OgdcArray<OgdcPoint2D>& points);			
	
public:
	//! \brief 用点数组来存储多个点坐标。
	OgdcArray<OgdcPoint2D> m_points;			

};

}

#endif // !defined(AFX_OGDCELEMCARDINAL_H__A491E56C_2000_4A40_B48E_C765927B73BF__INCLUDED_)

