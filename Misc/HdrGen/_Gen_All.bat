:: Author: Andreas Wenckebach, Vector Informatik Gmbh
:: Version: 1.01.00 (2014-02-18)
:: Version: 1.02.00 (2014-06-06) Create PLAIN_FOLDER if not existant, create info files.
:: Version: 1.03.00 (2015-02-09) Add compressed ARLE container generation, allow configuring optional example containers
@echo off

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: Configuration
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

::Path to HexView tool and external files
set HEXVIEW_EXE=..\..\..\..\Misc\Hexview\hexview.exe
set XML_MOD_DEF=ModGenBase.xml
set PRIVATE_KEY_SOURCE=..\..\..\..\Misc\DemoKey_2048\rsakeys_2048.txt
set CAL_FOLDER=DemoCal_withoutHeader

:: file format and file name extensions for generated files (added to module name)
set OUTFORMAT=gbf
set PEXT=_plain
set PLAIN_FOLDER=.\GeneratedAndToSignedByGm
set SEXT=_sign
set COMPEXT=_comp

:: Hexview configuration parameters (check hexview tool documentation):
:: - Fill all ( /FA )
:: - alignment (address /Ad<val> and length /Al)
:: - checksum param ("Cs5" for B. endian / "Cs6" for l. endian ECU)
:: - Call Hexview silently ("-s") or not (leave empty)
set ALIGN=/Ad:8 /Al /Af:0xCA
set CS=/Cs5
set FILL=/FA
set SILENT=-s

:: Configure to be gnerated example containers (signed / ARLE COMPRESSED)
set GEN_SIGNED_CONTAINERS=YES
set GEN_ARLE_COMPRESSED_CONTAINERS=NO

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: Check input file existence and create info files
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
IF NOT EXIST %PRIVATE_KEY_SOURCE% call :Error %PRIVATE_KEY_SOURCE%
IF NOT EXIST %HEXVIEW_EXE% call :Error %HEXVIEW_EXE%
IF NOT EXIST %XML_MOD_DEF% call :Error %XML_MOD_DEF%
IF NOT EXIST %CAL_FOLDER% mkdir %CAL_FOLDER%
IF NOT EXIST %PLAIN_FOLDER% mkdir %PLAIN_FOLDER%
cd. > ".\Files with _sign extension are Demo Download containers"
cd. > ".\%PLAIN_FOLDER%\Files with _plain extension are to be send to GM for signing"

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: Generation calls for different Standard download modules.
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: Required Function Parameters for GenAppHeaders (Application files), GenCalHeaders (calibration files):
:: - <module name> : "Module Name" - Needs to match configured module name (XML_MOD_DEF file)
:: - <file name>   : "File name" - File name of raw input file
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: call :<func name>   "<module name>" "<file name>"

:: For all cal files in CAL_FOLDER call function GenCalHeaders (Assumption: module name == file name without extension )
for /f %%f in ('dir /b .\%CAL_FOLDER%') do (
   call :GenCalHeaders "%%~nf" "%CAL_FOLDER%\%%f"
)
call :GenAppHeaders "DemoAppl"      "..\DemoAppl.hex"

:: Move formats not required for testdownload, but only for later signing by GM to defined folder
@echo on
move *%PEXT%.%OUTFORMAT% %PLAIN_FOLDER%
@echo off

goto :EOF

:Error
echo "Input file %~1 not found."
goto :end
goto :EOF

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: Functions to be used to generate files - !! Usually not required to be edited !!
::
:: In case of problems:
:: - check generated error files
:: - check hexview.exe parameters used when calling the below functions in batch and compare to Hexview manual for plausibility
:: - check GENy file module configuration (configuration base for XML_MOD_DEF) and XML_MOD_DEF output for plausibility
:: - Please contact our support in case of questions
::
:: Required Function Parameters are same as for above described for GenAppHeaders, GenCalHeaders.
:: ( GenSignAppFunc and GenSignCalFunc require only the "Module Name" parameter (%~1) )
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:GenAppHeaders
IF NOT EXIST %~2 (
 call :Error %~2
 echo "ERROR: Application file cannot be generated."
 goto :EOF
)
call :GenPlainAppFunc %~1 %~2
IF /I "%GEN_SIGNED_CONTAINERS%" EQU "YES" (
call :GenSignAppFunc %~1
)
IF /I "%GEN_ARLE_COMPRESSED_CONTAINERS%" EQU "YES" (
call :GenCompressedModuleFunc %~1
)
goto :EOF

:GenCalHeaders
IF NOT EXIST %~2 (
 call :Error %~2
 echo "Continue without considering cal file %~1 for header generation."
 goto :EOF
)
call :GenPlainCalFunc %~1 %~2
IF /I "%GEN_SIGNED_CONTAINERS%" EQU "YES" (
call :GenSignCalFunc %~1
)
IF /I "%GEN_ARLE_COMPRESSED_CONTAINERS%" EQU "YES" (
call :GenCompressedModuleFunc %~1
)
goto :EOF

:GenPlainAppFunc
@echo on
%HEXVIEW_EXE% %~2 %SILENT% -xgc_app_plain=%~1 -xml=%XML_MOD_DEF% %FILL% %ALIGN% %CS% /E:%~1%PEXT%.err -o %~1%PEXT%.%OUTFORMAT%
@echo off
goto :EOF

:GenPlainCalFunc
@echo on
%HEXVIEW_EXE% %~2 %SILENT% -xgc_cal_plain=%~1 -xml=%XML_MOD_DEF% %FILL% %ALIGN% %CS% /E:%~1%PEXT%.err -o %~1%PEXT%.%OUTFORMAT%
@echo off
goto :EOF

:GenSignAppFunc
@echo on
%HEXVIEW_EXE% %~1%PEXT%.%OUTFORMAT% %SILENT% -xgc_app_sign -xml=%XML_MOD_DEF% %ALIGN% /cs20 /dp32:%PRIVATE_KEY_SOURCE% /E:%~1%SEXT%.err -o %~1%SEXT%.%OUTFORMAT%
@echo off
goto :EOF

:GenSignCalFunc
@echo on
%HEXVIEW_EXE% %~1%PEXT%.%OUTFORMAT% %SILENT% -xgc_cal_sign -xml=%XML_MOD_DEF% %ALIGN% /cs20 /dp32:%PRIVATE_KEY_SOURCE% /E:%~1%SEXT%.err -o %~1%SEXT%.%OUTFORMAT%
@echo off
goto :EOF

:GenCompressedModuleFunc
@echo on
%HEXVIEW_EXE% %~1%SEXT%.%OUTFORMAT% %SILENT% -dp36 -xgc_sign_cmpr /E:%~1%COMPEXT%.err -o %~1%COMPEXT%.%OUTFORMAT%
@echo off
goto :EOF

:end
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
