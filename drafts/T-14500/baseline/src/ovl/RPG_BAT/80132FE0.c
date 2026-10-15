#include "common.h"
#include "ovl/RPG_BAT.h"

void func_80132FE0(void) {
    switch (*D_8015ED8C) {
    case 0:
        func_800AE0F0(D_8015E828, "マタギ「こったらとこ来たらだめだべ！");
        func_800AE0F0(D_8015E850, "早くけえれ！");
        func_8014B738((s32) D_8015E8F0, 2, 0xAA);
        *D_8015ED8C += 1;
        return;
    case 1:
        func_8013F1C4(0x0B000001, 0x02000008, 0);
        *D_8015ED8C += 1;
        return;
    case 2:
        func_8013F250(1);
        return;
    case 3:
        func_8013F1C4(0x1E000047, 0x0F00004B, 1);
        *D_8015ED8C += 1;
        return;
    case 4:
        func_8013F250(0);
        return;
    case 5:
        func_8013F220();
        func_8014EF3C();
        /* fallthrough */
    default:
        return;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80132FE0", func_80133108);
