# Microsoft Developer Studio Project File - Name="Element" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Element - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Element.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Element.mak" CFG="Element - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Element - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Element - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Element - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../../Include" /I "../../../LibShared/freetype/include" /I "../../../LibShared/SGISTL40/stlport/" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "ELEMENT_EXPORTS" /FD /c
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
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"../Bin/SuElement600.dll" /implib:"../Bin/SuElement600.lib" /libpath:"../Bin/" /libpath:"../../../LibShared/SGISTL40/lib/"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Element - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../Include" /I "../../../LibShared/SGISTL40/stlport/" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "ELEMENT_EXPORTS" /FD /GZ /c
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
# ADD LINK32 /nologo /subsystem:windows /dll /pdb:"Debug/SuElement600.pdb" /debug /machine:I386 /nodefaultlib:"MSVCRT" /out:"../BinD/SuElement600d.dll" /implib:"../BinD/SuElement600d.lib" /pdbtype:sept /libpath:"../BinD/" /libpath:"../../../LibShared/SGISTL40/lib/"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Element - Win32 Release"
# Name "Element - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Element.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Element\OgdcCoordSys.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Element\OgdcElemArc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Element\OgdcElemBSpline.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Element\OgdcElemCardinal.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Element\OgdcElemChord.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Element\OgdcElemCircle.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Element\OgdcElemCompound.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Element\OgdcElemCurve.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Element\OgdcElemEllipse.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Element\OgdcElemEllipticArc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Element\OgdcElement.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Element\OgdcElemFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Element\OgdcElemLine.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Element\OgdcElemLine3D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Element\OgdcElemLineM.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Element\OgdcElemMultiPoint.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Element\OgdcElemMultiPoint3D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Element\OgdcElemPie.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Element\OgdcElemPoint.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Element\OgdcElemPoint3D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Element\OgdcElemRect.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Element\OgdcElemRectRound.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Element\OgdcElemRegion.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Element\OgdcElemRegion3D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Element\OgdcElemText.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Element\OgdcElemUser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Element\OgdcFeature.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Element\OgdcFieldInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Element\OgdcPrjParams.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Element\OgdcRasterBlock.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\Element\OgdcRasterScanline.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Element.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Element\OgdcCoordSys.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Element\OgdcElemArc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Element\OgdcElemBSpline.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Element\OgdcElemCardinal.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Element\OgdcElemChord.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Element\OgdcElemCircle.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Element\OgdcElemCompound.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Element\OgdcElemCurve.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Element\OgdcElemEllipse.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Element\OgdcElemEllipticArc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Element\OgdcElement.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Element\OgdcElemFactory.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Element\OgdcElemLine.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Element\OgdcElemLine3D.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Element\OgdcElemLineM.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Element\OgdcElemMultiPoint.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Element\OgdcElemMultiPoint3D.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Element\OgdcElemPie.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Element\OgdcElemPoint.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Element\OgdcElemPoint3D.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Element\OgdcElemRect.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Element\OgdcElemRectRound.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Element\OgdcElemRegion.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Element\OgdcElemRegion3D.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Element\OgdcElemText.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Element\OgdcElemUser.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Element\OgdcFeature.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Element\OgdcFieldInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Element\OgdcPrjParams.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Element\OgdcRasterBlock.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Include\Element\OgdcRasterScanline.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# End Target
# End Project
