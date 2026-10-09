#include "common.h"
#include "game.h"

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80053650);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_800536AC);

INCLUDE_ASM("asm/nonmatchings/main/80053650", card_ev_set);

INCLUDE_ASM("asm/nonmatchings/main/80053650", Sw_Start);

INCLUDE_ASM("asm/nonmatchings/main/80053650", Hw_Start);

INCLUDE_ASM("asm/nonmatchings/main/80053650", Sw_Test);

INCLUDE_ASM("asm/nonmatchings/main/80053650", Sw_Clear);

INCLUDE_ASM("asm/nonmatchings/main/80053650", Hw_Test);

INCLUDE_ASM("asm/nonmatchings/main/80053650", Hw_Clear);

INCLUDE_ASM("asm/nonmatchings/main/80053650", Hw_Stop);

void func_80053CAC(u8 arg0) {
    D_800E7395 = 0;
    D_800E739C = arg0;
}

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80053CC0);

void func_80053CE0(void) {
    D_800E62B5 = 1;
    D_800E739C = 0;
    D_800E7395 = 0;
    D_800E739D = 0;
    D_800E73A0 = 0;
}

void func_80053D10(void) {
    D_800E62B5 = 0;
    D_800E73A0 = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80053D24);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80053DDC);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80053F04);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80054108);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80054140);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80054284);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80054388);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_8005448C);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80054590);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80054694);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80054704);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_8005478C);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80054884);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80054AF4);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80054BA8);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80054BE4);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80054C4C);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80054C74);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80054E6C);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80054FC0);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_800552DC);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_8005557C);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80055A38);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80055AFC);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80056070);

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80056284);
