# Microsoft Developer Studio Project File - Name="optmesh" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=optmesh - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "optmesh.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "optmesh.mak" CFG="optmesh - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "optmesh - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "optmesh - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "optmesh - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "optmesh - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /D "_DFILE" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "optmesh - Win32 Release"
# Name "optmesh - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\file.cxx
# End Source File
# Begin Source File

SOURCE=.\linbcg.cxx
# End Source File
# Begin Source File

SOURCE=.\lodprique.cxx
# End Source File
# Begin Source File

SOURCE=.\matlib.cxx
# End Source File
# Begin Source File

SOURCE=.\meshtoss.cxx
# End Source File
# Begin Source File

SOURCE=.\mytime.cxx
# End Source File
# Begin Source File

SOURCE=.\oppd.cxx
# End Source File
# Begin Source File

SOURCE=.\ppd.cxx
# End Source File
# Begin Source File

SOURCE=.\ppdedge.cxx
# End Source File
# Begin Source File

SOURCE=.\ppdface.cxx
# End Source File
# Begin Source File

SOURCE=.\ppdlist.cxx
# End Source File
# Begin Source File

SOURCE=.\ppdnormal.cxx
# End Source File
# Begin Source File

SOURCE=.\ppdpart.cxx
# End Source File
# Begin Source File

SOURCE=.\ppdsolid.cxx
# End Source File
# Begin Source File

SOURCE=.\ppdvertex.cxx
# End Source File
# Begin Source File

SOURCE=.\ps.cxx
# End Source File
# Begin Source File

SOURCE=.\qem.cxx
# End Source File
# Begin Source File

SOURCE=.\simpmesh.cxx
# End Source File
# Begin Source File

SOURCE=.\slpopt.cxx
# End Source File
# Begin Source File

SOURCE=.\star.cxx
# End Source File
# Begin Source File

SOURCE=.\subdiv.cxx
# End Source File
# Begin Source File

SOURCE=.\veclib.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\file.h
# End Source File
# Begin Source File

SOURCE=.\linbcg.h
# End Source File
# Begin Source File

SOURCE=.\lodprique.h
# End Source File
# Begin Source File

SOURCE=.\matlib.h
# End Source File
# Begin Source File

SOURCE=.\mytime.h
# End Source File
# Begin Source File

SOURCE=.\oppd.h
# End Source File
# Begin Source File

SOURCE=.\params.h
# End Source File
# Begin Source File

SOURCE=.\ppd.h
# End Source File
# Begin Source File

SOURCE=.\ppdedge.h
# End Source File
# Begin Source File

SOURCE=.\ppdface.h
# End Source File
# Begin Source File

SOURCE=.\ppdlist.h
# End Source File
# Begin Source File

SOURCE=.\ppdnormal.h
# End Source File
# Begin Source File

SOURCE=.\ppdpart.h
# End Source File
# Begin Source File

SOURCE=.\ppdsolid.h
# End Source File
# Begin Source File

SOURCE=.\ppdvertex.h
# End Source File
# Begin Source File

SOURCE=.\ps.h
# End Source File
# Begin Source File

SOURCE=.\qem.h
# End Source File
# Begin Source File

SOURCE=.\simpmesh.h
# End Source File
# Begin Source File

SOURCE=.\slpopt.h
# End Source File
# Begin Source File

SOURCE=.\smd.h
# End Source File
# Begin Source File

SOURCE=.\star.h
# End Source File
# Begin Source File

SOURCE=.\subdiv.h
# End Source File
# Begin Source File

SOURCE=.\veclib.h
# End Source File
# End Group
# End Target
# End Project
