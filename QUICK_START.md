# Quick Start Guide - Advanced Image Editor

This guide will help you get the Advanced Image Editor up and running quickly for development and testing.

## Prerequisites

### System Requirements
- **Operating System**: Windows 10+, macOS 10.15+, or Ubuntu 20.04+
- **RAM**: 8GB minimum, 16GB recommended
- **Storage**: 10GB free space
- **GPU**: NVIDIA GPU with CUDA support (optional but recommended)

### Required Software
- **Git**: Version control
- **Docker**: Container runtime
- **Docker Compose**: Service orchestration
- **CMake**: Build system (3.20+)
- **Compiler**: C++20 compatible compiler
  - Windows: Visual Studio 2019+ or MSVC
  - macOS: Xcode 12+ or Clang
  - Linux: GCC 10+ or Clang 12+

## Quick Setup

### 1. Clone the Repository
```bash
git clone <repository-url>
cd NewProject
```

### 2. Start Microservices
```bash
# Start all services in background
docker-compose up -d

# Check service status
docker-compose ps

# View service logs
docker-compose logs -f ai-inference
```

### 3. Build the Application
```bash
# Make build script executable
chmod +x scripts/build.sh

# Build the project
./scripts/build.sh

# Or build manually
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### 4. Run the Application
```bash
# From build directory
./bin/image-editor

# Or from install directory
./install/bin/image-editor
```

## Development Workflow

### Starting Development
```bash
# Start services in development mode
docker-compose -f docker-compose.yml -f docker-compose.dev.yml up -d

# Build in debug mode
BUILD_TYPE=Debug ./scripts/build.sh

# Run with debugging
gdb ./build/bin/image-editor
```

### Testing
```bash
# Run unit tests
cd build
make test

# Run specific tests
ctest -R "test_name"

# Run with coverage
make coverage
```

### Code Quality
```bash
# Format code
find src -name "*.cpp" -o -name "*.h" | xargs clang-format -i

# Static analysis
cd build
make cppcheck
make clang-tidy
```

## Service Architecture

### Core Services
- **AI Inference** (Port 8080): AI model serving
- **Model Registry** (Port 8081): Model management
- **Collaboration** (Port 8082): Real-time editing
- **Assets** (Port 8083): Asset management
- **Auth** (Port 8084): Authentication

### Infrastructure
- **PostgreSQL** (Port 5432): Database
- **Redis** (Port 6379): Cache
- **MinIO** (Port 9000): Object storage
- **Nginx** (Port 80/443): Reverse proxy

## Configuration

### Environment Variables
```bash
# AI Service
export AI_SERVICE_URL=http://localhost:8080
export AI_API_KEY=your-api-key

# Database
export DB_HOST=localhost
export DB_PORT=5432
export DB_NAME=imageeditor
export DB_USER=postgres
export DB_PASSWORD=password

# Storage
export S3_ENDPOINT=http://localhost:9000
export S3_ACCESS_KEY=minioadmin
export S3_SECRET_KEY=minioadmin
```

### Configuration Files
```bash
# Application config
cp config/config.example.ini config/config.ini
# Edit config/config.ini with your settings

# Service configs
cp services/ai-inference/.env.example services/ai-inference/.env
# Edit service environment files
```

## Troubleshooting

### Common Issues

#### Build Failures
```bash
# Clean build
rm -rf build install
./scripts/build.sh

# Check dependencies
./scripts/install-deps.sh
```

#### Service Connection Issues
```bash
# Check service health
curl http://localhost:8080/health

# View service logs
docker-compose logs service-name

# Restart services
docker-compose restart
```

#### GPU Issues
```bash
# Check CUDA installation
nvidia-smi

# Verify Docker GPU support
docker run --rm --gpus all nvidia/cuda:11.8-base-ubuntu20.04 nvidia-smi
```

### Performance Tuning
```bash
# Enable GPU acceleration
export GPU_ACCELERATION=true

# Adjust tile size
export TILE_SIZE=512

# Enable performance monitoring
export PERFORMANCE_MONITORING=true
```

## Next Steps

### 1. Explore the Codebase
- **Core Engine**: `src/core/` - Main application logic
- **UI Components**: `src/ui/` - User interface
- **AI Integration**: `src/ai/` - AI service client
- **Microservices**: `services/` - Backend services

### 2. Run Examples
```bash
# Load sample images
./tools/converter --input samples/ --output output/

# Test AI features
# Use the AI panel in the application
```

### 3. Contribute
- Check `CONTRIBUTING.md` for guidelines
- Create feature branches
- Write tests for new functionality
- Update documentation

### 4. Advanced Features
- **Plugin Development**: See `docs/developer/plugins.md`
- **AI Model Integration**: See `docs/developer/ai-models.md`
- **Performance Optimization**: See `docs/developer/performance.md`

## Support

### Documentation
- **User Guide**: `docs/user-guide/`
- **API Reference**: `docs/api/`
- **Developer Guide**: `docs/developer/`

### Community
- **Issues**: GitHub Issues
- **Discussions**: GitHub Discussions
- **Wiki**: Project Wiki

### Getting Help
1. Check the documentation
2. Search existing issues
3. Create a new issue with details
4. Join community discussions

## Quick Commands Reference

```bash
# Development
docker-compose up -d          # Start services
./scripts/build.sh            # Build application
./build/bin/image-editor      # Run application
docker-compose down           # Stop services

# Testing
make test                     # Run tests
ctest --verbose               # Verbose test output
make coverage                 # Generate coverage report

# Code Quality
clang-format -i src/**/*.{cpp,h}  # Format code
make cppcheck                 # Static analysis
make clang-tidy               # Code quality checks

# Monitoring
docker-compose logs -f        # Follow service logs
curl localhost:8080/health    # Check service health
open http://localhost:3000    # Grafana dashboard
```

This quick start guide should get you up and running with the Advanced Image Editor in no time. Happy coding!
