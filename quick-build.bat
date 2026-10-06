@echo off
echo.
echo ========================================
echo NIGHTMARE GENESIS - QUICK BUILD
echo ========================================
echo.

REM Try default SGDK paths on Windows
set SGDK_PATH=

if exist "C:\sgdk" (
    set SGDK_PATH=C:\sgdk
) else if exist "C:\Program Files\sgdk" (
    set SGDK_PATH=C:\Program Files\sgdk
) else if exist "%ProgramFiles%\sgdk" (
    set SGDK_PATH=%ProgramFiles%\sgdk
) else if not "%SGDK_PATH%"==" " (
    REM Use existing environment variable
    goto :check_env
) else (
    echo.
    echo ERROR: SGDK not found in common locations.
    echo.
    echo Install SGDK or set SGDK_PATH environment variable.
    echo.
    pause
    exit /b 1
)

:check_env
if "%SGDK_PATH%"==" " (
    echo ERROR: SGDK_PATH not set and not found.
    pause
    exit /b 1
)

echo [*] SGDK path: %SGDK_PATH%
echo.

if not exist "%SGDK_PATH%" (
    echo ERROR: SGDK not found at %SGDK_PATH%
    pause
    exit /b 1
)

echo [*] Starting build...
echo.

cd nightmare-genesis
make SGDK="%SGDK_PATH%"

if errorlevel 1 (
    echo ERROR: Build failed.
    pause
    exit /b 1
)

echo.
echo [+] Build successful!
echo [+] ROM is in: bin\
echo.
pause
