//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)              
//                                                           
//!  \file OgdcMetadata.h
//!  \brief OgdcMetadata类的接口定义
//!  \details Metadata为一个元数据集，Metadata可以是一个数据集的元数据，也可以是一个数据源的元数据
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCMETADATA_H__6100835C_9516_424B_9D85_CCDF0C281B3D__INCLUDED_)
#define AFX_OGDCMETADATA_H__6100835C_9516_424B_9D85_CCDF0C281B3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base/ogdcdefs.h"
#include "Base/OgdcString.h"

namespace OGDC
{

class OgdcMetaDatabase;

class OgdcDataSource;
class OgdcDataset;

//! \brief Ogdc元数据集类。
class OGDC_API OgdcMetadata  
{
public:
	//! \brief 默认构造函数
	OgdcMetadata();

	//! \brief 拷贝构造函数
	OgdcMetadata(OgdcMetaDatabase * pMetaDatabase);

	//! \brief 默认析构函数
	virtual ~OgdcMetadata();

	//! \brief 进入下一级标签
	//! \return 成功，返回TRUE, 否则返回FALSE。
	virtual OgdcBool IntoTag();

	//! \brief 返回上一级标签
	//! \return 成功，返回TRUE, 否则返回FALSE。
	virtual OgdcBool OutOfTag();

	//! \brief 在当前层级中下移到下一个标签
	//! \return 成功，返回TRUE, 否则返回FALSE。
	virtual OgdcBool MoveNext()= 0;

	//! \brief 回到当前级别标签 的顶层
	//! \return 成功，返回TRUE, 否则返回FALSE。
	virtual OgdcBool ResetPos();

	//! \brief 是否已经到达最后一个标签
	virtual OgdcBool IsEof() const = 0 ;

	//! \brief 按标签名查找标签项
	//! \return 成功，返回TRUE，并定位到该标签； 否则返回FALSE。
	virtual OgdcBool FindTag(const OgdcString &strTagName);

	//! \brief 获取标签名
	//! \return 成功，返回标签名, 否则返回""。
	virtual OgdcString GetTagName() const = 0;

	//! \brief 获取标签值
	//! \return 成功，返回标签值, 否则返回""。
	virtual OgdcString GetData() const = 0;

	//! \brief 设置标签值
	//! \return 成功，返回标签值, 否则返回""。
	virtual OgdcBool SetData(const OgdcString &strData)  = 0;

	//! \brief 从数据库中读取指定的 m_strMetadataUUID 的元数据的详细内容。
	//! \return 读取成功返回TRUE，否则返回FALSE。
	virtual OgdcBool Fetch() = 0;	

	//! \brief 将当前元数据的内容更新到数据库中，所有的修改在Update之前都保存在内存中
	//! \return 更新成功返回TRUE，否则返回FALSE。
	virtual OgdcBool Update() = 0;

public:
	//! \brief 当前元数据的UUID
	//! \remarks 在 Import 到元数据库时自动生成，也可以从数据源或数据集获取，不能修改
	OgdcString m_strMetadataUUID;

};

}

#endif // !defined(AFX_OGDCMETADATA_H__6100835C_9516_424B_9D85_CCDF0C281B3D__INCLUDED_)

