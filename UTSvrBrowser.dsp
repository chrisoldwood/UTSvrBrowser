# Microsoft Developer Studio Project File - Name="UTSvrBrowser" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=UTSvrBrowser - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "UTSvrBrowser.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "UTSvrBrowser.mak" CFG="UTSvrBrowser - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "UTSvrBrowser - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "UTSvrBrowser - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Win32/UTSvrBrowser", FADAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "UTSvrBrowser - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W4 /WX /GX /O2 /I "..\wcl" /I "..\ncl" /I "..\mdbl" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX"AppHeaders.hpp" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /i "..\wcl" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "UTSvrBrowser - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /WX /Gm /GX /ZI /Od /I "..\wcl" /I "..\ncl" /I "..\mdbl" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX"AppHeaders.hpp" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /i "..\wcl" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "UTSvrBrowser - Win32 Release"
# Name "UTSvrBrowser - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AppCmds.cpp
# End Source File
# Begin Source File

SOURCE=.\AppDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AppToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\AppWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\FavFileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FavFiles.cpp
# End Source File
# Begin Source File

SOURCE=.\Favourites.cpp
# End Source File
# Begin Source File

SOURCE=.\FavouritesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FilterDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FiltersDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FindDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FltMiscPage.cpp
# End Source File
# Begin Source File

SOURCE=.\FltModsPage.cpp
# End Source File
# Begin Source File

SOURCE=.\FltNamePage.cpp
# End Source File
# Begin Source File

SOURCE=.\FltPortsPage.cpp
# End Source File
# Begin Source File

SOURCE=.\GameInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GameServer.cpp
# End Source File
# Begin Source File

SOURCE=.\GameTypes.cpp
# End Source File
# Begin Source File

SOURCE=.\MasterQueryDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MasterServer.cpp
# End Source File
# Begin Source File

SOURCE=.\Mods.cpp
# End Source File
# Begin Source File

SOURCE=.\MtrAddressPage.cpp
# End Source File
# Begin Source File

SOURCE=.\MtrFirewallPage.cpp
# End Source File
# Begin Source File

SOURCE=.\MtrTransformPage.cpp
# End Source File
# Begin Source File

SOURCE=.\PingOptsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\QueryJob.cpp
# End Source File
# Begin Source File

SOURCE=.\QueryResponse.cpp
# End Source File
# Begin Source File

SOURCE=.\Servers.cpp
# End Source File
# Begin Source File

SOURCE=.\Summary.cpp
# End Source File
# Begin Source File

SOURCE=.\SummaryDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UTSvrBrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\UTSvrBrowser.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AboutDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\AppCmds.hpp
# End Source File
# Begin Source File

SOURCE=.\AppDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\AppHeaders.hpp
# End Source File
# Begin Source File

SOURCE=.\AppToolBar.hpp
# End Source File
# Begin Source File

SOURCE=.\AppWnd.hpp
# End Source File
# Begin Source File

SOURCE=.\FavFileDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\FavFiles.hpp
# End Source File
# Begin Source File

SOURCE=.\Favourites.hpp
# End Source File
# Begin Source File

SOURCE=.\FavouritesDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\Filter.hpp
# End Source File
# Begin Source File

SOURCE=.\FilterDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\FiltersDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\FindDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\FltMiscPage.hpp
# End Source File
# Begin Source File

SOURCE=.\FltModsPage.hpp
# End Source File
# Begin Source File

SOURCE=.\FltNamePage.hpp
# End Source File
# Begin Source File

SOURCE=.\FltPortsPage.hpp
# End Source File
# Begin Source File

SOURCE=.\GameInfoDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\GameServer.hpp
# End Source File
# Begin Source File

SOURCE=.\GameTypes.hpp
# End Source File
# Begin Source File

SOURCE=.\MasterQueryDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\MasterQueryOpts.hpp
# End Source File
# Begin Source File

SOURCE=.\MasterServer.hpp
# End Source File
# Begin Source File

SOURCE=.\Mods.hpp
# End Source File
# Begin Source File

SOURCE=.\MtrAddressPage.hpp
# End Source File
# Begin Source File

SOURCE=.\MtrFirewallPage.hpp
# End Source File
# Begin Source File

SOURCE=.\MtrTransformPage.hpp
# End Source File
# Begin Source File

SOURCE=.\PingOpts.hpp
# End Source File
# Begin Source File

SOURCE=.\PingOptsDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\QueryJob.hpp
# End Source File
# Begin Source File

SOURCE=.\QueryResponse.hpp
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Servers.hpp
# End Source File
# Begin Source File

SOURCE=.\Summary.hpp
# End Source File
# Begin Source File

SOURCE=.\SummaryDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\UTSvrBrowser.hpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\App.ico
# End Source File
# Begin Source File

SOURCE=.\App.rcv
# End Source File
# Begin Source File

SOURCE=.\ListIcons.bmp
# End Source File
# Begin Source File

SOURCE=.\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\RelNotes.txt
# End Source File
# Begin Source File

SOURCE=.\TODO.txt
# End Source File
# Begin Source File

SOURCE=.\UTSBMods.ini
# End Source File
# Begin Source File

SOURCE=.\UTSvrBrowser.ini
# End Source File
# End Target
# End Project
