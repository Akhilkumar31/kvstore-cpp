#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
rm -rf build
mkdir -p build && cd build
cmake ..
make -j
echo "Built: build/kvstore and build/kvclient"
