#if !defined(__UGC_ELEMENT_H__)
#define __UGC_ELEMENT_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "../Base/Base.h"

#ifdef _DEBUG
	#pragma message("自动连接动态库 SuElement600d.dll ...")
	#pragma comment(lib,"SuElement600d.lib") 

#else
	#pragma message("自动连接动态库 SuElement600.dll ...") 
	#pragma comment(lib,"SuElement600.lib") 
#endif

#endif //__UGC_ELEMENT_H__