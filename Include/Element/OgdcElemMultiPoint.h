// OgdcElemMultiPoint.h: interface for the OgdcElemMultiPoint class.
//
//////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file OgdcElemMultiPoint.h
//!  \brief 多点
//!  \attention 
//!   Copyright(c) 2007-2012  SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCELEMMULTIPOINT_H__A103F871_584F_46BD_BAEB_0423BA288815__INCLUDED_)
#define AFX_OGDCELEMMULTIPOINT_H__A103F871_584F_46BD_BAEB_0423BA288815__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OgdcElement.h"

namespace OGDC {
//!  \brief 多点对象类
class ELEMENT_API OgdcElemMultiPoint : public OgdcElement  
{
public:
	//!  \brief 多点对象默认构造函数
	OgdcElemMultiPoint();
	//!  \brief 多点对象默认析构函数
	virtual ~OgdcElemMultiPoint();

public:
	//! \brief  得到多点对象类型。
	virtual OgdcInt GetType() const;

	//! \brief  得到对象风格维数，0维。
	virtual OgdcInt GetStyleDimension() const;	

	//! \brief  创建多点对象
	OgdcBool Make(const OgdcElemMultiPoint* pElement);
		
	//! \brief  创建多点对象
	//! \param  points 点数组。
	OgdcBool Make(const OgdcArray<OgdcPoint2D>& points);			
	
public:
	//! \brief 内部用点数组来存储多个点坐标。
	OgdcArray<OgdcPoint2D> m_points;			

};

}

#endif // !defined(AFX_OGDCELEMMULTIPOINT_H__A103F871_584F_46BD_BAEB_0423BA288815__INCLUDED_)

