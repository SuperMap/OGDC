#if !defined(__UGC_BASE_H__)
#define __UGC_BASE_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifdef _DEBUG
	#pragma message("自动连接动态库 SuBased.dll ...")
	#pragma comment(lib,"SuBased.lib") 
#else
	#pragma message("自动连接动态库 SuBase.dll ...") 
	#pragma comment(lib,"SuBase.lib") 
#endif

#endif //__UGC_BASE_H__