#include "common.h"
#include "game.h"

INCLUDE_ASM("asm/nonmatchings/main/80085E30", func_80085E30);

void Vblnk_Timer_Init(void) {
    D_800E36F0 = D_800E7374;
    D_800E36F4 = 0;
}

s32 Vblnk_Timer(void) {
    D_800E36F4 = D_800E7374 - D_800E36F0;
    return D_800E36F4;
}

INCLUDE_ASM("asm/nonmatchings/main/80085E30", Scroll);

INCLUDE_ASM("asm/nonmatchings/main/80085E30", Rot_2D);

void Fade_Out_Init(void) {
    Vblnk_Timer_Init();
}

INCLUDE_ASM("asm/nonmatchings/main/80085E30", Fade_Out_Main);

void Fade_In_Init(void) {
    Vblnk_Timer_Init();
}

INCLUDE_ASM("asm/nonmatchings/main/80085E30", Fade_In_Main);

void Yubi(s32 idx) {
    u8 *p;
    func_8006BA40();
    p = D_8011ECD0 + idx * 0x44;
    *(s16 *)(p + 0x26) = D_8011ECF6;
    *(s16 *)(p + 0x2A) = D_8011ECFA;
}

INCLUDE_ASM("asm/nonmatchings/main/80085E30", Default_Disp);

INCLUDE_ASM("asm/nonmatchings/main/80085E30", func_8008647C);

INCLUDE_ASM("asm/nonmatchings/main/80085E30", func_80086640);

INCLUDE_ASM("asm/nonmatchings/main/80085E30", func_8008667C);

INCLUDE_ASM("asm/nonmatchings/main/80085E30", func_800866A0);

void func_800867CC(void) {
    if (D_80122EA0 == 0) {
        if (func_80044E8C() == 1) {
            D_80122EA0 = 1;
        }
    }
}
