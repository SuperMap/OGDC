// OgdcElemUser.h: interface for the OgdcElemUser class.
//
//////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)               
//                                                           
//!  \file OgdcElemUser.h
//!  \brief 用户定义几何对象
//!  \attention 
//!   Copyright(c) 2007-2012  SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCELEMUSER_H__4364E4C7_CE3C_4D28_BCDD_435E9389DC7A__INCLUDED_)
#define AFX_OGDCELEMUSER_H__4364E4C7_CE3C_4D28_BCDD_435E9389DC7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OgdcElement.h"

namespace OGDC {
//!  \brief 用户定义几何对象类
class ELEMENT_API OgdcElemUser : public OgdcElement  
{
public:
	//!  \brief 用户定义几何对象默认构造函数
	OgdcElemUser();
	//!  \brief 用户定义几何对象默认析构函数
	virtual ~OgdcElemUser();

public:
	//! \brief  得到用户定义几何对象类型
	virtual OgdcInt GetType() const;

	//! \brief  得到对象风格维数，-1维。
	virtual OgdcInt GetStyleDimension() const;	

	//! \brief  创建用户定义几何对象
	OgdcBool Make(const OgdcElemUser* pElement);		
	
public:
	//! \brief 用户对象数据。
	OgdcArray<OgdcUchar> m_data;			

};

}

#endif // !defined(AFX_OGDCELEMUSER_H__4364E4C7_CE3C_4D28_BCDD_435E9389DC7A__INCLUDED_)

