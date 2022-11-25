; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=COPCClientDlg
LastTemplate=CListBox
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "OPCClient.h"

ClassCount=3
Class1=COPCClientApp
Class2=COPCClientDlg

ResourceCount=3
Resource1=IDR_MAINFRAME
Resource2=IDD_OPCClient_DIALOG
Class3=CMyListCtrl
Resource3=IDD_OPCClient_DIALOG (English (U.S.))

[CLS:COPCClientApp]
Type=0
HeaderFile=OPCClient.h
ImplementationFile=OPCClient.cpp
Filter=N
LastObject=COPCClientApp

[CLS:COPCClientDlg]
Type=0
HeaderFile=OPCClientDlg.h
ImplementationFile=OPCClientDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=COPCClientDlg

[DLG:IDD_OPCClient_DIALOG]
Type=1
ControlCount=1
Control1=IDC_STATIC,static,1342308352
Class=COPCClientDlg

[DLG:IDD_OPCClient_DIALOG (English (U.S.))]
Type=1
Class=COPCClientDlg
ControlCount=3
Control1=IDC_LIST1,SysListView32,1350664193
Control2=IDC_STAT,SysListView32,1350664193
Control3=IDC_LIST2,SysListView32,1350649857

[CLS:CMyListCtrl]
Type=0
HeaderFile=MyListCtrl.h
ImplementationFile=MyListCtrl.cpp
BaseClass=CListCtrl
Filter=W
LastObject=CMyListCtrl
VirtualFilter=FWC

