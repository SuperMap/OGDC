#if !defined(__UGC_OGDC_H__)
#define __UGC_OGDC_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//OGDC 小版本号
#define OGDCVERSION 9222

#include "../Element/Element.h"

#ifdef _DEBUG
	#pragma message("自动连接动态库 SuOGDC600d.dll ...")
	#pragma comment(lib,"SuOGDC600d.lib") 

#else
	#pragma message("自动连接动态库 SuOGDC600.dll ...") 
	#pragma comment(lib,"SuOGDC600.lib") 
#endif

#endif //__UGC_OGDC_H__