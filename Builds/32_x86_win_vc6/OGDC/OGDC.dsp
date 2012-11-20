# Microsoft Developer Studio Project File - Name="OGDC" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=OGDC - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "OGDC.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OGDC.mak" CFG="OGDC - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OGDC - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "OGDC - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "OGDC - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../../Include" /I "../../../LibShared/freetype/include" /I "../../../LibShared/SGISTL40/stlport/" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "OGDC_EXPORTS" /FD /c
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
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"../Bin/SuOGDC600.dll" /implib:"../Bin/SuOGDC600.lib" /libpath:"../Bin/" /libpath:"../../../LibShared/SGISTL40/lib/"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "OGDC - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../Include" /I "../../../LibShared/freetype/include" /I "../../../LibShared/SGISTL40/stlport/" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "OGDC_EXPORTS" /FD /GZ /c
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
# ADD LINK32 /nologo /subsystem:windows /dll /pdb:"Debug/SuOGDC600.pdb" /debug /machine:I386 /out:"../BinD/SuOGDC600d.dll" /implib:"../BinD/SuOGDC600d.lib" /pdbtype:sept /libpath:"../BinD/" /libpath:"../../../LibShared/SGISTL40/lib/"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "OGDC - Win32 Release"
# Name "OGDC - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\OGDC.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\OGDC\OgdcDataset.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\OGDC\OgdcDatasetRaster.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\OGDC\OgdcDatasetRasterInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\OGDC\OgdcDatasetVector.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\OGDC\OgdcDatasetVectorInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\OGDC\OgdcDataSource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\OGDC\OgdcDsConnection.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\OGDC\OgdcMetaData.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\OGDC\OgdcMetaDatabase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\OGDC\OgdcProvider.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\OGDC\OgdcProviderManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\OGDC\OgdcQueryDef.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\OGDC\OgdcRecordset.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\OGDC.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\OGDC\OgdcDataset.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\OGDC\OgdcDatasetRaster.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\OGDC\OgdcDatasetRasterInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\OGDC\OgdcDatasetVector.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\OGDC\OgdcDatasetVectorInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\OGDC\OgdcDataSource.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\OGDC\OgdcDsConnection.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\OGDC\OgdcMetaData.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\OGDC\OgdcMetaDatabase.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\OGDC\OgdcProvider.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\OGDC\OgdcProviderManager.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\OGDC\OgdcQueryDef.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\OGDC\OgdcRecordset.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# End Target
# End Project
