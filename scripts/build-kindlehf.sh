#!/usr/bin/env bash
set -e

# Script biên dịch chéo ARMv7 Hard-Float (Cortex-A9 i.MX6 SoloLite) cho Kindle PW2/PW3
# Sử dụng Toolchain arm-linux-gnueabihf hoặc Docker kindle-toolchain

echo "=== GACH Engine: ARMv7 Hard-Float Cross Compilation ==="

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BIN_DIR="$ROOT_DIR/bin"
mkdir -p "$BIN_DIR"

if command -v arm-linux-gnueabihf-g++ >/dev/null 2>&1; then
    echo "Phát hiện arm-linux-gnueabihf-g++. Đang tiến hành biên dịch chéo..."
    arm-linux-gnueabihf-g++ -std=c++17 -O2 -march=armv7-a -mfpu=neon -mfloat-abi=hard \
        -I "$ROOT_DIR/src" \
        $(pkg-config --cflags gtk+-3.0 cairo sqlite3 2>/dev/null || true) \
        "$ROOT_DIR/src/main.cpp" \
        "$ROOT_DIR/src/domain/phonics_syllable.cpp" \
        "$ROOT_DIR/src/domain/flow_fsm.cpp" \
        "$ROOT_DIR/src/application/story_interactor.cpp" \
        "$ROOT_DIR/src/adapters/db/sqlite_story_repository.cpp" \
        "$ROOT_DIR/src/adapters/ui/cairo_renderer.cpp" \
        "$ROOT_DIR/src/adapters/ui/ui_layout.cpp" \
        "$ROOT_DIR/src/adapters/ui/gtk_touch_adapter.cpp" \
        "$ROOT_DIR/src/infrastructure/kindle_power.cpp" \
        "$ROOT_DIR/src/infrastructure/app_config.cpp" \
        -lgtk-3 -lcairo -lsqlite3 \
        -o "$BIN_DIR/gach-armv7"
    echo "Biên dịch ARMv7 thành công: $BIN_DIR/gach-armv7"
else
    echo "[INFO] Chưa cài đặt arm-linux-gnueabihf-g++ trên máy host."
    echo "[INFO] Để build target Kindle, sử dụng Docker toolchain hoặc GitHub Actions CI."
fi
