# Microsoft Developer Studio Project File - Name="optmesh" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** ??W?????????????? **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=optmesh - Win32 Debug
!MESSAGE ?????L??????????????????B ??????????????????????? NMAKE ???g?p????????????B
!MESSAGE [???????????] ???????g?p??????s???????????
!MESSAGE 
!MESSAGE NMAKE /f "optmesh.mak".
!MESSAGE 
!MESSAGE NMAKE ????s????\?????w???????
!MESSAGE ????? ???????????`??????B??:
!MESSAGE 
!MESSAGE NMAKE /f "optmesh.mak" CFG="optmesh - Win32 Debug"
!MESSAGE 
!MESSAGE ?I????\??????? ???:
!MESSAGE 
!MESSAGE "optmesh - Win32 Release" ("Win32 (x86) Static Library" ?p)
!MESSAGE "optmesh - Win32 Debug" ("Win32 (x86) Static Library" ?p)
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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\optmesh" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /YX /FD /c
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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\optmesh" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /D "_DFILE" /FR /YX /FD /GZ /c
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

SOURCE=..\optmesh\file.cxx
# End Source File
# Begin Source File

SOURCE=..\optmesh\linbcg.cxx
# End Source File
# Begin Source File

SOURCE=..\optmesh\lodprique.cxx
# End Source File
# Begin Source File

SOURCE=..\optmesh\matlib.cxx
# End Source File
# Begin Source File

SOURCE=..\optmesh\meshtoss.cxx
# End Source File
# Begin Source File

SOURCE=..\optmesh\mytime.cxx
# End Source File
# Begin Source File

SOURCE=..\optmesh\oppd.cxx
# End Source File
# Begin Source File

SOURCE=..\optmesh\ppd.cxx
# End Source File
# Begin Source File

SOURCE=..\optmesh\ppdedge.cxx
# End Source File
# Begin Source File

SOURCE=..\optmesh\ppdface.cxx
# End Source File
# Begin Source File

SOURCE=..\optmesh\ppdlist.cxx
# End Source File
# Begin Source File

SOURCE=..\optmesh\ppdnormal.cxx
# End Source File
# Begin Source File

SOURCE=..\optmesh\ppdpart.cxx
# End Source File
# Begin Source File

SOURCE=..\optmesh\ppdsolid.cxx
# End Source File
# Begin Source File

SOURCE=..\optmesh\ppdvertex.cxx
# End Source File
# Begin Source File

SOURCE=..\optmesh\ps.cxx
# End Source File
# Begin Source File

SOURCE=..\optmesh\qem.cxx
# End Source File
# Begin Source File

SOURCE=..\optmesh\simpmesh.cxx
# End Source File
# Begin Source File

SOURCE=..\optmesh\slpopt.cxx
# End Source File
# Begin Source File

SOURCE=..\optmesh\star.cxx
# End Source File
# Begin Source File

SOURCE=..\optmesh\subdiv.cxx
# End Source File
# Begin Source File

SOURCE=..\optmesh\veclib.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\optmesh\file.h
# End Source File
# Begin Source File

SOURCE=..\optmesh\linbcg.h
# End Source File
# Begin Source File

SOURCE=..\optmesh\lodprique.h
# End Source File
# Begin Source File

SOURCE=..\optmesh\matlib.h
# End Source File
# Begin Source File

SOURCE=..\optmesh\mytime.h
# End Source File
# Begin Source File

SOURCE=..\optmesh\oppd.h
# End Source File
# Begin Source File

SOURCE=..\optmesh\params.h
# End Source File
# Begin Source File

SOURCE=..\optmesh\ppd.h
# End Source File
# Begin Source File

SOURCE=..\optmesh\ppdedge.h
# End Source File
# Begin Source File

SOURCE=..\optmesh\ppdface.h
# End Source File
# Begin Source File

SOURCE=..\optmesh\ppdlist.h
# End Source File
# Begin Source File

SOURCE=..\optmesh\ppdnormal.h
# End Source File
# Begin Source File

SOURCE=..\optmesh\ppdpart.h
# End Source File
# Begin Source File

SOURCE=..\optmesh\ppdsolid.h
# End Source File
# Begin Source File

SOURCE=..\optmesh\ppdvertex.h
# End Source File
# Begin Source File

SOURCE=..\optmesh\ps.h
# End Source File
# Begin Source File

SOURCE=..\optmesh\qem.h
# End Source File
# Begin Source File

SOURCE=..\optmesh\simpmesh.h
# End Source File
# Begin Source File

SOURCE=..\optmesh\slpopt.h
# End Source File
# Begin Source File

SOURCE=..\optmesh\smd.h
# End Source File
# Begin Source File

SOURCE=..\optmesh\star.h
# End Source File
# Begin Source File

SOURCE=..\optmesh\subdiv.h
# End Source File
# Begin Source File

SOURCE=..\optmesh\veclib.h
# End Source File
# End Group
# End Target
# End Project
