@echo off

cd /d "%~dp0"
cd ../build

:: See https://stackoverflow.com/a/3432895
for /F %%i in ("%1") do set ARCHITECTURE=%%~ni

:: libwdi contains a "detect_64build" vcproj which has the sole purpose
:: of defining a (NO_)BUILD64 define
if /I %ARCHITECTURE%==x64 (
  goto build64
) else (
  goto build32
)

:build64
echo #define BUILD64 > ..\deps\libwdi\libwdi\build64.h
goto eof

:build32
echo #define NO_BUILD64 > ..\deps\libwdi\libwdi\build64.h
goto eof

:eof
