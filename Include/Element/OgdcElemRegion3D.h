// OgdcElemRegion3D.h: interface for the OgdcElemRegion3D class.
//
//////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)              
//                                                           
//!  \file OgdcElemRegion3D.h
//!  \brief 三维多边形
//!  \attention 
//!   Copyright(c) 2007-2012  SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCELEMREGION3D_H__8154B56F_4FD0_4F26_A4EA_FF0D2C696F35__INCLUDED_)
#define AFX_OGDCELEMREGION3D_H__8154B56F_4FD0_4F26_A4EA_FF0D2C696F35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OgdcElement.h"
#include "Base/OgdcPoint3D.h"

namespace OGDC {
//!  \brief 三维多边形对象类
class ELEMENT_API OgdcElemRegion3D : public OgdcElement  
{
public:
	//!  \brief 三维多边形默认构造函数
	OgdcElemRegion3D();
	//!  \brief 三维多边形默认析构函数
	virtual ~OgdcElemRegion3D();

public:
	//! \brief  得到三维多边形对象类型
	virtual OgdcInt GetType() const;

	//! \brief  得到对象风格维数，2维。
	virtual OgdcInt GetStyleDimension() const;	

	//! \brief  创建三维多边形对象
	OgdcBool Make(const OgdcElemRegion3D* pElement);
		
	//! \brief  创建三维多边形对象
	//! \param  points 三维点数组。
	//! \param  polyCounts 子对象点个数信息。
	OgdcBool Make(const OgdcArray<OgdcPoint3D>& points, 
				const OgdcArray<OgdcInt>& polyCounts);			
	
public:
	//! \brief 内部用点数组来存储多个点坐标。
	OgdcArray<OgdcPoint3D> m_points;
	//! \brief 所有子对象点个数信息
	OgdcArray<OgdcInt> m_polyCounts;			

};

}

#endif // !defined(AFX_OGDCELEMREGION3D_H__8154B56F_4FD0_4F26_A4EA_FF0D2C696F35__INCLUDED_)

