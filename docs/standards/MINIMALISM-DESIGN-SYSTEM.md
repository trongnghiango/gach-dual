# GACH MINIMALISM DESIGN SYSTEM (GMDS)
## Chuan Muc Thiet Ke Giao Dien Toi Gian & Than Thai Apple Cho Thiet Bi Nhung E-Ink

> **Phien ban**: 1.0.0 (Apple HIG & Dieter Rams Compliant for E-Ink)
> **Ap dung cho**: Man hinh muc dien tu E-Ink (Kindle PW2/PW3 300 PPI), Ung dung Giao duc Mam non (Tre 3-6 tuoi).
> **Muc tieu**: Dat trang thai hoan my (Aesthetic State of Perfection) trong tung khung hinh, loai bo 100% rac thi giac, giup AI Agent va ky su tao ra giao dien dep tua nhu mot tac pham nghe thuat sach tranh Nhat Ban (Ehon) ket hop su chuan chi cua Apple Human Interface Guidelines.

---

## 1. TRIET LY THIET KE: "LESS, BUT BETTER" (DIETER RAMS & STEVE JOBS)

1. **Ve Dep cua Khoang Trong (Negative Space as Content)**:
   - Khoang trang (White Space) tren man hinh E-Ink khong phai la khoang trong vo nghia, ma la **khoang tho (Breathing Room)** cho tam tri tre nho.
   - Khi mot thanh phan khong thuc su can thiet cho viec hieu noi dung: **HAY XOA NO DI**.

2. **Khac Kho voi Chi Tiet Thua (Radical Anti-Clutter)**:
   - Tuyet doi KHONG vien dong khung hop quanh toan bo man hinh.
   - Tuyet doi KHONG do bong gia 3D da huong (no drop shadows), KHONG gradient chuyen sac mo (E-Ink chi co 16 cap do xam, gradient se bi vo soc tho kech).
   - Moi duong net (Stroke) phai co ly do ton tai: Day dan, sac net, dong nhat.

3. **Mo Hinh Khac Hoa Tu Nhien (Frameless Nature)**:
   - Tranh ve va chu viet phai hoa nhap truc tiep vao be mat trang giay E-Ink. Khong dong khung tranh nhu mot buc tranh treo tuong, hay de tranh tho nhu mot cuon sach Ehon cao cap.

---

## 2. TOAN HOC & BANG TOKEN CHUAN (DESIGN TOKENS & RATIOS)

Moi kich thuoc va khoang cach trong he thong deu tuan thu **He thong Luoi 4px/8px (8-Point Grid System)** va **Ty le Vang Fibonacci (~1.618)**:

### 2.1. Thuoc do Khoang cach (Spacing Tokens - Base 8px tren Canvas 1072x1448)

| Token Name | Gia tri (PW3 1072x1448) | Quy tac Ung Dung |
| :--- | :---: | :--- |
| SPACE_XS | 8 px | Khoang cach bieu tuong va nhan chu ben trong button |
| SPACE_SM | 16 px | Dem trong (Inner padding) cua the am vi, nut phu |
| SPACE_MD | 24 px | Khoang cach doc mac dinh (gap) giua cac tang Flexbox |
| SPACE_LG | 36 px | Le tren/duoi cua man hinh (Top/Bottom Safe Inset) |
| SPACE_XL | 48 px | Le trai/phai toan trang (Page Horizontal Margin - Can doi mat) |
| SPACE_2XL| 64 px | Khoang cach phan doan lon khi chuyen nhip |

---

### 2.2. Phan Bo Ty Le Chieu Cao Man Hinh (Golden Ratio Vertical Budget)

Tong chieu cao kha dung tren Kindle PW3: H = 1448 px:

- **1. TOP HEADER & NAVIGATION** (Cao: 60 px ~ 4.1% H):
  * Chi so trang o giua: "1 / 16"
  * Nut dieu huong/thoat o goc phai/trai: (Thoat x)
- **2. HERO FRAMELESS ART STAGE** (Cao: 620 px ~ 42.8% H):
  * Tranh minh hoa net muc den thuan khiet
  * Duong chan troi mat dat huu co o Y ~ 72% chieu cao tranh
- **3. STORY NARRATION CARD** (Cao: 140 px ~ 9.7% H):
  * Loi dan truyen 2 dong chu Sans-Serif dam da
  * Can giua trang sach, thoang dang, de doc
- **4. PHONICS FOCUS TRAY** (Cao: 110 px ~ 7.6% H):
  * Ranh ghep van truc giac: [ ? ] + van
  * Khi hoan thanh: Tu hoan chinh noi bat o giua
- **5. INTERACTION & ACTION ZONE** (Cao: 160 px ~ 11.0% H):
  * Khi cho ghep: 3 manh ghep chu [ t ] [ c ] [ b ]
  * Khi hoan thanh: Nut Pill den tuyen [ Tiep tuc sang trang -> ]
- **6. BOTTOM BREATHING MARGIN** (Cao: 40 px ~ 2.8% H):
  * Khoang tho day man hinh, chong ngon tay can mep nhua

---

### 2.3. Bang Thu Bac Chu (Typography Hierarchy Tokens)

Chi su dung **Sans-Serif thuan khiet net deu (Geometric Sans-Serif)**. Khong dung font Serif co rau ria:

| Cap bac | Font Size | Line Height | Weight | Mau sac (Grayscale) | Muc dich su dung |
| :--- | :---: | :---: | :---: | :---: | :--- |
| **Hero Glyph** | **82 px** | 90 px | **Bold** | #000000 (Den 100%) | Chu cai tren cac manh ghep lua chon cham ngon tay |
| **Phonics Target** | **72 px** | 80 px | **Bold** | #000000 (Den 100%) | Tu ngu hoan chinh sau khi ghep dung |
| **Rhyme / Slot** | **66 px** | 76 px | **Bold** | #000000 (Den 100%) | Am vi / Van trong khay tieu diem |
| **Story Text** | **46 px** | **66 px** | **Bold** | #0D0D0D (Den 95%) | Loi dan truyen to ro cho ba me doc cung be thoai mai |
| **Button CTA** | **36 px** | 44 px | **Bold** | #FFFFFF tren nen den | Nut chuyen trang chinh (Primary Action) |
| **Meta / Status**| **22 px** | 28 px | **Normal**| #737373 (Xam 45%) | Chi so trang, thong so phu |
| **Utility Label**| **19 px** | 24 px | **Normal**| #595959 (Xam 35%) | Nhan nut Thoat, nut Quay lai, nut Nghe lai |

---

### 2.4. Bang Bo Goc (Corner Radii) & Be Rong Vien (Stroke Weights)

- **Bo goc (Border Radii)**:
  * Nut nho / Utility Pill: radius = 18 px (Bo tron nua chieu cao).
  * Manh ghep chu cai / The khay: radius = 16 - 18 px (Mem mai, than thien voi tre).
  * Nut hanh dong chinh (Primary Pill): radius = 50 px (Bo tron hoan hao hai dau).
  * Hop thoai Modal: radius = 24 px.
- **Do day net ve (Stroke Weights)**:
  * Vien manh thanh lich (Subtle Border): **1.5 px** (Dung cho khay tieu diem, nut phu).
  * Vien nut lua chon (Interactive Boundary): **2.0 px** (Dut khoat, sac sao).
  * Vien net dut (Dashed Slot): **2.0 px** (dashes: 6px line, 4px space).
  * Net ve nhan vat chinh (Character Outlines): **3.5 - 4.5 px**.

---

## 3. DANH MUC THANH PHAN CHUAN (CANONICAL COMPONENT LIBRARY)

Duoi day la 7 Components chuan muc duoc thiet ke dong bo cho toan bo he sinh thai GACH:

### Component 1: Floating Utility Pill (Nut Tien Ich Dinh Man Hinh)
Dung cho: **Nut Thoat (Exit)**, **Nut Quay lai (Back)**, **Nut Nghe phat am (Audio Replay)**.
- **Vi tri**: Nam o tang Header (top: 24px), dung PositionType::Absolute, z_index: 10.
  * Nut Thoat: Neo o goc tren ben phai (right: 48px).
  * Nut Quay lai: Neo o goc tren ben trai (left: 48px).
- **Kich thuoc**: Rong **95px**, Cao **36px**, Bo goc **18px**.
- **Cau truc**:
  * Vien xam manh 1.5px (#D9D9D9).
  * Chu xam dam 19px (#595959).
  * Bieu tuong vector ve truc tiep bang Cairo (dau x, mui ten <- hoac loa), khong dung ky tu Unicode de chong loi font.

### Component 2: Hero Frameless Stage (San Khau Minh Hoa Khong Vien)
Dung cho: **Toan bo phan tranh ve minh hoa tinh huong cau chuyen**.
- **Vi tri**: Tang thu 2 cua cay Flexbox Column, flex_grow: 1.0f (Co gian thong minh).
- **Quy tac Minimalism**:
  * **Tuyet doi khong ve vien khung bao quanh**. Tranh ve phai hoa nhap truc tiep vao nen trang cua trang sach.
  * Duong mat dat nam o vi tri 70-72% chieu cao khung tranh, net ve co do rung tu nhien (Scribble Jitter +-1.5px).

### Component 3: Narration Box (Hop Loi Dan Truyen)
Dung cho: **Cau chu loi dan cua tung trang sach**.
- **Vi tri**: Dat ngay duoi khung tranh, cach tranh SPACE_MD (24px).
- **Kich thuoc**: Tu dong do dac theo so dong, chieu cao 2 x 56px + 20px = 132px.
- **Quy tac Typography**:
  * Can giua trang sach (TextAlign::Center).
  * Ngat dong can doi (2-3 dong toi da), khong de rot lai 1 tu co doc o dong cuoi (Widow word avoidance).

### Component 4: Phonics Focus Tray (Khay Tieu Diem Am Vi)
Dung cho: **Vung ghep van trung tam cua phuong phap Visual Phonics**.
- **Kich thuoc**: Rong bang khung noi dung (W - 96px), Cao **110px**, Bo goc **16px**.
- **Cau truc mau sac**:
  * Nen xam tro sieu nhe #FAFAFA de tach biet nhe nhang voi nen trang giay ma khong gay choi.
  * Vien xam nhat 1.5px (#D1D1D1).
- **Trang thai 1: Khi dang cho ghep (AWAIT_INPUT)**:
  * Khuyet am dau: [ ? ] (ranh bo goc 12px vien net dut 6-4) dat ben trai, van chu dam 66px dat ben phai.
  * Khuyet van: Am dau dat ben trai, [ ? ] dat ben phai.
- **Trang thai 2: Khi da ghep dung (TRANSFORM & COMPLETED)**:
  * Ranh bien mat, tu hoan chinh hien thi to **72px** mau den tuyen #000000 o chinh giua khay.

### Component 5: Choice Tile (Manh Ghep Lua Chon Chu Cai)
Dung cho: **3 lua chon am vi de tre cham ngon tay**.
- **Vi tri**: Nam trong container FlexDirection::Row, dung JustifyContent::SpaceBetween voi khoang cach gap: 26px.
- **Kich thuoc**: Chieu cao **145px**, Chieu rong tu dong chia deu (~280px moi o), Bo goc **18px**.
- **Trang thai (Visual States)**:
  1. **Binh thuong (Default)**: Nen trang tinh khiet, vien den 2.0px (#262626), chu den 82px. Khong do bong!
  2. **Goi y dan loi (Hinted / Inverted)**: Khi tre ngap ngung > 6.0s, o dung chuyen sang **Nen den tuyen, chu trang tinh** (High-contrast Swiss Style).
  3. **Khoa cham (Disabled)**: Giam do dam net chu xuong xam nhat #A6A6A6 khi dang trong nhip chuyen hoa.

### Component 6: Primary Action Pill (Nut Hanh Dong Chinh)
Dung cho: **Nut "Tiep tuc sang trang ->"**, **Nut "Bat dau doc"**.
- **Vi tri**: Xuat hien thay the vi tri hang 3 manh ghep o nhip COMPLETED. Can giua trang sach.
- **Kich thuoc**: Rong **480px**, Cao **100px**, Bo tron vien thuoc radius: 50px.
- **Mau sac**:
  * **Nen den tuyen #000000**.
  * Chu trang tinh khiet #FFFFFF, co **36px**, Bold, dut khoat.
  * Tao luc hut thi giac toi cao (Call to Action), thuc day cam giac thanh tuu khi be hoan thanh thu thach.

### Component 7: E-Ink Modal Dialog (Hop Thoai Xac Nhan Toi Gian)
Dung cho: **Xac nhan thoat sach**, **Thong bao hoan thanh 16 trang**.
- **Vi tri**: Can chinh giua man hinh (x = 136, y = 474), Kich thuoc: Rong **800px**, Cao **500px**, Bo goc **24px**.
- **Cau truc**:
  * Nen trang tinh khiet co vien den dam 3.0px.
  * Tieu de to ro 44px: "Be co muon nghi ngoi khong?"
  * 2 Nut lua chon dat canh nhau:
    * Nut Phu (O lai doc tiep): Vien den mong 2px, nen trang chu den 32px.
    * Nut Chinh (Thoat): Nen den dac, chu trang 32px.

---

## 4. QUY TAC BO TRI KHONG GIAN (SPATIAL PLACEMENT RULES)

1. **Quy tac Doi xung & Can Truc Doc (Center-Axis Alignment)**:
   - Tat ca cac khoi chu (Loi dan, Khay am vi, Nut bam) deu phai duoc **khoa chat vao truc doi xung giua man hinh (X = W / 2)**.
   - Tranh viec lech trai hoac lech phai gay cam giac chong chenh, mat can bang thi giac.

2. **Quy tac Cong thai hoc Ngon tay Tre nho (Touch Ergonomics)**:
   - Chieu cao toi thieu cua bat ky vung tuong tac nao cho tre mam non la **90px** (tuong duong > 12mm tren man hinh 300 PPI).
   - Khoang cach giua hai nut cham lien ke toi thieu la **24px** de chong cham nham.

---

## 5. BANG KIEM 7 BUOC CHO AI AGENT DAT DO HOAN MY (THE 7 GATES OF PERFECTION)

Truoc khi nghiem thu bat ky ma nguon ve giao dien (Cairo/GTK) nao, AI Agent **BAT BUOC** phai tu cham diem qua 7 cau hoi sau:

1. **Cong 1: Khong Khung Hop Rac (Zero Clutter Gate)**: Man hinh co xuat hien vien bao ngoai 4 canh hay vien kep quanh tranh khong? *(Neu co: Bat buoc xoa bo)*.
2. **Cong 2: Khong Bong Do Gia (No Fake Shadow Gate)**: Co thanh phan nao dung bong do lech xam khong? *(Phong cach toi gian chi dung mat phang thuan khiet va vien net sac)*.
3. **Cong 3: Do Dac Dong Khong Chong Lan (Flexbox Dynamic Measure Gate)**: Chieu cao cac khoi co duoc tinh toan bang InkFlex khong? Co gan cung toa do y = prev_y + 100 khong? *(Tuyet doi cam gan pixel tinh)*.
4. **Cong 4: Font Chu Su Pham (Typography Purity Gate)**: Toan bo chu co dang dung Sans-Serif dam, tron deu, khong chan rau ria khong?
5. **Cong 5: Tuong Phan Muc E-Ink (E-Ink Contrast Gate)**: Chu chinh co dat do den sau #000000 tren nen trang tinh khiet #FFFFFF khong?
6. **Cong 6: Truc Giac Tre Nho (Comprehensible Input Gate)**: Nhin vao khay am vi [ ? ] ao, tre co hieu ngay can lam gi ma khong can doc chu huong dan ruom ra khong?
7. **Cong 7: An Toan Phim Thoat & Cham (Touch & Exit Safety Gate)**: Nut thoat (Thoat x) va phim Escape/Q co luon hoat dong tin cay trong moi tinh huong khong?

---
*Tai lieu nay la Hien phap UI/UX chinh thuc cua du an GACH. Moi AI Agent va lap trinh vien ke thua deu co nghia vu tuan thu vo dieu kien.*
