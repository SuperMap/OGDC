//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file OgdcProviderManager.h
//!  \brief OgdcProviderManager 的接口定义。
//!  \details Ogdc引擎动态加载管理器的接口定义。
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////
#ifndef OGDCPROVIDERMANAGER_H
#define OGDCPROVIDERMANAGER_H

#include "OGDC/OgdcProvider.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace  OGDC
{


//! \brief BeforeLoadSDX 事件回调函数
typedef void (OGDCSTDCALL *BeforeLoadSDXProc)(OgdcInt nType,OgdcBool& bCancel);


//! \brief Ogdc引擎动态加载管理器。
class OGDC_API OgdcProviderManager  
{
public:
	//! \brief 默认构造函数
	OgdcProviderManager();
	//! \brief 默认析构函数
#ifdef SYMBIAN60
	~OgdcProviderManager();
#else
	virtual ~OgdcProviderManager();
#endif

public:
	//! \brief 	创建数据源实例。
	//! \param 	nEngineType 数据引擎类型，可以是用户扩展的引擎类型。
	//! \return new出来的数据源指针，如果创建不成功就返回NULL。
	//! \remarks 本函数并不会真正创建一个数据源，不会生成数据库，
	//!	仅仅根据指定的数据源类型创建一个UGDataSource实例，并返回其指针。
#if !defined SYMBIAN60 && !defined OS_ANDROID && !defined IOS
	static OgdcDataSource* CreateOgdcDataSource(OgdcInt nEngineType);

	//! \brief 	创建Feature实例。
	//! \param 	nEngineType 数据引擎类型，可以是用户扩展的引擎类型。
	//! \return new出来的Feature指针，如果创建不成功就返回NULL。
	static OgdcFeature* CreateOgdcFeature(OgdcInt nEngineType);
#endif 

	//! \brief 得到引擎插件数目。
	static OgdcInt GetProviderCount();	

	//! \brief 得到引擎插件定义。
	//! \param nIndex [in] 索引。
	static OgdcProvider GetAt(OgdcInt nIndex);	

	//! \brief 获取引擎插件名称
	//! \param nType 传入的引擎类型
	static OgdcString GetProviderName(OgdcInt nEngineType);	

	//! \brief 动态加载所有后缀名为sdx、odx的OGDC引擎插件动态库。
	static void LoadAllProvider();

	//! \brief 卸载所加载的动态库
	static void UnloadAllProvider();

	//! \brief 加载一个后缀名为sdx指定文件名的OGDC引擎插件动态库。
	//! \param strPathName [in]。
	//! \return 加载成功返回true。
	static OgdcBool LoadProvider(const OgdcString& strPathName);	

	//! \brief 卸载后缀名为sdx的OGDC引擎插件动态库。
	//! \param hHandle [in] 动态库句柄。
	//! \return 成功卸载返回true。
	static OgdcBool UnloadProvider(void* hHandle);	

	//! \brief 设置回调函数
	static void SetBeforeLoadSDXCallBack(BeforeLoadSDXProc pBeforeLoadSDXCallBack);

public:
	//! \brief 根据数据源引擎类型查找一个引擎插件定义。
	//! \param nEngineType 数据源引擎类型[in]。
	//! \param provider 引擎插件定义[in]。
	//! \return 如果找到就返回true，否则就返回false。
	OgdcBool FindProvider(OgdcInt nEngineType, OgdcProvider& provider);		

protected:

	//! \brief 动态加载所有后缀名为sdx、odx的OGDC引擎插件动态库。
	void LoadProvider();

	//! \brief 释放动态加载的OGDC数据引擎动态库。
	void UnloadProvider();
	
	//! \brief 判断OGDC数据引擎插件句柄是否有效，如果有效就构造一个引擎插件定义。
	//! \param hHandle 引擎插件句柄[in]。
	//! \param provider 引擎插件定义[in]。
	//! \return 引擎插件句柄有效就返回true，否则就返回false。。
	OgdcBool CheckProvider(void* hHandle, OgdcProvider& provider);	
	
public:
	//! \brief 动态加载的OGDC引擎插件数组。
	OgdcArray<OgdcProvider> m_providers;

	//! \brief 回调函数标识
	static BeforeLoadSDXProc ms_BeforeLoadSDXCallBack;
};

extern OGDC_API OgdcProviderManager g_providerManager;

}
#endif // !defined(OGDCPROVIDERMANAGER_H)

