// OGDCProviderMdb.h : main header file for the OGDCProviderMdb DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// COGDCProviderMdbApp
// See OGDCProviderMdb.cpp for the implementation of this class
//

class COGDCProviderMdbApp : public CWinApp
{
public:
	COGDCProviderMdbApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
