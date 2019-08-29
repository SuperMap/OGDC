// SuperMapMenuDec.h: plugin class for 3DMAX.
////////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file headerfile.h
//!  \brief Class 插件类的描述类。
//!  \details 描述此插件类的类别和生成插件类，继承3DMAX提供的ClassDesc2。
//!  \author sunye。
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

#include "max.h"
#include "iparamb2.h"
#include "UGMenuGUP.h" 
//#include "UGSuActions.h"
#ifndef MenuDec_H
#define MenuDec_H

//该类方法描述参看MAXSDK，这是Max与插件进行交互的结构 Class_ID由Class_ID专门的生成器获取，由完整版的3dsMax安装版中提供
class UGMenuDec:public ClassDesc2{
public: 
	UGMenuDec(HINSTANCE outHInstance){hInstance=outHInstance;}

	int IsPublic(){ return TRUE; }

	void* Create(BOOL loading = FALSE) { return new UGMenuGUP(hInstance);} 

	const TCHAR* ClassName(){ return L"SuperMapMenuDec"; }

	SClass_ID SuperClassID(){ return GUP_CLASS_ID; } 

	Class_ID ClassID(){ return Class_ID(0x153f7592, 0x340a6dfd); }

	const TCHAR* Category(){ return  L"SuperMap"; }

	int NumActionTables(){return 1;}

	ActionTable* GetActionTable(int i){
		UGSuActions suAction(hInstance);
		return suAction.GetActions();
	}
private:
	HINSTANCE hInstance;	
};
#endif
