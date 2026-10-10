#include "common.h"
#include "ovl/RPG_BAT.h"

void func_801477E0(void) {
    s32 temp_t4;
    s32 var_a0;

    switch (D_8015EB9C) {
    case 2:
        D_8015EBB0 = func_8013EA00(0x4E2, 0x2EE) + 1;
        return;
    case 5:
        D_8015EBB0 = func_8013EA00(0x7D0, 0x3E8) + 1;
        return;
    case 3:
        if (D_8015EDC4 & 0x600000) {
            D_8015EBB0 = func_8013EA00(0x3E8, 0x1F4) + 1;
        } else {
            D_8015EBB0 = func_8013EA00(0x258, 0x12C) + 1;
            D_8015EBB4 = func_8013EA00(0x258, 0x12C) + 1;
            D_8015EBB8 = func_8013EA00(0x258, 0x12C) + 1;
        }
        temp_t4 = (s32) D_8015ED94 / 4;
        var_a0 = temp_t4;
        if (temp_t4 >= 0x65) {
            var_a0 = 0x64;
        }
        D_8015EBBC = func_8013EA00(var_a0, var_a0);
        return;
    case 4:
        D_8015EBB0 = func_8013EA00(0x4B, 0x7D) + 1;
        return;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801477E0", func_80147930);
