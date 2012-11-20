# Microsoft Developer Studio Project File - Name="Viewer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Viewer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Viewer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Viewer.mak" CFG="Viewer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Viewer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Viewer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Viewer - Win32 Release"

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
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../../Include" /I "../../../LibShared/SGISTL40/stlport/" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"../Bin/Viewer.exe" /libpath:"../Bin/" /libpath:"../../../LibShared/SGISTL40/lib/"

!ELSEIF  "$(CFG)" == "Viewer - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../Include" /I "../../../LibShared/SGISTL40/stlport/" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"../BinD/Viewer.exe" /pdbtype:sept /libpath:"../BinD/" /libpath:"../../../LibShared/SGISTL40/lib/"

!ENDIF 

# Begin Target

# Name "Viewer - Win32 Release"
# Name "Viewer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\Samples\Viewer\CopyDatasetDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\Viewer\DatasetDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\Viewer\Drawing.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\Viewer\LayersManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\Viewer\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\Viewer\ObjectAttDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\Viewer\OpenDBDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\Viewer\Viewer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\Viewer\Viewer.rc
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\Viewer\ViewerDoc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\Viewer\ViewerView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\Viewer\ViewerWnd.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\Samples\Viewer\CopyDatasetDlg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\Viewer\DatasetDlg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\Viewer\Drawing.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\Viewer\LayersManager.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\Viewer\MainFrm.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\Viewer\ObjectAttDlg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\Viewer\OpenDBDlg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\Viewer\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\Viewer\Viewer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\Viewer\ViewerDoc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\Viewer\ViewerView.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\Viewer\ViewerWnd.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\..\..\Samples\Viewer\res\Pan.cur
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\Viewer\res\Select.cur
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\Viewer\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\Viewer\res\Viewer.ico
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\Viewer\res\ViewerDoc.ico
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\Viewer\res\ZoomIn.cur
# End Source File
# Begin Source File

SOURCE=..\..\..\Samples\Viewer\res\ZoomOut.cur
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\Samples\Viewer\ReadMe.txt
# End Source File
# End Target
# End Project
# Section Viewer : {9D94318A-4BDB-421B-8882-02A49F13768D}
# 	1:21:IDD_DIALOG_DATASOURCE:102
# 	2:16:Resource Include:resource.h
# 	2:21:CLASS: CUGDatabaseDlg:CUGDatabaseDlg
# 	2:10:ENUM: enum:enum
# 	2:21:IDD_DIALOG_DATASOURCE:IDD_DIALOG_DATASOURCE
# 	2:17:UGDatabaseDlg.cpp:UGDatabaseDlg.cpp
# 	2:15:UGDatabaseDlg.h:UGDatabaseDlg.h
# 	2:19:Application Include:viewer.h
# End Section
# Section Viewer : {CC1BC2EE-069A-4CE5-BB03-7B222DE85138}
# 	1:18:IDD_DLG_MAP_SELECT:103
# 	2:16:Resource Include:resource.h
# 	2:10:ENUM: enum:enum
# 	2:16:MapSelectDlg.cpp:MapSelectDlg.cpp
# 	2:18:IDD_DLG_MAP_SELECT:IDD_DLG_MAP_SELECT
# 	2:14:MapSelectDlg.h:MapSelectDlg.h
# 	2:20:CLASS: CMapSelectDlg:CMapSelectDlg
# 	2:19:Application Include:viewer.h
# End Section
