@echo off
rem *****************************************************************************
rem * MAKESUPPORT_DIR has to be set to MakeSupport root directory
rem * There MUSTN'T be any blanks between the '=' sign and the path string
rem * Example: 
rem *     set MAKESUPPORT_DIR=..\..\MakeSupport
rem *****************************************************************************

set MAKESUPPORT_DIR=..\..\..\MakeSupport

rem *****************************************************************************
rem * DO NOT EDIT ANYTHING BELOW THIS
rem *****************************************************************************

if "%MAKESUPPORT_DIR% "==" "   goto ErrorNotSet
if not exist %MAKESUPPORT_DIR% goto ErrorWrongPath

set PATH_OLD=%PATH%
set PATH=%MAKESUPPORT_DIR%\cmd;%PATH%
set CYGWIN=nodosfilewarning

%MAKESUPPORT_DIR%\cmd\make %1 %2 %3 %4 %5 %6 %7 %8 %9

set PATH=%PATH_OLD%
set PATH_OLD=

goto End

:ErrorNotSet
echo ************************************************************************
echo BRS warning: MAKESUPPORT_DIR has to be set to MakeSupport\cmd directory!
echo              Please correct setting in this batch file and try again!
echo ************************************************************************
goto End

:ErrorWrongPath
echo ************************************************************************
echo BRS warning: %MAKESUPPORT_DIR% does not exist
echo              Please correct setting in this batch file and try again!
echo ************************************************************************
goto End

:End
set MAKESUPPORT_DIR=
