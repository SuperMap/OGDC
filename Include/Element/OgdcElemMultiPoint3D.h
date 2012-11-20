// OgdcElemMultiPoint3D.h: interface for the OgdcElemMultiPoint3D class.
//
//////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)              
//                                                           
//!  \file OgdcElemMultiPoint3D.h
//!  \brief 三维多点
//!  \details 
//!  \attention 
//!   Copyright(c) 2007-2012  SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCELEMMULTIPOINT3D_H__07575210_9EAA_40E3_8E8E_7E326EC91765__INCLUDED_)
#define AFX_OGDCELEMMULTIPOINT3D_H__07575210_9EAA_40E3_8E8E_7E326EC91765__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OgdcElement.h"
#include "Base/OgdcPoint3D.h"

namespace OGDC {
//!  \brief 三维多点对象
class ELEMENT_API OgdcElemMultiPoint3D : public OgdcElement  
{
public:
	//!  \brief 三维多点对象默认构造函数
	OgdcElemMultiPoint3D();
	//!  \brief 三维多点对象默认析构函数
	virtual ~OgdcElemMultiPoint3D();

public:
	//! \brief  得到三维多点对象类型。
	virtual OgdcInt GetType() const;

	//! \brief  得到对象风格维数，0维。
	virtual OgdcInt GetStyleDimension() const;	

	//! \brief  创建三维多点对象
	OgdcBool Make(const OgdcElemMultiPoint3D* pElement);
		
	//! \brief  创建三维多点对象
	//! \param  points 三维点数组。
	OgdcBool Make(const OgdcArray<OgdcPoint3D>& points);			
	
public:
	//! \brief 内部用点数组来存储多个点坐标。
	OgdcArray<OgdcPoint3D> m_points;			

};

}

#endif // !defined(AFX_OGDCELEMMULTIPOINT3D_H__07575210_9EAA_40E3_8E8E_7E326EC91765__INCLUDED_)

