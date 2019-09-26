
// OGDC__XML+OSGToUDB.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号



// COGDC__XMLOSGToUDBApp:
// 有关此类的实现，请参阅 OGDC__XML+OSGToUDB.cpp
//
using namespace std;
class COGDC__XMLOSGToUDBApp : public CWinApp
{
public:
	COGDC__XMLOSGToUDBApp();
public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
};

extern COGDC__XMLOSGToUDBApp theApp;