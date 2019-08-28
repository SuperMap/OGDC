//
//  Author:  Peter Bonsma
//  $Date: 2015-05-14 23:59:59 +0000 (Thu, 14 Jan 2015) $
//  $Revision: 3999 $
//  Project: IFC Engine DLL and Geometry Kernel
//
//  In case of use of the DLL:
//  be aware of license fee for use of this DLL when used commercially
//  more info for commercial use:  peter.bonsma@rdf.bg
//

#ifndef __RDF_LTD__ENGDEF_H
#define __RDF_LTD__ENGDEF_H


#include <stdlib.h>


//#if !defined (_WINDOWS) && (defined (WIN32) || defined (WIN64))
//#define _WINDOWS
//#endif

#ifndef __ANDROID__
#if defined (_WINDOWS) && (_MSC_VER <= 1600)
	// I guess that this distinction would be required only for some vc++ versions.
#if !defined (_STDINT) && !defined (_STDINT_PROXY_)
#define _STDINT_PROXY_
	typedef __int64 int64_t;
	typedef __int32 int32_t;
	typedef unsigned __int64 uint64_t;
	typedef unsigned __int32 uint32_t;

	#define INTMAX_MAX		0x7fffffffffffffff
	#define UINTMAX_MAX		0xffffffffffffffffU
#endif
#else
	#include <stdint.h>
#endif
#endif

#if defined(WIN64) || defined(__LP64__)
	typedef int64_t int_t;
#ifndef __ANDROID__
	typedef uint64_t uint_t;
#endif
#else
	typedef int32_t int_t;
#ifndef __ANDROID__
	typedef uint32_t uint_t;
#endif
#endif

/*
#if defined(WIN64) || defined(__LP64__)
	inline char * int_t_toa_s(int_t		i,
							  char		* s,
							  size_t	n,
							  int		r)
	{
	#if defined (_MSC_VER)
		if (_i64toa_s(i,s,n,r) != 0) {
			if (s && n > 0) {
				s[0] = '0';
			}
		}
	#else
			sprintf(s, n, "%lld", i)
	#endif
		return s;
	}
#else
	inline char * int_t_toa_s(int_t		i,
							  char		* s,
							  size_t	n,
							  int		r)
	{
	#if defined (_MSC_VER)
		if (_itoa_s(i,s,n,r) != 0) {
			if (s && n > 0) {
				s[0] = '0';
			}
		}
		return s;
	#else
		return itoa(i,s,r);
	#endif
	}	
#endif	//	*/

#ifndef _CHAR_NAT_
#define _CHAR_NAT_
	#ifdef	_UNICODE

		typedef wchar_t						char_t;

		#define STR_LIT(a)					L##a
		#define CPS_LIT(a)					((char*) STR_LIT(a))
		#define CPS_VAR(a)					((char*) (a))

		#define STR_HND(s)					((char**) (&(s)))

		#define STRLEN(s)					wcslen(s)
		#define STRCPY(d,s)					STRCPYS((d), STRLEN(s) + 1U, (s))

		#if defined (_MSC_VER)
			#define STOI(s)					_wtoi(s)
			#define STRCATS(d, n, s)		wcscat_s((d), (n), (s))
			#define STRCPYS(d, n, s)		wcscpy_s((d), (n), (s))
		#else
			// Could be an inline function...
			#define STOI(s)					((int_t)wcstol((s), 0, 10))
		#endif
		#if defined (_MSC_VER)
			#define ITOS_SEC(i,s,n,r)		_itow_s((i),(s),(n), (r))

			#if defined(WIN64) || defined(__LP64__)
				#define ITTOS_SEC(i,s,n,r)	_i64tow_s((i),(s),(n), (r))
			#else
				#define ITTOS_SEC(i,s,n,r)	_itow_s((i),(s),(n), (r))
			#endif
		#else
			// Could be an inline function...
			#define ITOS_SEC(i,s,n,r)		swprintf((s), (n), L"%d", (i))

			#if defined(WIN64) || defined(__LP64__)
			#else
			#endif
		#endif
		#if defined (_MSC_VER)
			#define FOPEN_SEC(f,n,m)		_wfopen_s((f),(n),(m))
			#define FGET_SEC(s,n,f)			fgetws((s),(n),(f))
		#endif
		#define IS_UNICODE					true
		#define sdaiTEXT					sdaiUNICODE
		#define sdaiOpenModelBNTEXT			sdaiOpenModelBNUnicode
		#define sdaiCreateModelBNTEXT		sdaiCreateModelBNUnicode
		#define sdaiSaveModelBNTEXT			sdaiSaveModelBNUnicode
		#define sdaiSaveModelAsXmlBNTEXT	sdaiSaveModelAsXmlBNUnicode
	#else

		typedef char						char_t;

		#define STR_LIT(a)					a
		#define CPS_LIT(a)					(a)
		#define CPS_VAR(a)					(a)

		#define STR_HND(s)					(&(s))

		#define STRLEN(s)					strlen(s)
		#define STRCPY(d,s)					StrCpy((d),(s))

		#if defined (_MSC_VER)
			#define STOI(s)					atoi(s)
			#define STRCATS(d, n, s)		strcat_s((d), (n), (s))
			#define STRCPYS(d, n, s)		strcpy_s((d), (n), (s))
		#else
			// Could be an inline function...
			#define STOI(s)					((int_t)strtol((s), 0, 10))
		#endif
		#if defined (_MSC_VER)
			#define ITOS_SEC(i,s,n,r)		_itoa_s((i),(s),(n), (r))

			#if defined(WIN64) || defined(__LP64__)
				#define ITTOS_SEC(i,s,n,r)	_i64toa_s((i),(s),(n), (r))
			#else
				#define ITTOS_SEC(i,s,n,r)	_itoa_s((i),(s),(n), (r))
			#endif
		#else
			// Could be an inline function...
			#define ITOS_SEC(i,s,n,r)		(sprintf((s), "%d", (i)), (s))

			#if defined(WIN64) || defined(__LP64__)
			#else
			#endif
		#endif
		#if defined (_MSC_VER)
			#define FOPEN_SEC(f,n,m)		fopen_s((f),(n),(m))
			#define FGET_SEC(s,n,f)			fgets((s),(n),(f))
		#endif
		#define IS_UNICODE					false
		#define sdaiTEXT					sdaiSTRING
		#define sdaiOpenModelBNTEXT			sdaiOpenModelBN
		#define sdaiCreateModelBNTEXT		sdaiCreateModelBN
		#define sdaiSaveModelBNTEXT			sdaiSaveModelBN
		#define sdaiSaveModelAsXmlBNTEXT	sdaiSaveModelAsXmlBN
	#endif
#endif


#endif
