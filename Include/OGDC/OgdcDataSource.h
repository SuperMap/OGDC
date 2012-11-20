//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)               
//                                                           
//!  \file OgdcDataSource.h
//!  \brief OgdcDataSource 的接口定义。
//!  \details Ogdc数据源的接口定义。
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#ifndef OGDCDATASOURCE_H
#define OGDCDATASOURCE_H

#include "OGDC/OgdcDsConnection.h"
#include "OGDC/OgdcDatasetVector.h"
#include "OGDC/OgdcDatasetRaster.h"
#include "OgdcMetaDatabase.h"

namespace OGDC
{

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//! \brief Ogdc数据源。
class OGDC_API OgdcDataSource  
{
public:
	//! \brief 默认构造函数
	OgdcDataSource();
	//! \brief 默认析构函数
	virtual ~OgdcDataSource();

// 纯虚函数
public:
	//! \brief 创建数据源,需要预先设置好连接信息。
	virtual OgdcBool Create() =0;
	
	//! \brief 打开数据源,需要预先设置好连接信息。
	virtual OgdcBool Open() =0;

	//! \brief 关闭数据源。
	virtual void Close() = 0;

	//! \brief 判断数据源是否已经打开。
	//! \return 打开返回TRUE，失败返回FALSE。
	virtual OgdcBool IsOpen() const = 0;

	//! \brief 获取数据集个数。
	//! \return 数据集个数。
	virtual OgdcInt GetDatasetCount() const = 0;

	//! \brief 通过序号获取数据集指针。
	//! \return 数据集指针。
	virtual OgdcDataset* GetDataset(OgdcInt nIndex) const = 0;

	//! \brief 获取指定名称的数据集指针。
	//! \param strDatasetName 数据集名[in]。
	//! \return 数据集指针。
	virtual OgdcDataset* GetDataset(const OgdcString& strDatasetName) const = 0;
	
	//! \brief 通过数据集名称删除数据集。
	//! \param strDatasetName 数据集名称[in]。
	//! \return 是否成功删除，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool DeleteDataset(const OgdcString& strDatasetName) =0;
	
	//! \brief 通过矢量数据集描述信息创建数据。
	//! \param vInfo 矢量数据集描述信息对象[in]。
	//! \return 矢量数据集指针。
	virtual OgdcDatasetVector* CreateDatasetVector(const OgdcDatasetVectorInfo& vInfo) =0;	
	
	//! \brief 通过栅格数据集描述信息创建数据集。
	//! \param rInfo 栅格数据集描述信息对象[in]。
	//! \param bIsMBRaster 是否以多波段方式创建数据集[in]。
	//! \return 栅格数据集指针。
	virtual OgdcDatasetRaster* CreateDatasetRaster(const OgdcDatasetRasterInfo& rInfo,OgdcBool bIsMBRaster = FALSE) =0;	

	//! \brief 通过现有数据集模板创建数据集。
	//! \param &strNewName 新数据集名[in]。
	//! \param pReferenceDataset 参考数据集指针，即模板创建数据集指针[in]。
	//! \return 新数据集指针。
	virtual OgdcDataset* CreateDatasetFrom(const OgdcString &strNewName, OgdcDataset* pReferenceDataset) = 0;

	//! \brief 复制数据集。
	//! \param pSrcDataset 源数据集指针[in]。
	//! \param strDestDatasetName 目的数据集名[in]。
	//! \param bBuildIndexOrPyramid 是否给栅格数据集建立金字塔，默认不建[in]。
	//! \return 目的数据集指针。
	virtual OgdcDataset* CopyDataset(OgdcDataset* pSrcDataset, 
		const OgdcString& strDestDatasetName, OgdcBool bBuildIndexOrPyramid = FALSE) = 0;

	//! \brief 保存数据信息。
	virtual OgdcBool SaveInfo() = 0;

public:	
	//! \brief 设置投影坐标系。
	//! \param coordSys [in]。
	virtual OgdcBool SetCoordSys(const OgdcCoordSys& coordSys);	

	//! \brief 得到投影坐标系。
	virtual OgdcBool GetCoordSys(OgdcCoordSys& coordSys);	

	//! \brief 执行SQL语句。
	//! \param strSQL SQL语句[in]。
	//! \return 是否成功执行，成功返回TRUE，失败返回FALSE。
	//! \remarks 主要用于在数据库级执行的数据操作。
	virtual OgdcBool Execute(const OgdcString& strSQL);	
	
	//! \brief 数据源是否和数据连接。
	virtual OgdcBool IsConnected();
	
	//! \brief 仅仅连接数据库,不刷新数据集等。
	virtual OgdcBool Connect();	
	
	//! \brief 获取最后更新时间。
	virtual OgdcTime GetDateLastUpdated();
	
	//! \brief 获取创建时间。
	virtual OgdcTime GetDateCreated();	
	
	//! \brief 修改密码。
	//! \param strNewPassword 新密码[in]。
	//! \return 修改成功返回TRUE，失败返回FALSE。
	virtual OgdcBool ChangePassword(const OgdcString& strNewPassword);
	
	//! \brief 获取引擎名称。
	//! \return 引擎名称。
	virtual OgdcString GetEngineName() const ;
	
	//! \brief 生成一个数据源中目前没有的数据集名字。
	//! \param strDatasetName 指定一个数据集名[in]。
	//! \return 根据指定名生成的加有唯一识别的数据集名。
	virtual OgdcString GetUnoccupiedDatasetName(const OgdcString& strDatasetName);	
	
	//! \brief 判断数据集名称是否合法。
	//! \param strDatasetName 数据集名[in]。
	//! \param nState 数据集或字段命名错误类型[out]。
	//! \return 名称是否合法，是返回TRUE，否则返回FALSE。
	virtual OgdcBool IsAvailableDatasetName(const OgdcString& strDatasetName);

	//! \brief 获取数据源版本号。
	//! \return 数据源版本号。
	virtual OgdcInt GetVersion();		
	
	//! \brief 开始事务。
	//! \return 是否成功开始事务，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool BeginTrans();
	
	//! \brief 提交事务。
	//! \return 是否成功提交事务，成功返回TRUE，失败返回FALSE。
	virtual void Commit();
	
	//! \brief 撤消事务。
	//! \return 是否成功撤消事务，成功返回TRUE，失败返回FALSE。
	virtual void Rollback();
	
	//! \brief 判断事务是否开始 
	virtual OgdcBool IsTransStarted() const ;

	//! \brief 判断数据源是否支持短事务操作
	//! \return 支持返回TRUE，不支持返回FALSE。
	virtual OgdcBool IsTransacted() const;
	
	//! \brief 获取密码。
	//! \remarks 通过获取连接信息也可以得到密码。
	virtual OgdcString GetPassword() const;

	//! \brief 获取数据源名称。
	virtual OgdcString GetName() const ;

	//! \brief 获取引擎类型
	virtual OgdcInt GetEngineType() const;

	//! \brief 获取引擎分类
	virtual  OgdcInt GetEngineClass() const;

	//! \brief 获取文件型引擎的后缀名
	virtual OgdcString GetEnginePostfix() const;

	//! \brief 判断数据源是否被修改。
	//! \return 被修改返回TRUE，否则返回FALSE。
	virtual OgdcBool IsModified();

	//! \brief 设置数据源是否被修改。
	//! \param bModified 数据源是否被修改布尔值，默认为修改[in]。
	virtual void SetModifiedFlag(OgdcBool bModified = TRUE);
	
	//! \brief 获取数据源的别名。
	//! \return 数据源的别名。
	virtual OgdcString GetAlias() const;

	//! \brief 设置数据源的别名
	//! \param strAlias 数据源的别名[in]。
	virtual void SetAlias(const OgdcString& strAlias);
	
	//! \brief 判断传入的字段名或表名是否可用。
	//! \param strFieldOrTableName 字段名或表名
	//! \return	可用返回TRUE，不可用返回FALSE。
	virtual OgdcBool IsValidFieldOrTableName(const OgdcString& strFieldOrTableName);
	
	//! \brief 设置数据源描述信息。
	//! \param strDescription 数据源描述信息[in]。
	virtual void SetDescription(OgdcString strDescription);
	
	//! \brief 获取数据源描述信息。
	//! \return 数据源描述信息。
	virtual OgdcString GetDescription();

	//! \brief 删除当前的数据源(会自动删除所有数据集，最后删除所有系统表和系统对象)
	//! \remarks 调用此方法后，可以再次创建同参数的数据源
	//! \attention 仅数据库类型的数据源需要实现此方法，文件型数据源不处理
	virtual OgdcBool DropDataSource();

	//! \brief 初始化元数据库类
	//! \return 元数据库类的指针
	virtual OgdcMetaDatabase * InitMetaDatabase();

	//! \brief 获取元数据库类
	//! \return 元数据库类的指针
	//! \remarks 若在调用此函数之前没有调用InitMetaDatabase，则返回为NULL
	virtual OgdcMetaDatabase * GetMetaDatabase();

	//! \brief 设置元数据UUID
	//! \param strMetaUUID 元数据UUID
	//! \return 成功返回TRUE
	virtual OgdcBool SetMetaUUID(const OgdcString strMetaUUID);

	//! \brief 获取元数据UUID
	//! \return 返回元数据UUID，失败返回""
	virtual OgdcString GetMetaUUID();

	//! \brief 通过元数据的 UUID 获取数据集
	//! \param strMetaUUID[in] 元数据的 UUID
	//! \return 返回的数据集指针，如果该数据源不存在符合条件的数据集，则返回NULL
	virtual OgdcDataset* FindDatasetByMetaData(OgdcString strMetaUUID);

public:
	//! \brief 数据源连接信息
	OgdcDsConnection m_connection;

	//! \brief 引擎分类，1.网络数据库，2.本地文件型，3.Web服务型。
	OgdcInt m_nEngineClass;

	//! \brief 2本地文件型数据源的文件后缀名。
	OgdcString m_strPostfix;
};

}
#endif // !defined(OGDCDATASOURCE_H)

