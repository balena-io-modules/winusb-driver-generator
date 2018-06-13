@echo off

:: node-gyp will put us in the top level build/ directory by default
cd ..\deps\libwdi

:: Configure libwdi with the found WinDDK installation
type ..\config.h
copy ..\config.h msvc\config.h

exit /b %ERRORLEVEL%
