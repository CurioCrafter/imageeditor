#!/bin/bash

# Start script for AI Inference Service

echo "Starting AI Inference Service..."

# Set default environment variables
export HOST=${HOST:-0.0.0.0}
export PORT=${PORT:-8080}
export LOG_LEVEL=${LOG_LEVEL:-info}
export WORKERS=${WORKERS:-1}

# Start the service
exec python main.py
