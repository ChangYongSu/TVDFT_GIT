# Microsoft Developer Studio Project File - Name="DATsys" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=DATsys - Win32 Release_DFT3
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DATsys.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DATsys.mak" CFG="DATsys - Win32 Release_DFT3"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DATsys - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "DATsys - Win32 Debug_DFT2" (based on "Win32 (x86) Application")
!MESSAGE "DATsys - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "DATsys - Win32 Release_DFT2" (based on "Win32 (x86) Application")
!MESSAGE "DATsys - Win32 Debug_DFT3" (based on "Win32 (x86) Application")
!MESSAGE "DATsys - Win32 Release_DFT3" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DATsys - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /GX /Zi /Od /I "./EditLog" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_DFT_SYSTEM1" /FR /YX"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 dft3_dll.lib psapi.lib version.lib winmm.lib dinput.lib ws2_32.lib Keydll1.lib Keydll2.lib Keydll3.lib jpeglib_d.lib /nologo /stack:0x200000 /subsystem:windows /incremental:no /map /debug /machine:I386 /out:"Debug/UHD_DFT1_3IN1_D.exe" /libpath:"PsApi" /MAPINFO:LINES
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy Debug\UHD_DFT1_3IN1_D.exe D:\DFT
# End Special Build Tool

!ELSEIF  "$(CFG)" == "DATsys - Win32 Debug_DFT2"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DATsys___Win32_Debug_DFT20"
# PROP BASE Intermediate_Dir "DATsys___Win32_Debug_DFT20"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_DFT2"
# PROP Intermediate_Dir "Debug_DFT2"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "HwCtrl\Mil" /I "HwCtrl\Mil\MilDyn" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_DFT_SYSTEM1" /FR /YX"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "HwCtrl\Mil" /I "HwCtrl\Mil\MilDyn" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_DFT_SYSTEM2" /FR /YX"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 psapi.lib version.lib winmm.lib dinput.lib mil.lib ApiEtg.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/DFT1.exe" /pdbtype:sept /libpath:"HwCtrl\Mil\Lib" /libpath:"PsApi"
# ADD LINK32 psapi.lib version.lib winmm.lib dinput.lib mil.lib ApiEtg.lib ws2_32.lib Keydll1.lib Keydll2.lib Keydll3.lib /nologo /stack:0x200000 /subsystem:windows /incremental:no /map /debug /machine:I386 /out:"Debug_DFT2/DFT2_3in1_D.exe" /pdbtype:sept /libpath:"HwCtrl\Mil\Lib" /libpath:"PsApi"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy Debug_DFT2\DFT2_3in1_D.exe d:\DFT
# End Special Build Tool

!ELSEIF  "$(CFG)" == "DATsys - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DATsys___Win32_Release"
# PROP BASE Intermediate_Dir "DATsys___Win32_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "HwCtrl\Mil" /I "HwCtrl\Mil\MilDyn" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_DFT_SYSTEM1" /D "_AFXDLL" /FR /YX"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "./EditLog" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_DFT_SYSTEM1" /FR /YX"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 psapi.lib version.lib winmm.lib dinput.lib mil.lib ApiEtg.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/DFT1.exe" /pdbtype:sept /libpath:"HwCtrl\Mil\Lib" /libpath:"PsApi"
# ADD LINK32 dft3_dll.lib psapi.lib version.lib winmm.lib dinput.lib ws2_32.lib Keydll1.lib Keydll2.lib Keydll3.lib jpeglib.lib /nologo /stack:0x200000 /subsystem:windows /incremental:no /machine:I386 /out:"Release/UHD_DFT1_3in1.exe" /pdbtype:sept /libpath:"PsApi"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy Release\UHD_DFT1_3in1.exe D:\DFT\3in1	copy Release\UHD_DFT1_3in1.exe D:\DFT
# End Special Build Tool

!ELSEIF  "$(CFG)" == "DATsys - Win32 Release_DFT2"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DATsys___Win32_Release_DFT2"
# PROP BASE Intermediate_Dir "DATsys___Win32_Release_DFT2"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Release_DFT2"
# PROP Intermediate_Dir "Release_DFT2"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /Zi /O2 /I "HwCtrl\Mil" /I "HwCtrl\Mil\MilDyn" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_DFT_SYSTEM1" /YX"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "./EditLog" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_DFT_SYSTEM2" /FR /YX"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 psapi.lib version.lib winmm.lib dinput.lib mil.lib ApiEtg.lib /nologo /subsystem:windows /incremental:no /machine:I386 /out:"Release/DFT1.exe" /pdbtype:sept /libpath:"HwCtrl\Mil\Lib" /libpath:"PsApi"
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 dft3_dll.lib psapi.lib version.lib winmm.lib dinput.lib ws2_32.lib Keydll1.lib Keydll2.lib Keydll3.lib jpeglib.lib /nologo /stack:0x200000 /subsystem:windows /incremental:no /machine:I386 /out:"Release_DFT2/UHD_DFT2_3in1.exe" /pdbtype:sept /libpath:"PsApi"
# SUBTRACT LINK32 /debug
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy Release_DFT2\UHD_DFT2_3in1.exe D:\DFT\3in1
# End Special Build Tool

!ELSEIF  "$(CFG)" == "DATsys - Win32 Debug_DFT3"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DATsys___Win32_Debug_DFT3"
# PROP BASE Intermediate_Dir "DATsys___Win32_Debug_DFT3"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_DFT3"
# PROP Intermediate_Dir "Debug_DFT3"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "HwCtrl\Mil" /I "HwCtrl\Mil\MilDyn" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_DFT_SYSTEM2" /FR /YX"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "HwCtrl\Mil" /I "HwCtrl\Mil\MilDyn" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_DFT_SYSTEM3" /FR /YX"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 psapi.lib version.lib winmm.lib dinput.lib mil.lib ApiEtg.lib ws2_32.lib Keydll1.lib Keydll2.lib /nologo /stack:0x200000 /subsystem:windows /incremental:no /map /debug /machine:I386 /out:"Debug_DFT2/DFT2_D.exe" /pdbtype:sept /libpath:"HwCtrl\Mil\Lib" /libpath:"PsApi"
# ADD LINK32 psapi.lib version.lib winmm.lib dinput.lib mil.lib ApiEtg.lib ws2_32.lib Keydll1.lib Keydll2.lib Keydll3.lib /nologo /stack:0x200000 /subsystem:windows /incremental:no /map /debug /machine:I386 /out:"Debug_DFT3/DFT3_3in1_D.exe" /pdbtype:sept /libpath:"HwCtrl\Mil\Lib" /libpath:"PsApi"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy Debug_DFT3\DFT3_3in1_D.exe d:\DFT
# End Special Build Tool

!ELSEIF  "$(CFG)" == "DATsys - Win32 Release_DFT3"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DATsys___Win32_Release_DFT3"
# PROP BASE Intermediate_Dir "DATsys___Win32_Release_DFT3"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Release_DFT3"
# PROP Intermediate_Dir "Release_DFT3"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /Zi /O2 /I "./EditLog" /I "HwCtrl\Mil" /I "HwCtrl\Mil\MilDyn" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_DFT_SYSTEM2" /FR /YX"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "./EditLog" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_DFT_SYSTEM3" /FR /YX"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 psapi.lib version.lib winmm.lib dinput.lib mil.lib ApiEtg.lib ws2_32.lib Keydll1.lib Keydll2.lib /nologo /stack:0x200000 /subsystem:windows /incremental:no /machine:I386 /out:"Release_DFT2/DFT2.exe" /pdbtype:sept /libpath:"HwCtrl\Mil\Lib" /libpath:"PsApi"
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 dft3_dll.lib psapi.lib version.lib winmm.lib dinput.lib ws2_32.lib Keydll1.lib Keydll2.lib Keydll3.lib jpeglib.lib /nologo /stack:0x200000 /subsystem:windows /incremental:no /machine:I386 /out:"Release_DFT3/UHD_DFT3_3in1.exe" /pdbtype:sept /libpath:"PsApi"
# SUBTRACT LINK32 /debug
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy Release_DFT3\UHD_DFT3_3in1.exe D:\DFT\3in1
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "DATsys - Win32 Debug"
# Name "DATsys - Win32 Debug_DFT2"
# Name "DATsys - Win32 Release"
# Name "DATsys - Win32 Release_DFT2"
# Name "DATsys - Win32 Debug_DFT3"
# Name "DATsys - Win32 Release_DFT3"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AddModelDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AvConvertorCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\CommThread.cpp
# End Source File
# Begin Source File

SOURCE=.\CurrentSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\DATsys.rc
# End Source File
# Begin Source File

SOURCE=.\Dmm.cpp
# End Source File
# Begin Source File

SOURCE=.\Dmmdlg.cpp
# End Source File
# Begin Source File

SOURCE=.\field.cpp
# End Source File
# Begin Source File

SOURCE=.\FileTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\FileTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\FlexUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\font1.cpp
# End Source File
# Begin Source File

SOURCE=.\FullSeqDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GetToolOptionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Gmes.cpp
# End Source File
# Begin Source File

SOURCE=.\HdcpTestDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HdmiGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\I2cAdcManualCheckDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\InitDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Jpegfile.cpp
# End Source File
# Begin Source File

SOURCE=.\KeyDownloadCheckDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ButtonST\LedButton.cpp
# End Source File
# Begin Source File

SOURCE=.\LnbDetectorCtl.cpp
# End Source File
# Begin Source File

SOURCE=.\ManualTestResultInputDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ManualTestResultInputDlg.h
# End Source File
# Begin Source File

SOURCE=.\Markup.cpp
# End Source File
# Begin Source File

SOURCE=.\mmbevelx.cpp
# End Source File
# Begin Source File

SOURCE=.\mmconnectorx.cpp
# End Source File
# Begin Source File

SOURCE=.\mmdspcomponentx.cpp
# End Source File
# Begin Source File

SOURCE=.\mmledlabelx.cpp
# End Source File
# Begin Source File

SOURCE=.\mmledpanelx.cpp
# End Source File
# Begin Source File

SOURCE=.\mmoscopex.cpp
# End Source File
# Begin Source File

SOURCE=.\mmpeakdetectx.cpp
# End Source File
# Begin Source File

SOURCE=.\mmspectrumx.cpp
# End Source File
# Begin Source File

SOURCE=.\mmwaveinx.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelInfoCreate.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelInfoCreate_GraberOption.cpp
# End Source File
# Begin Source File

SOURCE=.\MyEchoSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\NoDifferentPointDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OptFolderPage.cpp
# End Source File
# Begin Source File

SOURCE=.\OptHdcpPage.cpp
# End Source File
# Begin Source File

SOURCE=.\picture.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RemoteDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ResultDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Rs232cAdcManualCheckDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ScannerCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectChassisModelDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SeqRemoteDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetAudioSpecDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetDateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SignalTestResultDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SortedArray.h
# End Source File
# Begin Source File

SOURCE=.\SoundCard.cpp
# End Source File
# Begin Source File

SOURCE=.\Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\StaticFilespec.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StopProcessingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StopStepDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Support.cpp
# End Source File
# Begin Source File

SOURCE=.\TextOutCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TrueColorToolBarCtrl_src\TrueColorToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\UserMsgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\vsflexdatasource.cpp
# End Source File
# Begin Source File

SOURCE=.\vsflexgrid.cpp
# End Source File
# Begin Source File

SOURCE=.\vsflexnode.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AddModelDlg.h
# End Source File
# Begin Source File

SOURCE=.\AvConvertorCtrl.h
# End Source File
# Begin Source File

SOURCE=.\CommThread.h
# End Source File
# Begin Source File

SOURCE=.\CSpreadSheet.h
# End Source File
# Begin Source File

SOURCE=.\CurrentSetting.h
# End Source File
# Begin Source File

SOURCE=.\Dmm.h
# End Source File
# Begin Source File

SOURCE=.\Dmmdlg.h
# End Source File
# Begin Source File

SOURCE=.\field.h
# End Source File
# Begin Source File

SOURCE=.\FlexUtil.h
# End Source File
# Begin Source File

SOURCE=.\font1.h
# End Source File
# Begin Source File

SOURCE=.\FullSeqDlg.h
# End Source File
# Begin Source File

SOURCE=.\GetToolOptionDlg.h
# End Source File
# Begin Source File

SOURCE=.\Gmes.h
# End Source File
# Begin Source File

SOURCE=.\HdcpTestDlg.h
# End Source File
# Begin Source File

SOURCE=.\HdmiGenerator.h
# End Source File
# Begin Source File

SOURCE=.\I2cAdcManualCheckDlg.h
# End Source File
# Begin Source File

SOURCE=.\Icmpdefs.h
# End Source File
# Begin Source File

SOURCE=.\InitDlg.h
# End Source File
# Begin Source File

SOURCE=.\Jpegfile.h
# End Source File
# Begin Source File

SOURCE=.\keydll1.h
# End Source File
# Begin Source File

SOURCE=.\keydll2.h
# End Source File
# Begin Source File

SOURCE=.\keydll3.h
# End Source File
# Begin Source File

SOURCE=.\KeyDownloadCheckDlg.h
# End Source File
# Begin Source File

SOURCE=.\ButtonST\LedButton.h
# End Source File
# Begin Source File

SOURCE=.\LnbDetectorCtl.h
# End Source File
# Begin Source File

SOURCE=.\Markup.h
# End Source File
# Begin Source File

SOURCE=.\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\mmbevelx.h
# End Source File
# Begin Source File

SOURCE=.\mmconnectorx.h
# End Source File
# Begin Source File

SOURCE=.\mmdspcomponentx.h
# End Source File
# Begin Source File

SOURCE=.\mmledlabelx.h
# End Source File
# Begin Source File

SOURCE=.\mmledpanelx.h
# End Source File
# Begin Source File

SOURCE=.\mmoscopex.h
# End Source File
# Begin Source File

SOURCE=.\mmpeakdetectx.h
# End Source File
# Begin Source File

SOURCE=.\mmspectrumx.h
# End Source File
# Begin Source File

SOURCE=.\mmwaveinx.h
# End Source File
# Begin Source File

SOURCE=.\ModelInfoCreate.h
# End Source File
# Begin Source File

SOURCE=.\ModelInfoCreate_GraberOption.h
# End Source File
# Begin Source File

SOURCE=.\MSVIDDRV.H
# End Source File
# Begin Source File

SOURCE=.\MSVIDEO.H
# End Source File
# Begin Source File

SOURCE=.\MyEchoSocket.h
# End Source File
# Begin Source File

SOURCE=.\NoDifferentPointDlg.h
# End Source File
# Begin Source File

SOURCE=.\OptFolderPage.h
# End Source File
# Begin Source File

SOURCE=.\OptHdcpPage.h
# End Source File
# Begin Source File

SOURCE=.\PciExpWrapper.h
# End Source File
# Begin Source File

SOURCE=.\picture.h
# End Source File
# Begin Source File

SOURCE=.\ProgDlg.h
# End Source File
# Begin Source File

SOURCE=.\RemoteDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ResultDlg.h
# End Source File
# Begin Source File

SOURCE=.\Rs232cAdcManualCheckDlg.h
# End Source File
# Begin Source File

SOURCE=.\ScannerCtrl.h
# End Source File
# Begin Source File

SOURCE=.\SelectChassisModelDlg.h
# End Source File
# Begin Source File

SOURCE=.\SeqRemoteDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetAudioSpecDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetDateDlg.h
# End Source File
# Begin Source File

SOURCE=.\SignalTestResultDlg.h
# End Source File
# Begin Source File

SOURCE=.\SoundCard.h
# End Source File
# Begin Source File

SOURCE=.\Splash.h
# End Source File
# Begin Source File

SOURCE=.\StaticFilespec.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StopProcessingDlg.h
# End Source File
# Begin Source File

SOURCE=.\StopStepDlg.h
# End Source File
# Begin Source File

SOURCE=.\Support.h
# End Source File
# Begin Source File

SOURCE=.\TextOutCtrl.h
# End Source File
# Begin Source File

SOURCE=.\TrueColorToolBarCtrl_src\TrueColorToolBar.h
# End Source File
# Begin Source File

SOURCE=.\UserMsgDlg.h
# End Source File
# Begin Source File

SOURCE=.\vsflexdatasource.h
# End Source File
# Begin Source File

SOURCE=.\vsflexgrid.h
# End Source File
# Begin Source File

SOURCE=.\vsflexnode.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_f.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_n.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_p.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004_2IN1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CHECK.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00002.cur
# End Source File
# Begin Source File

SOURCE=.\dat_logo.bmp
# End Source File
# Begin Source File

SOURCE=.\DatLog.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DatLog.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DATsys.ico
# End Source File
# Begin Source File

SOURCE=.\res\DATsys.rc2
# End Source File
# Begin Source File

SOURCE=.\res\DATsysDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\DefaultPattern.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DFT.ico
# End Source File
# Begin Source File

SOURCE=.\res\dtv3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DTVSplashWindow.BMP
# End Source File
# Begin Source File

SOURCE=.\res\FAIL.bmp
# End Source File
# Begin Source File

SOURCE=.\res\FAIL2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\idr_tree.cur
# End Source File
# Begin Source File

SOURCE=.\res\LedOff.ico
# End Source File
# Begin Source File

SOURCE=.\res\LedOn.ico
# End Source File
# Begin Source File

SOURCE=.\res\logo_2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\manual_check.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MixerControls.ico
# End Source File
# Begin Source File

SOURCE=.\res\NGbmp.bmp
# End Source File
# Begin Source File

SOURCE=.\res\OKbmp.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PASS.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PASS1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PASS2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\smallico.bmp
# End Source File
# Begin Source File

SOURCE=.\res\smallicon.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Splsh16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\start.bmp
# End Source File
# Begin Source File

SOURCE=.\res\StartBtn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Status.bmp
# End Source File
# Begin Source File

SOURCE=.\res\stop.bmp
# End Source File
# Begin Source File

SOURCE=.\res\StopBtn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tfdropcopy.cur
# End Source File
# Begin Source File

SOURCE=.\res\tfnodropcopy.cur
# End Source File
# Begin Source File

SOURCE=.\res\tfnodropmove.cur
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolbarDraw.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolbarDraw256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolbarDraw256_2IN1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolbarDrawDisabled.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolbarDrawDisabled256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolbarDrawDisabled256_2IN1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolbarDummy.bmp
# End Source File
# Begin Source File

SOURCE=.\res\xcanvas.bmp
# End Source File
# End Group
# Begin Group "Main Class"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DATsys.cpp
# End Source File
# Begin Source File

SOURCE=.\DATsys.h
# End Source File
# Begin Source File

SOURCE=.\DATsysDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\DATsysDoc.h
# End Source File
# Begin Source File

SOURCE=.\DATsysView.cpp
# End Source File
# Begin Source File

SOURCE=.\DATsysView.h
# End Source File
# Begin Source File

SOURCE=.\Defines.h
# End Source File
# Begin Source File

SOURCE=.\Global.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# End Group
# Begin Group "Image Process"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BaseImage.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseImage.h
# End Source File
# Begin Source File

SOURCE=.\ImageProc.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageProc.h
# End Source File
# End Group
# Begin Group "IniAccess"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\IniAccess.cpp
# End Source File
# Begin Source File

SOURCE=.\IniAccess.h
# End Source File
# End Group
# Begin Group "ProgressControl"

# PROP Default_Filter ""
# Begin Group "ProgressXControl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Drawgdix.h
# End Source File
# Begin Source File

SOURCE=.\ProgressCtrlX.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressCtrlX.h
# End Source File
# End Group
# End Group
# Begin Group "ColorStatic"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ColorStaticST.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorStaticST.h
# End Source File
# End Group
# Begin Group "ImageProcessingDlg"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GrabPage.cpp
# End Source File
# Begin Source File

SOURCE=.\GrabPage.h
# End Source File
# Begin Source File

SOURCE=.\ImageTotalDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageTotalDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParmAdjPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ParmAdjPage.h
# End Source File
# Begin Source File

SOURCE=.\ReviewPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ReviewPage.h
# End Source File
# End Group
# Begin Group "PatternGenControl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PatternGeneratorCtl.cpp
# End Source File
# Begin Source File

SOURCE=.\PatternGeneratorCtl.h
# End Source File
# End Group
# Begin Group "Parsor_Sequence"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\JumpStep.cpp
# End Source File
# Begin Source File

SOURCE=.\JumpStep.h
# End Source File
# Begin Source File

SOURCE=.\Parser.cpp
# End Source File
# Begin Source File

SOURCE=.\Parser.h
# End Source File
# Begin Source File

SOURCE=.\Realfunc_New2.cpp
# End Source File
# Begin Source File

SOURCE=.\RealMeas.h
# End Source File
# Begin Source File

SOURCE=.\Step.h
# End Source File
# Begin Source File

SOURCE=.\StepMeasFunc.cpp
# End Source File
# Begin Source File

SOURCE=.\StepMeasFunc.h
# End Source File
# Begin Source File

SOURCE=.\StringTokenizer.cpp
# End Source File
# Begin Source File

SOURCE=.\StringTokenizer.h
# End Source File
# End Group
# Begin Group "RS232"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TVCommCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TVCommCtrl.h
# End Source File
# End Group
# Begin Group "Sequence"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SeqEditorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SeqEditorDlg.h
# End Source File
# Begin Source File

SOURCE=.\SeqRoiDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SeqRoiDlg.h
# End Source File
# End Group
# Begin Group "AvSwitchBoxControl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AvSwitchBoxCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\AvSwitchBoxCtrl.h
# End Source File
# End Group
# Begin Group "MMTools"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Register.cpp
# End Source File
# Begin Source File

SOURCE=.\Register.h
# End Source File
# End Group
# Begin Group "I2cAdc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\I2cAdcCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\I2cAdcCtrl.h
# End Source File
# End Group
# Begin Group "OptionDlg"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\OptComportPage.cpp
# End Source File
# Begin Source File

SOURCE=.\OptComportPage.h
# End Source File
# Begin Source File

SOURCE=.\OptEtcPage.cpp
# End Source File
# Begin Source File

SOURCE=.\OptEtcPage.h
# End Source File
# Begin Source File

SOURCE=.\OptGrabberPage.cpp
# End Source File
# Begin Source File

SOURCE=.\OptGrabberPage.h
# End Source File
# Begin Source File

SOURCE=.\OptionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionDlg.h
# End Source File
# Begin Source File

SOURCE=.\OptMesPage.cpp
# End Source File
# Begin Source File

SOURCE=.\OptMesPage.h
# End Source File
# Begin Source File

SOURCE=.\OptSpecPage.cpp
# End Source File
# Begin Source File

SOURCE=.\OptSpecPage.h
# End Source File
# Begin Source File

SOURCE=.\OptUserInfoPage.cpp
# End Source File
# Begin Source File

SOURCE=.\OptUserInfoPage.h
# End Source File
# End Group
# Begin Group "EditLog"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EditLog\debug.h
# End Source File
# Begin Source File

SOURCE=.\EditLog\EditLog.cpp
# End Source File
# Begin Source File

SOURCE=.\EditLog\EditLog.h
# End Source File
# Begin Source File

SOURCE=.\EditLog\editlog_stream.h
# End Source File
# Begin Source File

SOURCE=.\EditLog\SubclassWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\EditLog\SubclassWnd.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\DFT_Update.txt
# End Source File
# Begin Source File

SOURCE=.\res\FINISH.wav
# End Source File
# Begin Source File

SOURCE=.\res\Msg.wav
# End Source File
# Begin Source File

SOURCE=.\res\NG.WAV
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\res\SOUND136.WAV
# End Source File
# Begin Source File

SOURCE=".\res\User Msg.wav"
# End Source File
# End Target
# End Project
# Section DATsys : {21C2A3C6-D79C-11D1-8818-C199198E9702}
# 	2:5:Class:CMMOscopeX
# 	2:10:HeaderFile:mmoscopex.h
# 	2:8:ImplFile:mmoscopex.cpp
# End Section
# Section DATsys : {C0A63B86-4B21-11D3-BD95-D426EF2C7949}
# 	2:21:DefaultSinkHeaderFile:vsflexgrid.h
# 	2:16:DefaultSinkClass:CVSFlexGrid
# End Section
# Section DATsys : {7519D541-D675-11D1-8818-C199198E9702}
# 	2:5:Class:CMMLabelX1
# 	2:10:HeaderFile:mmlabelx1.h
# 	2:8:ImplFile:mmlabelx1.cpp
# End Section
# Section DATsys : {21C2A3C1-D79C-11D1-8818-C199198E9702}
# 	2:5:Class:CMMConnectorX
# 	2:10:HeaderFile:mmconnectorx.h
# 	2:8:ImplFile:mmconnectorx.cpp
# End Section
# Section DATsys : {D4F4DEC1-D6BC-11D1-8818-C199198E9702}
# 	2:5:Class:CMMLEDLABELX
# 	2:10:HeaderFile:mmledlabelx.h
# 	2:8:ImplFile:mmledlabelx.cpp
# End Section
# Section DATsys : {21C2A3DD-D79C-11D1-8818-C199198E9702}
# 	2:5:Class:CMMSpectrumX
# 	2:10:HeaderFile:mmspectrumx.h
# 	2:8:ImplFile:mmspectrumx.cpp
# End Section
# Section DATsys : {E6E17E90-DF38-11CF-8E74-00A0C90F26F8}
# 	2:5:Class:CMSComm
# 	2:10:HeaderFile:mscomm.h
# 	2:8:ImplFile:mscomm.cpp
# End Section
# Section DATsys : {648A5600-2C6E-101B-82B6-000000000014}
# 	2:21:DefaultSinkHeaderFile:mscomm.h
# 	2:16:DefaultSinkClass:CMSComm
# End Section
# Section DATsys : {F66A4FC3-DC4E-11D1-8818-C199198E9702}
# 	2:21:DefaultSinkHeaderFile:mmwaveinx.h
# 	2:16:DefaultSinkClass:CMMWaveInX
# End Section
# Section DATsys : {9F6AA700-D188-11CD-AD48-00AA003C9CB6}
# 	2:5:Class:CRowCursor
# 	2:10:HeaderFile:rowcursor.h
# 	2:8:ImplFile:rowcursor.cpp
# End Section
# Section DATsys : {04598FC4-866C-11CF-AB7C-00AA00C08FCF}
# 	2:5:Class:CCommandButton
# 	2:10:HeaderFile:commandbutton.h
# 	2:8:ImplFile:commandbutton.cpp
# End Section
# Section DATsys : {D664851D-E237-11D1-8819-C199198E9702}
# 	2:5:Class:CMMPeakDetectX
# 	2:10:HeaderFile:mmpeakdetectx.h
# 	2:8:ImplFile:mmpeakdetectx.cpp
# End Section
# Section DATsys : {8CBBB803-D654-11D1-8818-C199198E9702}
# 	2:21:DefaultSinkHeaderFile:mmledpanelx.h
# 	2:16:DefaultSinkClass:CMMLEDPanelX
# End Section
# Section DATsys : {978C9E23-D4B0-11CE-BF2D-00AA003F40D0}
# 	2:21:DefaultSinkHeaderFile:labelcontrol.h
# 	2:16:DefaultSinkClass:CLabelControl
# End Section
# Section DATsys : {F797004A-D92A-11D1-8688-444553540000}
# 	2:5:Class:CMMDspComponentX
# 	2:10:HeaderFile:mmdspcomponentx.h
# 	2:8:ImplFile:mmdspcomponentx.cpp
# End Section
# Section DATsys : {3D008F56-CF82-11D1-8688-444553540000}
# 	2:5:Class:CMMBevelX
# 	2:10:HeaderFile:mmbevelx.h
# 	2:8:ImplFile:mmbevelx.cpp
# End Section
# Section DATsys : {5F4DF280-531B-11CF-91F6-C2863C385E30}
# 	2:5:Class:CMSFlexGrid
# 	2:10:HeaderFile:msflexgrid.h
# 	2:8:ImplFile:msflexgrid.cpp
# End Section
# Section DATsys : {F66A4FC1-DC4E-11D1-8818-C199198E9702}
# 	2:5:Class:CMMWaveInX
# 	2:10:HeaderFile:mmwaveinx.h
# 	2:8:ImplFile:mmwaveinx.cpp
# End Section
# Section DATsys : {21C2A3C8-D79C-11D1-8818-C199198E9702}
# 	2:21:DefaultSinkHeaderFile:mmoscopex.h
# 	2:16:DefaultSinkClass:CMMOscopeX
# End Section
# Section DATsys : {BEF6E003-A874-101A-8BBA-00AA00300CAB}
# 	2:5:Class:COleFont
# 	2:10:HeaderFile:font1.h
# 	2:8:ImplFile:font1.cpp
# End Section
# Section DATsys : {6262D3A0-531B-11CF-91F6-C2863C385E30}
# 	2:21:DefaultSinkHeaderFile:msflexgrid.h
# 	2:16:DefaultSinkClass:CMSFlexGrid
# End Section
# Section DATsys : {7519D543-D675-11D1-8818-C199198E9702}
# 	2:21:DefaultSinkHeaderFile:mmlabelx1.h
# 	2:16:DefaultSinkClass:CMMLabelX1
# End Section
# Section DATsys : {21C2A3DF-D79C-11D1-8818-C199198E9702}
# 	2:21:DefaultSinkHeaderFile:mmspectrumx.h
# 	2:16:DefaultSinkClass:CMMSpectrumX
# End Section
# Section DATsys : {C0A63B83-4B21-11D3-BD95-D426EF2C7949}
# 	2:5:Class:CVSFlexGrid
# 	2:10:HeaderFile:vsflexgrid.h
# 	2:8:ImplFile:vsflexgrid.cpp
# End Section
# Section DATsys : {8E203240-537D-11D3-BD8C-000000000000}
# 	2:5:Class:CVSFlexDataSource
# 	2:10:HeaderFile:vsflexdatasource.h
# 	2:8:ImplFile:vsflexdatasource.cpp
# End Section
# Section DATsys : {21C2A3C3-D79C-11D1-8818-C199198E9702}
# 	2:21:DefaultSinkHeaderFile:mmconnectorx.h
# 	2:16:DefaultSinkClass:CMMConnectorX
# End Section
# Section DATsys : {D4F4DEC3-D6BC-11D1-8818-C199198E9702}
# 	2:21:DefaultSinkHeaderFile:mmledlabelx.h
# 	2:16:DefaultSinkClass:CMMLEDLABELX
# End Section
# Section DATsys : {7BF80981-BF32-101A-8BBB-00AA00300CAB}
# 	2:5:Class:CPicture
# 	2:10:HeaderFile:picture.h
# 	2:8:ImplFile:picture.cpp
# End Section
# Section DATsys : {D7053240-CE69-11CD-A777-00DD01143C57}
# 	2:21:DefaultSinkHeaderFile:commandbutton.h
# 	2:16:DefaultSinkClass:CCommandButton
# End Section
# Section DATsys : {8CBBB801-D654-11D1-8818-C199198E9702}
# 	2:5:Class:CMMLEDPanelX
# 	2:10:HeaderFile:mmledpanelx.h
# 	2:8:ImplFile:mmledpanelx.cpp
# End Section
# Section DATsys : {D664851F-E237-11D1-8819-C199198E9702}
# 	2:21:DefaultSinkHeaderFile:mmpeakdetectx.h
# 	2:16:DefaultSinkClass:CMMPeakDetectX
# End Section
# Section DATsys : {82F2E224-92E8-11D3-9A1D-F2A67FD05A28}
# 	2:5:Class:CVSFlexNode
# 	2:10:HeaderFile:vsflexnode.h
# 	2:8:ImplFile:vsflexnode.cpp
# End Section
# Section DATsys : {04598FC1-866C-11CF-AB7C-00AA00C08FCF}
# 	2:5:Class:CLabelControl
# 	2:10:HeaderFile:labelcontrol.h
# 	2:8:ImplFile:labelcontrol.cpp
# End Section
