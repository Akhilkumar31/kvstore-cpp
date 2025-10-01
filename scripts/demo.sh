#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
./scripts/build.sh
./scripts/run_server.sh &
PID=$!
sleep 1
echo -e "PING
SET a 1
GET a
DEL a
GET a
" | ./build/kvclient 127.0.0.1 6380
kill $PID || true
