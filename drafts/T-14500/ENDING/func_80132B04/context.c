#include "common.h"
#include "ovl/ENDING.h"

void func_80132000(void) {
    D_8013C280 = 0x801F00B8;
    D_8013C284 = 0x801F05A8;
    D_8013C288 = 0x801F0C70;
    D_8013C28C = 0x801F0F34;
    D_8013C290 = 0x801F14C4;
    D_8013C294 = 0x801F1FF4;
    D_8013C298 = 0x801F29EC;
    D_8013C29C = 0x801F32F8;
    D_8013C2A0 = 0x801F4100;
    D_8013C2A4 = 0x801F4D18;
    D_8013C2A8 = 0x801F5BD4;
    D_8013C2AC = 0x801F6698;
    D_8013C2B0 = 0x801F6D9C;
    D_8013C2B4 = 0x801F70B4;
    D_8013C2B8 = 0x801F759C;
    D_8013C2BC = 0x801F7A70;
    D_8013C2C0 = 0x801F7F98;
    D_8013C2C4 = 0x801F00D0;
    D_8013C2C8 = 0x801F05C4;
    D_8013C2CC = 0x801F0C8C;
    D_8013C2D0 = 0x801F0F44;
    D_8013C2D4 = 0x801F14E4;
    D_8013C2D8 = 0x801F2028;
    D_8013C2DC = 0x801F2A0C;
    D_8013C2E0 = 0x801F331C;
    D_8013C2E4 = 0x801F4144;
    D_8013C2E8 = 0x801F4D30;
    D_8013C2EC = 0x801F5C1C;
    D_8013C2F0 = 0x801F66B4;
    D_8013C2F4 = 0x801F6DB8;
    D_8013C2F8 = 0x801F70BC;
    D_8013C2FC = 0x801F75B8;
    D_8013C300 = 0x801F7AAC;
    D_8013C304 = 0x801F7FB0;
    D_8013C308 = 0x801F01A0;
    D_8013C30C = 0x801F07F8;
    D_8013C310 = 0x801F0EC0;
    D_8013C314 = 0x801F0F84;
    D_8013C318 = 0x801F17DC;
    D_8013C31C = 0x801F24FC;
    D_8013C320 = 0x801F2CCC;
    D_8013C324 = 0x801F3684;
    D_8013C328 = 0x801F476C;
    D_8013C32C = 0x801F4F2C;
    D_8013C330 = 0x801F62EC;
    D_8013C334 = 0x801F68E8;
    D_8013C338 = 0x801F7008;
    D_8013C33C = 0x801F7114;
    D_8013C340 = 0x801F7808;
    D_8013C344 = 0x801F7CA0;
    D_8013C348 = 0x801F8174;
}

void func_80132334(void) {
    switch (D_800E6280.unk_1109) {
    case 0:
        func_80133030();
        return;
    case 1:
        func_80133540();
        return;
    case 2:
        func_80133AD0();
        return;
    case 3:
        func_80133F1C();
        return;
    case 4:
        func_80135DD0();
        return;
    case 5:
        func_80137340();
        return;
    case 6:
        func_801399B4();
        return;
    case 7:
        func_8013B760();
        return;
    default:
        func_80132400();
        return;
    }
}

void func_80132400(void) {
    main_state_set(0xC2);
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80132000", func_80132420);

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E7200;
extern u32 D_800E7384;
/* end bulk_m2c declarations */
void func_80132B04(s32 arg0, s32 arg1, s32 arg2) {
    s16 temp_v0_2;
    s16 temp_v0_3;
    s16 temp_v1;
    s16 temp_v1_2;
    s32 temp_t0;
    u32 temp_v0;

    if (D_80122D00 != 0) {
        if (check_end_k() != 0) {
            if (D_80122CF0 != 0) {
                if (D_80122CF0 == 1) {
                    if (func_80046094() == 9) {
                        goto block_5;
                    }
                }
            } else {
block_5:
                temp_v0 = D_800E7384 + 1;
                D_800E7384 = temp_v0;
                if (temp_v0 >= 0x3CU) {
                    D_80122D00 = 0;
                    k_reset(1);
                }
            }
        }
    } else if (D_80122D18 != 0) {
        if ((check_end_k() != 0) || ((check_end_k() == 0) && (D_800E7200 == 0))) {
            if (D_80122CF0 != 0) {
                if (D_80122CF0 == 1) {
                    if (func_80046094() == 9) {
                        goto block_14;
                    }
                }
            } else {
block_14:
                D_80122D18 = 0;
            }
        }
    } else if (D_80122CE4 != 0) {
        if ((check_end_k() != 0) || ((check_end_k() == 0) && (D_800E7200 == 0))) {
            if (D_80122CF0 != 0) {
                if (D_80122CF0 == 1) {
                    if (func_80046094() == 9) {
                        goto block_22;
                    }
                }
            } else {
block_22:
                D_80122CE4 = 0;
                k_reset(1);
            }
        }
    } else {
        D_800E7384 = 0;
        temp_v1 = *D_800CA134;
        temp_t0 = temp_v1 * 4;
        temp_v0_2 = *D_800CA138;
        func_80132420(*(s32 *)(*(s32 *)(D_800CA13C + temp_t0) + (temp_v0_2 * 4)), (s32) *(s32 *)(temp_v0_2 + *(s32 *)(D_800CA140 + temp_t0)), (s32) (u8) arg0, D_800CA144, (s32) temp_v1, (s32) temp_v0_2, (s32) (u8) arg2);
        if ((u8) D_8013C364 == 0) {
            if (((u8) arg1 != 0) && (D_800CA2A8 == 0)) {
                temp_v1_2 = *D_800CA134;
                temp_v0_3 = *D_800CA138;
                if (*(s32 *)(temp_v0_3 + *(s32 *)(D_800CA140 + (temp_v1_2 * 4))) == 0xFF) {
                    *D_800CA134 = (s16) (temp_v1_2 + 1);
                    *D_800CA138 = 0;
                } else {
                    *D_800CA138 = (s16) (temp_v0_3 + 1);
                }
            }
        }
    }
}

void func_80132D98(void) {
    D_800CA134 = (u8 *)&D_8013C358;
    D_800CA138 = (u8 *)&D_8013C35C;
    D_800CA13C = D_8013C34C;
    D_800CA140 = D_8013C350;
    D_800CA144 = D_8013C354;
    func_80132B04(0xFF, 1, 1);
}

void func_80132E10(void) {
    func_80044890(1, 0xBF98, 0xBF79, 0xCE6D, 0xCE33, 0xCE1E);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

void func_80132E70(void) {
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    if ((u32) D_800E6280.unk_1104.w++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E6280.unk_110A - 1) & 0xFF);
    }
}

void func_80132EF0(void) {
    func_80044750(0x200);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80132000", func_80132F18);
