#!/bin/sh
HERE="$(dirname "$0")"
EXTDIR="$(cd "$HERE/.." && pwd)"
cd "$EXTDIR" || exit 1

export GACH_HOME="$EXTDIR"
export LD_LIBRARY_PATH="$EXTDIR/lib:/usr/lib:$LD_LIBRARY_PATH"

LOGFILE="$EXTDIR/data/gach.log"
mkdir -p "$EXTDIR/data"

echo "=== [$(date)] Starting GACH Engine ===" >> "$LOGFILE"
echo "EXTDIR=$EXTDIR" >> "$LOGFILE"
echo "LD_LIBRARY_PATH=$LD_LIBRARY_PATH" >> "$LOGFILE"

# Chống tắt màn hình khi đang đọc sách
lipc-set-prop -i com.lab126.powerd preventScreenSaver 1 2>/dev/null || true

# Cấp quyền thực thi cho binary
[ -f "$EXTDIR/bin/gach" ] && chmod +x "$EXTDIR/bin/gach"

# Kiểm tra cơ sở dữ liệu trước khi khởi chạy
if [ ! -f "$EXTDIR/data/story_engine.db" ] && [ -f "$EXTDIR/data/schema.sql" ]; then
    echo "[INFO] Database missing. Auto-creating from schema..." >> "$LOGFILE"
    sqlite3 "$EXTDIR/data/story_engine.db" < "$EXTDIR/data/schema.sql" 2>> "$LOGFILE" || true
    if [ -f "$EXTDIR/data/seed_story_mit_16pages.sql" ]; then
        sqlite3 "$EXTDIR/data/story_engine.db" < "$EXTDIR/data/seed_story_mit_16pages.sql" 2>> "$LOGFILE" || true
    elif [ -f "$EXTDIR/data/seed_story_mit.sql" ]; then
        sqlite3 "$EXTDIR/data/story_engine.db" < "$EXTDIR/data/seed_story_mit.sql" 2>> "$LOGFILE" || true
    fi
fi

# Xóa màn hình E-Ink trước khi khởi chạy để loại bỏ ghosting của KUAL
which eips >/dev/null 2>&1 && eips -c 2>/dev/null || true

# Khởi chạy GACH Engine
if which nice >/dev/null 2>&1; then
    nice -n -2 "$EXTDIR/bin/gach" --pw3 --db "$EXTDIR/data/story_engine.db" >> "$LOGFILE" 2>&1
    RET=$?
else
    "$EXTDIR/bin/gach" --pw3 --db "$EXTDIR/data/story_engine.db" >> "$LOGFILE" 2>&1
    RET=$?
fi

echo "=== [$(date)] GACH Engine exited with code $RET ===" >> "$LOGFILE"

# Nếu có lỗi (exit code != 0), hiển thị thông báo lỗi lên màn hình Kindle trong 5s để dễ chẩn đoán
if [ "$RET" -ne 0 ]; then
    which eips >/dev/null 2>&1 && eips 0 10 "GACH Loi: Thoat voi ma $RET. Xem data/gach.log" 2>/dev/null || true
    sleep 5
fi

# Dọn dẹp và khôi phục màn hình khi thoát
which eips >/dev/null 2>&1 && eips -c 2>/dev/null || true
lipc-set-prop -i com.lab126.powerd preventScreenSaver 0 2>/dev/null || true
