#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80137280", func_80137280);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80137280", func_801373A8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80137280", func_801374D0);

s32 func_801375F8(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E9E0, "？？「ちょっと待てぇーい！");
        func_8014B738(D_8015EB70, 1, 0xA0);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x1A00008E, 0x14000095, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(0);
        return;
    case 3:
        func_8013F220();
        func_8014F500();
        return;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80137280", func_801376D4);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80137280", func_8013794C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80137280", func_80137AC8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80137280", func_80137C44);

s32 func_80137E74(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E9E0, "番長「なにぃぃぃー！？");
        func_8014B738(D_8015EB70, 1, 0xA0);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x1A000074, 0x1100007A, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(0);
        return;
    case 3:
        func_8013F220();
        func_8014F500();
        return;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80137280", func_80137F50);

void func_80138120(void) {
    switch (D_8015EDD8) {
    case 0:
        func_80137280();
        return;
    case 1:
        func_8014EF8C();
        return;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80137280", func_80138170);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80137280", func_80138324);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80137280", func_801387F0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80137280", func_80138B9C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80137280", func_80138F6C);

void func_80139460(void) {
    switch (D_8015EDD8) {
    case 0:
        D_8015ED98 = 0x1000;
        func_8013E7C0(0x30, 7, 1, 1);
        func_8004500C(1, 0x203);
        func_8014EF3C();
        return;
    case 1:
        func_8014F080(0x78);
        return;
    case 2:
        func_801373A8();
        return;
    case 3:
        D_8015EC14 = 1;
        func_80042808();
        return;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80137280", func_80139510);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80137280.rodata", D_8015B6F8);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80137280.rodata", D_8015B708);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80137280.rodata", D_8015B718);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80137280.rodata", D_8015B728);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80137280", func_80139E88);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80137280", func_8013A034);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80137280", func_8013A2F4);

void func_8013A464(s8 a0, s8 a1) {
    D_80121353 = a0;
    D_80121317 = a1;
}
