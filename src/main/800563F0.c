#include "common.h"
#include "game.h"

void func_800563F0(s16 arg0, s16 arg1) {
    D_800B58F8 = arg0;
    D_800B58FC = arg1;
}

INCLUDE_ASM("asm/nonmatchings/main/800563F0", func_80056414);

INCLUDE_ASM("asm/nonmatchings/main/800563F0", func_8005649C);

INCLUDE_ASM("asm/nonmatchings/main/800563F0", func_8005658C);

INCLUDE_ASM("asm/nonmatchings/main/800563F0", func_8005680C);

INCLUDE_ASM("asm/nonmatchings/main/800563F0", func_800568B8);

#ifdef NON_MATCHING
/* NON_MATCHING: T-0016. The original keeps the constant 1 in v1 across the
 * loop (uopt hoists integer constants out of loops); the project flag
 * -Wo,-no_const_in_reg stops that. Frame and everything else match. */
void func_80056AA8(SyncObj *arg0, s32 arg1) {
    volatile s32 timeout = 0x800000;

    while (arg0->flag == 0) {
        if (--timeout == 0) {
            arg0->flag = 1;
            if (arg0->idx != 0) {
                arg0->idx = 0;
            } else {
                arg0->idx = 1;
            }
            arg0->unk_24 = arg0->tbl[arg0->idx].unk_00;
            arg0->unk_26 = arg0->tbl[arg0->idx].unk_02;
        }
    }
    arg0->flag = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/main/800563F0", func_80056AA8);
#endif

INCLUDE_ASM("asm/nonmatchings/main/800563F0", func_80056B3C);

INCLUDE_ASM("asm/nonmatchings/main/800563F0", func_80056BA8);

INCLUDE_ASM("asm/nonmatchings/main/800563F0", func_800570B8);

INCLUDE_ASM("asm/nonmatchings/main/800563F0", func_8005715C);
