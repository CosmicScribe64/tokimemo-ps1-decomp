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

void func_8013288C(void) {
    GameState *var_s6;
    s32 temp_s2;
    s32 temp_s3;
    s32 temp_s4;
    s32 temp_v1;
    u32 temp_a0;
    u32 temp_t2;
    u32 var_s5;
    u8 temp_a0_2;
    u8 var_fp;
    u8 var_s0;

    var_fp = 0xFF;
    var_s5 = 4;
    D_80122D40 = 0;
    var_s6 = &D_800E6280;
    var_s0 = 0;
    do {
        if ((var_s6->unk_1BC[0].unk_0C.w & 2) && ((temp_v1 = get_g_zyotai_s(var_s0) & 0x7F, temp_a0 = temp_v1 & 0xFF, ((temp_a0 < var_s5) != 0)) || ((temp_a0 == var_s5) && (temp_a0 == var_s5) && (temp_s2 = get_h_yuukou(var_s0) & 0xFF, temp_s3 = get_h_tokimeki(var_s0) & 0xFF, temp_s4 = get_h_tokimeki(var_fp) & 0xFF, (((u32) (get_h_yuukou(var_fp) + temp_s4) < (u32) (temp_s3 + temp_s2)) != 0))))) {
            var_fp = var_s0 & 0xFF;
            var_s5 = temp_v1 & 0xFF & 0xFF;
        }
        var_s0 += 1;
        var_s6 += 0x38;
    } while (var_s0 != 0xB);
    temp_t2 = get_g_zyotai_s(var_fp) & 0x7F;
    if (var_fp != 0xFF) {
        temp_a0_2 = var_fp & 0xFF;
        if (temp_t2 < 2U) {
            D_80143B1C = var_fp;
            D_800E6280.unk_F5F = temp_a0_2;
            func_800847B8(temp_a0_2);
            func_80132A34();
            D_80143B00 = 0;
            D_80143B04 = 0;
            D_80143B14 = 1;
            D_80122D40 = var_fp + 1;
            D_800E6280.unk_71E |= 0x80;
            func_80042908(1);
            return;
        }
    }
    D_80143B14 = 0;
    func_80042908(2);
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80132550", func_80132A34);
