#include "common.h"
#include "game.h"

void func_80047550(void) {
    D_801255DC = 4;
}

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_80047560);

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_800476C0);

void tpage_buf_clear_all(void) {
    s32 i;

    for (i = 0; i < 0x40; i++) {
        if ((i & 0xF) < 5) {
            D_800E6280.unk_1228[i] = 0;
        } else {
            D_800E6280.unk_1228[i] = -1;
        }
    }
}

void tpage_buf_clear(void) {
    s32 i;

    for (i = 0; i < 0x40; i++) {
        if ((i & 0xF) < 5) {
            D_800E6280.unk_1228[i] = 0;
        } else {
            D_800E6280.unk_1228[i] = -1;
        }
    }
    _sys_default_tpage_set();
}

INCLUDE_ASM("asm/nonmatchings/main/80047550", _sys_default_tpage_set);

INCLUDE_ASM("asm/nonmatchings/main/80047550", load_tpage_buf_lock);

INCLUDE_ASM("asm/nonmatchings/main/80047550", search_load_tpage_buf_lock);

INCLUDE_ASM("asm/nonmatchings/main/80047550", search_tpage_multi);

INCLUDE_ASM("asm/nonmatchings/main/80047550", search_tpage);

INCLUDE_ASM("asm/nonmatchings/main/80047550", search_tpage8);

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_80048CF8);

void func_80048DAC(s32 arg0) {
    if (arg0 != 0) {
        D_800E6280.unk_1112 = 1;
    } else {
        D_800E6280.unk_1112 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_80048DD0);

void func_80048E78(void) {
    s32 i;

    for (i = 0x60; i < 0xA0; i++) {
        func_80048F64(i);
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_80048EB8);

void func_80048F64(s32 arg0) {
    u8 *p;

    if (arg0 < 0xA0) {
        p = D_8011ECD0 + arg0 * 0x44;
        p[0] = 0;
        *(s32 *)(p + 0x38) = 0;
        p[1] = 0;
        p[4] = 0;
        p[5] = 0;
        p[2] = 0;
        p[3] = 0;
        p[6] = 1;
        *(s16 *)(p + 8) = 0;
        *(s32 *)(p + 0xC) = (s32)D_800C975C;
        *(s32 *)(p + 0x10) = (s32)D_800C9A14;
        *(s32 *)(p + 0x34) = (s32)D_800C9730;
        *(s16 *)(p + 0x14) = 0;
        *(s16 *)(p + 0x16) = 0;
        *(s16 *)(p + 0x18) = 0;
        *(s16 *)(p + 0x1A) = 0x1000;
        *(s16 *)(p + 0x1C) = 0x1000;
        p[0x43] = 0;
        p[7] = 0x80;
        *(s32 *)(p + 0x20) = 0;
        *(s32 *)(p + 0x24) = 0;
        *(s32 *)(p + 0x28) = 0;
        *(s32 *)(p + 0x2C) = 0;
        *(s32 *)(p + 0x30) = 0;
        p[0x40] = 0;
        p[0x41] = 0;
        p[0x42] = 0;
    }
}

u8 func_8004901C(void) {
    return D_800E6280.unk_03A;
}

void func_8004902C(void) {
    D_800E6280.unk_03A = 0x80;
    D_800E6280.unk_03B = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_80049044);

void func_800490B4(u8 arg0) {
    D_800E6280.unk_03B = arg0;
}
