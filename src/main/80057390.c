#include "common.h"
#include "game.h"
#include "main_only.h"

void set_dec_bri(u8 arg0) {
    D_800B593C = arg0;
}

s32 get_last_gamen_mode(void) {
    return D_800B5948;
}

INCLUDE_ASM("asm/nonmatchings/main/80057390", dec_bg_reset);

void dec_bg_show_switch(s32 arg0) {
    D_800B5938[arg0] = 1 - D_800B5938[arg0];
}

void dec_bg_show_set(s32 arg0, s32 arg1) {
    D_800B5938[arg0] = arg1 & 1;
}

s32 dec_bg_cd_read(s32 arg0, s32 arg1) {
    if (arg1 == 1) {
        if (arg0 == D_800B5928) {
            return 0;
        }
        if (arg0 == D_800B592C) {
            return 1;
        }
        func_800462C8(0xA, 0x80162000, arg0);
        *(s32 *)((u8 *)&D_800B592C + -(D_800B5939 * 4)) = arg0;
        return -1;
    }
    if (arg0 == D_800B5920) {
        return 0;
    }
    if (arg0 == D_800B5924) {
        return 1;
    }
    func_800462C8(0xA, 0x80162000, arg0);
    *(s32 *)((u8 *)&D_800B5924 + -(D_800B5938[0] * 4)) = arg0;
    return -1;
}

INCLUDE_ASM("asm/nonmatchings/main/80057390", func_8005751C);

INCLUDE_ASM("asm/nonmatchings/main/80057390", func_80057640);

INCLUDE_ASM("asm/nonmatchings/main/80057390", func_80057710);

INCLUDE_ASM("asm/nonmatchings/main/80057390", func_800578F4);

void func_80057D1C(u8 arg0) {
    D_800B5940 = arg0;
}

INCLUDE_ASM("asm/nonmatchings/main/80057390", func_80057D28);

INCLUDE_ASM("asm/nonmatchings/main/80057390", func_80058398);
