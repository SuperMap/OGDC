// OgdcElemLine.h: interface for the OgdcElemLine class.
//
//////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)               
//                                                           
//!  \file OgdcElemLine.h
//!  \brief 线
//!  \attention 
//!   Copyright(c) 2007-2012  SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCELEMLINE_H__D3B68FE6_4851_4125_B584_EB53F60F784C__INCLUDED_)
#define AFX_OGDCELEMLINE_H__D3B68FE6_4851_4125_B584_EB53F60F784C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OgdcElement.h"

namespace OGDC {
//!  \brief 线对象类
class ELEMENT_API OgdcElemLine : public OgdcElement  
{
public:
	//!  \brief 线对象类默认构造函数
	OgdcElemLine();
	//!  \brief 线对象类默认析构函数
	virtual ~OgdcElemLine();

public:
	//! \brief  得到线对象类型。
	virtual OgdcInt GetType() const;

	//! \brief  得到对象风格维数，1维。
	virtual OgdcInt GetStyleDimension() const;	

	//! \brief  创建线对象
	OgdcBool Make(const OgdcElemLine* pElement);
		
	//! \brief  创建线对象。
	//! \param  points 点数组。
	//! \param  polyCounts 子对象点个数信息。
	OgdcBool Make(const OgdcArray<OgdcPoint2D>& points, 
				const OgdcArray<OgdcInt>& polyCounts);			
	
public:
	//! \brief 内部用点数组来存储多个点坐标。
	OgdcArray<OgdcPoint2D> m_points;
	//! \brief 所有子对象点个数信息
	OgdcArray<OgdcInt> m_polyCounts;			

};

}

#endif // !defined(AFX_OGDCELEMLINE_H__D3B68FE6_4851_4125_B584_EB53F60F784C__INCLUDED_)

