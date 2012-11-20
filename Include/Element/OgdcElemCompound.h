// OgdcElemCompound.h: interface for the OgdcElemCompound class.
//
//////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file OgdcElemCompound.h
//!  \brief 复合对象
//!  \details 可以组合所有对象
//!  \attention 
//!   Copyright(c) 2007-2012  SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCELEMCOMPOUND_H__BD229B91_EDA3_4357_9EC1_57E490A35122__INCLUDED_)
#define AFX_OGDCELEMCOMPOUND_H__BD229B91_EDA3_4357_9EC1_57E490A35122__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OgdcElement.h"

namespace OGDC {
//!  \brief 复合对象类
class ELEMENT_API OgdcElemCompound : public OgdcElement  
{
public:
	//! \brief 默认构造函数
	OgdcElemCompound();
	//! \brief 默认析构函数
	virtual ~OgdcElemCompound();

public:
	//! \brief  得到复合对象类型。
	virtual OgdcInt GetType() const;

	//! \brief  得到对象风格维数，-1维。
	virtual OgdcInt GetStyleDimension() const;

	//! \brief  创建复合对象
	OgdcBool Make(const OgdcElemCompound* pElement);	
	
	//! \brief  清空对象。
	void Clear();	
	
public:
	//! \brief 复合对象中包含的几何对象数组。
	OgdcArray<OgdcElement*> m_elements;			

};

}

#endif // !defined(AFX_OGDCELEMCOMPOUND_H__BD229B91_EDA3_4357_9EC1_57E490A35122__INCLUDED_)

