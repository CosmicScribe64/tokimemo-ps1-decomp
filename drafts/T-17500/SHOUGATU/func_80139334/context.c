#include "common.h"
#include "ovl/SHOUGATU.h"

void func_801390C0(void) {
    D_80145270 = (u8 *)0x801E8088;
    D_80145274 = (u8 *)0x801E808C;
    D_80145278 = (u8 *)0x801E80A0;
    D_8014527C = *(s16 *)0x801E80B4;
    D_80145280 = (u8 *)0x801B0000;
    D_80145284 = (u8 *)0x801B2000;
    D_80145288 = (u8 *)0x801B4000;
    D_8014528C = (u8 *)0x801B8000;
    D_80145290 = (u8 *)0x801BC000;
    D_80145294 = (u8 *)0x801C0000;
    D_80145298 = (u8 *)0x801C4000;
    D_8014529C = (u8 *)0x801C8000;
    D_801452A0 = (u8 *)0x801D0000;
    D_801452A4 = (u8 *)0x801D4000;
    D_801452A8 = (u8 *)0x801D8000;
    D_801452AC = (u8 *)0x801DC000;
    D_801452B0 = (u8 *)0x801E0000;
    D_801452B4 = (u8 *)0x801E4000;
}

void func_801391D4(void) {
    func_801391F4();
}

void func_801391F4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl62 tbl;

    tbl = D_801452B8;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
    if (D_80144E08 == 0) {
        if (D_80144E0C == 3) {
            D_80122CF4 = 0;
        }
    }
}

void func_801392A4(void) {
    k_reset(1);
    func_8004284C();
}

void func_801392CC(void) {
    cd_read_sectors(0x71, 0x801B0000, 0x85B6);
    func_801390C0();
    func_8004284C();
}

void func_80139304(void) {
    func_80044750(0x24);
    func_80044750(0x501);
    func_8004284C();
}

/* bulk_m2c declarations (T-11190) */
extern s8 D_800E71DF;
extern u32 D_800E7384;
extern u8 D_800E62BC;
/* end bulk_m2c declarations */
void func_80139334(void) {
    if (D_80144E0C == 2) {
        D_800E71DF = 7;
    }
    func_80137AB4();
    if (D_80144E0C == 2) {
        D_8011ED5B &= 0xFF7F;
    }
    if (D_80144E0C == 3) {
        D_80122D20 = 1;
        D_8011ED5B &= 0xFF7F;
    } else {
        D_80122D20 = 0;
    }
    if ((D_80144E0C == 2) && (check_end_k(D_80144E0C) != 0) && ((D_800E62BC == 0) || ((D_800E62BC != 0) && (func_80046094() == 9)))) {
        k_reset(1);
        D_80122CE4 = 0;
        D_80122CF0 = 0;
        D_8011ED5B &= 0xFF7F;
        D_800CA2AC = 0;
        D_800E7384 = 0;
    } else {
        if ((u32) D_800E7384++ >= 0x1AU) {
            if ((D_80144E0C == 3) && (check_end_k(D_80144E0C) != 0)) {
                if (D_800E62BC != 0) {
                    if (D_800E62BC != 0) {
                        if (func_80046094() == 9) {
                            goto block_19;
                        }
                    }
                } else {
block_19:
                    D_80122D20 = 1;
                    D_80144E08 += 1;
                    D_80122CE4 = 0;
                    D_80122CF0 = 0;
                    D_8011ED5B &= 0xFF7F;
                    D_800CA2AC = 0;
                    D_80144E0C = 0;
                    func_8004284C();
                }
            }
        }
    }
}

void func_80139514(void) {
    func_800438DC(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    func_800438F0(1);
    func_80048390();
    func_8004E58C();
    D_800E6280.unk_10A2 = 0;
    D_800E6280.unk_10E8 = 1;
    func_8008585C();
    D_800E6280.unk_03A = 0x80;
    D_800B593C = 0;
    D_800B5940 = 0;
    func_8007C740();
    func_800649D4();
    func_80064E84();
    func_80084E4C();
    *(u8 *)&D_800E6280.unk_75D = *(u8 *)&D_800E6280.unk_F5F = 7;
    func_800847B8(*(u8 *)&D_800E6280.unk_F5F);
    func_80136F90();
    D_80144DFC = D_80144D9C;
    D_80144E00 = D_80144DC0;
    D_80144E04 = D_80144DE4;
    func_8004284C();
}

void func_8013960C(void) {
    func_801390C0();
    if (((u8)D_800E6280.unk_03F >= 6U) && ((u8)D_800E6280.unk_03F < 0xAU)) {
        func_80043914((s32)D_80145280, 0x11, 1, 2, 0);
        func_80084E90((s32)D_80145288, (s32)D_8014528C, (s32)D_80145290, (s32)D_80145294, (s32)D_80145298, (s32)D_8014529C);
    } else {
        func_80043914((s32)D_80145284, 0x11, 1, 2, 0);
        func_80084E90((s32)D_801452A0, (s32)D_801452A4, (s32)D_801452A8, (s32)D_801452AC, (s32)D_801452B0, (s32)D_801452B4);
    }
    func_800850D4((s32)D_80145274, (s32)D_80145278, (s32)D_80145270, (s32)D_8014527C);
    D_800CA360 = 1;
    func_8004284C();
}

void func_80139734(void) {
    D_80144E08 = 0;
    D_80144E0C = 0;
    func_800AE0F0(D_800CA19C, "廊下");
    func_8004284C();
}

void func_80139778(void) {
    bg_read_sub2(0x4045);
    func_8004284C();
}

void func_801397A0(void) {
    bg_read_sub2(0x46F0);
    func_8004284C();
}
