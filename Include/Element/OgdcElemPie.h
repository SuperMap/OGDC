// OgdcElemPie.h: interface for the OgdcElemPie class.
//
//////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file OgdcElemPie.h
//!  \brief 扇形
//!  \attention 
//!   Copyright(c) 2007-2012  SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCELEMPIE_H__DF3EE547_4B7C_4130_9E03_3EB1B44CD068__INCLUDED_)
#define AFX_OGDCELEMPIE_H__DF3EE547_4B7C_4130_9E03_3EB1B44CD068__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OgdcElement.h"

namespace OGDC {
//!  \brief 扇形对象类
class ELEMENT_API OgdcElemPie : public OgdcElement  
{
public:
	//!  \brief 扇形对象默认构造函数
	OgdcElemPie();
	//!  \brief 扇形对象类默认析构函数
	virtual ~OgdcElemPie();

public:
	//! \brief  得到扇形对象类型。
	virtual OgdcInt GetType() const;

	//! \brief  得到对象风格维数，2维。
	virtual OgdcInt GetStyleDimension() const;	

	//! \brief  创建扇形对象
	OgdcBool Make(const OgdcElemPie* pElement);
		
	//! \brief  创建扇形对象
	//! \param  pntCenter 圆心
	//! \param  dSemiMajorAxis 长半轴
	//! \param  dSemiMinorAxis 短半轴
	//! \param  dAngle 旋转角度
	//! \param  dStartAngle 起角
	//! \param  dEndAngle 终角
	OgdcBool Make(const OgdcPoint2D& pntCenter,			
					OgdcDouble dSemiMajorAxis,		
					OgdcDouble dSemiMinorAxis,		
					OgdcDouble dAngle,		
					OgdcDouble dStartAngle,			
					OgdcDouble dEndAngle);		

public:
	//! \brief 圆心
	OgdcPoint2D m_pntCenter;
	//! \brief 长半轴
	OgdcDouble m_dSemiMajorAxis;
	//! \brief 短半轴
	OgdcDouble m_dSemiMinorAxis;
	//! \brief 旋转角度
	OgdcDouble m_dAngle;
	//! \brief 起角			
	OgdcDouble m_dStartAngle;
	//! \brief 终角
	OgdcDouble m_dEndAngle;				

};

}

#endif // !defined(AFX_OGDCELEMPIE_H__DF3EE547_4B7C_4130_9E03_3EB1B44CD068__INCLUDED_)

