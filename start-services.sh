#!/bin/bash

# Start microservices for Advanced Image Editor

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}Starting Advanced Image Editor microservices...${NC}"

# Check if Docker is running
if ! docker info &> /dev/null; then
    echo -e "${RED}Docker is not running. Please start Docker first.${NC}"
    exit 1
fi

# Check if docker-compose.yml exists in services directory
if [ ! -f "services/docker-compose.yml" ]; then
    echo -e "${RED}docker-compose.yml not found in services directory.${NC}"
    echo "Please make sure you're in the project root directory."
    exit 1
fi

# Change to services directory and start services
echo -e "${YELLOW}Starting services from services/ directory...${NC}"
cd services

# Start all services in background
echo "Starting AI Inference Service..."
echo "Starting Model Registry Service..."
echo "Starting Collaboration Service..."
echo "Starting Asset Management Service..."
echo "Starting Authentication Service..."
echo "Starting PostgreSQL Database..."
echo "Starting Redis Cache..."
echo "Starting MinIO Object Storage..."
echo "Starting Nginx Reverse Proxy..."
echo "Starting Prometheus Monitoring..."
echo "Starting Grafana Dashboard..."

docker-compose up -d

# Wait a moment for services to start
sleep 5

# Check service status
echo -e "${YELLOW}Checking service status...${NC}"
docker-compose ps

# Show service URLs
echo ""
echo -e "${GREEN}Services are starting up!${NC}"
echo ""
echo "Service URLs:"
echo "  AI Inference Service:     http://localhost:8080"
echo "  Model Registry:           http://localhost:8081"
echo "  Collaboration Service:    http://localhost:8082"
echo "  Asset Management:         http://localhost:8083"
echo "  Authentication:           http://localhost:8084"
echo "  MinIO Console:            http://localhost:9001"
echo "  Prometheus:               http://localhost:9090"
echo "  Grafana:                  http://localhost:3000"
echo ""
echo "To view service logs:"
echo "  docker-compose logs -f [service-name]"
echo ""
echo "To stop services:"
echo "  docker-compose down"
echo ""
echo "To stop and remove all data:"
echo "  docker-compose down -v"
echo ""
echo "Now you can build and run the main application:"
echo "  ./scripts/build.sh"
echo "  ./build/bin/image-editor"
