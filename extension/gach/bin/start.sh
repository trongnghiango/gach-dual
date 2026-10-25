#!/bin/sh
HERE="$(dirname "$0")"
EXTDIR="$(cd "$HERE/.." && pwd)"
cd "$EXTDIR" || exit 1

export GACH_HOME="$EXTDIR"
export LD_LIBRARY_PATH="$EXTDIR/lib:/usr/lib:$LD_LIBRARY_PATH"

LOGFILE="$EXTDIR/data/gach.log"
mkdir -p "$EXTDIR/data"

# Chống tắt màn hình khi đang đọc sách
lipc-set-prop -i com.lab126.powerd preventScreenSaver 1 2>/dev/null || true

# Cấp quyền thực thi cho binary
[ -f "$EXTDIR/bin/gach" ] && chmod +x "$EXTDIR/bin/gach"

# Xóa màn hình E-Ink trước khi khởi chạy để loại bỏ ghosting của KUAL
which eips >/dev/null 2>&1 && eips -c 2>/dev/null || true

# Khởi chạy GACH Engine
if which nice >/dev/null 2>&1; then
    nice -n -2 "$EXTDIR/bin/gach" --pw3 --db "$EXTDIR/data/story_engine.db" >> "$LOGFILE" 2>&1
else
    "$EXTDIR/bin/gach" --pw3 --db "$EXTDIR/data/story_engine.db" >> "$LOGFILE" 2>&1
fi

# Dọn dẹp và khôi phục màn hình khi thoát
which eips >/dev/null 2>&1 && eips -c 2>/dev/null || true
lipc-set-prop -i com.lab126.powerd preventScreenSaver 0 2>/dev/null || true
