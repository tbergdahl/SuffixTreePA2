#!/bin/bash

# Ensure the script stops on errors
set -e

# Create a build directory if it doesn't exist
BUILD_DIR="build"
if [ ! -d "$BUILD_DIR" ]; then
  mkdir "$BUILD_DIR"
fi


cd "$BUILD_DIR"

cmake ..

cmake --build .

./PA2 
