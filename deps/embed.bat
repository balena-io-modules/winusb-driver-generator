@echo off

:: node-gyp will put us in the top level build/ directory by default
copy Release\embedder.exe ..\deps\libwdi\libwdi
mkdir ..\deps\libwdi\Win32\Release\helper

:: Copy installers to locations hardcoded in embedder.exe
copy Release\installer_x86.exe ..\deps\libwdi\Win32\Release\helper\
mkdir ..\deps\libwdi\x64\Release\helper\

copy Release\installer_x64.exe ..\deps\libwdi\x64\Release\helper\
cd ..\deps\libwdi\libwdi

:: Run embedder to include installer & resource binaries in libwdi
embedder.exe embedded.h

exit /b %ERRORLEVEL%
