@echo off
del %1.exe 2>nul
gcc %1.c -o %1.exe
%1.exe
del %1.exe
echo.
pause
