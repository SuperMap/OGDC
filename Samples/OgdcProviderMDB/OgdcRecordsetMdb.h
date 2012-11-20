//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)        
//                                                           
//!  \file OgdcRecordsetMdb.h
//!  \brief OGDC记录集的接口定义。
//!  \details OGDC记录集各种操作的接口
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Limited Company.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCRECORDSETMDB_H__EE00C954_14D4_4C36_9BEA_84CED998068D__INCLUDED_)
#define AFX_OGDCRECORDSETMDB_H__EE00C954_14D4_4C36_9BEA_84CED998068D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OGDC/OgdcRecordset.h"
#include "OgdcDatasetVectorMdb.h"

namespace OGDC
{

//! \brief 查询记录集的类
//! \remarks 主要对查询记录进行各种操作的接口
class OGDC_API OgdcRecordsetMdb : public OgdcRecordset  
{
	//! \brief 声明友元类
	friend class OgdcDatasetVectorMdb;

public:
	//! \brief 记录集类默认构造函数
	OgdcRecordsetMdb();

	//! \brief 带指向矢量数据集指针的构造函数
	//! \param pDataset 指向矢量数据集的指针[in]。
	OgdcRecordsetMdb(OgdcDatasetVectorMdb *pDataset);

	//! \brief 记录集类默认析构函数
	virtual ~OgdcRecordsetMdb();

public:
	//! \brief 是否记录集为空(没有记录)。
	//! \return 记录集为空返回TRUE，非空返回FALSE。
	virtual OgdcBool IsEmpty() ; 	
	
	//! \brief 判断当前记录指针是否指向头。
	//! \return 指针指向头返回TRUE，否则返回FALSE。
	virtual OgdcBool IsBOF() ;	
	
	//! \brief 判断当前记录指针是否指向尾。
	//! \return 指针指向尾返回TRUE，否则返回FALSE。
	virtual OgdcBool IsEOF() ;	
	
	//! \brief 关闭记录集
	virtual void Close() ;
	
	//! \brief 通过字段名获取字段值。
	//! \param strName 字段名[in]。
	//! \param varVal 字段值[out]。
	//! \return 是否成功获取，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool GetFieldValue(const OgdcString& strName, OgdcVariant& varVal) ;	
	
	//! \brief 通过字段序号获取字段值。
	//! \param nIndex 字段序号[in]。
	//! \param varVal 字段值[out]。
	//! \return 是否成功获取，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool GetFieldValue(OgdcInt nIndex,OgdcVariant& varVal) ;	
	
	//! \brief 通过字段名设置当前记录的字段值。
	//! \param strName 字段名[in]。
	//! \param varVal 字段值[in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	//! \remarks 字段值为变体类型
	virtual OgdcBool SetFieldValue(const OgdcString& strName,const OgdcVariant& varVal) ;	
	
	//! \brief 通过字段序号设置当前记录的字段值。
	//! \param nIndex 字段序号[in]。
	//! \param varVal 字段值[in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	//! \remarks 字段值为变体类型。
	virtual OgdcBool SetFieldValue(OgdcInt nIndex,const OgdcVariant& varVal) ;	
	
	//! \brief 查找指定条件的记录。
	//! \param strFilter 为查询条件，例如 “ID = 5",查找ID为5的记录[in]。
	//! \param nFindMode 为查找模式，查找当前模式下满足条件的记录，默认为First[in]。
	//! \return 是否查找成功。
	//! \remarks 查找模式 First--第一个，Last--最后一个，Next--下一个，Prev--前一个  
	virtual OgdcBool Find(const OgdcString& strFilter, 
		OgdcRecordset::FindMode nFindMode = OgdcRecordset::First) ;	
	
	//! \brief 记录集游标移动到指定位置。
	//! \param nFromPosition Move位置，默认为Current[in]。
	//! \param 移动的次数[in]。
	//! \return 是否已经移动到指定位置。
	//! \remarks Move位置：Current--当前位置，Begin--开头，End--末尾  
	virtual OgdcBool Move(OgdcRecordset::MovePosition nFromPosition = OgdcRecordset::Current, 
		OgdcInt nRows = 1) ;	
	
	//! \brief 获取记录个数。
	//! \return 记录个数。
	virtual OgdcInt GetRecordCount() ;	
	
	//! \brief 收集所有字段的信息。
	//! \param fieldInfos 用来收集所有字段的字段信息数组的引用[out]。
	//! \return 是否收集成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool GetFieldInfos(OgdcFieldInfos& fieldInfos) ;	
	
	//! \brief 通过字段名获取字段信息。
	//! \param strName 字段名[in]。
	//! \param fieldInfo 字段信息引用[out]。
	//! \return 是否获取成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool GetFieldInfo(const OgdcString& strName,OgdcFieldInfo& fieldInfo)  ;			

	//! \brief 按某种统计方法统计某个字段。
	//! \param strFieldName 统计的字段名[in]。
	//! \param nMode 统计模式[in]。
	//! \return 统计值。
	//! \remarks 能统计的字段类型为短整型，整型，长整型，单，双精度浮点数
	virtual OgdcVariant Statistic(const OgdcString& strFieldName,
		OgdcStatisticMode nMode) ;	

	//! \brief 重新查询，刷新记录集。
	//! \return 是否成功查询，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool ReQuery() ;	

	//! \brief 计算记录集的地理范围。
	//! \param rc2Bounds 记录集的地理范围[out]。
	//! \param &dMinZ 记录集最小高程[in]。
	//! \param &dMaxZ 记录集最大高程[in]。
	//! \return 是否记算成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool ComputeBounds(OgdcRect2D &rc2Bounds,
		OgdcDouble &dMinZ, OgdcDouble &dMaxZ) ;	

public:
	//! \brief 复制字段值。
	//! \param strSrcField 源字段名[in]。
	//! \param strDestField 目的字段名[in]。
	//! \return 是否成功复制，成功返回TRUE，失败返回FALSE。
	//! \attention strSrcField和trDestField必须都存在且字段类型匹配。
	virtual OgdcBool CopyField(const OgdcString& strSrcExpression,
		const OgdcString& strDestFieldName) ;	
	
	//! \brief 获取当前记录的ID。
	//! \return 当前记录的ID。
	virtual OgdcInt GetID() ;	

	//! \brief 获取几何对象，返回的几何对象指针在函数内分配内存，。
	//! \param pElement 空几何对象指针用来取出几何对象[in]。
	//! \return 是否取几何对象成功，成功返回TRUE，失败返回FALSE。
	//! \attention 外面负责删除，否则会有内存泄漏。
	virtual OgdcBool GetElement(OgdcElement*& pElement) ;	

	//! \brief 设置当前记录的几何对象。
	//! \param pElement 几何对象指针[in]。
	//! \return 是否设置几何对象成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool SetElement(OgdcElement* pElement) ;	

	//! \brief 增加新记录,成功，返回TRUE值，失败返回FALSE值；
	//! \param pGeometry [in] 添加的Geometry指针
	//! \param bReturnID=FALSE [in] 此参数已经不起作用。
	//! \return True 和 False 的值
	//! \remarks 如果想得到新对象的ID，那么马上调用GetId方法
	virtual OgdcInt AddNew(OgdcElement *pElement, OgdcBool bReturnID=FALSE) ;

	//! \brief 取出数据集中某行特征要素。
	//! \param nID 数据集记录ID号[in]。
	//! \return 特征要素指针。
	//! \attention 外面负责删除，否则会有内存泄漏。
	virtual OgdcFeature* GetFeature() ;	

	//! \brief 将特征要素更新到数据集中。
	//! \param pFeature 特征要素指针[in]。
	//! \return 是否保存成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool UpdateFeature(const OgdcFeature* pFeature) ;
	
	//! \brief 将特征要素追加到数据集未尾。
	//! \param pFeature 特征要素指针[in]。
	//! \return 是否追加成功，成功返回TRUE，失败返回FALSE。
	//! \remarks 追加时特征要素关联的数据集记录ID将不起作用。
	virtual OgdcBool AddFeature(const OgdcFeature* pFeature) ;

	//! \brief 通过ID定位记录指针。
	//! \param nID 记录ID[in]。
	//! \return 定位成功返回TRUE，失败返回FALSE。
	virtual OgdcBool SeekID(OgdcInt nID) ;	

	//! \brief 将记录集更新到数据集。
	//! \return 更新记录的ID。
	virtual OgdcInt Update() ;	

	//! \brief 取消修改。
	//! \return 成功取消返回TRUE，失败返回FALSE。
	virtual OgdcBool CancelUpdate() ;	
	
	//! \brief 开始修改数据。
	//! \return 成功开始返回TRUE，失败返回FALSE。
	virtual OgdcBool Edit() ;	

	//! \brief 是否可更新
	//! \return 可更新返回TRUE，否则返回FALSE。
	virtual OgdcBool CanUpdate() ;	
	
	//! \brief 删除当前记录。
	//! \return 是否成功删除，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool Delete() ;	

	//! \brief 删除所有记录。
	//! \return 删除成功返回TRRUE，失败返回FALSE。
	virtual OgdcBool DeleteAll() ;
	
	//! \brief 获取当前数据集指针。
	//! \return 指向当前数据集的指针。
	virtual OgdcDatasetVector* GetDataset() ;	

protected:
	
	//! \brief 在查询时设置记录个数
	void SetRecordCount();

protected:
	//! \brief 指向当前数据集的指针
	OgdcDatasetVectorMdb *m_pDataset;

	//! \brief 存每个字段数据的数组
	OgdcArray<OgdcByte*> m_fieldValues;

	//! \brief 记录集ID
	OgdcInt m_nID;

	//! \brief 查询MDB时使用
	CDaoRecordset* m_pDaoRecordset;	   	
	
	//! \brief 属性查询条件
	OgdcString m_strFilter;

	//! \brief 数据集的表名
	OgdcString m_strTableName;

	//! \brief 用来标志是否执行了Edit()函数
	OgdcBool m_bEdited;   

	//! \brief Edit状态
	//! \remarks 0--EditNone ;1--EditInprogress; 2--EditAdd; 3--EditOfGeometry
	OgdcInt	m_nEditMode; 

	// \brief 当前结果集中的记录数目
	OgdcLong m_nRecordCount;

	//! \brief 添加的记录是否为首记录的标志
	OgdcBool m_bFirstAddNew;
	
	//! \brief 指向该记录的几何对象的指针
	OgdcElement* m_pElement;

};

}

#endif // !defined(AFX_OGDCRECORDSETMDB_H__EE00C954_14D4_4C36_9BEA_84CED998068D__INCLUDED_)
