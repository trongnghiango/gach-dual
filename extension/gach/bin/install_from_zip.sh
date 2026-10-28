#!/bin/sh
# ==============================================================================
# GACH Engine: Trình Cài Đặt / Cập Nhật Trực Tiếp Từ File ZIP Trên Kindle PW2/PW3
# Không cần cắm cáp USB - Hỗ trợ nhận file qua KOReader (FTP / Web / WiFi)
# ==============================================================================

HERE="$(dirname "$0")"
EXTDIR="$(cd "$HERE/.." && pwd)"
LOGFILE="$EXTDIR/data/install.log"
mkdir -p "$EXTDIR/data"

log_and_eips() {
    row="$1"
    text="$2"
    echo "[$(date '+%Y-%m-%d %H:%M:%S')] $text" >> "$LOGFILE"
    if which eips >/dev/null 2>&1; then
        eips 1 "$row" "$text" 2>/dev/null || true
    fi
}

clear_screen() {
    if which eips >/dev/null 2>&1; then
        eips -c 2>/dev/null || true
    fi
}

clear_screen
log_and_eips 3 "========================================"
log_and_eips 4 "   GACH ENGINE: CAP NHAT TU FILE ZIP    "
log_and_eips 5 "========================================"
log_and_eips 7 "Dang tim kiem file ZIP tren Kindle..."

ZIP_FILE=""

# Danh sách các đường dẫn ưu tiên kiểm tra
# 1. Thư mục documents (Nơi KOReader Receive Files / Web / Downloads lưu mặc định)
# 2. Thư mục gốc /mnt/us (Nơi người dùng hay thả qua FTP)
# 3. Thư mục downloads
CANDIDATES="
/mnt/us/documents/gach-kual.zip
/mnt/us/gach-kual.zip
/mnt/us/downloads/gach-kual.zip
/mnt/us/documents/gach.zip
/mnt/us/gach.zip
"

for path in $CANDIDATES; do
    if [ -f "$path" ]; then
        ZIP_FILE="$path"
        break
    fi
done

# Nếu chưa thấy file cụ thể, thử quét pattern gach*.zip
if [ -z "$ZIP_FILE" ]; then
    for path in /mnt/us/documents/gach*.zip /mnt/us/gach*.zip; do
        if [ -f "$path" ]; then
            case "$path" in
                *.zip)
                    ZIP_FILE="$path"
                    break
                    ;;
            esac
        fi
    done
fi

if [ -z "$ZIP_FILE" ]; then
    log_and_eips 9  "LOI: Khong tim thay file gach-kual.zip!"
    log_and_eips 11 "Hay dung KOReader (Web/FTP) gui file vao:"
    log_and_eips 12 "  -> /mnt/us/documents/gach-kual.zip"
    log_and_eips 14 "Sau do chon lai menu nay tren KUAL."
    sleep 6
    clear_screen
    exit 1
fi

ZIP_NAME="$(basename "$ZIP_FILE")"
log_and_eips 9  "Tim thay: $ZIP_NAME"
log_and_eips 11 "Kich thuoc: $(ls -lh "$ZIP_FILE" 2>/dev/null | awk '{print $5}')"

# 1. Sao lưu database SQLite hiện tại để bảo toàn tiến trình đọc của bé
BACKUP_DB="/tmp/gach_story_engine_backup_$$.db"
rm -f "$BACKUP_DB"
if [ -f "$EXTDIR/data/story_engine.db" ]; then
    log_and_eips 13 "Dang sao luu tien do doc cua be..."
    cp -f "$EXTDIR/data/story_engine.db" "$BACKUP_DB"
fi

# 2. Giải nén vào thư mục tạm
TEMP_UNPACK="/tmp/gach_unpack_$$"
rm -rf "$TEMP_UNPACK"
mkdir -p "$TEMP_UNPACK"

log_and_eips 15 "Dang giai nen ban cap nhat..."
unzip -o -q "$ZIP_FILE" -d "$TEMP_UNPACK" 2>> "$LOGFILE"
UNZIP_STATUS=$?

if [ $UNZIP_STATUS -ne 0 ]; then
    log_and_eips 17 "LOI: File ZIP bi hong hoac loi giai nen!"
    rm -rf "$TEMP_UNPACK" "$BACKUP_DB"
    sleep 5
    clear_screen
    exit 1
fi

# 3. Chép đè nội dung mới vào extension
log_and_eips 17 "Dang ghi vao /mnt/us/extensions/gach/..."
if [ -d "$TEMP_UNPACK/extensions/gach" ]; then
    cp -rf "$TEMP_UNPACK/extensions/gach/"* "$EXTDIR/" 2>> "$LOGFILE"
elif [ -d "$TEMP_UNPACK/gach" ]; then
    cp -rf "$TEMP_UNPACK/gach/"* "$EXTDIR/" 2>> "$LOGFILE"
else
    cp -rf "$TEMP_UNPACK/"* "$EXTDIR/" 2>> "$LOGFILE"
fi
rm -rf "$TEMP_UNPACK"

# 4. Khôi phục lại database đã sao lưu
if [ -f "$BACKUP_DB" ]; then
    log_and_eips 19 "Khoi phuc database: Da bao toan tien do."
    cp -f "$BACKUP_DB" "$EXTDIR/data/story_engine.db"
    rm -f "$BACKUP_DB"
fi

# 5. Cấp quyền thực thi và đồng bộ bộ nhớ flash
chmod +x "$EXTDIR/bin/"* 2>/dev/null || true
sync

# 6. Đổi tên file zip để không bị ghi đè lặp lại
mv "$ZIP_FILE" "${ZIP_FILE}.installed" 2>/dev/null || true

# 7. Thông báo hoàn tất
clear_screen
log_and_eips 3 "========================================"
log_and_eips 4 "    CAP NHAT GACH THANH CONG 100%!     "
log_and_eips 5 "========================================"
log_and_eips 7 "Da cai dat tu: $ZIP_NAME"
log_and_eips 9 "Binary & Tai nguyen E-Ink moi nhat da san sang."
log_and_eips 11 "Tien do doc cua be van duoc giu nguyen."
log_and_eips 13 "-> Bam vao 'GACH Story Phonics' de doc!"
log_and_eips 15 "Man hinh se tro ve KUAL sau 5 giay..."

sleep 5
clear_screen
exit 0
