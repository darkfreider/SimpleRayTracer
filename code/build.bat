@echo off

REM Run this file to compile raytracer

SET compiler_options=-nologo -Zi -Fm

IF NOT EXIST "..\build" mkdir ..\build
pushd ..\build

cl %compiler_options% ..\code\main.cpp

popd