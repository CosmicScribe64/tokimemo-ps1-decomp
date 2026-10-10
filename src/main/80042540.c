#include "common.h"
#include "game.h"

INCLUDE_ASM("asm/nonmatchings/main/80042540", func_80042540);

void func_80042798(void) {
    D_800E6280.unk_1108 += 1;
    D_800E6280.unk_1109 = 0;
    D_800E6280.unk_110A = 0;
    D_800E6280.unk_10FC = 0;
    D_800E6280.unk_1100 = 0;
    D_800E6280.unk_1104.w = 0;
    D_800E6280.unk_110B = 0;
    D_800E6280.unk_110C = 0;
    D_800E6280.unk_110D = 0;
    func_80042960();
}

INCLUDE_ASM("asm/nonmatchings/main/80042540", func_80042808);

INCLUDE_ASM("asm/nonmatchings/main/80042540", func_8004284C);

void func_80042878(s32 arg0) {
    u32 t;

    D_800E6280.unk_1108 = arg0;
    D_800E6280.unk_1109 = 0;
    D_800E6280.unk_110A = 0;
    D_800E6280.unk_10FC = 0;
    D_800E6280.unk_1100 = 0;
    D_800E6280.unk_1104.w = 0;
    D_800E6280.unk_110B = 0;
    D_800E6280.unk_110C = 0;
    D_800E6280.unk_110D = 0;
    func_80042960();
    t = D_800E6280.unk_110E;
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
