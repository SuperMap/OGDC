//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file OgdcRecordset.h
//!  \brief OgdcRecordset 的接口定义。
//!  \details Ogdc记录集的接口定义。
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////
#ifndef OGDCRECORDSET_H
#define OGDCRECORDSET_H

#include "OGDC/OgdcQueryDef.h"
#include "Element/OgdcFeature.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace OGDC
{
class OgdcDatasetVector;

//! \brief Ogdc记录集。
class OGDC_API OgdcRecordset
{
public:	
	//! \brief 字段统计模式
	enum OgdcStatisticMode
	{
		//! \brief 最大值。
		smMax				= 1,
		//! \brief 最小值。
		smMin				= 2,
		//! \brief 平均值。
		smAvg				= 3,
		//! \brief 总和。
		smSum				= 4,
		//! \brief 标准差。
		smStdev				= 5,
		//! \brief 方差。
		smVar				= 6	
	};

	//! \brief 数据集编辑模式。
	enum EditMode 
	{
		//! \brief 未做任何编辑
		EditNone			= 0x00000000,
		//! \brief 已执行过AddNew操作
		EditAdd				= 0x00000001,
		//! \brief 正在编辑
		EditInProgress		= 0x00000002,
		//! \brief 已更改属性数据
		EditSetFieldValue	= 0x00000004, 
		//! \brief 已执行过SetGeometry操作
		EditSetGeometry		= 0x00000008,
		//! \brief 已删除记录
		EditDeleted			= 0x00000010 
	};

	//! \brief 查找模式
	enum FindMode 
	{
		//! \brief 第一个
		First			= 0,
		//! \brief 最后一个
		Last			= 1,
		//! \brief 下一个
		Next			= 2,
		//! \brief 前一个
		Prev			= 3, 
	};

	//! \brief Move位置
	enum MovePosition 
	{
		//! \brief 当前位置
		Current			= 0,
		//! \brief 开头
		Begin			= 1,
		//! \brief 末尾
		End				= 2,
	};
	
public:
	//! \brief 默认构造函数
	OgdcRecordset();
	//! \brief 默认析构函数
	virtual ~OgdcRecordset();

public:
	//! \brief 是否记录集为空(没有记录)。
	//! \return 记录集为空返回TRUE，非空返回FALSE。
	virtual OgdcBool IsEmpty() = 0; 	
	
	//! \brief 判断当前记录指针是否指向头。
	//! \return 指针指向头返回TRUE，否则返回FALSE。
	virtual OgdcBool IsBOF() =0;	
	
	//! \brief 判断当前记录指针是否指向尾。
	//! \return 指针指向尾返回TRUE，否则返回FALSE。
	virtual OgdcBool IsEOF() =0;	
	
	//! \brief 关闭记录集
	virtual void Close() =0;
	
	//! \brief 通过字段名获取字段值。
	//! \param strName 字段名[in]。
	//! \param varVal 字段值[out]。
	//! \return 是否成功获取，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool GetFieldValue(const OgdcString& strName, OgdcVariant& varVal) =0;	
	
	//! \brief 通过字段序号获取字段值。
	//! \param nIndex 字段序号[in]。
	//! \param varVal 字段值[out]。
	//! \return 是否成功获取，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool GetFieldValue(OgdcInt nIndex,OgdcVariant& varVal) =0;	
	
	//! \brief 通过字段名设置当前记录的字段值。
	//! \param strName 字段名[in]。
	//! \param varVal 字段值[in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	//! \remarks 字段值为变体类型
	virtual OgdcBool SetFieldValue(const OgdcString& strName,const OgdcVariant& varVal) =0;	
	
	//! \brief 通过字段序号设置当前记录的字段值。
	//! \param nIndex 字段序号[in]。
	//! \param varVal 字段值[in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	//! \remarks 字段值为变体类型。
	virtual OgdcBool SetFieldValue(OgdcInt nIndex,const OgdcVariant& varVal) =0;
	
	//! \brief 查找指定条件的记录。
	//! \param strFilter 为查询条件，例如 “ID = 5",查找ID为5的记录[in]。
	//! \param nFindMode 为查找模式，查找当前模式下满足条件的记录，默认为First[in]。
	//! \return 是否查找成功。
	//! \remarks 查找模式 First--第一个，Last--最后一个，Next--下一个，Prev--前一个  
	virtual OgdcBool Find(const OgdcString& strFilter, 
		OgdcRecordset::FindMode nFindMode = OgdcRecordset::First) =0;
	
	//! \brief 记录集游标移动到指定位置。
	//! \param nFromPosition Move位置，默认为Current[in]。
	//! \param 移动的次数[in]。
	//! \return 是否已经移动到指定位置。
	//! \remarks Move位置：Current--当前位置，Begin--开头，End--末尾
	virtual OgdcBool Move(OgdcRecordset::MovePosition nFromPosition = OgdcRecordset::Current, 
		OgdcInt nRows = 1) =0;	
	
	//! \brief 获取记录个数。
	//! \return 记录个数。
	virtual OgdcInt GetRecordCount() =0;	
	
	//! \brief 收集所有字段的信息。
	//! \param fieldInfos 用来收集所有字段的字段信息数组的引用[out]。
	//! \return 是否收集成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool GetFieldInfos(OgdcFieldInfos& fieldInfos) =0;	
	
	//! \brief 通过字段名获取字段信息。
	//! \param strName 字段名[in]。
	//! \param fieldInfo 字段信息引用[out]。
	//! \return 是否获取成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool GetFieldInfo(const OgdcString& strName,OgdcFieldInfo& fieldInfo)  =0;			

	//! \brief 按某种统计方法统计某个字段。
	//! \param strFieldName 统计的字段名[in]。
	//! \param nMode 统计模式[in]。
	//! \return 统计值。
	virtual OgdcVariant Statistic(const OgdcString& strFieldName,
		OgdcStatisticMode nMode) =0;	

	//! \brief 重新查询，刷新记录集。
	//! \return 是否成功查询，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool ReQuery() =0;	

	//! \brief 计算记录集的地理范围。
	//! \param rc2Bounds 记录集的地理范围[out]。
	//! \param &dMinZ 记录集最小高程[in]。
	//! \param &dMaxZ 记录集最大高程[in]。
	//! \return 是否记算成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool ComputeBounds(OgdcRect2D &rc2Bounds,
		OgdcDouble &dMinZ, OgdcDouble &dMaxZ) =0;	

public:
	//! \brief 复制字段值。
	//! \param strSrcField 源字段名[in]。
	//! \param strDestField 目的字段名[in]。
	//! \return 是否成功复制，成功返回TRUE，失败返回FALSE。
	//! \attention strSrcField和trDestField必须都存在且字段类型匹配。
	virtual OgdcBool CopyField(const OgdcString& strSrcExpression,
		const OgdcString& strDestFieldName) =0;	
	
	//! \brief 获取当前记录的ID。
	//! \return 当前记录的ID。
	virtual OgdcInt GetID() = 0;	

	//! \brief 获取几何对象，返回的几何对象指针在函数内分配内存，。
	//! \param pElement 空几何对象指针用来取出几何对象[in]。
	//! \return 是否取几何对象成功，成功返回TRUE，失败返回FALSE。
	//! \attention 外面负责删除，否则会有内存泄漏。
	virtual OgdcBool GetElement(OgdcElement*& pElement) =0;	

	//! \brief 设置当前记录的几何对象。
	//! \param pElement 几何对象指针[in]。
	//! \return 是否设置几何对象成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool SetElement(OgdcElement* pElement) =0;	

	//! \brief 增加新记录,成功，返回TRUE值，失败返回FALSE值；
	//! \param *pGeometry [in] 添加的Geometry指针
	//! \param bReturnID=FALSE [in] 此参数已经不起作用。
	//! \return True 和 False 的值
	//! \remarks 如果想得到新对象的ID，那么马上调用GetId方法
	virtual OgdcInt AddNew(OgdcElement *pElement, OgdcBool bReturnID=FALSE) =0;

	//! \brief 取出数据集中某行特征要素。
	//! \param nID 数据集记录ID号[in]。
	//! \return 特征要素指针。
	virtual OgdcFeature* GetFeature() = 0;	

	//! \brief 将特征要素更新到数据集中。
	//! \param pFeature 特征要素指针[in]。
	//! \return 是否保存成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool UpdateFeature(const OgdcFeature* pFeature) = 0;
	
	//! \brief 将特征要素追加到数据集未尾。
	//! \param pFeature 特征要素指针[in]。
	//! \return 是否追加成功，成功返回TRUE，失败返回FALSE。
	//! \remarks 追加时特征要素关联的数据集记录ID将不起作用。
	virtual OgdcBool AddFeature(const OgdcFeature* pFeature) = 0;

	//! \brief 通过ID定位记录指针。
	//! \param nID 记录ID[in]。
	//! \return 定位成功返回TRUE，失败返回FALSE。
	virtual OgdcBool SeekID(OgdcInt nID) =0;	

	//! \brief 将记录集更新到数据集。
	//! \return 更新记录的ID。
	virtual OgdcInt Update() =0;	

	//! \brief 取消修改。
	//! \return 成功取消返回TRUE，失败返回FALSE。
	virtual OgdcBool CancelUpdate() =0;	
	
	//! \brief 开始修改数据。
	//! \return 成功开始返回TRUE，失败返回FALSE。
	virtual OgdcBool Edit() =0;	

	//! \brief 是否可更新
	//! \return 可更新返回TRUE，否则返回FALSE。
	virtual OgdcBool CanUpdate() =0;	
	
	//! \brief 删除当前记录。
	//! \return 是否成功删除，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool Delete() =0;	

	//! \brief 删除所有记录。
	//! \return 删除成功返回TRRUE，失败返回FALSE。
	virtual OgdcBool DeleteAll() =0;
	
	//! \brief 获取当前数据集指针。
	//! \return 指向当前数据集的指针。
	virtual OgdcDatasetVector* GetDataset() =0;		

public:
	//! \brief 查询定义
	OgdcQueryDef m_QueryDef;

};

}
#endif // !defined(OGDCRECORDSET_H)

