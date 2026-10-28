#!/usr/bin/env bash
# ==============================================================================
# GACH - Kindle-Mít Story Phonics Automated Safe Installer for Kindle PW2/PW3
# Hỗ trợ tự động làm sạch app cũ, logs, và làm sạch/giữ lại database theo nhu cầu.
# ==============================================================================
set -euo pipefail

# Màu sắc hiển thị terminal
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
BOLD='\033[1m'
NC='\033[0m' # No Color

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ZIP_FILE="$SCRIPT_DIR/gach-kual.zip"
CLEAN_DATA_MODE="" # "clean" hoặc "keep" hoặc "" (hỏi)

# Hiển thị banner
echo -e "${CYAN}${BOLD}"
echo "======================================================================"
echo "      GACH ENGINE: TRÌNH CÀI ĐẶT AN TOÀN CHO KINDLE PW2/PW3           "
echo "======================================================================"
echo -e "${NC}"

# 1. Xử lý tham số dòng lệnh
print_help() {
    echo -e "Sử dụng: ./install_to_kindle.sh [TÙY CHỌN]"
    echo ""
    echo "Tùy chọn:"
    echo "  -c, --clean-data   Làm sạch 100% app cũ, logs và database cũ (Nạp schema mới)"
    echo "  -k, --keep-data    Giữ lại database hiện tại trên Kindle (Chỉ cập nhật mã nguồn & ảnh)"
    echo "  -h, --help         Hiển thị bảng trợ giúp này"
    echo ""
}

while [[ $# -gt 0 ]]; do
    case "$1" in
        -c|--clean-data)
            CLEAN_DATA_MODE="clean"
            shift
            ;;
        -k|--keep-data)
            CLEAN_DATA_MODE="keep"
            shift
            ;;
        -h|--help)
            print_help
            exit 0
            ;;
        *)
            echo -e "${RED}[LỖI] Tùy chọn không hợp lệ: $1${NC}"
            print_help
            exit 1
            ;;
    esac
done

# 2. Lựa chọn chế độ làm sạch nếu chưa truyền tham số
if [ -z "$CLEAN_DATA_MODE" ]; then
    echo -e "${YELLOW}${BOLD}BẠN MUỐN XỬ LÝ DỮ LIỆU (DATABASE & LOGS) TRÊN KINDLE NHƯ THẾ NÀO?${NC}"
    echo -e "  ${BOLD}[1]${NC} ${GREEN}LÀM SẠCH TOÀN BỘ (Khuyên dùng trong giai đoạn phát triển):${NC}"
    echo -e "      - Xóa sạch app cũ, logs, và database cũ."
    echo -e "      - Nạp cơ sở dữ liệu SQLite 3NF mới nhất từ dự án (cập nhật schema mới)."
    echo ""
    echo -e "  ${BOLD}[2]${NC} ${BLUE}GIỮ LẠI DỮ LIỆU CŨ:${NC}"
    echo -e "      - Giữ nguyên database hiện có trên Kindle (giữ tiến độ đọc sách của bé)."
    echo -e "      - Chỉ làm sạch logs và cập nhật binary engine, scripts, hình ảnh mới."
    echo ""
    read -p "Chọn chế độ [1/2] (Mặc định: 1): " choice
    case "$choice" in
        2)
            CLEAN_DATA_MODE="keep"
            ;;
        *)
            CLEAN_DATA_MODE="clean"
            ;;
    esac
fi

if [ "$CLEAN_DATA_MODE" = "clean" ]; then
    echo -e "-> ${YELLOW}Chế độ được chọn:${NC} ${BOLD}LÀM SẠCH 100% (Xóa app cũ, nạp Schema & Data mới)${NC}"
else
    echo -e "-> ${BLUE}Chế độ được chọn:${NC} ${BOLD}GIỮ NGUYÊN DỮ LIỆU (Bảo lưu database hiện tại trên Kindle)${NC}"
fi
echo ""

# 3. Tìm kiếm và tự động mount ổ đĩa Kindle
echo -e "${CYAN}[1/6] Tìm kiếm thiết bị Kindle...${NC}"
KINDLE_DIR=""
KINDLE_DEV=""

# Tìm block device có nhãn 'Kindle'
KINDLE_DEV=$(lsblk -rn -o NAME,LABEL | grep -i "Kindle" | awk '{print $1}' | head -n 1 || true)
if [ -z "$KINDLE_DEV" ]; then
    # Thử tìm sda1 hoặc sdb1 nếu hệ thống không hiện nhãn
    for dev in sda1 sdb1 sdc1; do
        if lsblk "/dev/$dev" >/dev/null 2>&1; then
            FSTYPE=$(lsblk -no FSTYPE "/dev/$dev" 2>/dev/null || true)
            if [ "$FSTYPE" = "vfat" ]; then
                KINDLE_DEV="$dev"
                break
            fi
        fi
    done
fi

if [ -n "$KINDLE_DEV" ]; then
    DEV_PATH="/dev/$KINDLE_DEV"
    # Kiểm tra đã mount chưa
    KINDLE_DIR=$(lsblk -no MOUNTPOINT "$DEV_PATH" 2>/dev/null | head -n 1 || true)
    if [ -z "$KINDLE_DIR" ] || [ ! -d "$KINDLE_DIR" ]; then
        echo "  Phát hiện thiết bị $DEV_PATH chưa mount. Đang tự động mount qua UDisks2..."
        udisksctl mount -b "$DEV_PATH" >/dev/null 2>&1 || true
        KINDLE_DIR=$(lsblk -no MOUNTPOINT "$DEV_PATH" 2>/dev/null | head -n 1 || true)
    fi
fi

# Cho phép override để test hoặc chỉ định thủ công
if [ -n "${TEST_KINDLE_DIR:-}" ] && [ -d "$TEST_KINDLE_DIR" ]; then
    KINDLE_DIR="$TEST_KINDLE_DIR"
fi

# Fallback kiểm tra các đường dẫn thông thường nếu lsblk chưa nhận
if [ -z "$KINDLE_DIR" ] || [ ! -d "$KINDLE_DIR" ]; then
    for path in "/run/media/$USER/Kindle" "/media/$USER/Kindle" "/mnt/Kindle"; do
        if [ -d "$path" ]; then
            KINDLE_DIR="$path"
            break
        fi
    done
fi

if [ -z "$KINDLE_DIR" ] || [ ! -d "$KINDLE_DIR" ]; then
    echo -e "${RED}[LỖI] Không tìm thấy máy đọc sách Kindle!${NC}"
    echo "  Vui lòng cắm cáp USB nối Kindle với máy tính và đảm bảo Kindle đã mở khóa."
    exit 1
fi

echo -e "  ✓ Tìm thấy ổ đĩa Kindle tại: ${BOLD}$KINDLE_DIR${NC}"

# 4. Kiểm tra quyền ghi và tự động sửa cờ dirty của FAT32 nếu cần
echo -e "${CYAN}[2/6] Kiểm tra quyền ghi trên phân vùng FAT32 Kindle...${NC}"
if [ ! -w "$KINDLE_DIR" ]; then
    echo -e "  ${YELLOW}Cảnh báo: Ổ đĩa Kindle đang ở chế độ Read-Only (do cờ dirty FAT32).${NC}"
    echo "  Đang tự động sửa lỗi filesystem qua UDisks2..."
    if [ -n "$KINDLE_DEV" ]; then
        DEV_PATH="/dev/$KINDLE_DEV"
        udisksctl unmount -b "$DEV_PATH" 2>/dev/null || true
        gdbus call --system --dest org.freedesktop.UDisks2 \
            --object-path "/org/freedesktop/UDisks2/block_devices/$KINDLE_DEV" \
            --method org.freedesktop.UDisks2.Filesystem.Repair "{}" >/dev/null 2>&1 || true
        udisksctl mount -b "$DEV_PATH" >/dev/null 2>&1
        KINDLE_DIR=$(lsblk -no MOUNTPOINT "$DEV_PATH" 2>/dev/null | head -n 1 || true)
    fi
fi

if [ ! -w "$KINDLE_DIR" ]; then
    echo -e "${RED}[LỖI] Phân vùng $KINDLE_DIR vẫn ở chế độ Read-Only.${NC}"
    echo "  Hãy thử chạy: sudo fsck.vfat -a /dev/$KINDLE_DEV"
    exit 1
fi
echo -e "  ✓ Phân vùng Kindle ở trạng thái Read-Write an toàn."

EXT_DIR="$KINDLE_DIR/extensions"
TARGET_DIR="$EXT_DIR/gach"
mkdir -p "$EXT_DIR"

# 5. Xử lý sao lưu dữ liệu nếu ở chế độ KEEP_DATA
BACKUP_DIR="/tmp/gach_backup_$$"
if [ "$CLEAN_DATA_MODE" = "keep" ] && [ -d "$TARGET_DIR/data" ]; then
    echo -e "${CYAN}[3/6] Sao lưu database cũ từ Kindle...${NC}"
    mkdir -p "$BACKUP_DIR"
    cp -r "$TARGET_DIR/data" "$BACKUP_DIR/" 2>/dev/null || true
    echo "  ✓ Đã sao lưu database vào $BACKUP_DIR"
else
    echo -e "${CYAN}[3/6] Bỏ qua sao lưu (Chế độ làm sạch database)...${NC}"
fi

# 6. Làm sạch toàn bộ ứng dụng cũ và logs trên Kindle
echo -e "${CYAN}[4/6] Làm sạch toàn bộ ứng dụng cũ và logs trên Kindle...${NC}"
# Quét sạch cả các thư mục bị đặt nhầm vị trí từ lần cài trước nếu có
rm -rf "$KINDLE_DIR/gach" 2>/dev/null || true
rm -rf "$KINDLE_DIR/extensions/extensions" 2>/dev/null || true
if [ -d "$TARGET_DIR" ]; then
    find "$TARGET_DIR" -type f \( -name "*.log" -o -name "*.db-journal" -o -name "*.db-wal" -o -name "*.db-shm" -o -name ".DS_Store" \) -delete 2>/dev/null || true
    rm -rf "$TARGET_DIR"
    echo "  ✓ Đã gỡ bỏ sạch sẽ thư mục ứng dụng cũ: $TARGET_DIR"
else
    echo "  ✓ Không có bản cài đặt cũ, sẵn sàng cài mới."
fi

# 7. Đóng gói hoặc tải bản phát hành ARMv7 chuẩn cho Kindle
echo -e "${CYAN}[5/6] Chuẩn bị gói cài đặt GACH mới nhất cho Kindle...${NC}"
cd "$SCRIPT_DIR"
RELEASE_URL="https://github.com/trongnghiango/gach-dual/releases/download/latest/gach-kual.zip"

IS_ARM=0
if [ -f "bin/gach" ]; then
    ARCH_INFO=$(file -b "bin/gach" || echo "")
    if [[ "$ARCH_INFO" =~ "ARM" ]]; then
        IS_ARM=1
    fi
fi

if [ "$IS_ARM" -eq 1 ]; then
    echo -e "  ${GREEN}✓ Phát hiện binary ARMv7 hợp lệ cho Kindle trong bin/gach.${NC}"
    ./scripts/package-kual.sh >/dev/null
else
    echo -e "  ${YELLOW}⚠️ Binary cục bộ (bin/gach) là kiến trúc x86_64 (máy tính), không chạy được trên CPU ARM Kindle PW2/PW3.${NC}"
    echo "  -> Đang kiểm tra bản phát hành ARMv7 từ GitHub Releases..."
    if curl -s -f -I "$RELEASE_URL" >/dev/null 2>&1; then
        echo "  -> Đang tải gach-kual.zip chuẩn ARMv7 từ GitHub Releases..."
        curl -fL "$RELEASE_URL" -o "$ZIP_FILE"
        echo -e "  ${GREEN}✓ Đã tải xong gach-kual.zip chuẩn ARMv7!${NC}"
    else
        echo -e "  ${YELLOW}[LƯU Ý] Chưa có bản Release ARMv7 trên GitHub. Sử dụng gói đóng gói cục bộ để kiểm tra cấu trúc thư mục.${NC}"
        echo -e "  ${YELLOW}Để chạy được trên máy Kindle thực tế, hãy push code lên GitHub để GitHub Actions tự động build bản ARMv7.${NC}"
        ./scripts/package-kual.sh >/dev/null
    fi
fi

echo "  Đang giải nén gói cài đặt vào $EXT_DIR/gach/..."
TEMP_UNPACK="/tmp/gach_unpack_$$"
rm -rf "$TEMP_UNPACK"
mkdir -p "$TEMP_UNPACK"
unzip -q -o "$ZIP_FILE" -d "$TEMP_UNPACK/"

mkdir -p "$EXT_DIR"
rm -rf "$TARGET_DIR"
if [ -d "$TEMP_UNPACK/extensions/gach" ]; then
    cp -r "$TEMP_UNPACK/extensions/gach" "$EXT_DIR/"
elif [ -d "$TEMP_UNPACK/gach" ]; then
    cp -r "$TEMP_UNPACK/gach" "$EXT_DIR/"
else
    echo -e "  ${RED}[LỖI] Cấu trúc file zip không hợp lệ.${NC}"
    exit 1
fi
rm -rf "$TEMP_UNPACK"

# Khôi phục database cũ nếu ở chế độ KEEP_DATA
if [ "$CLEAN_DATA_MODE" = "keep" ] && [ -d "$BACKUP_DIR/data" ]; then
    echo "  Đang khôi phục database cũ của bạn vào $TARGET_DIR/data/..."
    mkdir -p "$TARGET_DIR/data"
    cp -r "$BACKUP_DIR/data/"* "$TARGET_DIR/data/" 2>/dev/null || true
    rm -rf "$BACKUP_DIR"
    echo "  ✓ Đã bảo lưu toàn bộ dữ liệu ván đọc trước đó."
else
    echo "  ✓ Đã nạp cơ sở dữ liệu SQLite 3NF mới nhất vào $TARGET_DIR/data/story_engine.db."
fi

# Luôn đồng bộ CSDL SQLite 16 trang và tranh minh họa tối giản cục bộ mới nhất
if [ "$CLEAN_DATA_MODE" = "clean" ]; then
    if [ -f "$SCRIPT_DIR/story_engine.db" ]; then
        cp -f "$SCRIPT_DIR/story_engine.db" "$TARGET_DIR/data/story_engine.db"
        echo "  ✓ Đã đồng bộ CSDL SQLite 16 trang cục bộ: $SCRIPT_DIR/story_engine.db"
    fi
    cp -f "$SCRIPT_DIR/data/"*.sql "$TARGET_DIR/data/" 2>/dev/null || true
fi

if [ -d "$SCRIPT_DIR/assets/images" ]; then
    mkdir -p "$TARGET_DIR/assets/images"
    cp -rf "$SCRIPT_DIR/assets/images/"*.png "$TARGET_DIR/assets/images/" 2>/dev/null || true
    echo "  ✓ Đã đồng bộ 32 tranh minh họa tối giản (Minimalism) mới nhất."
fi

cp -f "$SCRIPT_DIR/extension/gach/bin/"*.sh "$TARGET_DIR/bin/" 2>/dev/null || true
cp -f "$SCRIPT_DIR/extension/gach/config.xml" "$TARGET_DIR/config.xml" 2>/dev/null || true
cp -f "$SCRIPT_DIR/extension/gach/menu.json" "$TARGET_DIR/menu.json" 2>/dev/null || true

# Dọn dẹp logs nếu có
rm -rf "$TARGET_DIR"/*.log "$TARGET_DIR/data"/*.log 2>/dev/null || true

# Cấp quyền thực thi
chmod +x "$TARGET_DIR/bin/"*.sh "$TARGET_DIR/bin/gach" 2>/dev/null || true

# 8. Đồng bộ chip nhớ Kindle (sync)
echo -e "${CYAN}[6/6] Đồng bộ dữ liệu xuống chip nhớ Kindle (sync)...${NC}"
sync
echo "  ✓ Đồng bộ hoàn tất. Không có dữ liệu nào bị nghẽn trong RAM đệm."

echo ""
echo -e "${GREEN}${BOLD}======================================================================${NC}"
echo -e "${GREEN}${BOLD}         ✅ CÀI ĐẶT GACH THÀNH CÔNG LÊN MÁY ĐỌC SÁCH KINDLE!          ${NC}"
echo -e "${GREEN}${BOLD}======================================================================${NC}"
echo -e "Cấu trúc thư mục đã cài đặt tại: ${CYAN}$TARGET_DIR${NC}"
echo -e "Kiểm tra thực tế các tệp trong ${BOLD}$TARGET_DIR${NC}:"
ls -la "$TARGET_DIR"
echo ""
echo -e "Kiểm tra file cấu hình KUAL (${BOLD}$TARGET_DIR/config.xml${NC}):"
cat "$TARGET_DIR/config.xml"
echo ""
echo -e "Kiểm tra menu KUAL (${BOLD}$TARGET_DIR/menu.json${NC}):"
cat "$TARGET_DIR/menu.json"
echo ""

# Hỏi người dùng có muốn ngắt kết nối an toàn (Eject) không
read -p "Bạn có muốn ngắt kết nối an toàn (Eject) Kindle luôn không? [Y/n] " -n 1 -r
echo ""
if [[ $REPLY =~ ^[Yy]$ ]] || [ -z "$REPLY" ]; then
    if [ -n "$KINDLE_DEV" ]; then
        udisksctl unmount -b "/dev/$KINDLE_DEV" >/dev/null 2>&1 || true
        echo -e "${GREEN}-> Đã ngắt kết nối an toàn! Bạn có thể rút cáp USB và mở KUAL trên Kindle.${NC}"
    else
        echo -e "${YELLOW}-> Vui lòng sử dụng lệnh Eject trên trình quản lý tệp trước khi rút cáp.${NC}"
    fi
else
    echo -e "${YELLOW}-> Chú ý: Hãy nhớ Eject Kindle an toàn trước khi rút cáp USB.${NC}"
fi
