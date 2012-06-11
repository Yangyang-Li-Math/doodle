# Microsoft Developer Studio Project File - Name="Doodle" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Doodle - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Doodle.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Doodle.mak" CFG="Doodle - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Doodle - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Doodle - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Doodle - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "bin"
# PROP Intermediate_Dir "objects"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "src" /I "src\objects" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "YY_NEVER_INTERACTIVE" /YX /FD /c
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "Doodle - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "bin"
# PROP Intermediate_Dir "src\objects"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "src" /I "src\objects" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "YY_NEVER_INTERACTIVE" /YX /FD /GZ /c
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Doodle - Win32 Release"
# Name "Doodle - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Objects"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\objects\lex.yy.cpp
# End Source File
# Begin Source File

SOURCE=.\src\objects\parser.tab.cpp
# End Source File
# Begin Source File

SOURCE=.\src\objects\parser.tab.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\arrow.cpp
# End Source File
# Begin Source File

SOURCE=.\src\color.cpp
# End Source File
# Begin Source File

SOURCE=.\src\date.cpp
# End Source File
# Begin Source File

SOURCE=.\src\diag_header.cpp
# End Source File
# Begin Source File

SOURCE=.\src\edge.cpp
# End Source File
# Begin Source File

SOURCE=.\src\face.cpp
# End Source File
# Begin Source File

SOURCE=.\src\main.cpp
# End Source File
# Begin Source File

SOURCE=.\src\open_arrow.cpp
# End Source File
# Begin Source File

SOURCE=.\src\parser.l

!IF  "$(CFG)" == "Doodle - Win32 Release"

# Begin Custom Build - lex $(InputPath)
InputPath=.\src\parser.l

"src\objects\lex.yy.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	c:\usr\local\bin\flex.exe -osrc\objects\lex.yy.cpp $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "Doodle - Win32 Debug"

# Begin Custom Build - lex $(InputPath)
InputPath=.\src\parser.l

"src\objects\lex.yy.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	\usr\local\bin\flex.exe -osrc\objects\lex.yy.cpp $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\parser.y

!IF  "$(CFG)" == "Doodle - Win32 Release"

# Begin Custom Build - Bison $(InputPath)
InputPath=.\src\parser.y

BuildCmds= \
	c:\usr\local\bin\bison.exe --no-lines -t -v -b parser -d $(InputPath) \
	copy parser.tab.c src\objects\parser.tab.cpp > nul \
	copy parser.tab.h src\objects\parser.tab.h > nul \
	del parser.* \
	

"src\objects\parser.tab.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"src\objects\parser.tab.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Doodle - Win32 Debug"

# Begin Custom Build - bison $(InputPath)
InputPath=.\src\parser.y

BuildCmds= \
	\usr\local\bin\bison.exe --no-lines -t -v -b parser -d $(InputPath) \
	copy parser.tab.c src\objects\parser.tab.cpp > nul \
	copy parser.tab.h src\objects\parser.tab.h > nul \
	del parser.* \
	

"src\objects\parser.tab.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"src\objects\parser.tab.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\ps_output.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ps_prologue.cpp
# End Source File
# Begin Source File

SOURCE=.\src\push_arrow.cpp
# End Source File
# Begin Source File

SOURCE=.\src\read_instruction.cpp
# End Source File
# Begin Source File

SOURCE=.\src\repeat_arrow.cpp
# End Source File
# Begin Source File

SOURCE=.\src\step.cpp
# End Source File
# Begin Source File

SOURCE=.\src\vect.cpp
# End Source File
# Begin Source File

SOURCE=.\src\vertex.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\src\arrow.h
# End Source File
# Begin Source File

SOURCE=.\src\arrow_symbol.h
# End Source File
# Begin Source File

SOURCE=.\src\color.h
# End Source File
# Begin Source File

SOURCE=.\src\context.h
# End Source File
# Begin Source File

SOURCE=.\src\date.h
# End Source File
# Begin Source File

SOURCE=.\src\diag_header.h
# End Source File
# Begin Source File

SOURCE=.\src\edge.h
# End Source File
# Begin Source File

SOURCE=.\src\face.h
# End Source File
# Begin Source File

SOURCE=.\src\global_def.h
# End Source File
# Begin Source File

SOURCE=.\src\macro.h
# End Source File
# Begin Source File

SOURCE=.\src\open_arrow.h
# End Source File
# Begin Source File

SOURCE=.\src\preprocessor.h
# End Source File
# Begin Source File

SOURCE=.\src\ps_output.h
# End Source File
# Begin Source File

SOURCE=.\src\ps_prologue.h
# End Source File
# Begin Source File

SOURCE=.\src\push_arrow.h
# End Source File
# Begin Source File

SOURCE=.\src\read_instruction.h
# End Source File
# Begin Source File

SOURCE=.\src\repeat_arrow.h
# End Source File
# Begin Source File

SOURCE=.\src\step.h
# End Source File
# Begin Source File

SOURCE=.\src\unistd.h
# End Source File
# Begin Source File

SOURCE=.\src\vect.h
# End Source File
# Begin Source File

SOURCE=.\src\vertex.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\src\TODO
# End Source File
# End Target
# End Project
