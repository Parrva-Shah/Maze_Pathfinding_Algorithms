@echo off
setlocal enabledelayedexpansion
echo === Maze Pathfinding Visualizer ===

REM ── Step 1: Ensure Chocolatey is available ──────────────────────────────────
where choco >nul 2>&1
if %errorlevel% neq 0 (
    echo [!] Chocolatey not found.
    echo     Install it from https://chocolatey.org/install, then re-run this script.
    pause
    exit /b 1
)

REM ── Step 2: Ensure MinGW (g++) is installed ──────────────────────────────────
where g++ >nul 2>&1
if %errorlevel% neq 0 (
    echo [*] g++ not found. Installing MinGW via Chocolatey...
    choco install mingw -y
    call refreshenv
)

REM ── Step 3: Locate SFML ──────────────────────────────────────────────────────
REM Look for SFML in a few common locations
set SFML_DIR=

for %%P in (C:\sfml C:\SFML C:\libs\sfml C:\libs\SFML) do (
    if exist "%%P\include\SFML\Graphics.hpp" (
        set SFML_DIR=%%P
        goto :sfml_found
    )
)

echo.
echo [!] SFML not found. One-time setup required:
echo.
echo     1. Download the MinGW 64-bit build from:
echo        https://www.sfml-dev.org/download.php
echo        (choose "GCC ... MinGW ... 64-bit")
echo.
echo     2. Extract the zip and rename/move the folder to:  C:\sfml
echo        It should look like:  C:\sfml\include\SFML\Graphics.hpp
echo.
echo     3. Re-run this script.
echo.
pause
exit /b 1

:sfml_found
echo [*] SFML found at %SFML_DIR%

REM ── Step 4: Compile ──────────────────────────────────────────────────────────
echo [*] Building...
g++ -O2 -std=c++17 *.cpp -o maze_visualizer.exe ^
    -I "%SFML_DIR%\include" ^
    -L "%SFML_DIR%\lib" ^
    -lsfml-graphics -lsfml-window -lsfml-system

if %errorlevel% neq 0 (
    echo [!] Build failed. Check the errors above.
    pause
    exit /b 1
)

REM ── Step 5: Copy DLLs next to the executable ─────────────────────────────────
echo [*] Copying SFML DLLs...
copy /y "%SFML_DIR%\bin\*.dll" . >nul

REM ── Step 6: Run ──────────────────────────────────────────────────────────────
echo [*] Launching...
maze_visualizer.exe
