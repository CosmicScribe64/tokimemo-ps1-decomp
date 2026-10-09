#include "common.h"
#include "game.h"

INCLUDE_ASM("asm/nonmatchings/main/80042540", func_80042540);

void func_80042798(void) {
    D_800E7388 += 1;
    D_800E7389 = 0;
    D_800E738A = 0;
    D_800E737C = 0;
    D_800E7380 = 0;
    D_800E7384 = 0;
    D_800E738B = 0;
    D_800E738C = 0;
    D_800E738D = 0;
    func_80042960();
}

INCLUDE_ASM("asm/nonmatchings/main/80042540", func_80042808);

INCLUDE_ASM("asm/nonmatchings/main/80042540", func_8004284C);

void func_80042878(s32 arg0) {
    u8 t;

    D_800E7388 = arg0;
    D_800E7389 = 0;
    D_800E738A = 0;
    D_800E737C = 0;
    D_800E7380 = 0;
    D_800E7384 = 0;
    D_800E738B = 0;
    D_800E738C = 0;
    D_800E738D = 0;
    func_80042960();
    t = D_800E738E;
    D_800CA2AC = 0;
    if (t != 0x5E) {
        if (t != 0x50) {
            func_80065F34(0);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80042540", func_80042908);

INCLUDE_ASM("asm/nonmatchings/main/80042540", func_80042940);

INCLUDE_ASM("asm/nonmatchings/main/80042540", func_80042960);
