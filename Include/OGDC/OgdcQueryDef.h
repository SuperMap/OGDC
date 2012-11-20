//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)             
//                                                           
//!  \file OgdcQueryDef.h
//!  \brief OgdcQueryDef 的接口定义。
//!  \details Ogdc查询定义接口定义。
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#ifndef OGDCQUERYDEF_H
#define OGDCQUERYDEF_H

#include "Base/OgdcVariant.h"
#include "Base/OgdcRect2D.h"
#include "Base/OgdcArray.h"
#include "OGDC/OgdcDsConnection.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace OGDC
{

class OgdcElement;
class OgdcRecordset;

//! \brief 关联查询项
class OGDC_API OgdcJoinItem
{	
public:
	//! \brief 数据表连接类型枚举。
	enum  JoinType
	{
		//! \brief 内连接。
		InnerJoin	=0,
		//! \brief 左连接
		LeftJoin	=1,
		//! \brief 右连接
		RightJoin	=2,
		//! \brief 全连接
		FullJoin	=3 
	};

public:
	//! \brief 默认构造函数
	OgdcJoinItem();
	//! \brief 默认析构函数
#ifdef SYMBIAN60
	~OgdcJoinItem();
#else
	virtual ~OgdcJoinItem();
#endif

	//! \brief 拷贝构造函数。
	//! \param joinItem 关联查询项对象引用[in]。
	OgdcJoinItem(const OgdcJoinItem& joinItem);	

	//! \brief 构造函数。
	//! \param strName 名称[in]。
	//! \param nJoinType 连接类型[in]。
	//! \param strForeignTable 外部关联表[in]。
	//! \param strJoinFilter 关联条件[in]。
	OgdcJoinItem(const OgdcString& strName,
				JoinType nJoinType,
				const OgdcString& strForeignTable,
				const OgdcString& strJoinFilter);	

public:
	//! \brief 重载操作符 =
	const OgdcJoinItem& operator =(const OgdcJoinItem& joinItem);
	//! \brief 重载操作符 ==
	OgdcBool operator ==(const OgdcJoinItem& joinItem);
	//! \brief 重载操作符 !=
	OgdcBool operator !=(const OgdcJoinItem& joinItem);

public:	
	//! \brief 名称。
	OgdcString m_strName;	
	//! \brief 连接类型
	JoinType m_nJoinType;
	//! \brief 外部关联表
	OgdcString m_strForeignTable;
	//! \brief 关联条件
	//! \remarks 表tb1的Field1字段与 tb2的Field1关联则关联条件为:
	//! "tb1.Field1 = tb2.Field2"。	
	OgdcString m_strJoinFilter;
};
	
//! \brief 关联查询项
class OGDC_API OgdcLinkItem
{	
public:
	//! \brief 默认构造函数
	OgdcLinkItem();
	//! \brief 默认析构函数
#ifdef SYMBIAN60
	~OgdcLinkItem();
#else
	virtual ~OgdcLinkItem();
#endif
	
	//! \brief 拷贝构造函数
	OgdcLinkItem(const OgdcLinkItem& linkItem);	

public:
	//! \brief 重载操作符 =
	const OgdcLinkItem&  operator =(const OgdcLinkItem& linkItem);
	//! \brief 重载操作符 ==
	OgdcBool operator ==(const OgdcLinkItem& linkItem);
	//! \brief 重载操作符 !=
	OgdcBool operator !=(const OgdcLinkItem& linkItem);

public:	
	//! \brief 连接的名称。
	OgdcString m_strName;
	
	//! \brief 外部连接的连接参数。
	OgdcDsConnection m_dsForeignConnect;	

	//! \brief 外部属性表名。
	//! \remarks 如果是SuperMap的数据集，则应为数据集名字。
	OgdcString m_strObjectName;
	
	//! \brief 主表主键集合。
	OgdcArray<OgdcString> m_aryPrimaryKeys;	

	//! \brief 关联表外键集合。
	OgdcArray<OgdcString> m_aryForeignKeys;

	//! \brief 关联查询条件。
	OgdcString m_strFilter;
	
	//! \brief 要使用的外部属性表的字段列表。
	OgdcArray<OgdcString> m_aryFields;
};

//! \brief Ogdc查询定义。
class OGDC_API OgdcQueryDef  
{
public:
	//! \brief 查询类型枚举
	enum QueryType
	{
		//! \brief 一般的属性条件查询
		General		=0,
		//! \brief 使用地理范围矩形查询
		Bounds		=1,
		//! \brief 指定ID数组查询
		IDs			=2,
		//! \brief 距离查询
		Distance			=15,

		//! \brief 空间查询：同一。\n
		//!返回被搜索图层中与搜索对象完全相同的对象，包括对象类型和坐标
		Identical			=20,

		//! \brief 空间查询：相离。\n
		//!返回被搜索图层中与搜索对象相离的对象。
		Disjoint			=21,
		
		//! \brief 空间查询：交叠。\n
		//!如果搜索对象是面，返回全部或部分被搜索对象包含的对象以及全部或部分包含搜索对象的对象；如果搜索对象不是面，返回全部或部分包含搜索对象的对象（面）。 
		Intersects			=22,
		
		//! \brief 空间查询：接触。\n
		//!返回被搜索图层中其边界与搜索对象边界相触的对象。 
		Touches				=23,
		
		//! \brief 空间查询：重叠。\n
		//!返回被搜索图层中与搜索对象部分有重叠的对象。
		Overlaps			=24,
		
		//! \brief 空间查询：相交。\n
		//!返回被搜索图层中与搜索对象（线或面）相交的所有对象（点、线或面）。 
		Crosses				=25,	
		
		//! \brief 空间查询：被包含\n
		//!返回被搜索图层中包含搜索对象的对象。\n
		//!如果返回的对象是面，其必须全部包含（包括边接触）搜索对象；
		//!如果返回的对象是线，其必须完全包含搜索对象；\n
		//!如果返回的对象是点，其必须与搜索对象重合。 
		Within				=26,	
		
		//! \brief 空间查询：包含\n
		//!返回被搜索图层中被搜索对象包含的对象。包括在搜索对象边界上的情况。 
		Contains			=27,	
		
		//! \brief 空间查询：边界矩形相交\n
		//!返回被搜索图层中其边界矩形与搜索对象的边界矩形相交但其自身不与搜索对象相交的所有对象。 
		ExtentIntersect		=28,	
		
		//! \brief 空间查询：自定义算子
		Relate				=29,			
		
		//! \brief 空间查询算子\n
		//!返回被搜索图层中与搜索对象至少有一个公共节点的所有对象。
		CommonPoint			=30,
		
		//! \brief 空间查询算子\n
		//!返回被搜索图层中与搜索对象至少有一条公共边（完全重合，即在重合的边上共所有节点（顶点Vertex或端点Endpoint））的所有对象。
		CommonSegment		=31,
		
		//! \brief 空间查询算子\n
		//!返回被搜索图层中内点在搜索对象内部的面对象。 
		CentroidInPolygon	=32,
	};	
	
	//! \brief 查询选项枚举
	enum QueryOption
	{		
		//! \brief 查询几何对象
		Geometry	= 1,
		//! \brief 只查询属性信息
		Attribute	= 2,
		//! \brief 几何对象和属性都查询
		Both		= 3 
	};
	
	//! \brief 查询模式枚举
	enum QueryMode
	{
		//! \brief 一般查询
		GeneralQuery	=0x00000000, 
		//! \brief 模糊查询
		FuzzyQuery		=0x00000001
	};
	
	//! \brief 游标类型枚举
	enum CursorType
	{
		//! \brief 未定义的游标类型
		OpenUnspecified		=-1,		
		//! \brief 只向前游标
		OpenForwardOnly		=0, 
		//! \brief 关键集游标
		OpenKeyset			=1, 
		//! \brief 动态游标
		OpenDynamic			=2, 
		//! \brief 静态游标
		OpenStatic			=3, 
	};
	
	//! \brief 游标位置枚举
	enum CursorLocation
	{
		//! \brief 不指定游标位置
		UseDefault		=1, 
		//! \brief 服务器端游标
		UseServer		=2, 
		//! \brief 使用客户端游标
		UseClient		=3  
	};

public:
	//! \brief 默认构造函数
	OgdcQueryDef();
	//! \brief 默认析构函数
	~OgdcQueryDef();
	//! \brief 拷贝构造函数
	OgdcQueryDef(const OgdcQueryDef& queryDef);

public:
	//! \brief 重载操作符 =
	const OgdcQueryDef& operator=(const OgdcQueryDef& queryDef);
	//! \brief 重载操作符 ==
	OgdcBool operator==(const OgdcQueryDef& queryDef);
	//! \brief 重载操作符 !=
	OgdcBool operator!=(const OgdcQueryDef& queryDef);

public:
	//! \brief 查询类型(General,Bounds,IDs及各种空间查询等）
	//! \remarks 必须 (默认 General)
    QueryType	m_nType;
	
	//! \brief 查询选项(是否返回Geometry或Attribute,或两者都有等）
	//! \remarks 必须 （默认 Both）
	QueryOption	m_nOptions;
	
	//! \brief 查询方式(GeneralQuery或FuzzyQuery)
	//! \remarks 可选 默认呢，GeneralQuery
	QueryMode	m_nMode;
	
	//! \brief 属性数据查询条件
	//! \remarks 可选 默认 ""
	OgdcString	m_strFilter;
	
	//! \brief 属性数据查询排序
	//! \remarks 可选 默认 ""
	OgdcString	m_strSort;

	//! \brief 属性数据分组语句
	//! \remarks 可选 默认 ""
	OgdcString	m_strGroup;

	//! \brief 属性数据查询字段名
	//! \remarks 可选 默认 ""
	OgdcArray<OgdcString>	m_Fields;
	
	//! \brief 存储外部关联条件
	//! \remarks 可选 默认 ""
	OgdcArray<OgdcJoinItem>	m_JoinItems;
	
	//! \brief 外挂关联属性表
	OgdcArray<OgdcLinkItem>	m_aryLinkItems;
	
	//! \brief Bounds查询时的外接矩形
	//! \remarks 可选 默认 empty
	OgdcRect2D	m_rc2Bounds;
	
	//! \brief ID查询时的ID数组
	//! 可选 默认为空数组
	OgdcArray<OgdcInt>	m_IDs;
	
	//! \brief ID查询时的ID队列,目前只支持数值型字段
	//! \remarks 可选 默认为空
	OgdcString	m_strIDFieldName;
	
	//! \brief 游标(光标)类型
	//! \remarks 可选 默认 OpenDynamic
	CursorType	m_nCursorType;
	
	//! \brief 游标位置
	CursorLocation	m_nCursorLocation;

	//! \brief 查询颗粒容限, 当查询模式为模糊查询时,为忽略小对象容限 默认 0；
	//! 空间查询时,为空间查询容限，默认为10的负10次方；距离查询时，为查询距离，由用户指定。
	OgdcDouble m_dGranule;
	
	//! \brief 空间查询或距离查询几何对象。
	OgdcElement* m_pElement;

	//! \brief 空间查询或距离查询记录集。
	OgdcRecordset* m_pRecordset;

	//! \brief 空间查询模式为Relate时使用。
	OgdcString m_strSpatialRelate;
	
	//! \brief 距离查询单位，经纬度数据下可以采用米(AU_METER)等单位。
	OgdcInt m_nDistUnit;
};

}
#endif // !defined(OGDCQUERYDEF_H)

