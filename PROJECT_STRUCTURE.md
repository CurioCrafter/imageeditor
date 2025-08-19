# Advanced Image Editor - Project Structure

## Overview
This document provides a comprehensive overview of the project structure for the Advanced Image Editor, a Photoshop-class application with AI microservices.

## Directory Structure

```
NewProject/
├── README.md                    # Main project documentation
├── CMakeLists.txt              # Main CMake configuration
├── PROJECT_STRUCTURE.md        # This file - project structure overview
├── scripts/                    # Build and utility scripts
│   ├── build.sh               # Main build script
│   ├── install-deps.sh        # Dependency installation
│   └── run-tests.sh           # Test execution
├── src/                        # Source code
│   ├── main.cpp               # Application entry point
│   ├── core/                  # Core engine components
│   │   ├── engine.h          # Main engine coordinator
│   │   ├── document.h        # Document management
│   │   ├── layer.h           # Layer system
│   │   ├── selection.h       # Selection management
│   │   ├── history.h         # Undo/redo system
│   │   ├── color.h           # Color management
│   │   ├── file.h            # File I/O operations
│   │   └── config.h          # Configuration management
│   ├── ui/                    # User interface components
│   │   ├── mainwindow.h      # Main application window
│   │   ├── canvaswidget.h    # Canvas for image editing
│   │   ├── layerpanel.h      # Layer management panel
│   │   ├── toolpanel.h       # Tool selection panel
│   │   ├── colorpanel.h      # Color picker panel
│   │   ├── historypanel.h    # History/undo panel
│   │   ├── propertiespanel.h # Properties inspector
│   │   └── aipanel.h         # AI tools panel
│   ├── gpu/                   # GPU acceleration layer
│   │   ├── renderer.h        # GPU renderer
│   │   ├── shaders.h         # Shader management
│   │   ├── textures.h        # Texture management
│   │   └── compute.h         # Compute shaders
│   ├── plugins/               # Plugin system
│   │   ├── plugin.h          # Plugin interface
│   │   ├── manager.h         # Plugin manager
│   │   └── api.h             # Plugin API
│   ├── ai/                    # AI integration layer
│   │   ├── aiservice.h       # AI service client
│   │   ├── models.h          # AI model management
│   │   └── processing.h      # AI processing pipeline
│   └── tools/                 # Command-line utilities
│       ├── converter.h        # Image format converter
│       └── batch.h            # Batch processing tool
├── services/                   # Microservices
│   ├── docker-compose.yml     # Service orchestration
│   ├── ai-inference/          # AI inference service
│   │   ├── Dockerfile        # Service container
│   │   ├── requirements.txt   # Python dependencies
│   │   ├── main.py           # FastAPI application
│   │   └── app/              # Service application code
│   ├── collaboration/         # Real-time collaboration
│   ├── assets/                # Asset management
│   ├── auth/                  # Authentication service
│   └── model-registry/        # AI model registry
├── docs/                       # Documentation
│   ├── architecture/          # System architecture docs
│   ├── api/                   # API documentation
│   ├── user-guide/            # User manual
│   └── developer/             # Developer guides
├── tests/                      # Test suite
│   ├── unit/                  # Unit tests
│   ├── integration/           # Integration tests
│   └── performance/           # Performance tests
├── resources/                  # Application resources
│   ├── images/                # Icons and images
│   ├── shaders/               # GPU shaders
│   ├── brushes/               # Default brushes
│   └── styles/                # UI stylesheets
├── third_party/                # Third-party libraries
├── build/                      # Build output (generated)
├── install/                    # Installation directory (generated)
└── .gitignore                  # Git ignore patterns
```

## Core Components

### 1. Core Engine (`src/core/`)
The core engine provides the foundation for the entire application:

- **Engine**: Central coordinator managing all subsystems
- **Document**: Image document with layers, metadata, and state
- **Layer**: Individual layers with various types (raster, adjustment, vector, text)
- **Selection**: Selection state and mask management
- **History**: Undo/redo system with branching support
- **Color**: Color management with ICC profiles and OCIO support
- **File**: File I/O operations for various formats

### 2. User Interface (`src/ui/`)
Qt-based user interface components:

- **MainWindow**: Primary application window with dockable panels
- **CanvasWidget**: High-performance canvas for image editing
- **LayerPanel**: Layer management and organization
- **ToolPanel**: Tool selection and configuration
- **ColorPanel**: Color picker and palette management
- **HistoryPanel**: Undo/redo history visualization
- **PropertiesPanel**: Property inspector for selected elements
- **AIPanel**: AI tools and processing options

### 3. GPU Acceleration (`src/gpu/`)
GPU-accelerated rendering and processing:

- **Renderer**: GPU renderer using Vulkan/Metal/Direct3D
- **Shaders**: Shader compilation and management
- **Textures**: Texture management and caching
- **Compute**: Compute shaders for image processing

### 4. Plugin System (`src/plugins/`)
Extensible plugin architecture:

- **Plugin**: Plugin interface and lifecycle management
- **Manager**: Plugin loading and management
- **API**: Plugin development API

### 5. AI Integration (`src/ai/`)
AI service integration layer:

- **AIService**: Client for AI microservices
- **Models**: AI model management and versioning
- **Processing**: AI processing pipeline and tile management

## Microservices Architecture

### 1. AI Inference Service (`services/ai-inference/`)
- **Purpose**: AI model serving and inference
- **Technologies**: FastAPI, PyTorch, ONNX Runtime, TensorRT
- **Features**: Object segmentation, inpainting, upscaling, style transfer
- **Deployment**: Docker with GPU support

### 2. Collaboration Service (`services/collaboration/`)
- **Purpose**: Real-time collaborative editing
- **Technologies**: WebSocket, CRDT, Redis
- **Features**: Multi-user editing, conflict resolution, presence

### 3. Asset Management Service (`services/assets/`)
- **Purpose**: Asset storage and version control
- **Technologies**: PostgreSQL, MinIO, S3-compatible storage
- **Features**: Asset deduplication, versioning, search

### 4. Authentication Service (`services/auth/`)
- **Purpose**: User authentication and authorization
- **Technologies**: JWT, OAuth, PostgreSQL, Redis
- **Features**: User management, role-based access, licensing

### 5. Model Registry Service (`services/model-registry/`)
- **Purpose**: AI model management and distribution
- **Technologies**: PostgreSQL, model versioning, distribution
- **Features**: Model discovery, versioning, deployment

## Build System

### CMake Configuration
- **Main CMakeLists.txt**: Project configuration and dependencies
- **Subdirectory CMakeLists.txt**: Component-specific build rules
- **Cross-platform support**: Windows, macOS, Linux

### Build Scripts
- **build.sh**: Main build script with configuration options
- **install-deps.sh**: Dependency installation script
- **run-tests.sh**: Test execution script

## Development Workflow

### 1. Setup
```bash
# Clone repository
git clone <repository-url>
cd NewProject

# Install dependencies
./scripts/install-deps.sh

# Build project
./scripts/build.sh
```

### 2. Development
```bash
# Start microservices
docker-compose up -d

# Run application
./install/bin/image-editor

# Run tests
./scripts/run-tests.sh
```

### 3. Testing
- **Unit tests**: Component-level testing
- **Integration tests**: Service integration testing
- **Performance tests**: Performance benchmarking
- **UI tests**: User interface testing

## Deployment

### Local Development
- **Docker Compose**: Local microservice orchestration
- **Development mode**: Hot reloading and debugging

### Production Deployment
- **Kubernetes**: Container orchestration
- **Monitoring**: Prometheus + Grafana
- **Logging**: Structured logging with ELK stack
- **Security**: HTTPS, authentication, authorization

## Technology Stack

### Core Technologies
- **C++20**: Core engine and performance-critical components
- **Qt 6**: Cross-platform user interface
- **Vulkan/Metal/Direct3D**: GPU acceleration
- **CMake**: Build system

### AI and ML
- **PyTorch**: Deep learning framework
- **ONNX Runtime**: Model inference
- **TensorRT**: GPU optimization
- **FastAPI**: AI service API

### Infrastructure
- **PostgreSQL**: Primary database
- **Redis**: Caching and session management
- **MinIO**: Object storage
- **Docker**: Containerization
- **Nginx**: Reverse proxy

### Monitoring and Observability
- **Prometheus**: Metrics collection
- **Grafana**: Visualization and dashboards
- **OpenTelemetry**: Distributed tracing
- **Structured logging**: JSON-formatted logs

## Performance Considerations

### GPU Acceleration
- **Tile-based rendering**: Large image support
- **GPU compute**: Parallel processing
- **Memory management**: Out-of-core processing

### AI Processing
- **Tile-based inference**: Large image support
- **Progressive preview**: Real-time feedback
- **Local fallback**: Offline processing capability

### Scalability
- **Microservices**: Independent scaling
- **Load balancing**: Request distribution
- **Caching**: Performance optimization
- **Async processing**: Non-blocking operations

## Security Features

### Authentication and Authorization
- **JWT tokens**: Secure authentication
- **Role-based access**: Permission management
- **API security**: Rate limiting and validation

### Content Safety
- **AI content filtering**: Safety checks
- **Prompt validation**: Input sanitization
- **Output validation**: Result verification

### Data Protection
- **Encryption**: Data at rest and in transit
- **Access controls**: Fine-grained permissions
- **Audit logging**: Security event tracking

## Future Roadmap

### Phase 1: Foundation (Months 1-2)
- Core engine and UI framework
- Basic image editing capabilities
- GPU acceleration foundation

### Phase 2: Core Features (Months 3-4)
- Advanced editing tools
- Non-destructive adjustments
- Performance optimization

### Phase 3: AI Integration (Months 5-6)
- AI microservices deployment
- Advanced AI tools
- Collaboration features

### Phase 4: Enterprise Features (Months 7-8)
- Enterprise-grade security
- Advanced collaboration
- Plugin marketplace

### Phase 5: Optimization (Months 9-10)
- Performance tuning
- Cross-platform parity
- Advanced features

This project structure provides a solid foundation for building a professional-grade image editor with modern architecture, AI capabilities, and enterprise-grade features.
