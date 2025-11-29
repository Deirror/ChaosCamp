@echo off
set QTDIR=C:\Qt\6.10.1\msvc2022_64
cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH="%QTDIR%" -B build_vs
pause
