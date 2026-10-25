#!/usr/bin/env bash
set -e

echo "=== GACH Engine: Đóng Gói KUAL Extension Cho Kindle PW2/PW3 ==="

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$ROOT_DIR/dist/extensions/gach"
OUTPUT_ZIP="$ROOT_DIR/gach-kual.zip"

rm -rf "$ROOT_DIR/dist" "$OUTPUT_ZIP"
mkdir -p "$BUILD_DIR/bin" "$BUILD_DIR/data" "$BUILD_DIR/assets/images"

# 1. Sao chép KUAL configuration
cp "$ROOT_DIR/extension/gach/config.xml" "$BUILD_DIR/"
cp "$ROOT_DIR/extension/gach/menu.json" "$BUILD_DIR/"
cp "$ROOT_DIR/extension/gach/bin/start.sh" "$BUILD_DIR/bin/"
chmod +x "$BUILD_DIR/bin/start.sh"

# 2. Sao chép Binary thực thi
if [ -f "$ROOT_DIR/bin/gach" ]; then
    cp "$ROOT_DIR/bin/gach" "$BUILD_DIR/bin/"
    chmod +x "$BUILD_DIR/bin/gach"
    echo "  ✓ Đã nạp binary: $ROOT_DIR/bin/gach"
else
    echo "  [ERROR] Chưa tìm thấy $ROOT_DIR/bin/gach. Hãy chạy 'make' trước."
    exit 1
fi

# 3. Sao chép Database SQLite 3NF
if [ -f "$ROOT_DIR/story_engine.db" ]; then
    cp "$ROOT_DIR/story_engine.db" "$BUILD_DIR/data/"
    echo "  ✓ Đã nạp CSDL: $ROOT_DIR/story_engine.db"
fi

# 4. Sao chép Ảnh minh họa E-Ink
if [ -d "$ROOT_DIR/assets/images" ]; then
    cp -r "$ROOT_DIR/assets/images/"*.png "$BUILD_DIR/assets/images/" 2>/dev/null || true
    echo "  ✓ Đã nạp tài nguyên ảnh PNG Grayscale"
fi

# 5. Đóng gói ZIP chuẩn thư mục Kindle /mnt/us/extensions/gach
cd "$ROOT_DIR/dist"
zip -r "$OUTPUT_ZIP" extensions/
cd "$ROOT_DIR"
rm -rf "$ROOT_DIR/dist"

echo "=========================================================="
echo "  ĐÓNG GÓI THÀNH CÔNG: $OUTPUT_ZIP"
echo "  Kích thước gói: $(ls -lh "$OUTPUT_ZIP" | awk '{print $5}')"
echo "  Cách cài đặt: Giải nén vào thư mục /mnt/us/extensions/ trên Kindle"
echo "=========================================================="
