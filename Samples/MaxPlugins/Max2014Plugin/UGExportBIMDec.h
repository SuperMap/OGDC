//UGExportBIMDec.h: plugin class for 3DMAX.
////////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file headerfile.h
//!  \brief Class 描述BIM生成类的信息。
//!  \details 此类是与3dmax通讯的类，接口由3dmax提供。
//!  \author ljz。
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#ifndef UGExportBIMDec_H
#define UGExportBIMDec_H
#include "iparamb2.h"
#include "UGBIMExporter.h"
//该类方法描述参看MAXSDK，这是Max与插件进行交互的结构 Class_ID由Class_ID专门的生成器获取，由完整版的3dsMax安装版中提供
class UGExportBIMDec:public ClassDesc2{
public: 
	int IsPublic(){ return TRUE; }

	void* Create(BOOL loading = FALSE) {return new UGC::UGBIMExporter;} 

	const TCHAR* ClassName(){ return _U("SuperMapMenuDec"); }

	SClass_ID SuperClassID(){ return SCENE_EXPORT_CLASS_ID; } 

	Class_ID ClassID(){ return Class_ID(0x69e348c, 0x4f661e5b); }

	const TCHAR* Category(){ return _U("SuperMap"); }

};

#endif