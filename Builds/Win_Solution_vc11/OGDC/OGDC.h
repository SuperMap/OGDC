#if !defined(__UGC_OGDC_H__)
#define __UGC_OGDC_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//OGDC 小版本号
#define OGDCVERSION 10324

#include "../Element/Element.h"

#ifdef _DEBUG
	#pragma message("自动连接动态库 SuOGDCd.dll ...")
	#pragma comment(lib,"SuOGDCd.lib") 

#else
	#pragma message("自动连接动态库 SuOGDC.dll ...") 
	#pragma comment(lib,"SuOGDC.lib") 
#endif

#endif //__UGC_OGDC_H__