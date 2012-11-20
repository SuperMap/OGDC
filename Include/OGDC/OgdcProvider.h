//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)              
//                                                           
//!  \file OgdcProvider.h
//!  \brief OgdcProvider 的接口定义。
//!  \details Ogdc引擎插件定义类。
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#ifndef OGDCPROVIDER_H
#define OGDCPROVIDER_H

#if !defined SYMBIAN60 && !defined OS_ANDROID && !defined IOS
#include "OgdcDataSource.h"
#else
#include "Engine/UGDataSource.h"
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace OGDC
{

//! \brief Ogdc引擎插件定义类。
class OGDC_API OgdcProvider  
{
public:
	//! \brief 默认构造函数
	OgdcProvider();
	//! \brief 拷贝构造函数
	OgdcProvider(const OgdcProvider& provider);
	//! \brief 默认析构函数
#ifdef SYMBIAN60
	~OgdcProvider();
#else
	virtual ~OgdcProvider();
#endif

public:
	//! \brief 重载操作符 =
	const OgdcProvider& operator=(const OgdcProvider& provider);
	//! \brief 重载操作符 ==
	OgdcBool operator ==(const OgdcProvider& provider);
	//! \brief 重载操作符 !=
	OgdcBool operator !=(const OgdcProvider& provider);

	//! \brief 	创建数据源实例。
	//! \return new出来的数据源指针，如果创建不成功就返回NULL。
	//! \remarks 本函数并不会真正创建一个数据源，不会生成数据库，
	//!	仅仅根据指定的数据源类型创建一个UGDataSource实例，并返回其指针。
#if !defined SYMBIAN60 && !defined OS_ANDROID && !defined IOS
	OgdcDataSource* CreateOgdcDataSource();
	
	//! \brief 创建特征要素Feature。
	//! \return new出来的特征要素指针，如果创建不成功就返回NULL。
	OgdcFeature* CreateOgdcFeature();
#else
	UGC::UGDataSource* CreateDataSource();
#endif

	//! \brief 获取文件型引擎的后缀名
	OgdcString GetPostfix();

	//! \brief 获取引擎的分类
	OgdcInt GetEngineClass();
public:
	//! \brief 引擎类型。
	OgdcInt m_nType;

	//! \brief 引擎名称。
	OgdcString m_strName;

	//! \brief 引擎分类，1网络数据库，2本地文件型，3网络服务型。
	OgdcInt m_nEngineClass;

	//! \brief 2本地文件型数据源的文件后缀名。
	OgdcString m_strPostfix;

	//! \brief 引擎的指针。
	void* m_hHandle;
};

}
#endif // !defined(OGDCPROVIDER_H)

