#include "common.h"
#include "game.h"

void func_80042A00(void) {
    if (func_80042AC8() == 0) {
        if (D_800E71F4 == 0) {
            func_80042CB0();
        } else if (D_800E71F4 != 0xFF) {
            func_800430C0();
        }
        if (D_800B3CA0 != 0 && (D_800E7208 & 0xF9DF)) {
            D_800B3CA0 = 0;
        }
        if ((D_800E7200 & 0xF00000) == 0xF00000) {
            D_800B3CA0 = 1;
        }
        if (D_800E7208 & 0x08000000) {
            D_800E71EE = 1 - (u8)D_800E71EE;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80042A00", func_80042AC8);

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
    D_800E71FA = 0;
    D_800E71FC = 0;
    D_800E7200 = 0;
    D_800E7204 = 0;
    D_800E7208 = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/80042A00", func_80042CB0);

INCLUDE_ASM("asm/nonmatchings/main/80042A00", func_80043010);

INCLUDE_ASM("asm/nonmatchings/main/80042A00", func_800430C0);

INCLUDE_ASM("asm/nonmatchings/main/80042A00", func_80043448);

void func_80043504(void) {}
