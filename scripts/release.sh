#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR=build-release

cmake -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release
cmake --build "$BUILD_DIR" --target package --parallel

echo "Release packages are available in $BUILD_DIR" 
