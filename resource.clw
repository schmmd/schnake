; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=about
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "resource.h"
LastPage=0

ClassCount=1

ResourceCount=4
Resource1=IDA_MAIN
Resource2=IDD_SETTINGS
Resource3=IDM_MAIN
Class1=about
Resource4=IDD_ABOUT

[MNU:IDM_MAIN]
Type=1
Class=?
Command1=MNU_FILE_NEWGAME
Command2=MNU_FILE_PAUSEGAME
Command3=MNU_FILE_ENDGAME
Command4=MNU_FILE_EXIT
Command5=MNU_HELP_SETTINGS
Command6=MNU_HELP_SCHMITZTECH
Command7=MNU_HELP_ABOUT
CommandCount=7

[DLG:IDD_ABOUT]
Type=1
Class=about
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[ACL:IDA_MAIN]
Type=1
Class=?
Command1=MNU_HELP_SCHMITZTECH
Command2=MNU_HELP_ABOUT
Command3=MNU_FILE_ENDGAME
Command4=MNU_HELP_INFO
Command5=MNU_FILE_NEWGAME
Command6=MNU_FILE_PAUSEGAME
Command7=MNU_FILE_EXIT
Command8=MNU_HELP_SETTINGS
CommandCount=8

[DLG:IDD_SETTINGS]
Type=1
Class=?
ControlCount=17
Control1=IDC_OK,button,1342242817
Control2=IDC_CANCEL,button,1342242816
Control3=IDE_UP,edit,1350631552
Control4=IDE_LEFT,edit,1350631552
Control5=IDE_RIGHT,edit,1350631552
Control6=IDE_DOWN,edit,1350631552
Control7=IDF_KEYS,button,1342177287
Control8=IDL_LEFT,static,1342308353
Control9=IDL_UP,static,1342308353
Control10=IDL_DOWN,static,1342308353
Control11=IDL_RIGHT,static,1342308353
Control12=IDF_SPEED,button,1342177287
Control13=IDS_GAME_DELAY,scrollbar,1342177280
Control14=IDF_INITLENGTH,button,1342177287
Control15=IDS_INIT_LENGTH,scrollbar,1342177280
Control16=IDF_GROWTH,button,1342177287
Control17=IDS_INC_LENGTH,scrollbar,1342177280

[CLS:about]
Type=0
HeaderFile=about.h
ImplementationFile=about.cpp
BaseClass=CDialog
Filter=D
LastObject=about

