#include "common.h"
#include "ovl/SHOUGATU.h"

void func_80132550(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl13 tbl;

    tbl = D_80143B28;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_801325CC(void) {
    func_80044890(1, 0xC4E5, 0xC4C7, 0xD294, 0xD264, 0xD25E);
    if (func_80044E8C() == 1) {
        func_80044750(0x200);
        func_8004284C();
    }
    func_8004284C();
}

void func_80132634(void) {
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

void func_801326BC(void) {
    cd_read_sectors(0x34, 0x80197000, 0xAF48);
    func_80132000();
    func_8004284C();
}

void func_801326F8(void) {
    s16 i;
    GameState *g;
    u8 *flags;

    func_800438DC(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    func_800438F0(1);
    func_80048390();
    func_8004E58C();
    func_800AE0F0(D_800CA19C, "正月");
    D_800E6280.unk_10A2 = 0;
    D_800E6280.unk_10E8 = 1;
    func_8008585C();
    D_800E6280.unk_03A = 0x80;
    D_800B593C = 0;
    D_800B5940 = 0;
    func_8007259C(0);
    func_8007C740();
    func_800649D4();
    func_80064E84();
    func_80084E4C();
    func_80132000();
    func_800847B8(0U);
    D_80143B00 = 0;
    D_80143B04 = 0;
    D_80143B18 = 0;
    D_80143B1C = 0xFF;
    D_80143B20 = 0;
    D_80143B24 = 1;
    g = &D_800E6280;
    flags = D_800CA2B0;
    for (i = 0; i < 11; i++) {
        flags[i] = g->unk_1BC[i].unk_0C.f.b1 >= 1;
    }
    func_8004284C();
}

void func_80132864(void) {
    bg_read_sub2(0x4167);
    func_8004284C();
}

#ifdef NON_MATCHING
/* NON_MATCHING: score=110 (106/106 insns). Remaining diff is $s2/$s3 swap between best_s and s and the redundant s == best_s re-check before the tie-break sum. T-24116. */
void func_8013288C(void) {
    u8 i;
    u8 best_i;
    u32 best_s;
    u8 s;

    best_i = 0xFF;
    best_s = 4;
    D_80122D40 = 0;
    for (i = 0; i < 11; i++) {
        if (D_800E6280.unk_1BC[i].unk_0C.f.b1 != 0) {
            s = get_g_zyotai_s(i) & 0x7F;
            if (s < best_s ||
                (s == best_s && s == best_s &&
                 (get_h_tokimeki(i) & 0xFF) + (get_h_yuukou(i) & 0xFF) >
                     get_h_yuukou(best_i) + (get_h_tokimeki(best_i) & 0xFF))) {
                best_i = i;
                best_s = s;
            }
        }
    }
    s = get_g_zyotai_s(best_i) & 0x7F;
    if (best_i != 0xFF && s < 2U) {
        D_80143B1C = best_i;
        D_800E6280.unk_F5F = best_i;
        func_800847B8(D_800E6280.unk_F5F);
        func_80132A34();
        D_80143B00 = 0;
        D_80143B04 = 0;
        D_80143B14 = 1;
        D_80122D40 = best_i + 1;
        D_800E6280.unk_71E |= 0x80;
        func_80042908(1);
    } else {
        D_80143B14 = 0;
        func_80042908(2);
    }
}
#else
INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80132550", func_8013288C);
#endif

void func_80132A34(void) {
    u8 state;

    switch (D_800E6280.unk_F5F) {
    case 0:
        D_80143AF4 = D_80143A48;
        D_80143AF8 = D_80143A84;
        D_80143AFC = D_80143AC0;
        break;
    case 1:
        D_80143AF4 = D_80143A58;
        D_80143AF8 = D_80143A94;
        D_80143AFC = D_80143AD0;
        break;
    case 2:
        D_80143AF4 = D_80143A4C;
        D_80143AF8 = D_80143A88;
        D_80143AFC = D_80143AC4;
        break;
    case 3:
        D_80143AF4 = D_80143A50;
        D_80143AF8 = D_80143A8C;
        D_80143AFC = D_80143AC8;
        break;
    case 4:
        D_80143AF4 = D_80143A6C;
        D_80143AF8 = D_80143AA8;
        D_80143AFC = D_80143AE4;
        break;
    case 5:
        D_80143AF4 = D_80143A5C;
        D_80143AF8 = D_80143A98;
        D_80143AFC = D_80143AD4;
        break;
    case 6:
        D_80143AF4 = D_80143A64;
        D_80143AF8 = D_80143AA0;
        D_80143AFC = D_80143ADC;
        state = get_g_zyotai_s(6) & 0x7F;
        if (state < 2) {
            D_80143AF4 = D_80143A60;
            D_80143AF8 = D_80143A9C;
            D_80143AFC = D_80143AD8;
        }
        break;
    case 7:
        D_80143AF4 = D_80143A54;
        D_80143AF8 = D_80143A90;
        D_80143AFC = D_80143ACC;
        break;
    case 8:
        D_80143AF4 = D_80143A44;
        D_80143AF8 = D_80143A80;
        D_80143AFC = D_80143ABC;
        break;
    case 9:
        D_80143AF4 = D_80143A68;
        D_80143AF8 = D_80143AA4;
        D_80143AFC = D_80143AE0;
        break;
    case 10:
        D_80143AF4 = D_80143A74;
        D_80143AF8 = D_80143AB0;
        D_80143AFC = D_80143AEC;
        break;
    }
}
