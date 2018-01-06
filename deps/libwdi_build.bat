@echo off

set WINDDK_PATH=C:\WinDDK

:: See https://stackoverflow.com/a/3432895
for /F %%i in ("%1") do set ARCHITECTURE=%%~ni
if /I %ARCHITECTURE%==x64 set ARCHITECTURE=amd64
if /I %ARCHITECTURE%==ia32 set ARCHITECTURE=x86

if exist %WINDDK_PATH% goto winddk_exists
echo Make sure WinDDK is installed in %WINDDK_PATH% 1>&2
exit /b 1
:winddk_exists

:: See https://stackoverflow.com/a/11883623
for /f "delims=" %%F in ('dir %WINDDK_PATH% /b /o-n') do set WINDDK_VERSION=%%F

:: Get CoInstaller version
set COINSTALLER_PATH=%WINDDK_PATH%\%WINDDK_VERSION%\redist\wdf\amd64\WdfCoInstaller*.dll
:: See https://stackoverflow.com/a/3432895
for /F %%i in ("%COINSTALLER_PATH%") do set COINSTALLER_NAME=%%~ni
:: See https://ss64.com/nt/syntax-replace.html
set COINSTALLER_VERSION=%COINSTALLER_NAME:WdfCoInstaller0=%

echo Using %WINDDK_PATH%\%WINDDK_VERSION% with WdfCoInstaller %COINSTALLER_VERSION%

:: node-gyp will put us in the top level build/ directory by default
cd ..\deps\libwdi

:: Configure libwdi with the found WinDDK installation
copy ..\config.h msvc\config.h
:: This replacement is done because libwdi doesn't handle backslashes
set WINDDK_PATH_SLASH=%WINDDK_PATH:\=/%
echo #define WDK_DIR "%WINDDK_PATH_SLASH%/%WINDDK_VERSION%" >> msvc\config.h
echo #define WDF_VER %COINSTALLER_VERSION% >> msvc\config.h
type msvc\config.h

:: Fails with the following error if this directory in not in the PATH:
::   winnt.h: error C1189: #error: "No Target Architecture"
echo Adding architecture %ARCHITECTURE% to the path
set PATH=%WINDDK_PATH%\%WINDDK_VERSION%\bin\x86\%ARCHITECTURE%;%PATH%

:: This script builds a static DLL if the "DLL" option is not passed
wdk_build.cmd no_samples
exit /b %ERRORLEVEL%
