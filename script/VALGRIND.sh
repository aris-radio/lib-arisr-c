#!/bin/bash

# Move to the project root directory
cd "$(dirname "$0")/.."

# Set variables
DOCKER_IMAGE_NAME="valgrind-test"
DOCKERFILE_DIR="./docker"

# Build the Docker image
echo "Building Docker image: $DOCKER_IMAGE_NAME..."
docker build -t $DOCKER_IMAGE_NAME -f $DOCKERFILE_DIR/Dockerfile .

# Run the container
echo "Running Valgrind inside Docker..."
docker run --rm $DOCKER_IMAGE_NAME
