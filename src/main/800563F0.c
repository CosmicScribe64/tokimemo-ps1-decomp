#include "common.h"
#include "game.h"

void set_movie_offset(s16 arg0, s16 arg1) {
    D_800B58F8 = arg0;
    D_800B58FC = arg1;
}

void strInit(s32 arg0, s32 arg1) {
    DecDCTReset(0);
    *(s32 *)(D_80125C58 + 0x34) = 0;
    func_800869C8(arg1);
    func_80088150(D_80125C58 + 0x44, 0x20);
    func_80088180(D_800B5900 & 1, 1, -1, 0, 0);
    strKickCD(arg0);
}

INCLUDE_ASM("asm/nonmatchings/main/800563F0", strSetDefDecEnv);

INCLUDE_ASM("asm/nonmatchings/main/800563F0", strCallback);

INCLUDE_ASM("asm/nonmatchings/main/800563F0", strNextVlc);

INCLUDE_ASM("asm/nonmatchings/main/800563F0", strNext);

#ifdef NON_MATCHING
/* NON_MATCHING: T-0016. The original keeps the constant 1 in v1 across the
 * loop (uopt hoists integer constants out of loops); the project flag
 * -Wo,-no_const_in_reg stops that. Frame and everything else match. */
void strSync(SyncObj *arg0, s32 arg1) {
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
INCLUDE_ASM("asm/nonmatchings/main/800563F0", strSync);
#endif

INCLUDE_ASM("asm/nonmatchings/main/800563F0", strKickCD);

INCLUDE_ASM("asm/nonmatchings/main/800563F0", func_80056BA8);

INCLUDE_ASM("asm/nonmatchings/main/800563F0", func_800570B8);

INCLUDE_ASM("asm/nonmatchings/main/800563F0", func_8005715C);
