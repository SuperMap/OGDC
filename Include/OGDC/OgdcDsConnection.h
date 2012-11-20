//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | \ | | | __  | | \ | | |                       
//                   | |_/ | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file OgdcDsConnection.h
//!  \brief OgdcDsConnection 的接口定义。
//!  \details Ogdc关于 数据库/数据源 连接参数的接口定义。
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#ifndef OGDCDSCONNECTION_H 
#define OGDCDSCONNECTION_H

#include "Base/OgdcString.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace OGDC
{
//! \brief 数据引擎类型, >= 1001 为用户扩展。
enum OgdcEngineType
{	
	//! \brief 未知类型引擎
	oeNone			=-1,
	//! \brief Oracle空间数据引擎	
	oeOracle			=12,
	//! \brief SyBase空间数据引擎	
	oeSybase		    =15,
	//! \brief Micresoft SQL server空间数据引擎	
	oeSQLServer		=16,
	//! \brief 达梦空间数据引擎	
	oeDM				=17,
	//! \brief IMB DB2空间数据引擎	
	oeDB2             =18,
	//! \brief 人大金仓空间数据引擎	
	oeKingbase		=19,
	//! \brief 用以访问OGC WMS/WFS/WCS等服务的数据引擎	
	oeOGC				=23,
	//! \brief 跨平台文件引擎	
// 	oeFile			=30, 
	oeFile			= 219,		//引擎类型值与类库保持一致,对应UDB
	//! \brief MySQL空间数据引擎	
//	oeMySQL			=31,
	oeMySQL			=32,		//引擎类型值与类库保持一致
};

//! \brief 定义数据源连接信息
#define OgdcConnection OgdcDsConnection

//! \brief 数据库连接信息类
class OGDC_API OgdcConnection  
{
public:
	//! \brief 默认构造函数
	OgdcConnection();

	//! \brief 拷贝构造函数。
	//! \param conn 拷贝引用对象[in]。
	OgdcConnection(const OgdcConnection& conn);	

	//! \brief 默认析构函数。
#ifdef SYMBIAN60
	~OgdcConnection();
#else
	virtual ~OgdcConnection();
#endif
	

public:
	//! \brief 赋值数据库连接对象。
	//! \param &conn 现有连接对象值[in]。
	const OgdcConnection& operator =(const OgdcConnection &conn);	
	
	//! \brief 当前对象与目标对象进行比较是否完全相同。
	//! \param &conn 目标连接对象[in]。
	//! \return 是否相同，相同返回TRUE，不同返回FALSE。
	OgdcBool operator ==(const OgdcConnection &conn);	

	//! \brief 当前对象与目标对象进行比较是否不相同。
	//! \param &conn 目标连接对象[in]。
	//! \return 是否不同，不同返回TRUE，相同返回FALSE。
	OgdcBool operator !=(const OgdcConnection &conn);	

public:
	//! \brief 数据库类型。
	OgdcInt	m_nType;

	//! \brief 服务名。
	//! \remarks 例如：Oracle 数据库的实例名，SSQL Sever的服务器名。
	OgdcString	m_strServer;
	
	//! \brief 用户名。
	OgdcString	m_strUser;
	
	//! \brief 密码。
	OgdcString	m_strPassword;
	
	//! \brief 是否只读打开。
	OgdcBool	m_bReadOnly;

	//! \brief 是否以事务方式打开。
	OgdcBool	m_bTransacted;
	
	//! \brief 是否独占打开。
	OgdcBool	m_bExclusive;
	
	//! \brief 数据库是否需要密码打开。
	OgdcBool	m_bEncrypt;
	
	//! \brief 数据库名。
	OgdcString	m_strDatabase;
	
	//! \brief ODBC Driver。
	OgdcString	m_strDriver;

	//! \brief  MySQL中是否用命名管道中连接方式
	OgdcBool   m_bPipe;

	//! \brief MySQL中普通连接方式的端口
	OgdcInt    m_nPort;
	
	//! \brief 数据库别名。
	OgdcString	m_strAlias;
	
	//! \brief 数据库是否在打开工作空间时自动打开。
	OgdcBool	m_bAutoConnect;
	
	//! \brief 数据库连接池的最大连接数
	OgdcInt		m_nConnMaxNum;
	
	//! \brief 数据库连接池的最小连接数
	OgdcInt		m_nConnMinNum;
	
	//! \brief 数据库连接池的增长连接数
	OgdcInt		m_nConnIncNum;
	
	//! \brief 数据库是否扩展打开	
	OgdcBool	m_bExpandOpen;	
};
}
#endif // !defined(OGDCDSCONNECTION_H)

