# QUY CHUAN DAC TA KICH BAN TRUYEN TUONG TAC GACH (COMMUNITY STORY SPECIFICATION)
## Huong Dan Toan Dien Cho Cong Dong Sang Tac & Phan Tich May Trang Thai Tuong Tac

> **Phien ban chuan**: 1.0.0 (Community Open Standard)
> **Ap dung cho**: Nen tang GACH Engine tren Kindle PW2/PW3 va Desktop Simulator.
> **Doi tuong**: Giao vien mam non, phu huynh, nha van thieu nhi, hoa si minh hoa, lap trinh vien.
> **Triet ly cot loi**: Giao duc nhan van (Nhan -> Duyen -> Qua), Comprehensible Input, Anti-Dopamine va My hoc toi gian.

---

## MUC LUC
1. [Phan Tich Chuyen Sau: May Trang Thai Hien Tai Co Qua Don Gian?](#1-phan-tich-chuyen-sau-may-trang-thai-hien-tai-co-qua-don-gian)
2. [Kien Truc May Trang Thai Phan Tang Mo Rong (HFSM)](#2-kien-truc-may-trang-thai-phan-tang-mo-rong-hfsm)
3. [Cau Truc Goi Truyen Chuan (.gach Package)](#3-cau-truc-goi-truyen-chuan-gach-package)
4. [Dac Ta Kich Ban story.json (Declarative Story Contract)](#4-dac-ta-kich-ban-storyjson-declarative-story-contract)
5. [Huong Dan Sang Tac Truyen Tung Buoc (Creator Tutorial)](#5-huong-dan-sang-tac-truyen-tung-buoc-creator-tutorial)
6. [Bo Quy Chuan Hinh Anh Muc E-Ink (Visual Guidelines)](#6-bo-quy-chuan-hinh-anh-muc-e-ink-visual-guidelines)
7. [Quy Trinh Dong Gop Vao Cong Dong GACH Hub](#7-quy-trinh-dong-gop-vao-cong-dong-gach-hub)

---

## 1. PHAN TICH CHUYEN SAU: MAY TRANG THAI HIEN TAI CO QUA DON GIAN?

### 1.1. Ban chat cua FSM 4 Nhip Hien Tai (FlowFSM)
Trong phien ban hien tai, dong chay moi trang duoc dieu phoi boi FSM 4 nhip tuyen tinh:
```text
OBSERVE (2.5s) ---> AWAIT_INPUT (6.0s Hint) ---> TRANSFORM (2.0s) ---> COMPLETED (Next Page)
```

- **Diem manh xuat sac**:
  1. *Bao ve tam tri tre mam non (3-6 tuoi)*: Ngan ngua hoan toan hoi chung bam loan man hinh (screen tapping frenzy).
  2. *Do tinh cua E-Ink*: Toi uu 100% cho toc do quet muc cua man hinh E-Ink, khong gay giat nhay mat.
  3. *Tap trung ngu am cao do*: Tre co du thoi gian lien ket hinh anh -> tu vung -> am vi.

- **Danh gia thang than: No co bi don gian qua khong? Co bao quat duoc tinh huong da dang khong?**
  **CO, NO DANG QUA DON GIAN NEU MO RONG RA TOAN BO CONG DONG SANG TAC.**

### 1.2. Nhung tinh huong doi thuc ma FSM 4 nhip tuyen tinh chua bao quat duoc:
1. **Kich ban co nga re dao duc (Branching Narrative / Choose Your Own Kindness)**:
   - *Vi du*: Mit thay ban khoc vi nga. Be co 2 lua chon:
     * Lua chon A: Do ban day va phui bui ao -> Chuyen sang Trang 4A.
     * Lua chon B: Chay di tim co giao giup -> Chuyen sang Trang 4B.
   - FSM hien tai chi ho tro thu tu trang tuyen tinh 1 -> 2 -> 3, khong co kha nang nhay trang theo do thi co huong (Directed Graph).
2. **Kich ban tuong tac vat the kieu Scratch / Scribblenauts (Multi-Object Stage Interaction)**:
   - *Vi du*: Be cam chiec xo nho muc nuoc o gieng -> roi moi dem tuoi cho choi cay. Hanh dong gom 2 buoc lien hoan truoc khi kich hoat ghep am vi.
3. **Kich ban ghep van nhieu buoc (Multi-Step Syllable Blending)**:
   - *Vi du*: Tu 'nghieng' phuc tap -> Buoc 1: Chon am dau ngh -> Buoc 2: Chon van ieng -> Buoc 3: Dat thanh bang.
4. **Trang dan nhap & Trang ket thuc thuan ke chuyen (Pure Narrative / Reflection Beats)**:
   - Mot so trang chi can loi dan em dem va tranh ve mo man cau chuyen ma khong can cau hoi do am vi. FSM hien tai ep buoc trang nao cung phai co cau hoi trac nghiem.

---

## 2. KIEN TRUC MAY TRANG THAI PHAN TANG MO RONG (HFSM)

De giai quyet bai toan tren ma **khong pha vo triet ly toi gian va tran RAM 12MB cua Kindle**, chung ta nang cap kien truc sang **May Trang Thai Phan Tang 3 Cap (Hierarchical Finite State Machine - HFSM)**:

```text
+------------------------------------------------------------------------+
| CAP 1: TANG DO THI CAU CHUYEN (STORY GRAPH FSM)                        |
| Dieu phoi moi quan he giua cac trang: Tuyen tinh, Re nhanh, Hoi quy   |
|               [Trang 1] ---> [Trang 2] ---> [Trang 3]                  |
|                                              |                         |
|                                       +------+------+                  |
|                                       v             v                  |
|                                  [Trang 4A]    [Trang 4B]              |
+------------------------------------------------------------------------+
                                    |
                                    v
+------------------------------------------------------------------------+
| CAP 2: TANG VONG DOI MAN DIEN (SCENE LIFECYCLE FSM)                    |
| [SCENE_ENTER] ---> [OBSERVE (Tinh)] ---> [INTERACTION] ---> [TRANSFORM]|
|                                                                        |
| Tuy theo kich ban, pha [INTERACTION] se kich hoat Driver tuong ung:    |
+------------------------------------------------------------------------+
                                    |
                                    v
+------------------------------------------------------------------------+
| CAP 3: BO DIEU PHOI TUONG TAC CAM RUT (PLUGGABLE INTERACTION DRIVERS)  |
|                                                                        |
| * Driver 1: PHONICS_SINGLE_BEAT (Mac dinh - Chuan 4 nhip hien tai)     |
|   Ghep 1 am vi khuyet voi 3 lua chon the chu cai.                      |
|                                                                        |
| * Driver 2: STAGE_OBJECT_INTERACTION (Kieu Scratch / Scribblenauts)    |
|   Tuong tac giua [Vat cong cu] va [Vat don nhan] tren san khau.        |
|                                                                        |
| * Driver 3: BRANCH_ETHICAL_DECISION (Lua chon nga re tu te)            |
|   Dua ra 2 quyet dinh dao duc, moi quyet dinh dan toi mot trang khac.  |
|                                                                        |
| * Driver 4: PURE_NARRATIVE (Trang dan nhap / Thuong thuc)              |
|   Danh cho trang mo man hoac ket truyen, cham nhe la sang trang.       |
+------------------------------------------------------------------------+
```

### Tinh Tuong Thich Nguoc Tuyet Doi (100% Backwards Compatible):
Tat ca cac tap truyen hien co (nhu *'Mot ngay tu te cua be Mit'*) mac dinh su dung Driver **`PHONICS_SINGLE_BEAT`**, dam bao tinh on dinh va khong can viet lai ma nguon nen tang.

---

## 3. CAU TRUC GOI TRUYEN CHUAN (.gach PACKAGE)

Moi cau chuyen la mot tep luu tru nen duy nhat mang phan mo rong **`.gach`** (ban chat la tep ZIP tieu chuan), co the phan phoi qua USB, the nho hoac tai truc tiep tu GitHub Community Hub:

```text
my_awesome_story.gach (Tep Zip nen)
|-- story.json                # Trai tim cau chuyen: Kich ban khai bao chuan JSON Schema
|-- thumbnail.png             # Anh bia vuong (400x400) hien thi o man hinh chon sach
`-- assets/
    `-- images/               # Toan bo tai nguyen do hoa muc E-Ink (Grayscale PNG 300 PPI)
        |-- bg_garden.png     # Phong nen boi canh (neu dung anh tinh)
        |-- mit_idle.png      # Manh nhan vat be Mit dung yen
        |-- mit_action.png    # Manh nhan vat be Mit hanh dong
        |-- plant_dry.png     # Vat the truoc chuyen hoa
        `-- plant_bloom.png   # Vat the sau chuyen hoa (Qua ngot)
```

---

## 4. DAC TA KICH BAN story.json (DECLARATIVE STORY CONTRACT)

Tep `story.json` tuan thu nghiem ngat theo luoc do chuan tai [`docs/community/story.schema.json`](story.schema.json).

Duoi day la mot vi du kich ban hoan chinh bao quat ca tinh huong **Ghep van truyen thong** va **Nga re dao duc**:

```json
{
  "schema_version": "1.0.0",
  "metadata": {
    "id": "mit_qua_tao_chia_se",
    "title": "Be Mit va Qua Tao Thom",
    "total_pages": 4,
    "target_age": "3-6",
    "author": "Nguyen Van A (Cong Dong GACH)",
    "language": "vi-VN",
    "pedagogy": "visual_phonics",
    "story_flow_mode": "branching"
  },
  "pages": [
    {
      "page_number": 1,
      "page_id": "page_intro",
      "lead_text": "Ba vua hai trong vuon mot qua tao do chin mong thom lung dem cho ban Mit...",
      "interaction_type": "phonics_single_beat",
      "stage_setup": {
        "legacy_initial_image": "assets/images/p1_holding_apple.png",
        "legacy_transform_image": "assets/images/p1_happy_apple.png"
      },
      "phonics_payload": {
        "full_word": "tao",
        "onset": "t",
        "rhyme": "ao",
        "tone": 2,
        "missing_part": "RHYME",
        "choices": [
          { "text": "ao", "is_correct": true },
          { "text": "eo", "is_correct": false },
          { "text": "ao", "is_correct": false }
        ]
      },
      "transition_timings": {
        "observe_delay_ms": 2500,
        "hint_delay_ms": 6000,
        "transform_delay_ms": 2000
      }
    },
    {
      "page_number": 2,
      "page_id": "page_ethical_crossroad",
      "lead_text": "Thay em nho ben nha dang buon vi chua co qua, Mit ban khoan suy nghi...",
      "interaction_type": "branch_choice",
      "branch_payload": {
        "prompt_question": "Be Mit se lam gi voi qua tao ngon?",
        "options": [
          {
            "label": "Chia doi qua tao cho em",
            "target_page_number": 3,
            "moral_impact": "Long hao hiep va biet se chia"
          },
          {
            "label": "Giu an mot minh",
            "target_page_number": 4,
            "moral_impact": "Cam giac co don khi khong chia se"
          }
        ]
      }
    },
    {
      "page_number": 3,
      "page_id": "page_shared_joy",
      "lead_text": "Mit vui ve be doi qua tao thom chia cho em. Nu cuoi rang ro no bung tren moi hai ban nho...",
      "interaction_type": "phonics_single_beat",
      "stage_setup": {
        "legacy_initial_image": "assets/images/p3_sharing.png",
        "legacy_transform_image": "assets/images/p3_bloom_hug.png"
      },
      "phonics_payload": {
        "full_word": "vui",
        "onset": "v",
        "rhyme": "ui",
        "tone": 0,
        "missing_part": "ONSET",
        "choices": [
          { "text": "v", "is_correct": true },
          { "text": "d", "is_correct": false },
          { "text": "b", "is_correct": false }
        ]
      }
    },
    {
      "page_number": 4,
      "page_id": "page_alone_lesson",
      "lead_text": "An tao mot minh trong goc san vang, Mit thay qua tao bong khong con ngot nhu moi khi...",
      "interaction_type": "pure_narrative",
      "stage_setup": {
        "legacy_initial_image": "assets/images/p4_alone.png",
        "legacy_transform_image": "assets/images/p4_alone.png"
      }
    }
  ]
}
```

---

## 5. HUONG DAN SANG TAC TRUYEN TUNG BUOC (CREATOR TUTORIAL)

Chi voi 5 buoc don gian, ban co the tao nen mot tap truyen hoan chinh:

### Buoc 1: Len Y Tuong Nhan Van & Chon Truc Am Vi
- Chon mot duc tinh tot dep (Hieu thao, Chia se, Kien tri, Giu trat tu, Yeu thien nhien).
- Chon 1 tu khoa tieng Viet chua am dau hoac van can luyen tap (Vi du: Cham soc hoa -> tu `tuoi` [t - uoi]).

### Buoc 2: Chuan Bi Tranh Minh Hoa
- Moi trang can chuan bi:
  1. Tranh khoi dau (`_dry.png`): The hien ngu canh va dong tac can hanh dong.
  2. Tranh qua ngot (`_bloom.png`): The hien niem vui, su dom hoa ket trai khi hanh dong tot hoan thanh.
- Xuat file dinh dang **PNG Grayscale 8-bit hoac RGB**, kich thuoc khuyen nghi: **1072 x 720 px**.

### Buoc 3: Soan Thao Kich Ban story.json
- Sao chep mau kich ban tu [`docs/community/example_story.json`](example_story.json).
- Dien cac truong: Loi dan chuyen, tu muc tieu am vi, cac dap an gay nhieu hop ly (Distractors).

### Buoc 4: Kiem Tham Dinh & Dong Goi
- Kiem tra tinh toan ven de dam bao khong sai sot chinh ta, thieu anh hay loi cu phap JSON.
- Nen toan bo thu muc thanh tep `my_story.gach`.

### Buoc 5: Chay Thu Nghiem Tren May Tinh
- Nap thu vao may tinh de kiem tra truc quan bang lenh:
  ```bash
  ./bin/gach --db my_test_story.db
  ```

---

## 6. BO QUY CHUAN HINH ANH MUC E-INK (VISUAL GUIDELINES)

Man hinh may doc sach Kindle la man hinh muc dien tu don sac (E-Ink Carta 300 PPI). De tranh hien thi dep nhu mot tac pham khac go nghe thuat:

1. **Tuyet Doi Khong Dung Mau Sac Phuc Tap**:
   - Tranh phai hien thi ro rang chi bang 2 sac do chinh: **Net muc den tuyen (#000000)** tren **Nen giay trang (#FFFFFF)**.
   - Co the dung toi da 1-2 sac thai xam nhat (#D9D9D9, #FAFAFA) de lam mang ao hoac bong mo nhe.
2. **Net Ve Dut Khoat (Line Weight)**:
   - Net nhan vat chinh: Day tu **3.5px - 4.5px**.
   - Net mat dat / phu canh: Manh nhe tu **1.5px - 2.0px**.
3. **Thu Nghiem Bong Den (The Silhouette Test)**:
   - Truoc khi xuat ban, hay thu to den toan bo nhan vat. Neu nguoi xem nhin vao bong den ma van nhan ra Mit dang tuoi cay, om ba hay nhat rac -> Buc tranh dat chuan Comprehensible Input!

---

## 7. QUY TRINH DONG GOP VAO CONG DONG GACH HUB

Cong dong GACH hoan nghenh moi dong gop tu khap noi tren the gioi:

1. **Fork Kho Ma Nguon**:
   ```bash
   git clone https://github.com/trongnghiango/gach-dual.git
   ```
2. **Tao Thu Muc Truyen Moi**:
   - Dat thu muc cau chuyen cua ban ben trong `stories/<story_id>/`.
   - Dat tep `story.json` va thu muc `assets/images/`.
3. **Mo Pull Request (PR)**:
   - Gui Pull Request len nhanh `main` voi tieu de:
     `feat(story): add new story 'Ten cau chuyen cua ban'`.
   - He thong CI tu dong cua GACH se chay kiem thu cu phap, tham dinh bo nho va dong goi tu dong thanh file `.gach` de moi nguoi dung Kindle tren toan cau co the tai ve!

---
*GACH Engine - Trao truyen tieng Viet va bai hoc tu te qua tung nhip cham em dem.*