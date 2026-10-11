#include "common.h"
#include "game.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
s32 D_800B5920[4] = { 0, 0, 0, 0 };
s32 D_800B5930 = 0;
s32 D_800B5934 = 0;
u8 D_800B5938[4] = { 0 };
u8 D_800B593C = 0;
u8 D_800B5940 = 0;
s32 D_800B5944 = 0;
s32 D_800B5948 = 0;
s32 D_800B594C = 0;

void set_dec_bri(u8 arg0) {
    D_800B593C = arg0;
}

s32 get_last_gamen_mode(void) {
    return D_800B5948;
}

void dec_bg_reset(void) {
    D_800B5920[3] = 0;
    D_800B5920[1] = 0;
    D_800B5920[2] = 0;
    D_800B5920[0] = 0;
    D_800B5938[1] = 0;
    D_800B5938[0] = 0;
    D_800B593C = 0x80;
    D_800B5948 = -1;
    D_800B5934 = 0;
    D_800B5938[2] = 0;
}

void dec_bg_show_switch(s32 arg0) {
    D_800B5938[arg0] = 1 - D_800B5938[arg0];
}

void dec_bg_show_set(s32 arg0, s32 arg1) {
    D_800B5938[arg0] = arg1 & 1;
}

s32 dec_bg_cd_read(s32 arg0, s32 arg1) {
    if (arg1 == 1) {
        if (arg0 == D_800B5920[2]) {
            return 0;
        }
        if (arg0 == D_800B5920[3]) {
            return 1;
        }
        func_800462C8(0xA, 0x80162000, arg0);
        *(s32 *)((u8 *)&D_800B5920[3] + -(D_800B5939 * 4)) = arg0;
        return -1;
    }
    if (arg0 == D_800B5920[0]) {
        return 0;
    }
    if (arg0 == D_800B5920[1]) {
        return 1;
    }
    func_800462C8(0xA, 0x80162000, arg0);
    *(s32 *)((u8 *)&D_800B5920[1] + -(D_800B5938[0] * 4)) = arg0;
    return -1;
}

s32 func_8005751C(s32 arg0) {
    D_800B594C = 0;
    func_80053650(0x80167000, 0x80162000, 0x80162000);
    if (arg0 == 0) {
        switch (1 - D_800B5938[arg0]) {
        case 0:
            D_800B594C = func_800536AC(0x2C0, 0xA0, 0x140, 0x58);
            return 0;
        case 1:
            D_800B594C = func_800536AC(0x140, 0x1A0, 0x140, 0x58);
            return 1;
        }
    } else if (arg0 == 1) {
        switch (1 - D_800B5938[arg0]) {
        case 0:
            D_800B594C = func_800536AC(0x2C0, 0, 0x140, 0xA0);
            return 0;
        case 1:
            D_800B594C = func_800536AC(0x140, 0x100, 0x140, 0xA0);
            return 1;
        }
    }
}

void func_80057640(s32 arg0) {
    s32 v;
    RECT rect;

    rect.x = 0;
    rect.y = D_8011ECA0 * 0xF0;
    rect.w = 0x140;
    rect.h = arg0;
    v = 1 - (s32)D_800B5939;
    (&D_800B5920[2])[v] = 0;
    switch (v) {
    case 0:
        func_8009C93C(&rect, 0x2C0, 0);
        v = 1 - (s32)D_800B5939;
        break;
    case 1:
        func_8009C93C(&rect, 0x140, 0x100);
        v = 1 - (s32)D_800B5939;
        break;
    }
    /* FAKE: the cast makes IDO recompute 1 - (s32)D_800B5939 for the argument instead of passing v (decomp-permuter, score 0); real source unknown. T-4090 */
    dec_bg_show_set(1, (u32) (1 - (s32)D_800B5939));
    D_800B5944 = 0;
    func_8009C674(0);
}

INCLUDE_ASM("asm/nonmatchings/main/80057390", func_80057710);

INCLUDE_ASM("asm/nonmatchings/main/80057390", func_800578F4);

void func_80057D1C(u8 arg0) {
    D_800B5940 = arg0;
}

INCLUDE_ASM("asm/nonmatchings/main/80057390", func_80057D28);

INCLUDE_ASM("asm/nonmatchings/main/80057390", func_80058398);
