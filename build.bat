@echo off
setlocal enabledelayedexpansion

echo.
echo ========================================
echo NIGHTMARE GENESIS - BUILD SCRIPT (Windows)
echo ========================================
echo.

REM Check if git is installed
git --version >nul 2>&1
if errorlevel 1 (
    echo ERROR: Git not found. Install Git from https://git-scm.com/
    pause
    exit /b 1
)

REM Define SGDK path
set SGDK_PATH=%SGDK_PATH%
if "%SGDK_PATH%"==" " (
    set SGDK_PATH=C:\sgdk
)

echo [*] Using SGDK path: %SGDK_PATH%
echo.

REM Check if SGDK exists
if not exist "%SGDK_PATH%" (
    echo.
    echo ERROR: SGDK not found at %SGDK_PATH%
    echo.
    echo Please install SGDK from: https://github.com/Stephane-D/SGDK
    echo.
    echo Or set the environment variable:
    echo   set SGDK_PATH=C:\your\path\to\sgdk
    echo.
    echo Then run this script again.
    echo.
    pause
    exit /b 1
)

echo [*] SGDK found at: %SGDK_PATH%
echo.

REM Clone or update repository
if not exist "nightmare-genesis" (
    echo [*] Cloning repository...
    git clone https://github.com/kiskusforar-droid/nightmare-genesis.git
    if errorlevel 1 (
        echo ERROR: Failed to clone repository.
        pause
        exit /b 1
    )
) else (
    echo [*] Repository already cloned.
    cd nightmare-genesis
    echo [*] Pulling latest changes...
    git pull
    cd ..
)

echo.
echo [*] Entering project directory...
cd nightmare-genesis

REM Create output directories
if not exist "bin" mkdir bin
if not exist "out" mkdir out

echo [*] Directories created.
echo.

REM Compile with make
echo [*] Starting compilation...
echo.

make SGDK="%SGDK_PATH%"

if errorlevel 1 (
    echo.
    echo ERROR: Compilation failed.
    echo.
    pause
    exit /b 1
)

echo.
echo [*] Compilation successful!
echo.

REM Copy ROM to bin folder
if exist "out\nightmare_genesis.bin" (
    echo [*] Copying ROM to bin folder...
    copy "out\nightmare_genesis.bin" "bin\nightmare_genesis.bin" >nul
    echo [*] ROM copied to: bin\nightmare_genesis.bin
) else if exist "out\nightmare_genesis.smd" (
    echo [*] Copying ROM to bin folder...
    copy "out\nightmare_genesis.smd" "bin\nightmare_genesis.smd" >nul
    echo [*] ROM copied to: bin\nightmare_genesis.smd
) else (
    echo WARNING: ROM file not found in out folder.
    echo Please check the compilation output above.
)

echo.
echo ========================================
echo BUILD COMPLETE!
echo ========================================
echo.
echo [+] ROM is ready at:
echo    nightmare-genesis\bin\
echo.
echo [+] Next steps:
echo    1. Open the ROM file with a Mega Drive emulator
echo    2. Recommended emulators:
    echo       - BlastEm (https://www.retrodev.com/blastem/)
echo       - Gens (http://gens.me/)
echo       - RetroArch with Genesis core
echo.
echo [+] Controls:
echo    - D-pad: Move
echo    - Start: Begin/Restart
echo    - Objective: Collect 3 keys and escape!
echo.
pause
