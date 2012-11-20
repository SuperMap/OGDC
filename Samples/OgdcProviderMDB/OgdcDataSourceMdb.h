//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)           
//                                                           
//!  \file OgdcDataSourceMdb.h
//!  \brief 数据源的接口定义。
//!  \details 数据源各种操作的接口
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Limited Company.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCDATASOURCEMDB_H__F613A905_BFAD_4DA1_B1D0_1E00090CB62A__INCLUDED_)
#define AFX_OGDCDATASOURCEMDB_H__F613A905_BFAD_4DA1_B1D0_1E00090CB62A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OGDC/OgdcDataSource.h"
#include "afxdao.h"

namespace OGDC
{

//! \brief OgdcMdb数据源的接口。
class OGDC_API OgdcDataSourceMdb : public OgdcDataSource  
{
public:
	//! \brief 数据源默认构造函数
	OgdcDataSourceMdb();
	//! \brief 数据源默认析构函数
	virtual ~OgdcDataSourceMdb();

	// 纯虚函数
public:
	//! \brief 创建数据源,需要预先设置好连接信息。
	virtual OgdcBool Create();
	
	//! \brief 打开数据源,需要预先设置好连接信息。
	virtual OgdcBool Open();
	
	//! \brief 关闭数据源。
	virtual void Close();
	
	//! \brief 判断数据源是否已经打开。
	//! \return 打开返回TRUE，失败返回FALSE。
	virtual OgdcBool IsOpen() const;
	
	//! \brief 获取数据集个数。
	//! \return 数据集个数。
	virtual OgdcInt GetDatasetCount() const;
	
	//! \brief 通过序号获取数据集指针。
	//! \return 数据集指针。
	virtual OgdcDataset* GetDataset(OgdcInt nIndex) const;
	
	//! \brief 获取指定名称的数据集指针。
	//! \param strDatasetName 数据集名[in]。
	//! \return 数据集指针。
	virtual OgdcDataset* GetDataset(const OgdcString& strDatasetName) const;
	
	//! \brief 通过数据集名称删除数据集。
	//! \param strDatasetName 数据集名称[in]。
	//! \return 是否成功删除，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool DeleteDataset(const OgdcString& strDatasetName);
	
	//! \brief 通过矢量数据集描述信息创建数据。
	//! \param vInfo 矢量数据集描述信息对象[in]。
	//! \return 矢量数据集指针。
	virtual OgdcDatasetVector* CreateDatasetVector(const OgdcDatasetVectorInfo& vInfo);	
	
	//! \brief 通过栅格数据集描述信息创建数据集。
	//! \param rInfo 栅格数据集描述信息对象[in]。
	//! \param bIsMBRaster 是否以多波段方式创建数据集[in]。
	//! \return 栅格数据集指针。
	virtual OgdcDatasetRaster* CreateDatasetRaster(const OgdcDatasetRasterInfo& rInfo,OgdcBool bIsMBRaster = FALSE);	
	
	//! \brief 通过现有数据集模板创建数据集。
	//! \param strNewName 新数据集名[in]。
	//! \param pReferenceDataset 参考数据集指针，即模板创建数据集指针[in]。
	//! \return 新数据集指针。
	virtual OgdcDataset* CreateDatasetFrom(const OgdcString &strNewName, OgdcDataset* pReferenceDataset);
	
	//! \brief 复制数据集。
	//! \param pSrcDataset 源数据集指针[in]。
	//! \param strDestDatasetName 目的数据集名[in]。
	//! \param bBuildIndexOrPyramid 是否给栅格数据集建立金字塔，默认不建[in]。
	//! \return 目的数据集指针。
	virtual OgdcDataset* CopyDataset(OgdcDataset* pSrcDataset, 
		const OgdcString& strDestDatasetName, OgdcBool bBuildIndexOrPyramid = FALSE);
	
	//! \brief 保存数据信息。
	virtual OgdcBool SaveInfo();

public:	
	//! \brief 获取数据源名称。
	//! \return 返回数据源名称。
	virtual OgdcString GetName() const ;	

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
	
	//! \brief 判断数据源是否被修改。
	//! \return 被修改返回TRUE，否则返回FALSE。
	virtual OgdcBool IsModified();

	//! \brief 设置数据源是否被修改。
	//! \param bModified 数据源是否被修改布尔值，默认为修改[in]。
	virtual void SetModifiedFlag(OgdcBool bModified = TRUE);
		
	//! \brief 判断传入的字段名或表名是否可用。
	//! \param 字段名或表名是否可用。
	//! \param nState 数据集或属性字段命名错误类型。
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
	
	//! \brief 获取数据源信息
	OgdcBool ReadInfo();

public:
	//! \brief Micorsoft Access MDB 数据库的 Database，相当于 SuperMap 中的数据源
	CDaoDatabase m_daoDatabase; 
	
protected:
	//! \brief 数据集指针数组，存每一个指向数据集的指针
	OgdcArray <OgdcDataset*> m_datasets;

	//! \brief 数据源版本
	OgdcLong m_nVersion;

	//! \brief 一个指向工作空间的指针，Workspace for access
	CDaoWorkspace* m_pDaoWorkspace;	
		
	//! \brief 数据源是否打开的标志
	OgdcBool m_bOpened;
	
	//! \brief 事务是否启动
	OgdcBool m_bTransStarted;
	
	//! \brief 是否被修改
	OgdcBool m_bModified;
		
	//! \brief 数据源的描述
	OgdcString	m_strDescription;
	
	//! \brief 坐标系信息
	OgdcCoordSys m_coordSys;
	
};

extern "C" OGDC_API OgdcDataSource* CreateOgdcDataSource();
extern "C" OGDC_API OgdcFeature* CreateOgdcFeature();
}
// 
#endif // !defined(AFX_OGDCDATASOURCEMDB_H__F613A905_BFAD_4DA1_B1D0_1E00090CB62A__INCLUDED_)
