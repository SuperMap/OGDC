// OgdcElemPoint3D.h: interface for the OgdcElemPoint3D class.
//
//////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)               
//                                                           
//!  \file OgdcElemPoint3D.h
//!  \brief 三维简单点
//!  \attention 
//!   Copyright(c) 2007-2012  SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCELEMPOINT3D_H__C43B8D48_A64B_4C05_8F08_00EC6B753EA8__INCLUDED_)
#define AFX_OGDCELEMPOINT3D_H__C43B8D48_A64B_4C05_8F08_00EC6B753EA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OgdcElement.h"
#include "Base/OgdcPoint3D.h"

namespace OGDC {
//!  \brief 三维简单点对象类
class ELEMENT_API OgdcElemPoint3D : public OgdcElement  
{
public:
	//!  \brief 三维简单点对象默认构造函数
	OgdcElemPoint3D();
	//!  \brief 三维简单点对象默认析构函数
	virtual ~OgdcElemPoint3D();

public:
	//! \brief  得到三维简单点对象类型。
	virtual OgdcInt GetType() const;

	//! \brief  得到对象风格维数，0维。
	virtual OgdcInt GetStyleDimension() const;	

	//! \brief  创建三维简单点对象
	OgdcBool Make(const OgdcElemPoint3D* pElement);
		
	//! \brief  创建三维简单点对象
	//! \param  point 三维点坐标。
	OgdcBool Make(const OgdcPoint3D& point);			
	
public:
	//! \brief 三维点坐标。
	OgdcPoint3D m_point;			

};

}

#endif // !defined(AFX_OGDCELEMPOINT3D_H__C43B8D48_A64B_4C05_8F08_00EC6B753EA8__INCLUDED_)

