/*! \file	 UGGeometryManager.h
 *  \brief	 用来创建Geometry的工厂类
 *  \author	 ugc team
 *  \attention 
 *  Copyright(c) 2000-2010 SuperMap Software Co., Ltd.<br>
 *  All Rights Reserved
 *  \version $Id: UGGeometryManager.h,v 1.8 2009/05/14 03:00:24 zhouqin Exp $
 */
#ifndef UGGEOMETRYMANAGER_H
#define UGGEOMETRYMANAGER_H

#include "Geometry/UGGeometryDefine.h"

namespace UGC{

//! \brief 用来创建Geometry的工厂类
class GEOMETRY_API UGGeometryManager
{
public: 
	//! \brief 构造函数
	UGGeometryManager();
	
	//! \brief 析构函数
#ifdef SYMBIAN60
	~UGGeometryManager();
#else
	virtual ~UGGeometryManager();
#endif

public:	
	//! \brief 得到几何工厂的数目。
	static UGint GetGeoFactoryCount();	

	//! \brief 传入Geometry类型，获取指定类型Geometry的名称
	//! \param nType 传入的Geometry的类型
	static UGString GetGeoFactoryName(UGGeometry::Type nType);	

	//! \brief 确保pGeometry是指定的类型
	//! \param pGeometry 几何对象指针
	//! \param eType 指定的类型
	//! \remarks 本函数主要用在: 从外面传入一个几何对象类型, 但有可能为NULL, 也有可能不是需要的类型
	//!			 这时,就需要 重新分配 Geometry指针,new 出需要类型的Geometry来; 主要用在Recordeset的AddNew等地方
	//! \return 如果对象重新分配空间了,返回true; 否则返回false
	static UGbool AssureGeoType(UGGeometry*& pGeometry, UGint nType);	

	//! \brief 传入Geometry类型，生成Geometry
	//! \param nType 要创建的几何对象类型，可以传入Element类型，也可以传入Geometry类型
	//! \attention 返回的Geometry指针, 请使用后delete
	//! \return 成功,返回创建的Geometry的指针; 失败返回NULL
	static UGGeometry* CreateGeometry(UGint nType);

	//! \brief 克隆Geometry
	//! \param pGeometry 要被克隆的Geometry对象指针, 不能为NULL
	//! \attention 返回的Geometry指针, 请使用后delete
	//! \return 成功,返回创建的Geometry的指针; 失败返回NULL
	static UGGeometry* CloneGeometry(const UGGeometry* pGeometry);
#if !defined SYMBIAN60 && !defined  OS_ANDROID && !defined (IOS) 
	//! \brief 克隆Element为Geometry
	//! \param pElement 要被克隆的Element对象指针, 不能为NULL
	//! \attention 返回的Geometry指针, 请使用后delete
	//! \return 成功,返回创建的Geometry的指针; 失败返回NULL
	static UGGeometry* CloneGeometry(const OgdcElement* pElement);
#endif	
	//! \brief 拷贝Geometry
	//! \param pGeometrySrc 原Geometry指针, 不得为NULL
	//! \param pGeometryTag 目标Geometry指针, 不得为NULL
	//! \remarks 两个Geometry参数类型必须一致
	//! \return 成功返回true,失败返回false
	static UGbool CopyGeometry(const UGGeometry* pGeometrySrc, UGGeometry* pGeometryTag);
#if !defined SYMBIAN60 && !defined  OS_ANDROID && !defined (IOS) 
	//! \brief 拷贝Element
	//! \param pElement 原Element指针, 不得为NULL
	//! \param pGeometry 目标Geometry指针, 不得为NULL
	//! \return 成功返回true,失败返回false
	static UGbool CopyGeometry(const OgdcElement* pElement, UGGeometry* pGeometry);
#endif
	//! \brief 判断是否支持该几何类型
	//! \param nType，传入的Geometry类型
	static UGbool IsSupport(UGint nGeoType);

	//! \brief 传入类型名，返回UGGeometry::Type
	//! \param strGeometryType 几何对象类型的名字
	//! \attention 严格区分大小写, 请参照"GeoPoint"的形式
	//! \return 成功返回几何对象类型, 失败返回0
	static UGint GetGeometryType(const UGString& strGeometryType);
	
	//! \brief 传入类型名，返回UGGeometry::Type
	//! \param strGeometryType 几何对象类型的名字
	//! \attention 严格区分大小写, 请参照"GeoPoint"的形式
	//! \return 成功返回几何对象类型, 失败返回0
	static UGString GetGeometryName(UGint nGeoType);

	//! \brief 加载一个指定文件名的动态库。
	//! \param strPathName [in]。
	//! \return 加载成功返回true。
	static UGbool LoadGeometryFactory(const UGString& strPathName);	

	//! \brief 卸载动态库。
	//! \param hHandle [in] 动态库句柄。
	//! \return 成功卸载返回true。
	static UGbool UnloadGeometryFactory(void* hHandle);	

	//! \brief 得到几何工厂定义。
	//! \param nIndex [in] 索引。
	//! \return 。
	static UGGeometryDefine* GetAt(UGint nIndex);

	// 添加 几何对象插件 用于静态加载 for Android by liucq
	UGbool AddGeometryProvider(void* handle);

	//! \brief 加载当前路径下所有GeometryFactory
	void LoadGeometryFactory();

private:
	//! \brief 检查动态库是否合法，如果合法则初始化变量
	//! \param hHandle [in] 动态库句柄。
	UGGeometryDefine* Check(void* hHandle);

	//! \brief 检查包含该几何类型的动态库是否已经加载
	//! \param nGeoType [in] 几何对象类型
	UGGeometryDefine* Find(UGint nGeoType);

	//! \brief 卸载已经加载的GeometryFactory
	void UnloadGeometryFactory();

private:
	UGArray<UGGeometryDefine*> m_geometryDefines;
	UGMutex m_mutex;
};

extern GEOMETRY_API UGGeometryManager g_geometryManager;

}
#endif // !defined(AFX_UGGEOFACTORY_H__78297849_2C23_469A_A6B2_24C8E982EE63__INCLUDED_)

