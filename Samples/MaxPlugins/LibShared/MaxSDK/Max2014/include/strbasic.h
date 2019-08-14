//**************************************************************************/
// Copyright (c) 1994-2011 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/

#pragma once

// To set up Max to use Unicode, define _UNICODE, and don't define _MBCS
// To set up Max to use multi-byte character sets, define _MBCS and 
//              don't define _UNICODE
// To set up Max to use single-byte ANSI character strings, don't define
//              either _UNICODE or _MBCS

#if defined(_UNICODE) && defined(_MBCS)
#error Cannot compile with both _UNICODE and _MBCS enabled!
#endif

#ifndef _MBCS
#define _MBCS   // if Unicode is off, turn on multi-byte character support
#endif


#ifdef _UNICODE

	#ifdef _MBCS
	#undef _MBCS    // can't have both Unicode and MBCS at once -- Unicode wins
	#endif

	#undef UNICODE
	#define UNICODE

	#undef STRCONST
	#define STRCONST L

#endif

// Bring in the generic international text header file
#include <tchar.h>

#undef MCHAR_IS_WCHAR
#undef MCHAR_IS_CHAR

#ifdef _UNICODE
	// If the module uses Unicode and needs to translate to
	// an MBCS Max, define _UNICODE_MODULE_FOR_MBCS_MAX
	#ifdef _UNICODE_MODULE_FOR_MBCS_MAX
		#define MCHAR_IS_CHAR
	#else
		#define MCHAR_IS_WCHAR
	#endif
#else
	#define MCHAR_IS_CHAR
#endif

/*! MBCS/Unicode helper defines
std::wofstream doesn't mix well with Unicode. If you try to output a character that is not representable 
in the current (window user) code page, the stream is flagged as 'in error', and no further 
output to that stream occurs. 
Plus, file output encoding should be that specified by the user, accessible via Interface14::DefaultTextSaveCodePage
Thus, it is recommended that M_STD_FOSTREAM not be used. It is much better to use the 
TextFile::Reader/Writer classes and use the codepage accessed via Interface14::DefaultTextSaveCodePage. 
Likewise, the use std::wifstream (and _tfopen/fopen in text mode) is not recommended when reading string data as the
current (window user) code page will be used. File input encoding should that specified by the user, accessible via 
Interface14::DefaultTextLoadCodePage
Thus, it is recommended that M_STD_FISTREAM (and _tfopen/fopen in text mode) not be used. It is much better to use the 
TextFile::Reader/Writer classes and use the codepage accessed via Interface14::DefaultTextSaveCodePage. 
!*/
#ifdef MCHAR_IS_WCHAR
	#define MCHAR					wchar_t
	#define LPCMSTR					const MCHAR*
	#define LPMSTR					MCHAR*
	#define _M_TEXT(x)				L ## x
	#define M_STD_STRING			std::wstring
	#define M_LOADSTRING			::LoadStringW
	#define M_STD_OSTRINGSTREAM		std::wostringstream
	#define M_STD_ISTRINGSTREAM		std::wistringstream
	#define M_STD_CERR				std::wcerr
	#define M_STD_CIN				std::wcin
	#define M_STD_OSTREAM			std::wostream
	#define M_STD_ISTREAM			std::wistream
	#define M_STD_COUT				std::wcout
	#define M_STD_FOSTREAM			std::wofstream
	#define M_STD_FISTREAM			std::wifstream
	#define M_STD_ISTRSTREAM		std::wistringstream
#else
	#define MCHAR					char
	#define LPCMSTR					const MCHAR*
	#define LPMSTR					MCHAR*
	#define _M_TEXT(x)				x
	#define M_STD_STRING			std::string
	#define M_LOADSTRING			::LoadStringA
	#define M_STD_OSTRINGSTREAM		std::ostringstream
	#define M_STD_ISTRINGSTREAM		std::istringstream
	#define M_STD_CERR				std::cerr
	#define M_STD_CIN				std::cin
	#define M_STD_OSTREAM			std::ostream
	#define M_STD_ISTREAM			std::istream
	#define M_STD_COUT				std::cout
	#define M_STD_FOSTREAM			std::ofstream
	#define M_STD_FISTREAM			std::ifstream
	#define M_STD_ISTRSTREAM		std::istrstream
#endif

/**
* The 'EOF' value is unicode/mbcs build dependent for _puttc, _puttch, 
* _ungettc, _ungettch, _ungettch_nolock, _fputts, _fputtc, _gettc, and 
* _fgettc (and maybe other methods)
*/
#ifdef MCHAR_IS_WCHAR
	#define M_EOF					WEOF
#else
	#define M_EOF					EOF
#endif

//! Used to wrap string literals
#define _M(x)		_M_TEXT(x)

// These macros work in conjunction with the VC conversion macros (A2W, A2T, W2T, etc)
// See the MSDN documentation for details.  If you get compile errors such as
// 
// error C2065: '_lpa' : undeclared identifier
// error C2065: '_convert' : undeclared identifier
// error C2065: '_acp' : undeclared identifier
// 
// when trying to use any of these macros, it's probably because there's a
// 
// USES_CONVERSION
// 
// missing in your function.

// M2A: Build-specific to ASCII
// A2M: ASCII to build-specific
// M2W: Build-specific to wide char
// W2M: Wide char to build-specific
// M2T: Max definition of TCHAR to current project definition of TCHAR
// T2M: current project definition of TCHAR to Max definition of TCHAR 

// NOTE: converted strings are stored in a stack-based buffer. so don't say
// something like:
//   return A2M(msg.data());

// NOTE: The CStr/WStr/MaxString conversion methods should be used instead of
// the VC conversion macros. In some (unusual) cases, the VC conversion macros
// return incorrect results.

#ifdef MCHAR_IS_WCHAR
	#define M2A(p)					CW2A(p)
	#define A2M(p)					CA2W(p)
	#define M2W(p)					(p)
	#define W2M(p)					(p)
	#define M2W_UTF(p)				(p)
	#define W2M_UTF(p)				(p)
	#define M2T(p)					(p)
	#define T2M(p)					(p)
	#define A2CT_UTF(lpa)			CA2W(lpa, CP_UTF8)
	#define T2CA_UTF(lpw)			CW2A(lpw, CP_UTF8)
	#define A2CM_CP(lpa)			CA2W(lpa, CP_UTF8) //These two macros are for when you are not sure the encoding, may change later
	#define M2CA_CP(lpw)			CW2A(lpw, CP_UTF8)
#else
	#define M2A(p)					(p)
	#define A2M(p)					(p)
	#define M2W(p)					CA2W(p)
	#define W2M(p)					CW2A(p)
	#define M2W_UTF(lpa)			CA2W(lpa, CP_UTF8)
	#define W2M_UTF(lpw)			CW2A(lpw, CP_UTF8)
	#define M2T(p)					CA2T(p)
	#define T2M(p)					CT2A(p)
	#define A2CT_UTF(lpa)			CW2A(CA2W(lpa, CP_UTF8), CP_ACP)
	#define T2CA_UTF(lpw)			CW2A(CA2W(lpw, CP_ACP), CP_UTF8)
	#define A2CM_CP(lpa)			CW2A(CA2W(lpa, CP_UTF8), CP_ACP) //These two macros are for when you are not sure the encoding, may change later
	#define M2CA_CP(lpw)			CW2A(CA2W(lpw, CP_ACP), CP_UTF8) 
#endif


