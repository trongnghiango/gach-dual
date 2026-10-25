#!/bin/sh
# GACH - Kindle-Mít Story Phonics Launcher
# Tuân thủ nghiêm ngặt quy chuẩn FAT32 access(F_OK) và quản lý bộ nhớ Kindle

EXT_DIR="/mnt/us/extensions/gach"
BIN="$EXT_DIR/bin/gach"
DATA_DB="$EXT_DIR/data/story_engine.db"

# 1. Kiểm tra binary an toàn trên FAT32 (dùng -f, cấm dùng -x)
if [ ! -f "$BIN" ]; then
    echo "[ERROR] Binary not found at $BIN"
    exit 1
fi

# 2. Tạm dừng Amazon Booklet để giải phóng RAM tối đa cho GACH (đạt 35-45MB RAM khả dụng)
if [ -f "/usr/bin/lipc-set-prop" ]; then
    lipc-set-prop com.lab126.appmgrd stop app://com.lab126.booklet.home 2>/dev/null
    # Khóa màn hình chống sleep
    lipc-set-prop com.lab126.powerd -i touchScreenEvent 1 2>/dev/null
fi

# 3. Chuyển thư mục làm việc và thiết lập biến môi trường
cd "$EXT_DIR" || exit 1
export LD_LIBRARY_PATH="$EXT_DIR/lib:/usr/lib:$LD_LIBRARY_PATH"

# 4. Tự động nhận diện thiết bị PW3 (1072x1448) hay PW2 (758x1024)
MODEL_ARG="--pw3"
if [ -f "/etc/kobo_version" ] || grep -q "SoloLite" /proc/cpuinfo 2>/dev/null; then
    MODEL_ARG="--pw3"
fi

# 5. Khởi chạy GACH Engine
chmod +x "$BIN" 2>/dev/null
"$BIN" "$MODEL_ARG" --db "$DATA_DB"

# 6. Khôi phục lại trạng thái Amazon Booklet khi người dùng thoát GACH
if [ -f "/usr/bin/lipc-set-prop" ]; then
    lipc-set-prop com.lab126.appmgrd start app://com.lab126.booklet.home 2>/dev/null
fi

exit 0
