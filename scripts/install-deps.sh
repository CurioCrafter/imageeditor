#!/bin/bash

# Dependency installation script for Advanced Image Editor

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}Installing dependencies for Advanced Image Editor...${NC}"

# Detect operating system
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    OS="linux"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    OS="macos"
elif [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "cygwin" ]]; then
    OS="windows"
else
    echo -e "${RED}Unsupported operating system: $OSTYPE${NC}"
    exit 1
fi

echo "Detected OS: $OS"

# Install system dependencies
if [[ "$OS" == "linux" ]]; then
    echo -e "${YELLOW}Installing Linux dependencies...${NC}"
    
    # Detect distribution
    if command -v apt-get &> /dev/null; then
        # Ubuntu/Debian
        sudo apt-get update
        sudo apt-get install -y \
            build-essential \
            cmake \
            qt6-base-dev \
            qt6-opengl-dev \
            libvulkan-dev \
            libopengl-dev \
            libpng-dev \
            libjpeg-dev \
            libtiff-dev \
            libwebp-dev \
            libopenexr-dev \
            git \
            curl \
            wget
    elif command -v yum &> /dev/null; then
        # CentOS/RHEL/Fedora
        sudo yum groupinstall -y "Development Tools"
        sudo yum install -y \
            cmake \
            qt6-qtbase-devel \
            qt6-qtbase-opengl \
            vulkan-devel \
            mesa-libGL-devel \
            libpng-devel \
            libjpeg-turbo-devel \
            libtiff-devel \
            libwebp-devel \
            openexr-devel \
            git \
            curl \
            wget
    else
        echo -e "${RED}Unsupported Linux distribution${NC}"
        exit 1
    fi
    
elif [[ "$OS" == "macos" ]]; then
    echo -e "${YELLOW}Installing macOS dependencies...${NC}"
    
    # Check if Homebrew is installed
    if ! command -v brew &> /dev/null; then
        echo "Installing Homebrew..."
        /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    fi
    
    # Install dependencies
    brew install \
        cmake \
        qt@6 \
        vulkan-headers \
        png \
        jpeg \
        tiff \
        webp \
        openexr \
        git \
        curl \
        wget
    
elif [[ "$OS" == "windows" ]]; then
    echo -e "${YELLOW}Installing Windows dependencies...${NC}"
    echo "Please install the following manually:"
    echo "1. Visual Studio 2019 or later with C++ development tools"
    echo "2. CMake 3.20 or later"
    echo "3. Qt 6.5 or later"
    echo "4. Vulkan SDK"
    echo "5. Git for Windows"
    echo ""
    echo "Or use Chocolatey:"
    echo "choco install cmake qt6 vulkan-sdk git"
fi

# Install Docker and Docker Compose
echo -e "${YELLOW}Checking Docker installation...${NC}"
if ! command -v docker &> /dev/null; then
    echo "Docker not found. Installing Docker..."
    
    if [[ "$OS" == "linux" ]]; then
        curl -fsSL https://get.docker.com -o get-docker.sh
        sudo sh get-docker.sh
        sudo usermod -aG docker $USER
        rm get-docker.sh
    elif [[ "$OS" == "macos" ]]; then
        echo "Please install Docker Desktop for macOS from https://www.docker.com/products/docker-desktop"
    elif [[ "$OS" == "windows" ]]; then
        echo "Please install Docker Desktop for Windows from https://www.docker.com/products/docker-desktop"
    fi
else
    echo "Docker is already installed"
fi

# Check Docker Compose
if ! command -v docker-compose &> /dev/null; then
    echo "Docker Compose not found. Installing Docker Compose..."
    
    if [[ "$OS" == "linux" ]]; then
        sudo curl -L "https://github.com/docker/compose/releases/latest/download/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose
        sudo chmod +x /usr/local/bin/docker-compose
    elif [[ "$OS" == "macos" ]]; then
        echo "Docker Compose should be included with Docker Desktop"
    elif [[ "$OS" == "windows" ]]; then
        echo "Docker Compose should be included with Docker Desktop"
    fi
else
    echo "Docker Compose is already installed"
fi

# Install Python dependencies for AI services
echo -e "${YELLOW}Installing Python dependencies...${NC}"
if command -v python3 &> /dev/null; then
    python3 -m pip install --upgrade pip
    python3 -m pip install \
        fastapi \
        uvicorn \
        torch \
        torchvision \
        opencv-python \
        pillow \
        numpy \
        scipy
else
    echo "Python 3 not found. Please install Python 3.9+ manually."
fi

echo -e "${GREEN}Dependency installation completed!${NC}"
echo ""
echo "Next steps:"
echo "1. Log out and back in (for Docker group changes to take effect)"
echo "2. Start the microservices: cd services && docker-compose up -d"
echo "3. Build the application: ./scripts/build.sh"
