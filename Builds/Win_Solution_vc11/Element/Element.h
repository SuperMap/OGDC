#if !defined(__UGC_ELEMENT_H__)
#define __UGC_ELEMENT_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "../Base/Base.h"

#ifdef _DEBUG
	#pragma message("自动连接动态库 SuElementd.dll ...")
	#pragma comment(lib,"SuElementd.lib") 

#else
	#pragma message("自动连接动态库 SuElement.dll ...") 
	#pragma comment(lib,"SuElement.lib") 
#endif

#endif //__UGC_ELEMENT_H__