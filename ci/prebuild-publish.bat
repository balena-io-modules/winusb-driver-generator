@echo off

if %APPVEYOR_REPO_BRANCH% == master (
  if %GITHUB_TOKEN% neq "" (
    call node_modules\.bin\prebuild --all --strip -u %GITHUB_TOKEN%
  )
)
