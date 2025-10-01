# KVStore (C++)

[![Build](https://github.com/Akhilkumar31/kvstore-cpp/actions/workflows/ci.yml/badge.svg)](https://github.com/Akhilkumar31/kvstore-cpp/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

A Redis-style key-value database written in **C++17**.  
Implements a simple text protocol (`PING`, `SET`, `GET`, `DEL`), a multithreaded TCP server, and **append-only file (AOF)** persistence with replay on startup.

---

## 🚀 Quickstart

```bash
# install build tools
sudo apt update && sudo apt install -y build-essential cmake git

# build
cd kvstore-cpp/scripts && ./build.sh

# run server
cd .. && ./scripts/run_server.sh

# new terminal: try commands
echo -e "PING\nSET foo bar\nGET foo\nDEL foo\n" | ./build/kvclient 127.0.0.1 6380
