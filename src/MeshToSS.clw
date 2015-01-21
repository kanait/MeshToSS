; CLW ファイルは MFC ClassWizard の情報を含んでいます。

[General Info]
Version=1
LastClass=CMainFrame
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MeshToSS.h"
LastPage=0

ClassCount=6
Class1=CMeshToSSApp
Class2=CMeshToSSDoc
Class3=CMeshToSSView
Class4=CMainFrame

ResourceCount=7
Resource1=IDD_MESHINFO
Resource2=IDD_MATERIAL_DIALOG
Class5=CAboutDlg
Resource3=IDD_ABOUTBOX
Resource4=IDR_MAINFRAME
Resource5=IDD_SET_RADIUS
Resource6=IDD_QUIT_DIALOG
Class6=CConvSubdiv
Resource7=IDD_CONVERT_SUBDIV

[CLS:CMeshToSSApp]
Type=0
HeaderFile=MeshToSS.h
ImplementationFile=MeshToSS.cpp
Filter=N
LastObject=CMeshToSSApp

[CLS:CMeshToSSDoc]
Type=0
HeaderFile=MeshToSSDoc.h
ImplementationFile=MeshToSSDoc.cpp
Filter=N
LastObject=CMeshToSSDoc

[CLS:CMeshToSSView]
Type=0
HeaderFile=MeshToSSView.h
ImplementationFile=MeshToSSView.cpp
Filter=C
LastObject=CMeshToSSView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=ID_VIEW_GRADIENT




[CLS:CAboutDlg]
Type=0
HeaderFile=MeshToSS.cpp
ImplementationFile=MeshToSS.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308491
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_OPEN_VRML
Command2=ID_FILE_OPEN_EVRML_POLY
Command3=ID_FILE_OPEN_EVRML_SS
Command4=ID_FILE_OPEN
Command5=ID_FILE_OPEN_SMF
Command6=ID_FILE_OPEN_VW
Command7=ID_FILE_SAVE_VRML_POLY
Command8=ID_FILE_SAVE_VRML_SUB
Command9=ID_FILE_SAVE_PPD_POLY
Command10=ID_FILE_SAVE_SRC
Command11=ID_FILE_SAVE_SMF_POLY
Command12=ID_FILE_SAVE_SMF
Command13=ID_FILE_SAVE_VW
Command14=ID_APP_EXIT
Command15=ID_MENU_EDIT_CONV
Command16=ID_MENU_SCREEN_SHADING
Command17=ID_MENU_SCREEN_WIREFRAME
Command18=ID_MENU_SCREEN_SUBBOWN
Command19=ID_MENU_EDIT_SUBDIV_UP
Command20=ID_MENU_EDIT_SUBDIV_DOWN
Command21=ID_MENU_SCREEN_ORGPPD
Command22=ID_VIEW_BACKGROUND
Command23=ID_VIEW_GRADIENT
Command24=ID_APP_ABOUT
CommandCount=24

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_OPEN_SP
Command2=ID_FILE_OPEN_VRML
Command3=ID_FILE_OPEN
Command4=ID_APP_EXIT
Command5=ID_FILE_SAVE_MDW
Command6=ID_FILE_OPEN_VW
CommandCount=6

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

[DLG:IDD_QUIT_DIALOG]
Type=1
Class=?
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

[DLG:IDD_SET_RADIUS]
Type=1
Class=?
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=ID_STATIC,static,1342308352
Control4=ID_STATIC2,static,1342308352
Control5=IDC_RADIUS_SPH,edit,1350631552
Control6=IDC_RADIUS_CYL,edit,1350631552

[DLG:IDD_MATERIAL_DIALOG]
Type=1
Class=?
ControlCount=3
Control1=IDCANCEL,button,1342242816
Control2=IDC_MATL_PPD_CHECK,button,1342242819
Control3=IDC_MATERIAL_LIST,listbox,1352732931

[DLG:IDD_MESHINFO]
Type=1
Class=?
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDC_MESH_INFO_EDIT,edit,1342245060

[DLG:IDD_CONVERT_SUBDIV]
Type=1
Class=CConvSubdiv
ControlCount=19
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDCLOSE,button,1342242816
Control4=IDC_CONV_CUR_VNUM,edit,1350633600
Control5=IDC_CONV_SIMP_VNUM,edit,1350631552
Control6=IDC_STATIC,static,1342308865
Control7=IDC_STATIC,static,1342308865
Control8=IDC_STATIC,button,1342177287
Control9=IDC_STATIC,button,1342177287
Control10=IDC_STATIC,static,1342308865
Control11=IDC_STATIC,static,1342308865
Control12=IDC_CONV_CUR_DIS,edit,1350633600
Control13=IDC_CONV_SIMP_DIS,edit,1350631552
Control14=IDOK2,button,1342242817
Control15=IDCANCEL2,button,1342242816
Control16=IDC_RADIO_NUMVERT,button,1342177289
Control17=IDC_RADIO_DIS,button,1342177289
Control18=IDC_STATIC,button,1342177287
Control19=IDC_BUTTON1,button,1342242816

[CLS:CConvSubdiv]
Type=0
HeaderFile=ConvSubdiv.h
ImplementationFile=ConvSubdiv.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_RADIO_NUMVERT

