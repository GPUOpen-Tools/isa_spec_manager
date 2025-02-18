@echo off
:: prebuild.bat --vs 2022
SETLOCAL

rem Print help message
if "%1"=="-h" goto :print_help
if "%1"=="-help" goto :print_help
if "%1"=="--h" goto :print_help
if "%1"=="--help" goto :print_help
if "%1"=="/?" goto :print_help

goto :start

:print_help
echo:
echo This script generates Visual Studio project and solution files for IsaSpecManager on Windows.
echo:
echo Usage:  prebuild.bat ^[options^]
echo:
echo Options:
echo    --cmake              Path to cmake executable to use. If not specified, the cmake from PATH env variable will be used.
echo    --tinyxml2_src_path  Path to TinyXML2 source. If not specified, the default TinyXML2 bundled with the isa_decoder will be used.
echo    --vs                 Microsoft Visual Studio version. Currently supported values are: "2015", "2017", "2019" and "2022". The default is "2022".
echo    --decoder_only       Only include the decoder library for this project; skip the command line interface, examples and tests.
echo:
echo Examples:
echo    prebuild_windows.bat
echo    prebuild_windows.bat --vs 2022
echo    prebuild_windows.bat --vs 2022 --tinyxml2_src_path [path to custom tinyxml2]

goto :exit

:start
set SCRIPT_DIR=%~dp0
set CURRENT_DIR=%CD%

rem Default values
set CMAKE_PATH=cmake
set VS_VER=2022
set DECODER_ONLY=
set TINYXML=

:begin
if [%1]==[] goto :start_cmake
if "%1"=="--cmake" goto :set_cmake
if "%1"=="--vs" goto :set_vs
if "%1"=="--tinyxml2_src_path" goto :set_tinyxml2_src_path
if "%1"=="--decoder_only" goto :set_decoder_only
goto :bad_arg

:set_cmake
set CMAKE_PATH=%2
goto :shift_2args

:set_vs
set VS_VER=%2
goto :shift_2args

:set_tinyxml2_src_path
set TINYXML=-DTINYXML_SRC_PATH=%2
goto :shift_2args

:set_verbose
@echo on
goto :shift_arg

:set_decoder_only
set DECODER_ONLY="-DEXCLUDE_ISA_CLI_EXAMPLES_TESTS=ON"
goto :shift_arg

:shift_2args
rem Shift to the next pair of arguments
shift
:shift_arg
shift
goto :begin

:bad_arg
echo Error: Unexpected argument: %1%. Aborting...
exit /b 1

:start_cmake
set CMAKE_VSARCH=
if "%VS_VER%"=="2015" (
    set CMAKE_VS="Visual Studio 14 2015 Win64"
) else (
    if "%VS_VER%"=="2017" (
        set CMAKE_VS="Visual Studio 15 2017 Win64"
    ) else (
        if "%VS_VER%"=="2019" (
            set CMAKE_VS="Visual Studio 16 2019"
            set CMAKE_VSARCH=-A x64
        ) else (
            if "%VS_VER%"=="2022" (
                set CMAKE_VS="Visual Studio 17 2022"
                set CMAKE_VSARCH=-A x64
            ) else (
                echo Error: Unknown VisualStudio version provided. Aborting...
                exit /b 1
            )
        )
    )
)

rem Create an output folder
set VS_FOLDER=VS%VS_VER%
set OUTPUT_FOLDER=%SCRIPT_DIR%windows\%VS_FOLDER%
if not exist %OUTPUT_FOLDER% (
    mkdir %OUTPUT_FOLDER%
)

rem Invoke cmake with required arguments.
echo:
echo Running cmake to generate a VisualStudio solution...
cd %OUTPUT_FOLDER%
%CMAKE_PATH% %DECODER_ONLY% %TINYXML% -G %CMAKE_VS% %CMAKE_VSARCH% ..\..\..
if not %ERRORLEVEL%==0 (
    echo "ERROR: cmake failed. Aborting..."
    exit /b 1
)
cd %CURRENT_DIR%
echo Done.
