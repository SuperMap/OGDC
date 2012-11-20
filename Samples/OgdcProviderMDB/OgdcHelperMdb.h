//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)               
//                                                           
//!  \file OgdcHelperMdb.h
//!  \brief OGDC辅助函数库的接口定义。
//!  \details 含有一些OGDC数据类型与其他数据类型的转换
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Limited Company.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCHELPERMDB_H__A37C8A53_6D17_4F8E_9CD0_C85821085D7D__INCLUDED_)
#define AFX_OGDCHELPERMDB_H__A37C8A53_6D17_4F8E_9CD0_C85821085D7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OGDC/OgdcDataSource.h"
#include "../../Include/Base/OgdcDict.h"
#include "afxdao.h"

namespace OGDC
{
	
//! \brief OgdcProviderMdb的帮助类
//! \remarks 提供了OGDC与MFC，DAO类型数据的转化和其他帮助等功能
class OGDC_API OgdcHelperMdb  
{
public:
	//! \brief 辅助接口类默认构造函数
	OgdcHelperMdb();	
	//! \brief 辅助接口类默认析构函数
	virtual ~OgdcHelperMdb();
	
public:
	//! \brief 将OGDC变体转化为OLE变体。
	//! \param ogdcVariant 源值[in]。
	//! \param coleVariant 目标值[out]。
	static void OgdcVariant2COleVariant(const OgdcVariant& ogdcVariant, COleVariant& coleVariant);
	
	//! \brief 将OLE变体转化为OGDC变体。。
	//! \param coleVariant 源值[in]。
	//! \param ogdcVariant 目标值[in]。
	static void COleVariant2OgdcVariant(const COleVariant& coleVariant, OgdcVariant& ogdcVariant);
	
	//! \brief 将OgdcFieldInfo类型转化成CDaoFieldInfo类型。
	//! \param fieldInfo 源值[in]。
	//! \param daoFieldInfo 目标值[in]。
	//! \remarks DAO进行数据库操作的时候要用CDaoFieldInfo类型。
	static void OgdcFieldInfo2CDaoFieldInfo(const OgdcFieldInfo& fieldInfo, CDaoFieldInfo& daoFieldInfo);
	
	//! \brief 将CDaoFieldInfo类型转化成OgdcFieldInfo类型。
	//! \param fieldInfo 源值[in]。
	//! \param daoFieldInfo 目标值[in]。
	//! \remarks DAO进行数据库操作的时候要用CDaoFieldInfo类型。
	static void CDaoFieldInfo2OgdcFieldInfo(const CDaoFieldInfo& daoFieldInfo, OgdcFieldInfo& fieldInfo);	
	
	//! \brief 判断strName是否为系统字段。
	//! \param strName [in]。
	//! \remarks 在这里，系统字段统一由OG开头。
	static OgdcBool IsSystemField(const OgdcString& strName);	
	
	//! \brief 判断strName是否为Access的关键字。
	//! \param strName [in]。
	//! \remarks Access关键字在第一次调用时初始化。
	static OgdcBool IsKeyWord(const OgdcString& strName);
	
	//! \brief 初始化Access的关键字
	//! \remarks 加载Access关键字到字典里
	static void InitKeyWords();

	//! \brief 设置字段标识
	static void SetFieldSign(OgdcFieldInfo& fieldInfo);

	static OgdcBool IsOGID(OgdcString strName);

	static OgdcBool IsGeoField(OgdcString strName);
	
private:
	
	//! \brief Access关键字的字典是否初始化的标志
	static OgdcBool m_bInited;
};


}
#endif 
