#include "common.h"
#include "ovl/SHOUGATU.h"

typedef struct {
    void (*f[11])();
} FnTbl11; /* size 0x2C */
extern FnTbl11 D_80146454;

typedef struct {
    void (*f[10])();
} FnTbl10; /* size 0x28 */
extern FnTbl10 D_8014633C;

typedef struct {
    void (*f[14])();
} FnTbl14; /* size 0x38 */
extern FnTbl14 D_80146304;

typedef struct {
    void (*f[17])();
} FnTbl17; /* size 0x44 */
extern FnTbl17 D_801462C0;

typedef struct {
    void (*f[12])();
} FnTbl12; /* size 0x30 */
extern FnTbl12 D_8014639C;

void func_8013F190(void) {
    D_80146130 = (u8 *)0x801D6360;
    D_80146134 = (u8 *)0x801D6384;
    D_80146138 = (u8 *)0x801D6414;
    D_8014613C = *(s16 *)0x801D641C;
    D_80146140 = (u8 *)0x801B0000;
}

void func_8013F1E0(void) {
    D_80146144 = 0x801DDBB4;
    D_80146148 = 0x801DDBB8;
    D_8014614C = 0x801DDC70;
    D_80146154 = 0x801DB000;
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013F190", func_8013F224);

void func_8013F360(void) {
    if ((D_80146168[D_80146158] == 0xB) && (D_800E6280.unk_03E == 0x60)) {
        func_8013D324();
        return;
    }
    func_8004284C();
}

s32 func_8013F3C0(void) {
    if (D_800E6280.unk_110D == 0) {
        func_8004E58C();
        func_800674B0();
        D_800E6280.unk_110D += 1;
    }
    if (D_800E6280.unk_110D == 1) {
        if ((u32)D_800E6280.unk_1104.w++ >= 0x400) {
            func_800452C4();
            func_8004482C();
            D_800E6280.unk_110D = 0;
        }
        if (func_80044E8C() == 1) {
            D_800E6280.unk_110D += 1;
        } else {
            return 0;
        }
    }
    func_80041584();
    return func_80072B5C(1);
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013F190", func_8013F494);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013F190", func_8013F9C4);

void func_8013FA80(void) {
    s16 a[4];
    s16 b[4];
    s16 c[4];
    s16 d[4];

    a[0] = -0x96;
    a[1] = -0x48;
    a[2] = 6;
    a[3] = 0x54;
    b[0] = b[1] = b[2] = b[3] = -0x40;
    c[0] = c[1] = c[2] = c[3] = 0x40;
    d[0] = d[1] = d[2] = d[3] = 0x40;
    func_8004F870(0, 4, a, b, c, d);
    D_80122D30 = 1;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013F190", func_8013FB28);

void func_8013FFA8(void) {
    bg_read_sub2(0x41E4);
    func_8004284C();
}

void func_8013FFD0(void) {
    s16 i;

    for (i = 0; i < 6; i++) {
        D_8011ECD0[i * 0x44 + 0x1983] = 0;
    }
    D_80146160 = 0;
    D_80146164 = 0;
    func_8004284C();
}

s32 func_80140030(void) {
    D_80122D2C = 0;
    if (D_80145FE4 == 0) {
        D_800E6280.unk_F5F = 0;
        D_80122CD4 = 0;
        D_80122CD0 = 7;
        func_80042908(6);
        D_800E6280.unk_56C[6] += 1;
        return 0;
    }
    if (D_80145FE4 == 7) {
        D_800E6280.unk_F5F = 7;
        D_80122CD4 = 7;
        D_80122CD0 = 7;
        func_80042908(7);
        D_800E6280.unk_56C[52] += 1;
        return 0;
    }
    if (D_80145FE4 == 9) {
        D_800E6280.unk_F5F = 9;
        D_80122CD4 = 9;
        D_80122CD0 = 7;
        func_80042908(8);
        D_800E6280.unk_56C[66] += 1;
        return 0;
    }
    func_8004284C();
}

void func_80140144(void) {
    if (D_80146168[D_80146158] == 0xB) {
        func_80044750(0x200);
    } else {
        func_80044750(0x201);
        D_800E6280.unk_1BC[D_80146168[D_80146158]].unk_06 += 1;
    }
    check_para_limit();
    func_8004284C();
}

void func_801401DC(void) {
    s16 v;

    v = D_801206B0 + 0x200;
    D_801206B0 = v;
    if (v >= 0x1000) {
        func_8004284C();
    }
}

void func_80140220(void) {
    func_80046318(0x4D, 0x801B0000, 0xBDAC);
    func_8004284C();
}

void func_80140250(void) {
    func_80046318(0xE, 0x801D7000, 0xBDF9);
    func_8004284C();
}

void func_80140284(void) {
    D_80145F30 = 0;
    if (D_80146168[D_80146158] == 0xB) {
        D_80145F2C = 0x27;
        D_800E6280.unk_11C.unk_02 += 0xA;
    } else {
        D_80145F2C = 0x26;
        D_800E6280.unk_11C.unk_02 -= 0xA;
    }
    check_para_limit();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013F190", func_80140314);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013F190", func_80140510);

void func_80140698(void) {
    s16 i;
    u8 *p;

    for (i = 0; i < 3; i++) {
        p = D_8011ECD0 + i * 0x44;
        if (*(s16 *)(p + 0x1A20) == 0xE && *(s16 *)(p + 0x1A10) == 9) {
            *(s16 *)(p + 0x1A1E) = func_800AE0D0() % 3;
        }
    }
}

void func_80140780(void) {
    D_80146230 = 0x801B21E0;
    D_80146234 = 0x801B21E4;
    D_80146238 = 0x801B2228;
    D_80146240 = 0x801B0000;
}

typedef struct {
    void (*f[31])();
} FnTbl31; /* size 0x7C */
extern FnTbl31 D_80146244;

void func_801407C0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl31 tbl;
    u32 j;

    tbl = D_80146244;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
    j = D_800E6280.unk_110A;
    if (normal_date_bg_fadein == tbl.f[j] || normal_date_bg_fadeout == tbl.f[j]) {
        D_80120653 |= 0x80;
        D_80120657 = D_800B593C;
    }
}

void func_8014088C(void) {
    func_80044750(0xBF);
    func_80044890(1, 0xBF98, 0xBF79, D_800B36AC, D_800B36EC, D_800B372C);
    if (func_80044E8C() == 1) {
        func_80044750(0x200);
        func_8004284C();
    }
    func_8004284C();
}

void func_80140908(void) {
    if (func_80044E8C() == 1) {
        func_80044750(0x200);
        func_8004284C();
    }
    if ((u32) D_800E6280.unk_1104.w++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E6280.unk_110A - 1) & 0xFF);
    }
}

void func_80140990(void) {
    func_80044750(0xBF);
    func_80044890(1, 0xC621, 0xC613, 0xD93B, 0xD8F0, 0xD8D0);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

void func_801409F8(void) {
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    if ((u32) D_800E6280.unk_1104.w++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E6280.unk_110A - 1) & 0xFF);
    }
}

void func_80140A78(void) {
    func_80046318(0xD, 0x801B0000, 0xBCBB);
    func_80140780();
    func_8004284C();
}

void func_80140AB0(void) {
    func_80044750(0x203);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013F190", func_80140AD8);

void func_80140BD8(void) {
    func_80065F34(0);
    func_8004E9F4(1);
    func_80140780();
    load_palette(D_80146240, 0x11, 1, 2, 0);
    func_80048F64(0x60);
    D_80120651 = 1;
    D_80120652 = 0;
    D_80120688 = 0x01000000;
    D_80120653 = 4;
    D_8012065C = (u8 *)D_80146234;
    D_80120660 = (u8 *)D_80146238;
    D_80120684 = (u8 *)D_80146230;
    D_80120664 = D_8014623C;
    D_80120666 = 0;
    D_80120668 = 0;
    D_80120656 = 1;
    D_80120693 = 0x11;
    D_80120676 = -0xA0;
    D_8012067A = -0x78;
    D_80120657 = 0;
    D_80120655 = 0;
    D_80120654 = 0;
    D_80145F2C = 0x13;
    D_80145F20 = D_80145AFC;
    D_80145F24 = D_80145B30;
    D_80145F28 = D_80145B64;
    func_8004284C();
}

void func_80140D30(void) {
    bg_read_sub2(0x41DC);
    func_8004284C();
}

void func_80140D58(void) {
    D_80120652 = 9;
    func_8004284C();
}

void func_80140D80(void) {
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 = 0x28;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013F190", func_80140DC0);

void func_80140F54(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl17 tbl;

    tbl = D_801462C0;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_80140FDC(void) {
    D_80145F2C = 3;
    func_8004284C();
}

void func_80141004(void) {
    func_80044750(0x506);
    func_8004284C();
}

void func_8014102C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl14 tbl;

    tbl = D_80146304;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_801410B4(void) {
    D_800E6280.unk_1BC[2].unk_0C.f.b2 = 1;
    D_80145F2C = 3;
    func_8004284C();
}

void func_801410F0(void) {
    if (((u32) D_800E6280.unk_1BC[2].unk_0C.b[2] >> 4) == 3) {
        D_80145F2C += 1;
    }
    func_8004284C();
}

void func_80141138(void) {
    if (((u32) D_800E6280.unk_1BC[2].unk_0C.b[2] >> 4) != 3) {
        D_80145F2C += 1;
    }
    func_8004284C();
}

void func_80141180(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl10 tbl;

    tbl = D_8014633C;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_801411FC(void) {
    D_80145F2C = 3;
    func_8004284C();
}

extern FnTbl14 D_80146364;

void func_80141224(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl14 tbl;

    tbl = D_80146364;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_801412AC(void) {
    func_80044750(0x501);
    func_8004284C();
}

void func_801412D4(void) {
    if (((u32) D_800E6280.unk_1BC[4].unk_0C.b[2] >> 4) == 7) {
        D_80145F2C += 3;
    }
    func_8004284C();
}

void func_8014131C(void) {
    D_800E6280.unk_1BC[4].unk_0C.f.b2 = 1;
    D_80145F2C = 3;
    func_8004284C();
}

void func_80141358(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl12 tbl;

    tbl = D_8014639C;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_801413CC(void) {
    D_80145F2C = 3;
    func_8004284C();
}

extern FnTbl17 D_801463CC;

void func_801413F4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl17 tbl;

    tbl = D_801463CC;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_8014147C(void) {
    D_800E6280.unk_1BC[6].unk_0C.f.b2 = 1;
    D_80145F2C = 4;
    func_8004284C();
}

void func_801414B8(void) {
    func_80044750(0x505);
    func_8004284C();
}

extern FnTbl17 D_80146410;

void func_801414E0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl17 tbl;

    tbl = D_80146410;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_80141568(void) {
    if (check_end_k() != 0) {
        k_reset(1);
        func_8004284C();
    }
}

void func_801415A0(void) {
    D_80145F2C = 3;
    func_8004284C();
}

void func_801415C8(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl11 tbl;

    tbl = D_80146454;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_80141650(void) {
    D_80145F2C = 3;
    func_8004284C();
}

extern FnTbl12 D_80146480;

void func_80141678(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl12 tbl;

    tbl = D_80146480;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_801416EC(void) {
    D_80145F2C = 3;
    func_8004284C();
}

void func_80141714(void) {
    func_80062CD0(0x56C0);
    func_8004284C();
}

void func_8014173C(void) {
    func_80042908(D_800E6280.unk_721);
    func_80042940(D_800E6280.unk_722);
}
