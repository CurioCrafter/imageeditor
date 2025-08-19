@echo off
REM Start microservices for Advanced Image Editor

echo Starting Advanced Image Editor microservices...

REM Check if Docker is running
docker info >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: Docker is not running. Please start Docker first.
    pause
    exit /b 1
)

REM Check if docker-compose.yml exists in services directory
if not exist "services\docker-compose.yml" (
    echo ERROR: docker-compose.yml not found in services directory.
    echo Please make sure you're in the project root directory.
    pause
    exit /b 1
)

REM Create missing service directories if they don't exist
echo Creating missing service directories...
if not exist "services\auth" mkdir "services\auth"
if not exist "services\collaboration" mkdir "services\collaboration"
if not exist "services\assets" mkdir "services\assets"
if not exist "services\model-registry" mkdir "services\model-registry"

REM Create basic service files if they don't exist
echo Creating basic service files...

REM Auth service
if not exist "services\auth\Dockerfile" (
    echo Creating auth service files...
    copy "services\auth\Dockerfile" "services\auth\Dockerfile" >nul 2>&1
    copy "services\auth\requirements.txt" "services\auth\requirements.txt" >nul 2>&1
    copy "services\auth\main.py" "services\auth\main.py" >nul 2>&1
)

REM Collaboration service
if not exist "services\collaboration\Dockerfile" (
    echo Creating collaboration service files...
    copy "services\collaboration\Dockerfile" "services\collaboration\Dockerfile" >nul 2>&1
    copy "services\collaboration\requirements.txt" "services\collaboration\requirements.txt" >nul 2>&1
    copy "services\collaboration\main.py" "services\collaboration\main.py" >nul 2>&1
)

REM Assets service
if not exist "services\assets\Dockerfile" (
    echo Creating assets service files...
    copy "services\assets\Dockerfile" "services\assets\Dockerfile" >nul 2>&1
    copy "services\assets\requirements.txt" "services\assets\requirements.txt" >nul 2>&1
    copy "services\assets\main.py" "services\assets\main.py" >nul 2>&1
)

REM Model registry service
if not exist "services\model-registry\Dockerfile" (
    echo Creating model registry service files...
    copy "services\model-registry\Dockerfile" "services\model-registry\Dockerfile" >nul 2>&1
    copy "services\model-registry\requirements.txt" "services\model-registry\requirements.txt" >nul 2>&1
    copy "services\model-registry\main.py" "services\model-registry\main.py" >nul 2>&1
)

REM Change to services directory and start services
echo Starting services from services/ directory...
cd services

REM Start all services in background
echo Starting AI Inference Service...
echo Starting Model Registry Service...
echo Starting Collaboration Service...
echo Starting Asset Management Service...
echo Starting Authentication Service...
echo Starting PostgreSQL Database...
echo Starting Redis Cache...
echo Starting MinIO Object Storage...
echo Starting Nginx Reverse Proxy...
echo Starting Prometheus Monitoring...
echo Starting Grafana Dashboard...

docker-compose up -d

REM Wait a moment for services to start
timeout /t 5 /nobreak >nul

REM Check service status
echo.
echo Checking service status...
docker-compose ps

REM Show service URLs
echo.
echo Services are starting up!
echo.
echo Service URLs:
echo   AI Inference Service:     http://localhost:8080
echo   Model Registry:           http://localhost:8081
echo   Collaboration Service:    http://localhost:8082
echo   Asset Management:         http://localhost:8083
echo   Authentication:           http://localhost:8084
echo   MinIO Console:            http://localhost:9001
echo   Prometheus:               http://localhost:9090
echo   Grafana:                  http://localhost:3000
echo.
echo To view service logs:
echo   docker-compose logs -f [service-name]
echo.
echo To stop services:
echo   docker-compose down
echo.
echo To stop and remove all data:
echo   docker-compose down -v
echo.
echo Now you can build and run the main application:
echo   scripts\build.bat
echo   build\bin\image-editor.exe
echo.
pause
