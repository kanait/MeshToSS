# Microsoft Developer Studio Project File - Name="MeshToSS" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MeshToSS - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "MeshToSS.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "MeshToSS.mak" CFG="MeshToSS - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "MeshToSS - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "MeshToSS - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MeshToSS - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\libvrml97\libvrml97core\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /FD /c
# SUBTRACT CPP /X
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ws2_32.lib opengl32.lib glu32.lib glaux.lib jpeg.lib zlibdll.lib zlibstat.lib libpng.lib libvrml97core.lib libvrml97js32.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libcp.lib" /nodefaultlib:"libc.lib" /libpath:"..\libvrml97\zlib" /libpath:"..\libvrml97\libpng" /libpath:"..\libvrml97\jpeg-6b" /libpath:"..\libvrml97\libvrml97core\win32\Release" /libpath:"..\libvrml97\libvrml97js\win32\Release" /libpath:"..\qslim-2.0\Release"
# SUBTRACT LINK32 /pdb:none /nodefaultlib

!ELSEIF  "$(CFG)" == "MeshToSS - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\libvrml97\libvrml97core\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_DFILE" /FR /YX /FD /GZ /c
# SUBTRACT CPP /X
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ws2_32.lib opengl32.lib glu32.lib glaux.lib jpeg.lib zlibdll.lib zlibstat.lib libpng.lib libvrml97core.lib fdlibm.lib libvrml97js32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcd.lib" /nodefaultlib:"libc.lib" /pdbtype:sept /libpath:"..\libvrml97\zlib" /libpath:"..\libvrml97\libpng" /libpath:"..\libvrml97\jpeg-6b" /libpath:"..\libvrml97\libvrml97core\win32\Debug" /libpath:"..\libvrml97\libvrml97js\win32\Debug" /libpath:"..\qslim-2.0\Debug"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "MeshToSS - Win32 Release"
# Name "MeshToSS - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\src\bitmap.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ConvSubdiv.cpp
# End Source File
# Begin Source File

SOURCE=..\src\draw.cpp
# End Source File
# Begin Source File

SOURCE=..\src\edit.cpp
# End Source File
# Begin Source File

SOURCE=..\src\glppd.cpp
# End Source File
# Begin Source File

SOURCE=..\src\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Material.cpp
# End Source File
# Begin Source File

SOURCE=..\src\MaterialDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\src\MeshToSS.cpp
# End Source File
# Begin Source File

SOURCE=..\src\MeshToSS.rc
# End Source File
# Begin Source File

SOURCE=..\src\MeshToSSDoc.cpp
# End Source File
# Begin Source File

SOURCE=..\src\MeshToSSView.cpp
# End Source File
# Begin Source File

SOURCE=..\src\pick.cpp
# End Source File
# Begin Source File

SOURCE=..\src\QuitDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\src\screen.cxx
# End Source File
# Begin Source File

SOURCE=..\src\sellist.cxx
# End Source File
# Begin Source File

SOURCE=..\src\smf.cxx
# End Source File
# Begin Source File

SOURCE=..\src\SRDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\src\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=..\src\swin.cpp
# End Source File
# Begin Source File

SOURCE=..\src\texture.cxx
# End Source File
# Begin Source File

SOURCE=..\src\vrml.cxx
# End Source File
# Begin Source File

SOURCE=..\src\vw.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\src\bitmap.h
# End Source File
# Begin Source File

SOURCE=..\src\color.h
# End Source File
# Begin Source File

SOURCE=..\src\ConvSubdiv.h
# End Source File
# Begin Source File

SOURCE=..\src\draw.h
# End Source File
# Begin Source File

SOURCE=..\src\edit.h
# End Source File
# Begin Source File

SOURCE=..\src\glppd.h
# End Source File
# Begin Source File

SOURCE=..\src\light.h
# End Source File
# Begin Source File

SOURCE=..\src\MainFrm.h
# End Source File
# Begin Source File

SOURCE=..\src\Material.h
# End Source File
# Begin Source File

SOURCE=..\src\MaterialDialog.h
# End Source File
# Begin Source File

SOURCE=..\src\MeshToSS.h
# End Source File
# Begin Source File

SOURCE=..\src\MeshToSSDoc.h
# End Source File
# Begin Source File

SOURCE=..\src\MeshToSSView.h
# End Source File
# Begin Source File

SOURCE=..\src\mtl.h
# End Source File
# Begin Source File

SOURCE=..\src\pick.h
# End Source File
# Begin Source File

SOURCE=..\src\QuitDialog.h
# End Source File
# Begin Source File

SOURCE=..\src\resource.h
# End Source File
# Begin Source File

SOURCE=..\src\screen.h
# End Source File
# Begin Source File

SOURCE=..\src\sellist.h
# End Source File
# Begin Source File

SOURCE=..\src\smf.h
# End Source File
# Begin Source File

SOURCE=..\src\SRDialog.h
# End Source File
# Begin Source File

SOURCE=..\src\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\src\swin.h
# End Source File
# Begin Source File

SOURCE=..\src\texture.h
# End Source File
# Begin Source File

SOURCE=..\src\vrml.h
# End Source File
# Begin Source File

SOURCE=..\src\vw.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\MeshToSS.ico
# End Source File
# Begin Source File

SOURCE=.\res\MeshToSS.rc2
# End Source File
# Begin Source File

SOURCE=.\res\MeshToSSDoc.ico
# End Source File
# Begin Source File

SOURCE=.\src\res\OpenGL.ico
# End Source File
# Begin Source File

SOURCE=..\src\res\OpenGL.ico
# End Source File
# Begin Source File

SOURCE=.\src\res\OpenGLDoc.ico
# End Source File
# Begin Source File

SOURCE=..\src\res\OpenGLDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\src\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=..\src\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
