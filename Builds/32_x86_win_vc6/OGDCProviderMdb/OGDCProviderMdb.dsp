# Microsoft Developer Studio Project File - Name="OGDCProviderMdb" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=OGDCProviderMdb - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "OGDCProviderMdb.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OGDCProviderMdb.mak" CFG="OGDCProviderMdb - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OGDCProviderMdb - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "OGDCProviderMdb - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "OGDCProviderMdb - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../../Include" /I "../../../LibShared/SGISTL40/stlport/" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "OGDC_EXPORTS" /FD /c
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
# ADD LINK32 SuBase600.lib SuElement600.lib SuOGDC600.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../Bin/SuOGDCProviderMdb600.odx" /implib:"../Bin/SuOGDCProviderMdb600.lib" /libpath:"../Bin/" /libpath:"../../../LibShared/SGISTL40/lib/"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "OGDCProviderMdb - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../Include" /I "../../../LibShared/SGISTL40/stlport/" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "OGDC_EXPORTS" /FD /GZ /c
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
# ADD LINK32 SuBase600d.lib SuElement600d.lib SuOGDC600d.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../BinD/SuOGDCProviderMdb600d.odx" /implib:"../BinD/SuOGDCProviderMdb600d.lib" /pdbtype:sept /libpath:"../BinD/" /libpath:"../../../LibShared/SGISTL40/lib/"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "OGDCProviderMdb - Win32 Release"
# Name "OGDCProviderMdb - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\Samples\OGDCProviderMdb\OgdcDatasetRasterMdb.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\OGDCProviderMdb\OgdcDatasetVectorMdb.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\OGDCProviderMdb\OgdcDataSourceMdb.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\OGDCProviderMdb\OgdcFeatureMdb.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\OGDCProviderMdb\OgdcHelperMdb.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\OGDCProviderMdb\OgdcRecordsetMdb.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\Samples\OGDCProviderMdb\OgdcDatasetRasterMdb.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\OGDCProviderMdb\OgdcDatasetVectorMdb.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\OGDCProviderMdb\OgdcDataSourceMdb.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\OGDCProviderMdb\OgdcFeatureMdb.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\OGDCProviderMdb\OgdcHelperMdb.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\OGDCProviderMdb\OgdcMacroDefineMdb.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\OGDCProviderMdb\OgdcRecordsetMdb.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
