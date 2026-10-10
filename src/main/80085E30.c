#include "common.h"
#include "game.h"
#include "main_only.h"

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

void Rot_2D(s32 arg0, s32 *arg1, s32 *arg2, s32 *arg3, s32 *arg4) {
    s32 s;
    s32 c;

    s = rsin();
    c = func_800A0140(arg0);
    *arg3 = c * *arg1 / 4096 - *arg2 * s / 4096;
    *arg4 = s * *arg1 / 4096 + *arg2 * c / 4096;
}

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

void Default_Disp(void) {
    k_disp_inc2();
    check_k_scroll();
    func_80083A10();
    message_window_show();
    hizuke_show();
    func_80066C08(2);
    func_80066334();
    func_80047560();
}

INCLUDE_ASM("asm/nonmatchings/main/80085E30", func_8008647C);

INCLUDE_ASM("asm/nonmatchings/main/80085E30", func_80086640);

INCLUDE_ASM("asm/nonmatchings/main/80085E30", func_8008667C);

void func_800866A0(void) {
    if (D_800E738D == 0) {
        func_80062CD0();
        D_800E738D += 1;
        return;
    }
    if (func_800460CC() & 1) {
        if (*(s32 *)D_800CA120 != 0x801820AC || *(s32 *)(D_800CA120 + 4) != 0x801860AC || *(s32 *)(D_800CA120 + 8) != 0x8018A0AC || *(s32 *)(D_800CA120 + 0xC) != 0x8018E0AC || *(s32 *)(D_800CA120 + 0x10) != 0x801920AC || *(s32 *)D_800CA128 != 0x8018005C || *(s32 *)(D_800CA128 + 4) != 0x80180070 || *(s32 *)D_800CA124 != 0x80180000) {
            D_800E738D = 0;
            return;
        }
        if (func_8004636C(0x2D, 0x80180000) == 0) {
            D_800E738D = 0;
            return;
        }
        func_8004284C();
    }
}

void func_800867CC(void) {
    if (D_80122EA0 == 0) {
        if (func_80044E8C() == 1) {
            D_80122EA0 = 1;
        }
    }
}
