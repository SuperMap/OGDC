//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file OgdcMetaDatabase.h
//!  \brief OgdcMetaDatabase类的接口定义
//!  \details MetaDatabase为元数据库
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCMETADATABASE_H__2CD5B324_0654_4083_A783_CFA11A788A15__INCLUDED_)
#define AFX_OGDCMETADATABASE_H__2CD5B324_0654_4083_A783_CFA11A788A15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base/ogdcdefs.h"
#include "Base/OgdcString.h"
#include "OGDC/OgdcDsConnection.h"
#include "OGDC/OgdcMetaData.h"

namespace OGDC
{

class OgdcDataSource;

// OgdcMetaQueryDefs 支持的查询模式：(标签名 条件操作符 标签值) 连接 (标签名 条件操作符 标签值) ...
// 不支持 基于标签名 经数学运算后的条件，如 标签A+标签B > 1000
// 范例1: 查询: Left>1000 and Right<2000 构造 OgdcMetaQueryDefs 如下
// OgdcMetaQueryDef def1, def2;
// OgdcMetaQueryDefs defs;
// def1.strTag = "Left";
// def1.strValue = "1000";
// def1.strType = "number";
// def1.strOperator = ">";
// def1.strCondjuction = "and";
// def2.strTag = "Right";
// def2.strValue = "2000";
// def2.strType = "number";
// def2.strOperator = "<";
// def2.strCondjuction = "";
// defs.Add(def1);
// defs.Add(def2);

// 范例2: 模糊查询: %单位 like '%SuperMap%' or %单位% = 'IGSNRR', 构造 OgdcMetaQueryDefs 如下
// OgdcMetaQueryDef def1, def2;
// OgdcMetaQueryDefs defs;
// def1.strTag = "%单位";
// def1.strValue = "%SuperMap%";
// def1.strType = "string";
// def1.strOperator = "like";
// def1.strCondjuction = "or";
// def2.strTag = "%单位%";
// def2.strValue = "IGSNRR";
// def2.strType = "string";
// def2.strOperator = "=";
// def2.strCondjuction = "";
// defs.Add(def1);
// defs.Add(def2);

//! \brief 元数据查询定义结构体。
struct OgdcMetaQueryDef
{
	//! \brief 标签名
	OgdcString strTag;

	//! \brief 标签类型,"string" 或者 "number"
	OgdcString strType;

	//! \brief 标签值
	OgdcString strValue;

	//! \brief 操作符
	OgdcString strOperator;

	//! \brief 与下一个 OgdcMetaQueryDef 的连接条件,"and"或者"or"
	OgdcString strCondjuction;
};

//! \brief 可以由多个 OgdcMetaQueryDef 进行多标签的组合查询
typedef OgdcArray<OgdcMetaQueryDef> OgdcMetaQueryDefs; 

//! \brief Ogdc元数据库类。
class OGDC_API OgdcMetaDatabase  
{
public:
	//! \brief 默认构造函数
	OgdcMetaDatabase();
	//! \brief 默认析构函数
	virtual ~OgdcMetaDatabase();

	//! \brief 创建一个新的元数据库
	//! \return 创建成功返回TRUE，失败则返回FALSE
	virtual OgdcBool Create() = 0;
	
	//! \brief 打开元数据库。
	//! \return 连接成功，返回TRUE, 否则返回FALSE。
	virtual OgdcBool Open() = 0;
	
	//! \brief 关闭元数据库，断开与数据库的连接。
	//! \return 关闭成功，返回TRUE, 否则返回FALSE。
	virtual OgdcBool Close() = 0;

	//! \brief 连接到元数据库。
	//! \return 连接成功，返回TRUE, 否则返回FALSE。
	virtual OgdcBool Connect();	

	//! \brief 数据源是否和数据连接。
	virtual OgdcBool IsConnected() const;
	
	//! \brief	导入元数据
	//! \param  strFilePath [in]元数据文件路径
	//! \return	OgdcString 成功返回导入后的元数据UUID,失败为""
	virtual OgdcString ImportMetaData(const OgdcString &strFilePath) = 0;

	//! \brief 获取指定的Metadata对象。
	//! \return 获取成功，返回TRUE, 否则返回FALSE。。
	//! \remarks 若指定的元数据不存在，返回FALSE，不改写metadata。

	//! \brief	导出元数据
	//! \param &strPath [in] 导出的文件路径
	//! \param &strMetadataUUID [in]　元数据的UUID。
	//! \return	OgdcBool 成功返回TRUE
	virtual OgdcBool ExportMetaData(const OgdcString& strPath, const OgdcString &strMetaUUID) = 0;

	//! \brief 获取指定的Metadata对象。
	//! \param &strMetadataUUID [in]　元数据的UUID。
	//! \param metadata [in/out] 元数据对象(引用型传入)。
	//! \return 获取成功，返回TRUE, 否则返回FALSE。。
	//! \remarks 若指定的元数据不存在，返回FALSE，不改写metadata。
	virtual OgdcBool GetMetadata(const OgdcString &strMetadataUUID, OgdcMetadata & metadata) = 0;

	//! \brief 移除指定的元数据。
	//! \param strMetadataUUID [in] 要删除的元数据的UUIDID。
	//! \return 成功，返回TRUE, 否则返回FALSE。
	//! \remarks 若指定的元数据不存在，则返回FALSE。
	virtual OgdcBool DropMetadata(const OgdcString &strMetadataUUID) = 0;

	//! \brief 查询满足条件的元数据对象的ID。
	//! \param &querydef [in] 组合查询条件。
	//! \param &aryMetadataUUIDs [out] 得到的元数据库中所有的元数据的UUID数组
	//! \return 获取成功，返回TRUE, 否则返回FALSE。
	//! \remarks aryMetadataUUIDs中已有的数据将被清空
	virtual OgdcBool Query(const OgdcMetaQueryDefs &querydefs, OgdcStringArray &aryMetadataUUIDs) = 0;

	//! \brief 获取元数据库中所有的元数据的UUID。
	//! \param &aryMetadataUUIDs [out] 得到的元数据库中所有的元数据的UUID数组
	//! \return 获取成功，返回TRUE, 否则返回FALSE。
	//! \remarks aryMetadataUUIDs 中已有的数据将被清空
	virtual OgdcBool FetchMetadataIDs(OgdcStringArray &aryMetadataUUIDs);

public:
	//! \brief 元数据库连接信息
	OgdcConnection m_connection;
};

}

#endif // !defined(AFX_OGDCMETADATABASE_H__2CD5B324_0654_4083_A783_CFA11A788A15__INCLUDED_)

