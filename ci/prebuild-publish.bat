@echo off

if %APPVEYOR_REPO_BRANCH% == master (
  if %GITHUB_TOKEN% neq "" (
    call node_modules\.bin\prebuild --strip --runtime electron --target 1.7.13 --target 1.8.7 --target 2.0.0 -u %GITHUB_TOKEN%
    call node_modules\.bin\prebuild --strip --runtime node --target 6.13.1 --target 8.10.0 --target 10.0.0 -u %GITHUB_TOKEN%
  )
)
