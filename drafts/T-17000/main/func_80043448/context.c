#include "common.h"
#include "game.h"

void func_80042A00(void) {
    if (func_80042AC8() == 0) {
        if (D_800E6280.unk_F74 == 0) {
            func_80042CB0();
        } else if (D_800E6280.unk_F74 != 0xFF) {
            func_800430C0();
        }
        if (D_800B3CA0 != 0 && (D_800E6280.unk_F88 & 0xF9DF)) {
            D_800B3CA0 = 0;
        }
        if ((D_800E6280.unk_F80 & 0xF00000) == 0xF00000) {
            D_800B3CA0 = 1;
        }
        if (D_800E6280.unk_F88 & 0x08000000) {
            D_800E6280.unk_F6E = 1 - (u8)D_800E6280.unk_F6E;
        }
    }
}

s32 func_80042AC8(void) {
    GameState *g = &D_800E6280;
    u8 old;

    old = g->unk_F74;
    if (D_800E8BF0[0] == 0) {
        if ((D_800E8BF0[1] & 0xF) * 2 <= 0x40) {
            if ((D_800E8BF0[1] & 0xF0) == 0x10) {
                g->unk_F74 = 0;
                g->unk_1092 = 0;
            } else {
                g->unk_F74 = 1;
            }
        }
    } else if (D_800E8BF0[0] != 0xFF || g->unk_035 == 0) {
        g->unk_F74 = 0xFF;
    }
    if (old != g->unk_F74 && (g->unk_F74 != 0xFF || old != 0xFF)) {
        if (g->unk_F74 == 0) {
            func_80042C30();
            func_80042C74();
            func_80042BD4();
            return 1;
        }
        if (g->unk_F74 == 1) {
            func_80042C74();
            func_80042C30();
        }
        return 1;
    }
    return 0;
}

void func_80042BD4(void) {
    InitMouse(D_800E8BF0, D_800E8C30);
    SenseMouse(3, 4);
    SetMouse(0, 0xA0, 0x78);
    RangeMouse(0, 0x140, 0, 0xF0);
}

void func_80042C30(void) {
    InitPAD(D_800E8BF0, 8, D_800E8C30, 8);
    StartPAD();
    ChangeClearPAD(0);
}

void func_80042C74(void) {
    D_8011ECF6 = 0;
    D_8011ECFA = 0;
    D_800E6280.unk_F7A = 0;
    D_800E6280.unk_F7C = 0;
    D_800E6280.unk_F80 = 0;
    D_800E6280.unk_F84 = 0;
    D_800E6280.unk_F88 = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/80042A00", func_80042CB0);

INCLUDE_ASM("asm/nonmatchings/main/80042A00", func_80043010);

INCLUDE_ASM("asm/nonmatchings/main/80042A00", func_800430C0);

/* bulk_m2c declarations (T-11190) */
extern s32 D_800B3CA8;
extern s32 D_800E7200;
extern u8 D_800B3D00;
/* end bulk_m2c declarations */
void func_80043448(void) {
    u8 temp_t1;
    u8 temp_t3;
    u8 temp_t7;

    if (D_800B3CFC != 0) {
        temp_t7 = (D_800B3D00 + 1) & 0xFF;
        D_800B3D00 = temp_t7;
        if (temp_t7 == 0) {
            D_800B3CFC = 0;
            D_800B3CF8 = 0;
        }
    } else {
        temp_t1 = (D_800B3CF8 + 1) & 0xFF;
        if (D_800E7200 == *(&D_800B3CA8 + (D_800B3CF8 * 4))) {
            D_800B3CF8 = temp_t1;
            if (temp_t1 >= 0x12U) {
                D_800B3CFC = 1;
            }
            D_800B3D00 = 0x10;
        }
        temp_t3 = (D_800B3D00 - 1) & 0xFF;
        D_800B3D00 = temp_t3;
        if (temp_t3 == 0) {
            D_800B3CF8 = 0;
        }
    }
}

void func_80043504(void) {}
