// OgdcElemPoint.h: interface for the OgdcElemPoint class.
//
//////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)               
//                                                           
//!  \file OgdcElemPoint.h
//!  \brief 简单点
//!  \attention 
//!   Copyright(c) 2007-2012  SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCELEMPOINT_H__8CEB9DD6_0A81_4F28_BD35_FFBBC3D1831B__INCLUDED_)
#define AFX_OGDCELEMPOINT_H__8CEB9DD6_0A81_4F28_BD35_FFBBC3D1831B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OgdcElement.h"

namespace OGDC {
//!  \brief 简单点对象类
class ELEMENT_API OgdcElemPoint : public OgdcElement  
{
public:
	//!  \brief 点对象默认构造函数
	OgdcElemPoint();
	//!  \brief 点对象默认析构函数
	virtual ~OgdcElemPoint();

public:
	//! \brief  得到简单点对象类型。
	virtual OgdcInt GetType() const;

	//! \brief  得到对象风格维数，0维。
	virtual OgdcInt GetStyleDimension() const;	

	//! \brief  创建简单点对象
	OgdcBool Make(const OgdcElemPoint* pElement);
		
	//! \brief  创建简单点对象
	//! \param  point 点坐标。
	OgdcBool Make(const OgdcPoint2D& point);			
	
public:
	//! \brief 点坐标。
	OgdcPoint2D m_point;			

};

}

#endif // !defined(AFX_OGDCELEMPOINT_H__8CEB9DD6_0A81_4F28_BD35_FFBBC3D1831B__INCLUDED_)

