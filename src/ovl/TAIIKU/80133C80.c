#include "common.h"
#include "ovl/TAIIKU.h"
extern u8 D_80149232;
extern u8 D_80149256;
extern u8 D_8014927A;

typedef struct {
    void (*f[10])();
} FnTbl10; /* size 0x28 */
extern FnTbl10 D_8014933C;

void func_80133C80(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    s32 pad; /* FAKE: unused local; the original frame has a second word above tbl (two-word case, T-3330), real source unknown. T-4080 */
    FnTbl10 tbl;

    tbl = D_8014933C;
    idx = D_800E6280.unk_1109;
    tbl.f[idx]();
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80133CFC);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80134060);

void func_801345A4(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        if (Fade_In_Main() != 0) {
            Vblnk_Timer_Init();
            func_8004284C();
        }
        func_80133438();
        break;
    case 1:
        if (Vblnk_Timer() >= 0x5B) {
            func_80044750(0x502);
            D_80122EA0 = 0;
            func_80044890(0, 0xBF98, 0xBF79, 0xC107, 0xC0C2, 0xD6B7);
            func_8004284C();
        }
        func_80133438();
        break;
    case 2:
        func_800867CC();
        func_801368B8();
        func_801369F0();
        func_80136DA4();
        func_80132E20((s32) D_8014929A, 3);
        func_80137EA8();
        func_801365C8();
        func_80136E80();
        func_80137C44();
        func_80137440();
        if (D_8014929A >= 0x140) {
            func_80042808();
        }
        break;
    default:
        func_80046500();
        break;
    }
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_801346FC);

void func_80134D94(void) {
    s32 var_s0;

    switch (D_800E6280.unk_110A) {
    case 0:
        func_801368B8();
        func_801369F0();
        func_801365C8();
        func_80136E80();
        func_80137C44();
        func_80136DA4();
        func_80132E20((s32) D_8014929A, 3);
        func_80137EA8();
        if (D_8014929A >= 0x401) {
            func_80044750(0x1700);
            func_80044750(0x605);
            func_8004284C();
        }
        break;
    case 1:
        func_801368B8();
        func_801369F0();
        func_80136E80();
        func_80137C44();
        func_80132E20((s32) D_8014929A, 3);
        func_80136DA4();
        if (func_80137EA8() != 0) {
            func_80044750(0x604);
            Fade_Out_Init();
            func_8004284C();
        }
        break;
    case 2:
        Scroll((s32) ((s16) D_8014929A >> 1) % 640, 0xF, 0, 0x70, 0x280);
        Scroll(D_8014929A & 0x7F, 0xF, 0x70, 0x80, 0x80);
        func_80132E20((s32) D_8014929A, 3);
        if (Fade_Out_Main() != 0) {
            func_80044750(0xC2);
            var_s0 = 0;
            do {
                func_80048F64(var_s0);
                var_s0 += 1;
            } while (var_s0 < 0x17);
            var_s0 = 0;
            do {
                func_80048F64(var_s0 + 0x4C);
                var_s0 += 1;
            } while (var_s0 != 0xA);
            func_80138224();
            func_80042808();
        }
        break;
    default:
        func_80046500();
        break;
    }
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80134F98);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80135480);

void func_801356E0(void) {
    s32 i;

    func_8004E44C(0, D_80149FAC, D_80149FA0);
    srn_init(0, D_80149FA8, 0);
    srn_vram_set(0, 5, 0x580, 0xF0);
    for (i = 0; i < 3; i++) {
        D_800E6280.unk_1228[i + 5] = 8;
    }
    for (i = 0; i < 3; i++) {
        D_800E6280.unk_1228[i + 0x25] = 8;
    }
}

void func_80135778(void) {
    func_80048F64(0x60);
    D_80120651 = 3;
    D_80120688 = 0x01000000;
    D_80120657 = 0x80;
    D_8012065C = (u8 *) D_80149FB0;
    D_80120660 = (u8 *) D_80149FE8;
    D_80120684 = &D_801492C4;
    D_80120664 = D_8014A020;
    D_80120654 = 0;
    D_80120655 = 0x80;
    D_8012066A = 0x1000;
    D_8012066C = 0x1000;
    D_80120656 = 2;
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80135830);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80135AD0);

s32 func_80135D24(void) {
    s32 row;
    s32 random;
    TaiRandomTable table;

    table = D_80149570;
    random = func_800AE0D0() % 11;
    row = random;
    if (random >= D_801491EC) {
        row = random + 1;
    }
    if (row >= 11) {
        row = func_800AE0D0() % 10;
    }
    random = func_800AE0D0();
    return table.value[row][random % 10];
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80135DF8);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80135F3C);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_801365C8);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_801368B8);

void func_801369F0(void) {
    func_80136A28();
    func_80136AB0();
    func_80136B18();
    func_80136BB4();
}

void func_80136A28(void) {
    if (D_800E6280.unk_F88 & 0x20) {
        D_80149204 = 1;
    }
    if (D_8014921C >= 0x10U) {
        if (D_80149204 != 0) {
            if (D_80149220 < 0) {
                D_80149220 = 0;
            }
            D_80149218 = D_801491E4;
        } else {
            D_80149218 = -0x300;
        }
        D_8014921C = 0;
        D_80149204 = 0;
    }
}

void func_80136AB0(void) {
    if ((u32)D_80149224 >= 0xB) {
        D_80149220 += D_80149218;
        if (D_80149220 < -0x10000) {
            D_80149220 = -0x10000;
        }
        if (D_801491E8 < D_80149220) {
            D_80149220 = D_801491E8;
        }
        D_80149224 = 0;
    }
}

void func_80136B18(void) {
    TaiSlot24 *state;

    state = (TaiSlot24 *)D_80149208;
    if (D_80149218 < 0 || D_8014929A == 0) {
        ((TaiSlot24 *)D_80149208)->unk_20.word -= D_8014929C;
    } else {
        ((TaiSlot24 *)D_80149208)->unk_20.word = ((TaiSlot24 *)D_80149208)->unk_20.word + 0x8000;
    }
    if (state->unk_20.halves.high >= -0x1F) {
        state->unk_20.halves.high = -0x20;
    }
    if (state->unk_20.halves.high < -0x9F) {
        state->unk_20.halves.high = -0xA0;
    }
    D_80120676 = state->unk_20.halves.high;
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80136BB4);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80136DA4);

void func_80136E80(void) {
    func_80136EC0();
    func_8013703C();
    func_80137090();
    func_80137228();
    func_8013732C();
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80136EC0);

typedef struct {
    s32 w[3];
} Tw3; /* size 0xC */
extern Tw3 D_801497FC;

void func_8013703C(void) {
    s32 i;
    Tw3 t;

    t = D_801497FC;
    for (i = 0; i < 3; i++) {
        *(s32 *)(D_80149208 + i * 0x24 + 0x34) = t.w[i];
    }
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80137090);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80137228);

void func_8013732C(void) {
    s32 i;
    u8 *p;
    u32 t;
    s32 k;
    for (i = 0; i < 3; i++) {
        p = D_80149208 + i * 0x24;
        if (p[0x25] != 0) {
            *(s16 *)&D_8011ECD0[i * 0x44 + 0x19DC] = i + 0x88;
        } else {
            t = *(u16 *)(p + 0x30);
            if (t >= 6 && !(*(s32 *)(p + 0x3C) < 0xD000) || t >= 8 && !(*(s32 *)(p + 0x3C) < 0x6801) || t >= 0xB) {
                k = p[0x27];
                if (k >= 5) {
                    p[0x27] = 0;
                    *(u16 *)(p + 0x30) = 0;
                } else {
                    p[0x27] = k + 1;
                    *(u16 *)(p + 0x30) = 0;
                }
                *(s16 *)&D_8011ECD0[i * 0x44 + 0x19DC] = *(s16 *)&D_801492AC[p[0x27] * 2] + i * 6 + 6;
            }
        }
    }
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80137440);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80137544);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80137784);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80137C44);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80137EA8);

/* FAKE: v[6] instead of three elements reproduces the original local offset
 * (0x1c); the real source is unknown. T-0016 */
s32 func_8013815C(void) {
    s16 v[6];
    s32 i;

    for (i = 0; i < 3; i++) {
        if (D_80149208[i * 0x24 + 0x2A] == 0) {
            v[i] = *(s16 *)(D_8011ECD0 + i * 0x44 + 0x19EA);
        } else {
            v[i] = -0xF00;
        }
    }
    if (v[0] >= v[1] && v[0] >= v[2]) {
        D_80149232 = 1;
        return 2;
    }
    if (v[1] >= v[2]) {
        D_80149256 = 1;
        return 3;
    }
    D_8014927A = 1;
    return 4;
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80138224);

s32 func_80138680(s32 arg0, s32 arg1) {
    s32 key;
    s32 slot;
    TaiWords4 small;
    TaiWords11 large;

    small = D_801498AC;
    large = D_801498BC;
    key = arg0 + 1;
    if (key == D_801491E0[0]) {
        slot = 0;
    }
    if (key == D_801491E0[1]) {
        slot = 1;
    }
    if (key == D_801491E0[2]) {
        slot = 2;
    }
    if (key == D_801491E0[3]) {
        slot = 3;
    }
    if (arg1 != 0) {
        slot = slot * 4 + arg0 * 16;
    } else {
        slot = small.value[slot];
        if (D_80122E94 == 1 && slot == 4) {
            slot += large.value[D_80149320[D_801491DC]];
        } else {
            slot += large.value[((TaiValue44 *)D_8011ECD0)[((TaiSlot24 *)D_80149208)[arg0].index + 46].unk_16 >> 3];
        }
    }
    return slot;
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80138800);

s16 func_801388E4(s32 arg0) {
    s32 key;
    s32 slot;

    key = arg0 + 1;
    if (key == D_801491E0[0]) {
        slot = 0;
    }
    if (key == D_801491E0[1]) {
        slot = 1;
    }
    if (key == D_801491E0[2]) {
        slot = 2;
    }
    if (key == D_801491E0[3]) {
        slot = 3;
    }
    return (slot << 6) + 0xA0;
}

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148A90);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148A98);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148AA0);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148AA8);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148AB0);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148AB8);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148AC0);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148AC8);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148AD0);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148AD8);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148ADC);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148AE4);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148AEC);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148AF0);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148AF8);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148B00);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148B08);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148B10);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148B18);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148B1C);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148B24);

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.rodata", D_80148B2C);
