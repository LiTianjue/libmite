; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CGenRsaKey
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "CSPtest.h"
LastPage=0

ClassCount=6
Class1=CCSPtestApp
Class2=CCSPtestDoc
Class3=CCSPtestView
Class4=CMainFrame

ResourceCount=3
Resource1=IDD_ABOUTBOX
Class5=CAboutDlg
Resource2=IDR_MAINFRAME
Class6=CGenRsaKey
Resource3=IDD_GEN_RSA_KEY

[CLS:CCSPtestApp]
Type=0
HeaderFile=CSPtest.h
ImplementationFile=CSPtest.cpp
Filter=N

[CLS:CCSPtestDoc]
Type=0
HeaderFile=CSPtestDoc.h
ImplementationFile=CSPtestDoc.cpp
Filter=N

[CLS:CCSPtestView]
Type=0
HeaderFile=CSPtestView.h
ImplementationFile=CSPtestView.cpp
Filter=C


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=IDC_RADIO1




[CLS:CAboutDlg]
Type=0
HeaderFile=CSPtest.cpp
ImplementationFile=CSPtest.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_GEN_RSA_KEY
Command2=ID_EXPORT_KEY
CommandCount=2

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[DLG:IDD_GEN_RSA_KEY]
Type=1
Class=CGenRsaKey
ControlCount=34
Control1=IDC_RADIO1,button,1342308361
Control2=IDC_RADIO2,button,1342177289
Control3=IDC_STATIC,button,1342178055
Control4=IDC_GEN_RSA_KEY,button,1342242816
Control5=IDC_EXPORT_KEY,button,1342242816
Control6=IDC_STATIC,button,1342178055
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_SRC_PATH,edit,1350631552
Control10=IDC_OPEN_BUTTON,button,1342242816
Control11=IDC_SAVE_BUTTON,button,1342242816
Control12=IDC_DEST_PATH,edit,1350631552
Control13=IDC_RSA_ENCRYPT,button,1342242816
Control14=IDC_RSA_DECRYPT,button,1342242816
Control15=IDC_STATIC,button,1342178055
Control16=IDC_STATIC,static,1342308352
Control17=IDC_PWD,edit,1350631552
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_SYM_SRC_PATH,edit,1350631552
Control21=IDC_SYM_OPEN_BUTTON,button,1342242816
Control22=IDC_SYM_SAVE_BUTTON,button,1342242816
Control23=IDC_SYM_DEST_PATH,edit,1350631552
Control24=IDC_ENCRYPT,button,1342242816
Control25=IDC_DECRYPT,button,1342242816
Control26=IDC_STATIC,button,1342178055
Control27=IDC_STATIC,static,1342308352
Control28=IDC_STATIC,static,1342308352
Control29=IDC_SIGN_SRC_PATH,edit,1350631552
Control30=IDC_SIGN_OPEN_BUTTON,button,1342242816
Control31=IDC_VERIFY_SAVE_BUTTON,button,1342242816
Control32=IDC_VERIFY_DEST_PATH,edit,1350631552
Control33=IDC_RSA_SIGN,button,1342242816
Control34=IDC_RSA_VERIFY,button,1342242816

[CLS:CGenRsaKey]
Type=0
HeaderFile=GenRsaKey.h
ImplementationFile=GenRsaKey.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_SIGN_SRC_PATH

