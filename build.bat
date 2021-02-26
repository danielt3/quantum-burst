@echo off

pushd "%~dp0"
set "TYPE=%1"
set "GDK=%CD%/ext/sgdk"
set "GDK=%GDK:\=/%"
set "GDK_WIN=%CD%\ext\sgdk"
set "PATH=%PATH%;%CD%\sgdk\bin"

if "%TYPE%" EQU "" (
  set "TYPE=release" 
) else if /i "%TYPE%" EQU "debug" (
  set "TYPE=debug"
) else if /i "%TYPE%" EQU "release" (
  set "TYPE=release"
) else if /i "%TYPE%" EQU "clean" (
  set "TYPE=clean"
) else if /i "%TYPE%" EQU "asm" (
  set "TYPE=asm"
) else (
  echo Incorrect build type. Must be debug, release, clean or asm
  popd
  exit /b 1
)

copy /b "%CD%\src\boot\rom_head.c" +,,"%CD%\src\boot\rom_head.c" >NUL 2>&1
"%GDK_WIN%\bin\make" -f "%GDK_WIN%\makefile.gen" %TYPE%

if %ERRORLEVEL% NEQ 0 (
  echo Build failed
  popd
  exit /b 1
)

popd
exit /b 0
