# Microsoft Developer Studio Project File - Name="fdlibm" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=fdlibm - Win32 Release
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "fdlibm.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "fdlibm.mak" CFG="fdlibm - Win32 Release"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "fdlibm - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "fdlibm - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "fdlibm - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir "."
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Target_Dir "."
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_IEEE_LIBM" /D "_AFXDLL" /YX /FD /c
# ADD BASE RSC /l 0x411
# ADD RSC /l 0x411 /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "fdlibm - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir "."
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Target_Dir "."
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /GX /Z7 /Od /I "..\..\glut" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_IEEE_LIBM" /D "_AFXDLL" /YX /FD /c
# ADD BASE RSC /l 0x411
# ADD RSC /l 0x411 /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "fdlibm - Win32 Release"
# Name "fdlibm - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=..\src\vrml97js\fdlibm\e_atan2.c
# End Source File
# Begin Source File

SOURCE=..\src\vrml97js\fdlibm\e_pow.c
# End Source File
# Begin Source File

SOURCE=..\src\vrml97js\fdlibm\e_sqrt.c
# End Source File
# Begin Source File

SOURCE=..\src\vrml97js\fdlibm\k_standard.c
# End Source File
# Begin Source File

SOURCE=..\src\vrml97js\fdlibm\s_atan.c
# End Source File
# Begin Source File

SOURCE=..\src\vrml97js\fdlibm\s_copysign.c
# End Source File
# Begin Source File

SOURCE=..\src\vrml97js\fdlibm\s_fabs.c
# End Source File
# Begin Source File

SOURCE=..\src\vrml97js\fdlibm\s_finite.c
# End Source File
# Begin Source File

SOURCE=..\src\vrml97js\fdlibm\s_isnan.c
# End Source File
# Begin Source File

SOURCE=..\src\vrml97js\fdlibm\s_matherr.c
# End Source File
# Begin Source File

SOURCE=..\src\vrml97js\fdlibm\s_rint.c
# End Source File
# Begin Source File

SOURCE=..\src\vrml97js\fdlibm\s_scalbn.c
# End Source File
# Begin Source File

SOURCE=..\src\vrml97js\fdlibm\w_atan2.c
# End Source File
# Begin Source File

SOURCE=..\src\vrml97js\fdlibm\w_pow.c
# End Source File
# Begin Source File

SOURCE=..\src\vrml97js\fdlibm\w_sqrt.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\src\vrml97js\fdlibm\fdlibm.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
