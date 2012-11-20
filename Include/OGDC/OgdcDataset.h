//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)               
//                                                           
//!  \file OgdcDataset.h
//!  \brief OgdcDataset 的接口定义。
//!  \details Ogdc数据集的接口定义。
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#ifndef OGDCDATASET_H
#define OGDCDATASET_H

#include "Base/OgdcVariant.h"
#include "Base/OgdcRect2D.h"
#include "Element/OgdcCoordSys.h"
#include "Element/OgdcFieldInfo.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace OGDC
{
class OgdcDataSource;

//! \brief Ogdc数据集类。
class OGDC_API OgdcDataset  
{
public:
	//! \brief 数据集类型，大于等于 10001 为用户扩展
	enum Type
	{
		Unknown		=-1,
		//! \brief 只有属性表，没有几何对象
		Tabular		=0,	
		//! \brief 点数据集
		Point		=1,	
		//! \brief 线数据集
		Line		=3,	
		//! \brief 多边形数据集
		Region		=5,		 
		//! \brief 文本数据集
		Text		=7,		 
		//! \brief 带Mesure维的数据集
		LineM		=35,
		//! \brief 影像
		Image		=81,
		//! \brief Grid数据集
		Grid		=83,
		//! \brief 三维点
		Point3D		=101,
		//! \brief 三维线
		Line3D		=103,
		//! \brief 三维面
		Region3D	=105,
		//! 复合D数据集
		Compound	=149,	
	};
	
public:
	//! \brief 默认构造函数
	OgdcDataset();
	//! \brief 默认析构函数
	virtual ~OgdcDataset();

public:
	//! \brief 打开数据集。
	//! \return 是否成功打开数据集，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool Open() = 0;	
	
	//! \brief 关闭数据集。
	//! \return 是否成功关闭数据集，成功返回TRUE，失败返回FALSE。
	virtual void Close() = 0;	

	//! \brief 判断数据集是否已经打开。
	//! \return 打开返回TRUE，否则返回FALSE。
	virtual OgdcBool IsOpen() const = 0;
	
	//! \brief 获取数据集类型。
	//! \return 数据集类型枚举值。
	virtual OgdcInt GetType() const = 0;	
	
	//! \brief 获取数据集名。
	//! \return 数据集名。
	//! \remarks 数据集名可以和表名不同。
	virtual OgdcString GetName() =0;
	
	//! \brief 重命名数据集。
	//! \param strNewName 新的数据集名[in]。
	//! \return 是否成功重命名数据集，成功返回TRUE，失败返回FALSE。
	//! \remarks 只改变数据集名,表名不变。
	virtual OgdcBool Rename(const OgdcString& strNewName) = 0;		

	//! \brief 判断当前数据集是否可编辑。
	//! \return 可编辑返回TRUE，不可返回FALSE。
	virtual OgdcBool CanUpdate() = 0;	
	
	//! \brief 返回数据集的风格维数。
	//! \return 数据集的维数。点、线、面风格为0、1、2维其他为-1维
	virtual OgdcInt GetStyleDimension() =0;	
	
	//! \brief 判断数据集是否是栅格数据集
	//! \return 数据集是栅格数据集返回TRUE，否则返回FALSE。
	virtual OgdcBool IsRaster() =0;	
	
	//! \brief 获取数据集的地理范围。
	//! \return 数据集的地理范围。
	virtual OgdcRect2D GetBounds() =0;	
	
	//! \brief 设置数据集的地理范围。
	//! \param rc2Bounds 数据集的地理范围[in]。
	virtual void SetBounds(const OgdcRect2D& rc2Bounds) = 0;	
	
	//! \brief 将数据集各种信息保存到数据库中。
	//! \return 是否成功保存，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool SaveInfo() = 0;	
	
	//! \brief 刷新数据集信息。
	//! \return 是否成功刷新，成功返回TRUE，失败返回FALSE。
	//! \remarks 从数据库中读取数据集的各种属性，刷新当前数据集的信息。
	virtual OgdcBool RefreshInfo() = 0;	

	//! \brief 返回数据源指针
	virtual OgdcDataSource* GetDataSource() const = 0;

public:	
	//! \brief 获取数据集对应表名
	virtual OgdcString GetTableName();	
	
	//! \brief 获取最后更新数据集的时刻。
	//! \return 最后更新数据集的时刻。
	virtual OgdcTime GetDateLastUpdated();	
	
	//! \brief 获取创建数据集的时刻。
	//! \return 创建数据集的时刻。
	virtual OgdcTime GetDateCreated();	
	
	//! \brief 判断数据集是否在打开后有过修改。
	//! \return 有过修改返回TRUE，没有返回FALSE。
	virtual OgdcBool IsModified() const;
	
	//! \brief 设置数据集是否有过修改。
	//! \param bModified 数据集是否有过修改，默认有修改[in]。
	virtual void SetModifiedFlag(OgdcBool bModified = TRUE);
	
	//! \brief 设置数据集字符编码方式
	//! \param textCodec 数据集字符编码方式[in]。
	virtual void SetCharset(OgdcString::Charset nCharset);

	//! \brief 获取数据集字符编码方式。
	//! \return 数据集字符集枚举。
	virtual OgdcString::Charset GetCharset() const;

	//! \brief 设置投影坐标系。
	//! \param coordSys [in]。
	virtual OgdcBool SetCoordSys(const OgdcCoordSys& coordSys);	

	//! \brief 得到投影坐标系。
	virtual OgdcBool GetCoordSys(OgdcCoordSys& coordSys) const;			
		
	//! \brief 获取数据集描述信息。
	//! \return 数据集描述信息。
	virtual OgdcString GetDescription() const;				
	
	//! \brief 设置数据集描述信息。
	//! \param strDescription 数据集描述信息[in]。
	virtual void SetDescription(const OgdcString& strDescription);		
	
	//! \brief 设置元数据UUID
	//! \param strMetaUUID 元数据UUID
	//! \return 成功返回TRUE
	virtual OgdcBool SetMetaUUID(const OgdcString strMetaUUID);

	//! \brief 获取元数据UUID
	//! \return 返回元数据UUID，失败返回""
	virtual OgdcString GetMetaUUID();
};

}
#endif // !defined(OGDCDATASET_H)


