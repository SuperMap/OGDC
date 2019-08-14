//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     7.2           
//                                                           
//!  \file UGExchangeParamsManager.h
//!  \brief 数据转换参数管理。
//!  \details 主要包括UGExchangeParamsManager转换参数管理类。
//!  \author 数据转换组。
//!  \attention 
//!   Copyright (c)   SuperMap Software Limited Company.  <br> 
//!   All Rights Reserved.
//!  \version 7.2
//////////////////////////////////////////////////////////////////////////

#ifndef AFX_UGEXCHANGEPARAMSMANAGER_H__INCLUDED_
#define AFX_UGEXCHANGEPARAMSMANAGER_H__INCLUDED_

#include "Stream/ugdefs.h"
#include "FileParser/UGFileParserConfigParams.h"

namespace UGC {

	//! \brief 转换参数管理类。
	//! \remarks 包括数据导入导出参数构建、释放、有效性检测。
	class FILEPARSER_API UGExchangeParamsManager  
	{
	public:
		//! \brief 构造函数。
		//! \remarks 构造一个新的转换参数管理对象。
		UGExchangeParamsManager();

		//! \brief 析构函数。
		//! \remarks 释放转换参数管理对象。
		virtual ~UGExchangeParamsManager();

	public:
		//! \brief 构建对应格式的导入参数。
		//! \param nType [in] 文件格式类型。
		//! \param strPwd [in] 密码。
		//! \return 成功返回导入参数，失败返回NULL。
		//! \remarks 用户需释放内存，密码只针对SIT文件格式起作用。
		//! \attention 文件类型敏感。
		static UGImportParams* MakeImportParams(UGint nType, UGString strPwd=_U(""));

		//! \brief 构建对应格式的导出参数。
		//! \param nType [in] 文件格式类型。
		//! \return 成功返回导出参数，失败返回NULL。
		//! \remarks 用户需释放内存。
		//! \attention 文件类型敏感。
		static UGExportParams* MakeExportParams(UGint nType);

		//! \brief 释放导入参数类对象。
		//! \param &pImportParams [in] 导入参数对象。
		static void DestoryImportParams(UGImportParams* &pImportParams);

		//! \brief 释放导出参数类对象。
		//! \param &pExportParams [in] 导出参数对象。
		static void DestoryExportParams(UGExportParams* &pExportParams);

		//! \brief 导入参数有效性检查。
		//! \param nFileType [in] 文件格式类型。
		//! \param impParams [in] 导入参数对象。
		//! \return true导入参数有效，false导入参数无效。
		static UGbool IsValidImportParams(UGint nFileType, const UGImportParams& impParams);

		//! \brief 导出参数有效性检查。
		//! \param nFileType [in] 文件格式类型。
		//! \param expParams [in] 导出参数对象。
		//! \return true导出参数有效，false导处参数无效。
		static UGbool IsValidExportParams(UGint nFileType, const UGExportParams& expParams);
	};

}
#endif // !defined(AFX_UGEXCHANGEPARAMSMANAGER_H__AB19AA8C_B64E_42F0_BFF3_DDE67CDBA62F__INCLUDED_)

