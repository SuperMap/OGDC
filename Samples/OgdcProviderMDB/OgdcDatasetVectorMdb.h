//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)          
//                                                           
//!  \file OgdcDatasetVectorMdb.h
//!  \brief 矢量数据集的接口定义。
//!  \details 矢量数据集的各种接口
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Limited Company.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCDATASETVECTORMDB_H__681E6C4F_B683_41E1_A7AF_B91345DAB853__INCLUDED_)
#define AFX_OGDCDATASETVECTORMDB_H__681E6C4F_B683_41E1_A7AF_B91345DAB853__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OGDC/OgdcDatasetVector.h"
#include "OgdcDataSourceMdb.h"

namespace OGDC
{

//! \brief 矢量数据集实现的类
//! \remarks 为常用的点，线，面，文本，属性数据集
class OGDC_API OgdcDatasetVectorMdb : public OgdcDatasetVector  
{
	//! \brief 声明友元类
	friend class OgdcDataSourceMdb;
	friend class OgdcRecordsetMdb;	 
	
public:
	//! \brief 矢量数据集默认构造函数
	OgdcDatasetVectorMdb();

	//! \brief 带指向数据源指针的构造函数
	//! \param pDataSource 指向数据源的指针[in]。
	OgdcDatasetVectorMdb(OgdcDataSourceMdb *pDataSource);

	//! \brief 矢量数据集默认析构函数
	virtual ~OgdcDatasetVectorMdb();

	// 纯虚函数	
public:
	//! \brief 判断数据集是否已经打开。
	//! \return 打开返回TRUE，否则返回FALSE。
	virtual OgdcBool IsOpen() const ;
	
	//! \brief 打开数据集。
	//! \return 是否成功打开数据集，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool Open() ;
	
	//! \brief 关闭数据集。
	//! \return 是否成功关闭数据集，成功返回TRUE，失败返回FALSE。
	//! \brief 关闭数据集。
	//! \return 是否成功关闭数据集，成功返回TRUE，失败返回FALSE。
	virtual void Close() ;	

	//! \brief 重命名数据集。
	//! \param strNewName 新的数据集名[in]。
	//! \return 是否成功重命名数据集，成功返回TRUE，失败返回FALSE。
	//! \remarks 只改变数据集名,表名不变。
	virtual OgdcBool Rename(const OgdcString& strNewName) ;

	//! \brief 判断当前数据集是否可编辑。
	//! \return 可编辑返回TRUE，不可返回FALSE。
	virtual OgdcBool CanUpdate() ;

	//! \brief 将数据集各种信息保存到数据库中。
	//! \return 是否成功保存，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool SaveInfo() ;

	//! \brief 刷新数据集信息。
	//! \return 是否成功刷新，成功返回TRUE，失败返回FALSE。
	//! \remarks 从数据库中读取数据集的各种属性，刷新当前数据集的信息。
	virtual OgdcBool RefreshInfo() ;	

	//! \brief 获取数据集中对象个数。
	//! \param  [in]。
	//! \return 对象个数。
	//! \remarks 速度慢,但值比较正确。
	//! \attention 通过Register表获取。
	virtual OgdcInt GetObjectCount() ;

	//! \brief 返回记录集个数。
	//! \return 记录集个数。
	virtual OgdcInt GetRecordsetCount() ;	
	
	//! \brief 通过记录集指针释放记录集内存空间。
	//! \param pRecordset 记录集指针[in]。
	//! \return 是否释放成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool ReleaseRecordset(OgdcRecordset *pRecordset) ;	

	//! \brief 释放所有记录集
	virtual void ReleaseAllRecordsets() ;	

	//! \brief 取出数据集中某行特征要素。
	//! \param nID 数据集记录ID号[in]。
	//! \return 行特征要素指针。
	virtual OgdcFeature* GetFeature(OgdcInt nID) ;	
	virtual UGC::UGFeature* GetFeatureEx(OgdcInt nID);

	//! \brief 将特征要素更新到数据集中。
	//! \param pFeature 特征要素指针[in]。
	//! \return 是否保存成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool UpdateFeature(const OgdcFeature* pFeature) ;
	virtual OgdcBool UpdateFeature(const UGC::UGFeature* pFeature);
	
	//! \brief 将特征要素追加到数据集未尾。
	//! \param pFeature 特征要素指针[in]。
	//! \return 是否追加成功，成功返回TRUE，失败返回FALSE。
	//! \remarks 追加时特征要素关联的数据集记录ID将不起作用。
	virtual OgdcBool AddFeature(const OgdcFeature* pFeature) ;	
	virtual OgdcBool AddFeature(const UGC::UGFeature* pFeature);

	//! \brief 将特征要素从数据集中删除。
	//! \param nID 数据集记录ID号[in]。
	//! \return 是否删除成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool DeleteFeature(OgdcInt nID) ;	
	
	//! \brief 通过ID数组删除数据集中的记录。
	//! \param pIDs ID数组指针[in]。
	//! \param nCount ID数组元素个数[in]。
	//! \return 成功删除返回TRUE，失败返回FALSE。
	virtual OgdcBool DeleteFeatures(const OgdcInt* pIDs, OgdcInt nCount) ;	
	
	//! \brief 删除表中所有数据。
	//! \return 是否删除成功，成功返回TRUE，失败返回FALSE。
	//! \remarks 清空表后表中的字段仍然保留。
	virtual OgdcBool Truncate() ;	

	//! \brief 根据当前索引状态重建索引
	//! \return 是否重建成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool BuildSpatialIndex() ;
	
	//! \brief 清除空间索引,为没有索引的状态
	//! \return 是否清除成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool DropSpatialIndex() ;
	
	//! \brief 是否需要重建空间索引
	//! \return 需要重建返回TRUE，不需要返回FALSE。
	//! \remarks 规定:如果没有索引返回FALSE,有索引看索引的情况
	virtual OgdcBool IsSpatialIndexDirty() ;

	//! \brief 查询通用入口，得到记录集。
	virtual OgdcRecordset* Query(const OgdcQueryDef& querydef) ;
	
	//! \brief 通过表达式更新字段值。
	//! \param strFieldName 更新的字段名[in]。
	//! \param strExpress 算术运算表达式字符串，
	//! 字段值是某个算术运算表达式的值，如strFieldName = Col1 + col2 * 2[in]。
	//! \param strFilter 更新字段时对数据行的过滤条件，
	//! 默认过滤条件置空，更新所有记录[in]。
	//! \return 是否更新字段值成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool UpdateField(
		const OgdcString& strFieldName,
		const OgdcString& strExpress,
		const OgdcString& strFilter = _U("")) ;	
	
	//! \brief 复制字段值,strSrcField和。
	//! \param strSrcFieldName 源字段名称[in]。
	//! \param strDestFieldName 目的字段名称[in]。
	//! \return 是否复制字段成功，成功返回TRUE，失败返回FALSE。
	//! \attention 目的字段名必须都存在且目的字段要与源字段类型匹配。
	virtual OgdcBool CopyField(
		const OgdcString& strSrcFieldName,
		const OgdcString& strDestFieldName) ;	
	
	//! \brief 创建普通索引。
	//! \param strFieldNameList 创建索引的字段列表，多个字段中间用","隔开[in]。
	//! \param strIndexName 索引名[in]。
	//! \return 创建普通索引是否成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool CreateFieldIndex(
		const OgdcString& strFieldNameList,
		const OgdcString& strIndexName) ;	

	//! \brief 删除索引。
	//! \param strIndexName 索引名[in]。
	//! \return 删除索引是否成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool DropFieldIndex(const OgdcString& strIndexName) ;	
	
	//! \brief 创建多个字段。
	//! \param fieldInfos 字段描述信息数组引用[in]。
	//! \return 创建多个字段是否成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool CreateFields(const OgdcFieldInfos& fieldInfos) ;	
		
	//! \brief 通过字段名称删除字段。
	//! \param strFieldName 字段名[in]。
	//! \return 是否删除成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool DeleteField(const OgdcString& strFieldName) ;
	
	//! \brief 获取数据表中所有字段的描述信息。
	//! \param fieldInfos 数据表中所有字段的描述信息数组[out]。
	//! \param bSystemFieldExcluded 是否排除系统字段，默认不排除[in]。
	//! \return 是否获取成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool GetFieldInfos(
		OgdcFieldInfos& fieldInfos,
		OgdcBool bSystemFieldExcluded = FALSE) ;	
	
	//! \brief 通过字段名获取字段描述信息。
	//! \param strFieldName 字段名[in]。
	//! \param fieldInfo 字段描述信息[out]。
	//! \return 是否获取成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool GetFieldInfo(
		const OgdcString& strFieldName,
		OgdcFieldInfo& fieldInfo) ;
	
	//! \brief 通过字段名设置字段信息。
	//! \param strFieldName 字段名[in]。
	//! \param fieldInfo 字段描述信息对象引用[in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool SetFieldInfo(
		const OgdcString& strFieldName,
		const OgdcFieldInfo& fieldInfo) ;	
	
	//! \brief 统计字段值。
	//! \param strFieldName 字段名[in]。
	//! \param nMode 统计模式[in]。
	//! \return 统计值。
	//! \remarks 能统计的字段类型为短整型，整型，长整型，单，双精度浮点数
	virtual OgdcVariant Statistic(
		const OgdcString& strFieldName,
		OgdcRecordset::OgdcStatisticMode nMode) ;	

	//! \brief 返回数据源指针
	virtual OgdcDataSource* GetDataSource() const;

public:
		
	//! \brief 判断数据集是否是栅格数据集
	//! \return 是栅格数据集返回TRUE，不是返回FALSE。
	virtual OgdcBool IsRaster();

	//! \brief 获取数据集的地理范围。
	//! \return 数据集的地理范围。
	virtual OgdcRect2D GetBounds();
			
	//! \brief 得到未被占用的字段名。
	//! \param strFieldName 指定的一个字段名[in]。
	//! \return 未被占用的字段名。
	virtual OgdcString GetUnoccupiedFieldName(const OgdcString& strFieldName);

	//! \brief 判断字段名是否有效。
	//! \param strFieldName 字段名[in]。
	//! \return 有效返回TRUE，无效返回FALSE。
	virtual OgdcBool IsAvailableFieldName(const OgdcString& strFieldName);

	//! \brief 判断数据集是否在打开后有过修改。
	//! \return 有过修改返回TRUE，没有返回FALSE。
	virtual OgdcBool IsModified() const;

	//! \brief 设置数据集是否有过修改。
	//! \param bModified 数据集是否有过修改，默认有修改[in]。
	virtual void SetModifiedFlag(OgdcBool bModified = TRUE);

	//! \brief 设置投影坐标系。
	//! \brief 该Provider仅支持内存中设置投影信息，暂未保存。
	//! \param coordSys [in]。
	virtual OgdcBool SetCoordSys(const OgdcCoordSys& coordSys);	

	//! \brief 得到投影坐标系。
	virtual OgdcBool GetCoordSys(OgdcCoordSys& coordSys) const;			

	//! \brief 矢量数据集能力描述：Provider本身是否支持相关查询
	virtual OgdcBool IsSupportQueryType(OgdcQueryDef::QueryType queryType) const;

	//! \brief 创建子数据集
	OgdcBool CreateSubDatasetVector(OgdcDatasetVectorInfo& vInfo);

protected:

	//! \brief 指向数据源的指针
	OgdcDataSourceMdb* m_pDataSource;

	//! \brief 矢量数据集中对象的个数
	OgdcInt m_nObjectCount;

	//! \brief CDaoTableDef指针
	//! \remarks 通过CDaoTableDef通用入口进行数据库操作
	CDaoTableDef* m_pDaoTableDef;	

	//! \brief 记录集ID
	OgdcInt m_nID;

	//! \brief OgdcFieldInfos类型结构体
	//! \remarks 存每个OgdcFieldInfo类型，保存字段信息
	OgdcFieldInfos m_fieldInfos;
	
	//! \brief 是否被修改的标志
	OgdcBool m_bModified;

	//! \brief 存每个查询记录集的指针
	OgdcArray <OgdcRecordset*> m_recordsets;

	//! \brief 数据集是否打开的标志
	OgdcBool m_bOpened;

	OgdcCoordSys m_cordsys;
	 
};

}

#endif // !defined(AFX_OGDCDATASETVECTORMDB_H__681E6C4F_B683_41E1_A7AF_B91345DAB853__INCLUDED_)
