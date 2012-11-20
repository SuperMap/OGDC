// OgdcElemArc.h: interface for the OgdcElemArc class.
//
//////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)            
//                                                           
//!  \file OgdcElemArc.h
//!  \brief 圆弧
//!  \attention 
//!   Copyright(c) 2007-2012  SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCELEMARC_H__C5139B5D_ECF1_4F93_AB99_C5718FBEE7EC__INCLUDED_)
#define AFX_OGDCELEMARC_H__C5139B5D_ECF1_4F93_AB99_C5718FBEE7EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OgdcElement.h"

namespace OGDC {
//!  \brief 圆弧对象类，存储了圆弧的起始点，中间点和终止点。
class ELEMENT_API OgdcElemArc : public OgdcElement  
{
public:
	//! \brief 默认构造函数
	OgdcElemArc();
	//! \brief 默认析构函数
	virtual ~OgdcElemArc();

public:
	//! \brief  得到ElemArc类型。
	virtual OgdcInt GetType() const;

	//! \brief  得到对象风格维数，1维。
	virtual OgdcInt GetStyleDimension() const;	

	//! \brief  生成OgdcElemArc对象。
	OgdcBool Make(const OgdcElemArc* pElement);

	 //! \brief  创建OgdcElemArc对象。
	 //! \param  pntStart 起始点。
	 //! \param  pntMiddle 中间点。
	 //! \param  pntEnd 终止点。
	OgdcBool Make(const OgdcPoint2D& pntStart,
				const OgdcPoint2D& pntMiddle,			
				const OgdcPoint2D& pntEnd);	

public:
	//! \brief 起始点
	OgdcPoint2D m_pntStart;	
	//! \brief 中间点			
	OgdcPoint2D m_pntMiddle;
	//! \brief 终止点			
	OgdcPoint2D m_pntEnd;					

};

}

#endif // !defined(AFX_OGDCELEMARC_H__C5139B5D_ECF1_4F93_AB99_C5718FBEE7EC__INCLUDED_)

