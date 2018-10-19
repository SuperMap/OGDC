/*! \file	 UGGeometryFactory.h
 *  \brief	 用来创建Geometry的工厂类
 *  \author	 ugc team
 *  \attention 
 *  Copyright(c) 2000-2010 SuperMap Software Co., Ltd.<br>
 *  All Rights Reserved
 *  \version $Id: UGGeometryFactory.h,v 1.7 2008/08/05 06:39:31 gaoff Exp $
 */
#ifndef UGGEOMETRYFACTORY_H
#define UGGEOMETRYFACTORY_H

#include "Geometry/UGGeometry.h"

namespace UGC{

//! \brief 回调函数，用于扩展对象绘制
//! \param 		pElement	外部要创建的扩展对象
//! \param 		nType		扩展对象的类型
//! \param 		pVoid		实现该回调函数对象的指针
typedef void (UGSTDCALL *CREATEGEOMETRYEXTENDCALLBACKPROC)
(UGGeometry** pGeometry, UGint nType, void* pVoid);

//! \brief 用来创建Geometry的工厂类
#ifdef SYMBIAN60
class GEOMETRY_APIC UGGeometryFactory
#else
class GEOMETRY_API UGGeometryFactory
#endif
{
public:
	//! \brief 构造函数。
	GEOMETRY_APIF UGGeometryFactory();    
	//! \brief 析构函数。
    virtual GEOMETRY_APIF ~UGGeometryFactory();

public:

	//! \brief 确保pGeometry是指定的类型
	//! \param pGeometry 几何对象指针
	//! \param eType 指定的类型
	//! \remarks 本函数主要用在: 从外面传入一个几何对象类型, 但有可能为NULL, 也有可能不是需要的类型
	//!			 这时,就需要 重新分配 Geometry指针,new 出需要类型的Geometry来; 主要用在Recordeset的AddNew等地方
	//! \return 如果对象重新分配空间了,返回true; 否则返回false
	virtual GEOMETRY_APIF UGbool AssureGeoType(UGGeometry*& pGeometry, UGint nType);	

	//! \brief 传入Geometry类型，生成Geometry
	//! \param nType 要创建的几何对象类型，可以传入Element类型，也可以传入Geometry类型
	//! \attention 返回的Geometry指针, 请使用后delete
	//! \return 成功,返回创建的Geometry的指针; 失败返回NULL
	virtual GEOMETRY_APIF UGGeometry* CreateGeometry(UGint nType);

	//! \brief 克隆Geometry
	//! \param pGeometry 要被克隆的Geometry对象指针, 不能为NULL
	//! \attention 返回的Geometry指针, 请使用后delete
	//! \return 成功,返回创建的Geometry的指针; 失败返回NULL
	virtual GEOMETRY_APIF UGGeometry* CloneGeometry(const UGGeometry* pGeometry);
#if !defined SYMBIAN60 && !defined  OS_ANDROID && !defined (IOS)
	//! \brief 克隆Element为Geometry
	//! \param pElement 要被克隆的Element对象指针, 不能为NULL
	//! \attention 返回的Geometry指针, 请使用后delete
	//! \return 成功,返回创建的Geometry的指针; 失败返回NULL
	virtual GEOMETRY_APIF UGGeometry* CloneGeometry(const OgdcElement* pElement);
#endif
	//! \brief 拷贝Geometry
	//! \param pGeometrySrc 原Geometry指针, 不得为NULL
	//! \param pGeometryTag 目标Geometry指针, 不得为NULL
	//! \remarks 两个Geometry参数类型必须一致
	//! \return 成功返回true,失败返回false
	virtual GEOMETRY_APIF UGbool CopyGeometry(const UGGeometry* pGeometrySrc, UGGeometry* pGeometryTag);
#if !defined SYMBIAN60 && !defined  OS_ANDROID && !defined (IOS)
	//! \brief 拷贝Element
	//! \param pElement 原Element指针, 不得为NULL
	//! \param pGeometry 目标Geometry指针, 不得为NULL
	//! \return 成功返回true,失败返回false
	virtual GEOMETRY_APIF UGbool CopyGeometry(const OgdcElement* pElement, UGGeometry* pGeometry);
#endif
	//! \brief 判断是否支持该几何类型
	//! \param nType，传入的Geometry类型
	virtual GEOMETRY_APIF UGbool IsSupport(UGint nType) const;
	
	//! \brief 获取Geometry工厂名称
	virtual GEOMETRY_APIF  UGString GetName() const;	

	//! \brief 传入类型名，返回UGGeometry::Type
	//! \param strGeometryType 几何对象类型的名字
	//! \attention 严格区分大小写, 请参照"GeoPoint"的形式
	//! \return 成功返回几何对象类型, 失败返回0
	virtual GEOMETRY_APIF UGint GetGeometryType(const UGString& strGeometryType);
	
	//! \brief 传入类型名，返回UGGeometry::Type
	//! \param strGeometryType 几何对象类型的名字
	//! \attention 严格区分大小写, 请参照"GeoPoint"的形式
	//! \return 成功返回几何对象类型, 失败返回0
	virtual GEOMETRY_APIF UGString GetGeometryName(UGint nGeoType);
};



}

#endif // !defined(AFX_UGGEOFACTORY_H__78297849_2C23_469A_A6B2_24C8E982EE63__INCLUDED_)

