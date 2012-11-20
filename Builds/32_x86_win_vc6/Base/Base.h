#if !defined(__UGC_BASE_H__)
#define __UGC_BASE_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifdef _DEBUG
	#pragma message("自动连接动态库 SuBase600d.dll ...")
	#pragma comment(lib,"SuBase600d.lib") 
#else
	#pragma message("自动连接动态库 SuBase600.dll ...") 
	#pragma comment(lib,"SuBase600.lib") 
#endif

#endif //__UGC_BASE_H__