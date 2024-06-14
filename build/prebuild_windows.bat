@echo off
:: prebuild.bat --vs 2019
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
echo    --vs                 Microsoft Visual Studio version. Currently supported values are: "2015", "2017", "2019" and "2022". The default is "2019".
echo:
echo Examples:
echo    prebuild.bat
echo    prebuild.bat --vs 2017

goto :exit

:start
set SCRIPT_DIR=%~dp0
set CURRENT_DIR=%CD%

rem Default values
set CMAKE_PATH=cmake
set VS_VER=2019
set TEST=-DTEST_PROJECT=ON
set BUILD_INTERNAL=-DBuildInternal=OFF

:begin
if [%1]==[] goto :start_cmake
if "%1"=="--no-test" goto :set_notest_flag
if "%1"=="--public-only" goto :set_public_build_flag
if "%1"=="--cmake" goto :set_cmake
if "%1"=="--vs" goto :set_vs
if "%1"=="--internal" goto :set_internal_build_flag
goto :bad_arg

:set_notest_flag
goto :start_cmake

:set_internal_build_flag
goto :start_cmake

:set_cmake
set CMAKE_PATH=%2
goto :shift_2args

:set_vs
set VS_VER=%2
goto :shift_2args

:set_verbose
@echo on
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
%CMAKE_PATH% -G %CMAKE_VS% %CMAKE_VSARCH% ..\..\..
if not %ERRORLEVEL%==0 (
    echo "ERROR: cmake failed. Aborting..."
    exit /b 1
)
cd %CURRENT_DIR%
echo Done.
