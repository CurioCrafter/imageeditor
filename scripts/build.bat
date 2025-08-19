@echo off
setlocal enabledelayedexpansion

echo ========================================
echo Advanced Image Editor - Build Script
echo ========================================

REM Set Vulkan SDK environment variables
set VULKAN_SDK=C:\VulkanSDK\1.4.321.1
set PATH=%VULKAN_SDK%\Bin;%PATH%
set VULKAN_SDK_INCLUDE_DIR=%VULKAN_SDK%\Include
set VULKAN_SDK_LIBRARY_DIR=%VULKAN_SDK%\Lib

REM Check if CMake is available
where cmake >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo ERROR: CMake not found in PATH
    echo Please install CMake and add it to your PATH
    pause
    exit /b 1
)

REM Check if Qt6 is available
where qmake >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo WARNING: Qt6 qmake not found in PATH
    echo Qt6 integration may not work properly
)

echo.
echo Configuring with CMake...
echo - Qt6 integration enabled
echo - Vulkan acceleration enabled
echo - OpenGL fallback enabled
echo - Enhanced layer system
echo - Tool framework

REM Set build directories
set BUILD_DIR=build
set INSTALL_DIR=install

REM Create build directory if it doesn't exist
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

REM Configure with CMake
cd "%BUILD_DIR%"
cmake .. -G "Visual Studio 17 2022" -A x64 ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DBUILD_UI=ON ^
    -DBUILD_GPU=ON ^
    -DBUILD_AI=OFF ^
    -DBUILD_PLUGINS=OFF ^
    -DBUILD_TESTS=OFF ^
    -DBUILD_DOCS=OFF ^
    -DVulkan_INCLUDE_DIR="%VULKAN_SDK_INCLUDE_DIR%" ^
    -DVulkan_LIBRARY="%VULKAN_SDK_LIBRARY_DIR%\vulkan-1.lib"

if %ERRORLEVEL% neq 0 (
    echo.
    echo ERROR: CMake configuration failed
    pause
    exit /b 1
)

echo.
echo Building project...
cmake --build . --config Release

if %ERRORLEVEL% neq 0 (
    echo.
    echo ERROR: Build failed
    pause
    exit /b 1
)

echo.
echo Installing...
cmake --install .

if %ERRORLEVEL% neq 0 (
    echo.
    echo WARNING: Install failed
)

echo.
echo ========================================
echo Build completed successfully!
echo ========================================
echo.
echo Build directory: %BUILD_DIR%
echo Install directory: %INSTALL_DIR%
echo.
echo The application is ready to run!
echo.
pause
