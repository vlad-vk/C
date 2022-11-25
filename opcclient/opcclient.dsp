# Microsoft Developer Studio Project File - Name="OPCClient" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=OPCClient - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "OPCClient.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OPCClient.mak" CFG="OPCClient - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OPCClient - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "OPCClient - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "OPCClient - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "OPCClient - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ole32.lib comsupp.lib ws2_32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "OPCClient - Win32 Release"
# Name "OPCClient - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\backclass.cpp
# End Source File
# Begin Source File

SOURCE=.\MyListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\OPCClient.cpp
# End Source File
# Begin Source File

SOURCE=.\OPCClient.rc
# End Source File
# Begin Source File

SOURCE=.\OPCClientDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\backclass.h
# End Source File
# Begin Source File

SOURCE=.\MyListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\OPCClient.h
# End Source File
# Begin Source File

SOURCE=.\OPCClientDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\OPCClient.ico
# End Source File
# Begin Source File

SOURCE=.\res\OPCClient.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section OPCClient : {BDD1F051-858B-11D1-B16A-00C0F0283628}
# 	2:5:Class:CColumnHeader
# 	2:10:HeaderFile:columnheader1.h
# 	2:8:ImplFile:columnheader1.cpp
# End Section
# Section OPCClient : {BEF6E003-A874-101A-8BBA-00AA00300CAB}
# 	2:5:Class:COleFont
# 	2:10:HeaderFile:font.h
# 	2:8:ImplFile:font.cpp
# End Section
# Section OPCClient : {F4D83603-895E-11D0-B0A6-000000000000}
# 	2:5:Class:CColumnHeaders
# 	2:10:HeaderFile:columnheaders.h
# 	2:8:ImplFile:columnheaders.cpp
# End Section
# Section OPCClient : {BDD1F04C-858B-11D1-B16A-00C0F0283628}
# 	2:5:Class:CListItems
# 	2:10:HeaderFile:listitems1.h
# 	2:8:ImplFile:listitems1.cpp
# End Section
# Section OPCClient : {BDD1F050-858B-11D1-B16A-00C0F0283628}
# 	2:5:Class:CColumnHeaders
# 	2:10:HeaderFile:columnheaders1.h
# 	2:8:ImplFile:columnheaders1.cpp
# End Section
# Section OPCClient : {7BF80981-BF32-101A-8BBB-00AA00300CAB}
# 	2:5:Class:CPicture
# 	2:10:HeaderFile:picture.h
# 	2:8:ImplFile:picture.cpp
# End Section
# Section OPCClient : {F4D83602-895E-11D0-B0A6-000000000000}
# 	2:5:Class:CListItem
# 	2:10:HeaderFile:listitem.h
# 	2:8:ImplFile:listitem.cpp
# End Section
# Section OPCClient : {BDD1F049-858B-11D1-B16A-00C0F0283628}
# 	2:5:Class:CListView1
# 	2:10:HeaderFile:listview1.h
# 	2:8:ImplFile:listview1.cpp
# End Section
# Section OPCClient : {BDD1F04B-858B-11D1-B16A-00C0F0283628}
# 	2:21:DefaultSinkHeaderFile:listview1.h
# 	2:16:DefaultSinkClass:CListView1
# End Section
# Section OPCClient : {F4D83601-895E-11D0-B0A6-000000000000}
# 	2:5:Class:CListItems
# 	2:10:HeaderFile:listitems.h
# 	2:8:ImplFile:listitems.cpp
# End Section
# Section OPCClient : {F4D83600-895E-11D0-B0A6-000000000000}
# 	2:5:Class:CListView1
# 	2:10:HeaderFile:listview.h
# 	2:8:ImplFile:listview.cpp
# End Section
# Section OPCClient : {BDD1F04E-858B-11D1-B16A-00C0F0283628}
# 	2:5:Class:CListItem
# 	2:10:HeaderFile:listitem1.h
# 	2:8:ImplFile:listitem1.cpp
# End Section
# Section OPCClient : {58DA8D8A-9D6A-101B-AFC0-4210102A8DA7}
# 	2:21:DefaultSinkHeaderFile:listview.h
# 	2:16:DefaultSinkClass:CListView1
# End Section
# Section OPCClient : {F4D83604-895E-11D0-B0A6-000000000000}
# 	2:5:Class:CColumnHeader
# 	2:10:HeaderFile:columnheader.h
# 	2:8:ImplFile:columnheader.cpp
# End Section
