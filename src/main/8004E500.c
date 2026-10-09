#include "common.h"
#include "game.h"

INCLUDE_ASM("asm/nonmatchings/main/8004E500", func_8004E500);

INCLUDE_ASM("asm/nonmatchings/main/8004E500", func_8004E58C);

void func_8004E750(u8 arg0) {
    if (arg0 == 0x10) {
        D_800B3F6A = 0x10;
    } else {
        D_800B3F6A = 0xE;
    }
}

void func_8004E780(void) {
}

INCLUDE_ASM("asm/nonmatchings/main/8004E500", func_8004E788);

INCLUDE_ASM("asm/nonmatchings/main/8004E500", func_8004E884);

void func_8004E93C(s32 arg0, s32 arg1) {
    if (arg1 > 0) {
        D_800B3DC7[arg0 * 8] = 1;
    } else {
        D_800B3DC7[arg0 * 8] = 0;
    }
}

u8 *func_8004E970(s32 arg0) {
    if (arg0 >= 0x35) {
        return D_800B3F58;
    }
    return D_800B3DC0 + arg0 * 8;
}

s16 *func_8004E99C(void) {
    return &D_800B3F60;
}

void func_8004E9A8(s32 arg0, Entry8 arg1) {
    Entry8 *p = (Entry8 *)D_800B3DC0 + arg0;

    p->unk_00 = arg1.unk_00;
    p->unk_02 = arg1.unk_02;
    p->unk_04 = arg1.unk_04;
    p->unk_05 = arg1.unk_05;
    p->unk_06 = arg1.unk_06;
    p->unk_07 = arg1.unk_07;
}

INCLUDE_ASM("asm/nonmatchings/main/8004E500", func_8004E9F4);

void func_8004EA98(void) {
    D_800B3F64 = D_800B3F60;
}

INCLUDE_ASM("asm/nonmatchings/main/8004E500", func_8004EAAC);

INCLUDE_ASM("asm/nonmatchings/main/8004E500", func_8004EAD4);

INCLUDE_ASM("asm/nonmatchings/main/8004E500", func_8004EAFC);

INCLUDE_ASM("asm/nonmatchings/main/8004E500", func_8004EBEC);

u8 func_8004EC14(void) {
    return D_800B3F66;
}

INCLUDE_ASM("asm/nonmatchings/main/8004E500", func_8004EC24);

s32 func_8004ECB4(void) {
    if (D_800B3F62 == D_800B3F68) {
        return 1;
    }
    return 0;
}

u8 func_8004ECE0(void) {
    return *(u8 *)&D_800B3F60;
}

INCLUDE_ASM("asm/nonmatchings/main/8004E500", func_8004ECF0);

INCLUDE_ASM("asm/nonmatchings/main/8004E500", func_8004EE10);

INCLUDE_ASM("asm/nonmatchings/main/8004E500", func_8004F268);

INCLUDE_ASM("asm/nonmatchings/main/8004E500", func_8004F364);

INCLUDE_ASM("asm/nonmatchings/main/8004E500", func_8004F4F0);
