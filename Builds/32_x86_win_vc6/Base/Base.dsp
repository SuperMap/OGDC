# Microsoft Developer Studio Project File - Name="Base" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Base - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Base.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Base.mak" CFG="Base - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Base - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Base - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Base - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../../Include" /I "../../../LibShared/SGISTL40/stlport/" /I "../../../LibShared/icu/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "BASE_EXPORTS" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 icuuc.lib icuin.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../Bin/SuBase600.dll" /implib:"../Bin/SuBase600.lib" /libpath:"../Bin/" /libpath:"../../../LibShared/SGISTL40/lib/" /libpath:"../../../LibShared/icu/lib/VC6"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Base - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../Include" /I "../../../LibShared/SGISTL40/stlport/" /I "../../../LibShared/icu/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "BASE_EXPORTS" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 icuucd.lib icuind.lib /nologo /subsystem:windows /dll /pdb:"Debug/SuBase600.pdb" /debug /machine:I386 /nodefaultlib:"MSVCRT" /out:"../BinD/SuBase600d.dll" /implib:"../BinD/SuBase600d.lib" /pdbtype:sept /libpath:"../BinD/" /libpath:"../../../LibShared/SGISTL40/lib/" /libpath:"../../../LibShared/icu/lib/VC6"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Base - Win32 Release"
# Name "Base - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Base.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Base\OgdcColorDictTable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Base\OgdcColorset.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Base\OgdcColorTable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Base\ogdcdefs.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Base\OgdcPlatform.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Base\OgdcPoint.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Base\OgdcPoint2D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Base\OgdcPoint3D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Base\OgdcRect.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Base\OgdcRect2D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Base\OgdcSize.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Base\OgdcSize2D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Base\OgdcSize3D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Base\OgdcString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Base\OgdcStyle.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Base\OgdcSystem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Base\OgdcTextStyle.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Base\OgdcTime.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Base\OgdcVariant.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Base.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Base\OgdcArray.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Base\ogdccolordef.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Base\OgdcColorDictTable.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Base\OgdcColorset.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Base\OgdcColorTable.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Base\ogdcdefs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Base\OgdcDict.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Base\ogdcexports.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Base\OgdcList.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Base\OgdcPlatform.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Base\OgdcPoint.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Base\OgdcPoint2D.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Base\OgdcPoint3D.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Base\OgdcRect.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Base\OgdcRect2D.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Base\OgdcSize.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Base\OgdcSize2D.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Base\OgdcSize3D.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Base\OgdcString.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Base\OgdcStyle.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Base\OgdcSystem.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Base\OgdcTextStyle.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Base\OgdcTime.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Base\OgdcVariant.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# End Target
# End Project
