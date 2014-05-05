@ECHO off

SETLOCAL

SET INSTALL_PATH=%~dp0..\..
SET GENERATOR=%~n0
SET DIRNAME=%~dp0..\%GENERATOR%

IF NOT EXIST "%DIRNAME%" MD "%DIRNAME%"

PUSHD .
CD "%DIRNAME%"
cmake "-DCMAKE_INSTALL_PREFIX=%INSTALL_PATH%" -G "%GENERATOR%" ..
POPD

ENDLOCAL
PAUSE