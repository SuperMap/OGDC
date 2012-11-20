//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)               
//                                                           
//!  \file OgdcDatasetVector.h
//!  \brief OgdcDatasetVector 的接口定义。
//!  \details Ogdc矢量数据集接口定义。
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////
#ifndef OGDCDATASETVECTOR_H
#define OGDCDATASETVECTOR_H

#include "OGDC/OgdcDatasetVectorInfo.h"
#include "OGDC/OgdcRecordset.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace OGDC
{

//! \brief Ogdc矢量数据集。
class OGDC_API OgdcDatasetVector : public OgdcDataset
{
public:
	//! \brief 默认构造函数
	OgdcDatasetVector();
	//! \brief 默认析构函数
	virtual ~OgdcDatasetVector();

// 纯虚函数
public:
	//! \brief 获取数据集中对象个数。
	//! \param  [in]。
	//! \return 对象个数。
	//! \remarks 速度慢,但值比较正确。
	//! \attention 通过Register表获取。
	virtual OgdcInt GetObjectCount() = 0;

	//! \brief 返回记录集个数。
	//! \return 记录集个数。
	virtual OgdcInt GetRecordsetCount() =0;	
	
	//! \brief 通过记录集指针释放记录集内存空间。
	//! \param *pRecordset 记录集指针[in]。
	//! \return 是否释放成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool ReleaseRecordset(OgdcRecordset *pRecordset) =0;	

	//! \brief 释放所有记录集
	virtual void ReleaseAllRecordsets() = 0;	

	//! \brief 取出数据集中某行特征要素。
	//! \param nID 数据集记录ID号[in]。
	//! \return 行特征要素指针。
	virtual OgdcFeature* GetFeature(OgdcInt nID) = 0;	

	//! \brief 将特征要素更新到数据集中。
	//! \param pFeature 特征要素指针[in]。
	//! \return 是否保存成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool UpdateFeature(const OgdcFeature* pFeature) = 0;
	
	//! \brief 将特征要素追加到数据集未尾。
	//! \param pFeature 特征要素指针[in]。
	//! \return 是否追加成功，成功返回TRUE，失败返回FALSE。
	//! \remarks 追加时特征要素关联的数据集记录ID将不起作用。
	virtual OgdcBool AddFeature(const OgdcFeature* pFeature) = 0;	

	//! \brief 将特征要素从数据集中删除。
	//! \param nID 数据集记录ID号[in]。
	//! \return 是否删除成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool DeleteFeature(OgdcInt nID) = 0;	
	
	//! \brief 通过ID数组删除数据集中的记录。
	//! \param pIDs ID数组指针[in]。
	//! \param nCount ID数组元素个数[in]。
	//! \return 成功删除返回TRUE，失败返回FALSE。
	virtual OgdcBool DeleteFeatures(const OgdcInt* pIDs, OgdcInt nCount) = 0;	
	
	//! \brief 删除表中所有数据。
	//! \return 是否删除成功，成功返回TRUE，失败返回FALSE。
	//! \remarks 清空表后表中的字段仍然保留。
	virtual OgdcBool Truncate() =0;	

	//! \brief 根据当前索引状态重建索引
	//! \return 是否重建成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool BuildSpatialIndex() = 0;
	
	//! \brief 清除空间索引,为没有索引的状态
	//! \return 是否清除成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool DropSpatialIndex() = 0;
	
	//! \brief 是否需要重建空间索引
	//! \return 需要重建返回TRUE，不需要返回FALSE。
	//! \remarks 规定:如果没有索引返回FALSE,有索引看索引的情况
	virtual OgdcBool IsSpatialIndexDirty() =0;

	//! \brief 查询通用入口，得到记录集。
	virtual OgdcRecordset* Query(const OgdcQueryDef& querydef) =0;
	
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
		const OgdcString& strFilter = _U("")) =0;	
	
	//! \brief 复制字段值,strSrcField和。
	//! \param strSrcFieldName 源字段名称[in]。
	//! \param strDestFieldName 目的字段名称[in]。
	//! \return 是否复制字段成功，成功返回TRUE，失败返回FALSE。
	//! \attention 目的字段名必须都存在且目的字段要与源字段类型匹配。
	virtual OgdcBool CopyField(
		const OgdcString& strSrcFieldName,
		const OgdcString& strDestFieldName) =0;	
	
	//! \brief 创建普通索引。
	//! \param strFieldNameList 创建索引的字段列表，多个字段中间用","隔开[in]。
	//! \param strIndexName 索引名[in]。
	//! \return 创建普通索引是否成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool CreateFieldIndex(
		const OgdcString& strFieldNameList,
		const OgdcString& strIndexName) =0;	

	//! \brief 删除索引。
	//! \param strIndexName 索引名[in]。
	//! \return 删除索引是否成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool DropFieldIndex(const OgdcString& strIndexName) = 0;	

	//! \brief 查找获得索引。
	//! \param 无。
	//! \return 索引值--索引所在字段对应Map。
	virtual std::map<OgdcString,OgdcString> GetFieldIndexes();	
	
	//! \brief 创建多个字段。
	//! \param fieldInfos 字段描述信息数组引用[in]。
	//! \return 创建多个字段是否成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool CreateFields(const OgdcFieldInfos& fieldInfos) =0;	
		
	//! \brief 通过字段名称删除字段。
	//! \param strFieldName 字段名[in]。
	//! \return 是否删除成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool DeleteField(const OgdcString& strFieldName) =0;
	
	//! \brief 获取数据表中所有字段的描述信息。
	//! \param fieldInfos 数据表中所有字段的描述信息数组[out]。
	//! \param bSystemFieldExcluded 是否获取系统字段，默认不取[in]。
	//! \return 是否获取成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool GetFieldInfos(
		OgdcFieldInfos& fieldInfos,
		OgdcBool bSystemFieldExcluded = FALSE) =0;	
	
	//! \brief 通过字段名获取字段描述信息。
	//! \param strFieldName 字段名[in]。
	//! \param fieldInfo 字段描述信息[out]。
	//! \return 是否获取成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool GetFieldInfo(
		const OgdcString& strFieldName,
		OgdcFieldInfo& fieldInfo) =0;
	
	//! \brief 通过字段名设置字段信息。
	//! \param strFieldName 字段名[in]。
	//! \param fieldInfo 字段描述信息对象引用[in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool SetFieldInfo(
		const OgdcString& strFieldName,
		const OgdcFieldInfo& fieldInfo) =0;	
	
	//! \brief 统计字段值。
	//! \param strFieldName 字段名[in]。
	//! \param nMode 统计模式[in]。
	//! \return 统计值。
	virtual OgdcVariant Statistic(
		const OgdcString& strFieldName,
		OgdcRecordset::OgdcStatisticMode nMode) =0;	

public:
	//! \brief 获取数据集类型
	//! \return 数据集类型。
	virtual OgdcInt GetType() const;
	
	//! \brief 获取数据集的维数
	//! \return 数据集的维数。
	virtual OgdcInt GetStyleDimension();	
	
	//! \brief 获取数据集名(可以和表名不同)
	//! \return 数据集名。
	virtual OgdcString GetName();	
	
	//! \brief 获取数据集描述信息。
	//! \return 数据集描述信息。
	virtual OgdcString GetDescription() const;	
	
	//! \brief 设置数据集描述信息
	//! \param strDesc [数据集描述信息in]。
	virtual void SetDescription(const OgdcString& strDesc);	
	
	//! \brief 判断数据集是否是栅格数据集
	//! \return 是栅格数据集返回TRUE，不是返回FALSE。
	virtual OgdcBool IsRaster();
	
	//! \brief 获取数据集的地理范围。
	//! \return 数据集的地理范围矩形。
	virtual const OgdcRect2D& GetBounds() const;
	
	//! \brief 设置数据集的范围。
	//! \param rcBounds 数据集的地理范围矩形[in]。
	virtual void SetBounds(const OgdcRect2D& rcBounds);
	
	//! \brief 得到未被占用的字段名。
	//! \param strFieldName 指定的一个字段名[in]。
	//! \return 未被占用的字段名。
	virtual OgdcString GetUnoccupiedFieldName(const OgdcString& strFieldName);

	//! \brief 判断字段名是否有效。
	//! \param strFieldName 字段名[in]。
	//! \return 有效返回TRUE，无效返回FALSE。
	virtual OgdcBool IsAvailableFieldName(const OgdcString& strFieldName);

	//! \brief 获取子数据集
	//! \return 如果有子数据集则填充数据集指针数组，否则数组为空
	virtual OgdcBool GetChildDataset(OgdcArray<OgdcDatasetVector *>& arrSubDatasets) const;

	//! \brief  获取父数据集指针
	//! \return 如果有父数据集，则返回父数据集指针；否则返回NULL
	virtual OgdcDatasetVector* GetParentDataset() const;
	
	//! \brief 矢量数据集能力描述：Provider本身是否支持相关查询
	virtual OgdcBool IsSupportQueryType(OgdcQueryDef::QueryType queryType) const;

public:
	//! \brief 矢量数据集描述信息
	OgdcDatasetVectorInfo m_info;	
};

}
#endif // !defined(OGDCDATASETVECTOR_H)

