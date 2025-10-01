#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
mkdir -p data
CONFIG_FILE=${CONFIG_FILE:-config.yaml} ./build/kvstore
