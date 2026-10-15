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

/* bulk_m2c declarations (T-11190) */
extern s8 D_800E7312;
extern u8 D_800E62B5;
extern u8 D_800E71F4;
extern u8 D_800E8BF1;
void func_80042C74(void);
void func_80042BD4(void);
/* end bulk_m2c declarations */
s32 func_80042AC8(void) {
    u8 temp_v0;
    u8 var_v1;

    var_v1 = D_800E71F4;
    temp_v0 = var_v1;
    if (*D_800E8BF0 == 0) {
        if (((u32) ((D_800E8BF1 & 0xF) * 2) < 0x41U) && (var_v1 = 1, ((D_800E8BF1 & 0xF0) == 0x10))) {
            var_v1 = 0;
            D_800E7312 = 0;
        }
    } else if ((*D_800E8BF0 != 0xFF) || (D_800E62B5 == 0)) {
        var_v1 = 0xFF & 0xFF;
    }
    D_800E71F4 = var_v1;
    if ((temp_v0 != var_v1) && ((var_v1 != 0xFF) || (D_800E71F4 = var_v1, (temp_v0 != 0xFF)))) {
        if (var_v1 == 0) {
            D_800E71F4 = var_v1;
            func_80042C30();
            func_80042C74();
            func_80042BD4();
            return 1;
        }
        D_800E71F4 = var_v1;
        if (var_v1 == 1) {
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

INCLUDE_ASM("asm/nonmatchings/main/80042A00", func_80043448);

void func_80043504(void) {}
