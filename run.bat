@echo off
setlocal enabledelayedexpansion
echo === Maze Pathfinding Visualizer ===

REM ── Step 1: Locate g++ ───────────────────────────────────────────────────────
set GPP=
where g++ >nul 2>&1
if %errorlevel% equ 0 (
    for /f "delims=" %%i in ('where g++') do set GPP=%%i & goto :gpp_found
)

REM Try common MSYS2 locations
for %%P in (C:\msys64\ucrt64\bin C:\msys64\mingw64\bin C:\msys64\mingw32\bin) do (
    if exist "%%P\g++.exe" (
        set GPP=%%P\g++.exe
        set PATH=%%P;%PATH%
        goto :gpp_found
    )
)

REM Fall back to Chocolatey
where choco >nul 2>&1
if %errorlevel% neq 0 (
    echo [!] g++ not found and Chocolatey is not installed.
    echo     Option A: Install MSYS2 from https://www.msys2.org/ and run:
    echo               pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-sfml
    echo     Option B: Install Chocolatey from https://chocolatey.org/install
    echo               then re-run this script.
    pause
    exit /b 1
)
echo [*] Installing MinGW via Chocolatey...
choco install mingw -y
call refreshenv

:gpp_found
echo [*] Using g++: %GPP%

REM ── Step 2: Locate SFML ──────────────────────────────────────────────────────
set SFML_DIR=
set SFML_BIN=

REM Check MSYS2 ucrt64 / mingw64 first (covers most Windows developer setups)
for %%P in (C:\msys64\ucrt64 C:\msys64\mingw64 C:\msys64\mingw32) do (
    if exist "%%P\include\SFML\Graphics.hpp" (
        set SFML_DIR=%%P
        set SFML_BIN=%%P\bin
        goto :sfml_found
    )
)

REM Check manual install locations
for %%P in (C:\sfml C:\SFML C:\libs\sfml C:\libs\SFML) do (
    if exist "%%P\include\SFML\Graphics.hpp" (
        set SFML_DIR=%%P
        set SFML_BIN=%%P\bin
        goto :sfml_found
    )
)

echo.
echo [!] SFML not found. Choose one of these one-time setup options:
echo.
echo     Option A (recommended - if you have MSYS2):
echo       Open the MSYS2 UCRT64 shell and run:
echo         pacman -S mingw-w64-ucrt-x86_64-sfml
echo.
echo     Option B (manual):
echo       1. Download MinGW 64-bit SFML from https://www.sfml-dev.org/download.php
echo       2. Extract and place at C:\sfml
echo          (so that C:\sfml\include\SFML\Graphics.hpp exists)
echo.
echo     Then re-run this script.
echo.
pause
exit /b 1

:sfml_found
echo [*] SFML found at %SFML_DIR%

REM ── Step 3: Compile ──────────────────────────────────────────────────────────
echo [*] Building...
"%GPP%" -O2 -std=c++17 *.cpp -o maze_visualizer.exe ^
    -I "%SFML_DIR%\include" ^
    -L "%SFML_DIR%\lib" ^
    -lsfml-graphics -lsfml-window -lsfml-system

if %errorlevel% neq 0 (
    echo [!] Build failed. Check the errors above.
    pause
    exit /b 1
)

REM ── Step 4: Copy DLLs (bundled first, then SFML install fallback) ────────────
echo [*] Copying DLLs...
if exist "libs\" (
    copy /y "libs\*.dll" . >nul 2>&1
)
for %%D in (
    libsfml-graphics-2.dll libsfml-window-2.dll libsfml-system-2.dll
    sfml-graphics-2.dll    sfml-window-2.dll    sfml-system-2.dll
    libgcc_s_seh-1.dll libstdc++-6.dll libwinpthread-1.dll
    libfreetype-6.dll libpng16-16.dll zlib1.dll
    libharfbuzz-0.dll libgraphite2.dll libglib-2.0-0.dll
    libbz2-1.dll libbrotlidec.dll libintl-8.dll
) do (
    if exist "%SFML_BIN%\%%D" copy /y "%SFML_BIN%\%%D" . >nul 2>&1
)

REM ── Step 5: Run ──────────────────────────────────────────────────────────────
echo [*] Launching...
maze_visualizer.exe
