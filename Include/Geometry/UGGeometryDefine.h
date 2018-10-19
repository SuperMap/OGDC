/*! \file	 UGGeometryDefine.h
 *  \brief	 用来定义Geometry的工厂类
 *  \author	 ugc team
 *  \attention 
 *  Copyright(c) 2000-2010 SuperMap Software Co., Ltd.<br>
 *  All Rights Reserved
 *  \version $Id: UGGeometryDefine.h,v 1.4 2009/05/14 03:00:24 zhouqin Exp $
 */

#ifndef UGGEOMETRYDEFINE_H
#define UGGEOMETRYDEFINE_H

#include "Geometry/UGGeometryFactory.h"
#if defined OS_ANDROID || defined IOS
#include "GeometryCAD/UGGeometryCADFactory.h"
#endif
namespace UGC{

//! \brief 用来定义Geometry的工厂类
class GEOMETRY_API UGGeometryDefine
{
public:
	//! \brief 构造函数。
	UGGeometryDefine();    
	//! \brief 析构函数。
#ifdef SYMBIAN60
    ~UGGeometryDefine();
#else
	virtual ~UGGeometryDefine();
#endif

public:
	//! \brief 名称
	UGString m_strName;

	void* m_hHandle;
	UGGeometryFactory *m_pGeoFactory;
};



}

#endif // UGGEOMETRYDEFINE_H

