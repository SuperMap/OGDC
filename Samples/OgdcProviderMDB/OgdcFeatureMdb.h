//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)       
//                                                           
//!  \file OgdcFeatureMdb.h
//!  \brief OgdcFeature 的接口定义。
//!  \details OGDC特征要素类的接口。
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Limited Company.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCFEATUREMDB_H__C646C050_13AF_4332_A835_9D5BE49B5571__INCLUDED_)
#define AFX_OGDCFEATUREMDB_H__C646C050_13AF_4332_A835_9D5BE49B5571__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OGDC/OgdcDataSource.h"

namespace OGDC
{

//! \brief Ogdc 特征要素类	
class OGDC_API OgdcFeatureMdb : public OgdcFeature  
{
	//! \brief 声明友元类
	friend class OgdcRecordsetMdb;

public:
	//! \brief 特征要素类默认构造函数
	OgdcFeatureMdb();
	//! \brief 特征要素类默认析构函数
	virtual ~OgdcFeatureMdb();
	
public:
	//! \brief 获取几何对象ID。
	virtual OgdcInt GetID() const ;	
	
	//! \brief 设置几何对象ID。
	//! \param nID 几何对象ID[in]。
	virtual void SetID(OgdcInt nID) ;	
	
	//! \brief 获取几何对象，外面不能释放。
	//! \return 是否取几何对象成功，成功返回几何对象，失败返回NULL。
	virtual OgdcElement* GetElement() ;	
	
	//! \brief 设置当前特征要素的几何对象，设置以后外面不能释放。
	//! \param pElement 几何对象指针[in]。
	virtual void SetElement(OgdcElement* pElement) ;	
	
	//! \brief 根据字段序号获取字段值。
	//! \param nIndex 字段序号[in]。
	virtual OgdcBool GetValue(OgdcInt nIndex, OgdcVariant& varValue) ;
	
	//! \brief 根据字段名获取字段值。
	//! \param strFieldName 字段名[in]。
	virtual OgdcBool GetValue(const OgdcString& strFieldName, OgdcVariant& varValue) ;	
	
	//! \brief 根据字段序号设置字段值。
	//! \param nIndex 字段序号[in]。
	//! \param varValue 字段值 [in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool SetValue(OgdcInt nIndex, const OgdcVariant& varValue) ;    
	
	//! \brief 根据字段名设置字段值。
	//! \param strFieldName 字段名[in]。
	//! \param varValue 字段值 [in]。
	//! \return 是否设置成功，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool SetValue(const OgdcString& strFieldName, const OgdcVariant& varValue) ;		
	
public:
	//! \brief 设置字段信息。
	//! \param fieldInfos 字段信息集合[in]。
	virtual void SetFieldInfos(const OgdcFieldInfos& fieldInfos);	
	
	//! \brief 返回字符串编码方式。
	//! \return 返回编码的方式。
	virtual OgdcString::Charset GetCharset() const;	
	
	//! \brief 设置字符串编码方式。
	//! \param nCharset [in]。。
	virtual void SetCharset(OgdcString::Charset nCharset);

protected:
	//! \brief 保存每个字段的值的数组
	OgdcArray<OgdcByte*> m_fieldValues;
	
	//! \brief 保存特征要素的几何对象指针
	OgdcElement* m_pElement;

	//! \brief 保存特征要素的ID
	OgdcInt m_nID;

	//! \brief 编码方式的值
	OgdcString::Charset m_nCharset;

};
}
#endif // !defined(AFX_OGDCFEATUREMDB_H__C646C050_13AF_4332_A835_9D5BE49B5571__INCLUDED_)
