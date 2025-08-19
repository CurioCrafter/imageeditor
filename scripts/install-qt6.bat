@echo off
echo ========================================
echo Installing Qt6 for Advanced Image Editor
echo ========================================
echo.

echo This script will help you install Qt6 for the UI components.
echo.

echo Option 1: Download Qt6 Online Installer (Recommended)
echo   - Go to: https://www.qt.io/download-qt-installer
echo   - Download the Qt Online Installer
echo   - Run the installer and select Qt 6.5+ with MSVC 2019/2022
echo   - Install to: C:\Qt
echo.

echo Option 2: Use vcpkg (Alternative)
echo   - Install vcpkg: git clone https://github.com/Microsoft/vcpkg.git
echo   - Run: vcpkg install qt6-base qt6-widgets qt6-tools
echo.

echo Option 3: Use Chocolatey
echo   - Install Chocolatey first
echo   - Run: choco install qt6
echo.

echo After installation, update your PATH to include:
echo   C:\Qt\6.5.x\msvc2019_64\bin
echo   C:\Qt\6.5.x\msvc2019_64\lib
echo.

echo Then rebuild with: scripts\build.bat -DBUILD_UI=ON
echo.

pause
