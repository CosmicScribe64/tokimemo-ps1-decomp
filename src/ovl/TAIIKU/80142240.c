#include "common.h"
#include "ovl/TAIIKU.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A110);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A118);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A11C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A120);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A124);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A128);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A12C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A130);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A134);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A138);
u8 D_8014A13C[0x90] = { 0 };
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A1CC);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A1D0);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A1D4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A1E8);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A1EC);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A1FC);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A208);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A228);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A254);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A27C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A290);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A2A4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A2C4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A2F0);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A304);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A318);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A320);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A328);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A334);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A340);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A34C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A358);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A360);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A370);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A374);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A378);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A37C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A380);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A384);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A388);
extern u8 D_8014A13C[]; /* func_801446A0: same shape as D_80149208 */
extern u8 D_8014A161;
extern u8 D_8014A185;
extern u8 D_8014A1A9;

typedef struct {
    void (*f[10])();
} FnTbl10; /* size 0x28 */
extern FnTbl10 D_8014A254;

void func_80142240(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl10 tbl;

    tbl = D_8014A254;
    idx = D_800E6280.unk_1109;
    tbl.f[idx]();
}

void func_801422BC(void) {
    if (D_800E6280.unk_110A == 0) {
        if ((func_8008667C(1) < 2) && (((u32) (D_800E6280.unk_1BC[1].unk_0C.w << 0x1E) >> 0x1F) == 1) && (D_800E6280.unk_56C[13] == 0)) {
            D_800E6280.unk_56C[13] = 1;
            D_8014A110 = 1;
            func_80044890(0, 0xBF98, 0xBF79, 0xD712, 0xD6C3, 0xD6B7);
            func_8004284C();
        } else {
            D_8014A110 = 0;
            func_80042908(3);
        }
        Default_Disp();
        return;
    }
    if (func_80044E8C() == 1) {
        func_80042808();
    }
    Default_Disp();
}

void func_801423A4(void) {
    s32 temp_v0;

    temp_v0 = func_80140A90();
    switch (temp_v0) {
    case 1:
        func_80042808();
        return;
    case 2:
        D_8014A110 = 0;
        func_80042808();
        /* fallthrough */
    case 0:
        return;
    }
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80142240", func_80142400);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80142240", func_8014280C);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80142240", func_80142D90);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80142240", func_80142FB8);

void func_80143398(void) {
    s32 i;

    if ((u8) D_8014A110 == 0) {
        func_8004E44C(0, D_8014A384, D_8014A370);
        srn_init(0, D_8014A37C, 0);
    } else {
        func_8004E44C(0, D_8014A388, D_8014A374);
        srn_init(0, D_8014A380, 0);
    }
    srn_vram_set(0, 5, 0x580, 0xF0);
    for (i = 0; i < 3; i++) {
        D_800E6280.unk_1228[i + 5] = 8;
    }
    for (i = 0; i < 3; i++) {
        D_800E6280.unk_1228[i + 0x25] = 8;
    }
}

void func_80143470(void) {
    func_80048F64(0x60);
    D_80120651 = 3;
    D_80120688 = 0x01000000;
    D_80120652 = 1;
    D_80120653 = 0xA4;
    D_80120657 = 0x80;
    D_8012065C = (u8 *) D_80149FC0;
    D_80120660 = (u8 *) D_80149FF8;
    D_80120684 = &D_8014A1FC;
    D_80120664 = D_8014A030;
    D_80120654 = 0;
    D_80120655 = 0x80;
    if ((u8) D_8014A110 == 0) {
        D_80120666 = 0;
    } else {
        D_80120666 = 2;
    }
    D_80120676 = 0x50;
    D_8012067A = 0xB0;
    D_8012066A = 0x1000;
    D_8012066C = 0x1000;
    D_80120656 = 2;
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80142240", func_8014357C);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80142240", func_80143814);

void func_80143AC8(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        *(s32 *)(D_8014A13C + i * 0x24 + 4) += 1;
        *(s32 *)(D_8014A13C + i * 0x24 + 0x10) += 1;
        *(u16 *)(D_8014A13C + i * 0x24 + 0x1A) += 1;
        *(u16 *)(D_8014A13C + i * 0x24 + 0x18) += 1;
    }
    D_8014A118 += 1;
}

void func_80143C04(void) {
    if ((D_80120778 != 0x2D) && (D_80120778 != 0x39)) {
        func_80143C84();
        func_80143CC0();
        func_80143D24();
        func_80143DD0();
        func_80143F50();
        return;
    }
    if ((u32)D_8014A154 >= 0x32) {
        D_80120778 = 0x2F;
    }
}

void func_80143C84(void) {
    if (D_8014A158 != 0) {
        D_8014A144 = D_8014A128;
        return;
    }
    D_8014A144 = -D_8014A128;
}

void func_80143CC0(void) {
    if ((u32)D_8014A14C >= 0xB) {
        D_8014A148 += D_8014A144;
        if (D_8014A148 < -0x4000) {
            D_8014A148 = -0x4000;
        }
        if (D_8014A12C < D_8014A148) {
            D_8014A148 = D_8014A12C;
        }
        D_8014A14C = 0;
    }
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80142240", func_80143D24);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80142240", func_80143DD0);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80142240", func_80143F50);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80142240", func_8014430C);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80142240", func_80144464);

/* FAKE: v[6] as in func_8013815C. T-0016 */
s32 func_801446A0(void) {
    s16 v[6];
    s32 i;

    for (i = 0; i < 3; i++) {
        if (D_8014A13C[i * 0x24 + 0x25] == 0) {
            v[i] = *(s16 *)(D_8011ECD0 + i * 0x44 + 0x19EA);
        } else {
            v[i] = -0xF00;
        }
    }
    if (v[0] >= v[1] && v[0] >= v[2]) {
        D_8014A161 = 1;
        return 2;
    }
    if (v[1] >= v[2]) {
        D_8014A185 = 1;
        return 3;
    }
    D_8014A1A9 = 1;
    return 4;
}

typedef struct {
    s16 v[3];
} Tri3S; /* size 0x6 */
extern Tri3S D_8014A320;

void func_80144768(void) {
    s32 i; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    Tri3S tbl;

    tbl = D_8014A320;
    for (i = 0; i < 3; i++) {
        if (*(u16 *)&D_8014A13C[i * 0x24 + 0x3C] >= 0x33U) {
            if (i + 0x36 == *(s16 *)&D_8011ECD0[i * 0x44 + 0x19DC]) {
                *(s16 *)&D_8011ECD0[i * 0x44 + 0x19DC] = tbl.v[i];
            }
        }
    }
    func_80144834();
    func_801448D4();
    func_801449A0();
    func_80144B40();
    func_80144C70();
}

typedef struct {
    s32 v[3];
} Tri3; /* size 0xC */
extern Tri3 D_8014A328;

void func_80144834(void) {
    s32 i; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    Tri3 tbl;

    tbl = D_8014A328;
    for (i = 0; i < 3; i++) {
        if (i + 0x36 != *(s16 *) (D_8011ECD0 + i * 0x44 + 0x19DC)) {
            if (*(s32 *) (D_8014A13C + i * 0x24 + 0x40) != 0) {
                *(s32 *) (D_8014A13C + i * 0x24 + 0x2C) = tbl.v[i];
            } else {
                *(s32 *) (D_8014A13C + i * 0x24 + 0x2C) = -0x500;
            }
        }
    }
}

extern Tri3 D_8014A334;

void func_801448D4(void) {
    s32 i; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    Tri3 tbl;
    u8 *q;
    u8 *r;
    s32 v;

    tbl = D_8014A334;
    for (i = 0, q = D_8011ECD0; i < 3; i++, q += 0x44) {
        if (i + 0x36 != *(s16 *)(q + 0x19DC)) {
            r = D_8014A13C + i * 0x24;
            if (*(u32 *)(r + 0x34) >= 0xBU) {
                v = *(s32 *)(r + 0x30) += *(s32 *)(r + 0x2C);
                if (v < 0) {
                    *(s32 *)(r + 0x30) = 0;
                    v = 0;
                }
                if (tbl.v[i] < v) {
                    *(s32 *)(r + 0x30) = tbl.v[i];
                }
                *(s32 *)(r + 0x34) = 0;
            }
        }
    }
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80142240", func_801449A0);

extern Tri3 D_8014A34C;

void func_80144B40(void) {
    s32 i;
    Tri3 tbl;

    tbl = D_8014A34C;
    for (i = 0; i < 3; i++) {
        if (i + 0x36 != *(s16 *)&D_8011ECD0[i * 0x44 + 0x19DC]) {
            if ((u32)tbl.v[i] < *(u32 *)&D_8014A13C[i * 0x24 + 0x28] && *(s32 *)&D_8014A13C[i * 0x24 + 0x40] == 1) {
                if ((s32)D_8014A13C[i * 0x24 + 0x24] >= 9) {
                    D_8014A13C[i * 0x24 + 0x24] = 2;
                    *(s32 *)&D_8014A13C[i * 0x24 + 0x28] = 0;
                } else {
                    D_8014A13C[i * 0x24 + 0x24] += 1;
                    *(s32 *)&D_8014A13C[i * 0x24 + 0x28] = 0;
                }
                *(s16 *)&D_8011ECD0[i * 0x44 + 0x19DC] = *(s16 *)&D_8014A1D4[D_8014A13C[i * 0x24 + 0x24] * 2] + i * 9 + 0x12;
            }
            if (D_8014A13C[i * 0x24 + 0x24] == 6) {
                D_8014A13C[i * 0x24 + 0x26] = 1;
            }
        }
    }
}

extern Tri3S D_8014A358;

void func_80144C70(void) {
    s32 i; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    Tri3S tbl;
    u8 *q;
    u8 *r;

    tbl = D_8014A358;
    for (i = 0, q = D_8011ECD0; i < 3; i++, q += 0x44) {
        if (i + 0x36 != *(s16 *)(q + 0x19DC)) {
            r = D_8014A13C + i * 0x24;
            if (*(s32 *)(r + 0x28) == 0 && (r[0x24] == 2 || r[0x24] == 6) && *(s32 *)(r + 0x40) == 1 &&
                tbl.v[i] < (func_800AE0D0() & 0xFF) && r[0x26] == 1) {
                *(s32 *)(r + 0x40) = 0;
                *(s16 *)(r + 0x3E) = 0;
                *(s32 *)(r + 0x30) = 0;
                *(s16 *)(q + 0x19DC) = func_800AE0D0() % 3 + i * 9 + 0x18;
            }
        }
    }
    for (i = 0, q = D_8011ECD0; i != 3; i++, q += 0x44) {
        if (i + 0x36 != *(s16 *)(q + 0x19DC)) {
            r = D_8014A13C + i * 0x24;
            if (*(s32 *)(r + 0x40) == 0 && *(u16 *)(r + 0x3E) >= 0x1EU) {
                *(s32 *)(r + 0x40) = 1;
                *(s16 *)(r + 0x3C) = 0;
                r[0x24] = 0;
                r[0x26] = 0;
                *(s16 *)(q + 0x19DC) = i + 0x36;
            }
        }
    }
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80142240", func_80144E54);

void func_80145044(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        *(s16 *)&D_8011ECD0[0x1998 + i * 0x44] += 1;
    }
    for (i = 0; i < 4; i++) {
        *(s16 *)&D_8011ECD0[0x1AA8 + i * 0x44] -= 4 - i;
    }
}

void func_801450C0(void) {
    s32 i;
    TkS16x4 t;

    t = D_8014A360;
    for (i = 0; i < 4; i++) {
        *(s16 *)&D_8011ECD0[0x1998 + i * 0x44] += t.v[i];
    }
}

void func_80145148(void) {
    if ((u32)D_8014A11C >= 0xF) {
        *(s16 *)(D_8011ECD0 + 0x19DC) ^= 1;
        *(s16 *)(D_8011ECD0 + 0x1A20) ^= 1;
        D_8014A11C = 0;
    }
}

void func_80145190(void) {
    D_8014A370 = 0x801AD000;
    D_8014A374 = 0x801AE018;
    D_8014A378 = 0x8018F000;
    D_8014A37C = 0x801AF1B0;
    D_8014A380 = 0x801B29EC;
    D_8014A384 = 0x801AD008;
    D_8014A388 = 0x801AE020;
}
