@echo off
set OUTPUT=program.exe

echo Compiling all C files...
gcc -Wall -Wextra -g main.c RM\*.c VM\*.c parser\*.c -o %OUTPUT%
if %errorlevel% neq 0 (
    echo Compilation failed.
    exit /b %errorlevel%
)

echo.
echo Running program:
echo -------------------------
%OUTPUT%
