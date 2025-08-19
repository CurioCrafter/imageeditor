@echo off
echo Building Image Editor...

REM Create build directory
if not exist build mkdir build
cd build

REM Configure with CMake - use VS 2022 for Qt 6.9.1 compatibility
cmake -G "Visual Studio 17 2022" -A x64 ^
    -DCMAKE_PREFIX_PATH=C:/Qt/6.9.1/msvc2022_64 ^
    -DBUILD_UI=ON ^
    -DBUILD_GPU=OFF ^
    -DBUILD_AI=OFF ^
    -DBUILD_PLUGINS=OFF ^
    -DBUILD_TESTS=OFF ^
    -DBUILD_DOCS=OFF ^
    ..

if %ERRORLEVEL% NEQ 0 (
    echo CMake configuration failed!
    pause
    exit /b 1
)

echo CMake configuration successful!
echo Building core application only (UI temporarily disabled due to Qt compatibility issues)...

REM Build with MSVC
cmake --build . --config Release

if %ERRORLEVEL% NEQ 0 (
    echo Build failed!
    pause
    exit /b 1
)

echo Build successful!
echo.
echo Note: UI is temporarily disabled due to Qt 6.9.1 + MSVC MOC compatibility issues.
echo The core engine has been built successfully.
echo.
pause
