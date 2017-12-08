@echo off

if %APPVEYOR_REPO_BRANCH% == master (
  if %GITHUB_TOKEN% neq "" (
    call node_modules\.bin\prebuild --runtime electron --target 1.7.9 --strip -u %GITHUB_TOKEN%
    call node_modules\.bin\prebuild --runtime node --target 6.1.0 --strip -u %GITHUB_TOKEN%
  )
)
