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
/* NON_MATCHING: T-0016, T-0017. One delay slot: IDO's as1 fills the loop's
 * back branch with the volatile load at the loop head, the original leaves a
 * nop there. Frame and constant hoisting match (with -Wo,-nokpicopt). */
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

void strKickCD(s32 arg0) {
    while (func_800879D0(0x15, arg0, 0) == 0) {
    }
    while (func_800880C0(0x1C0) == 0) {
    }
}

INCLUDE_ASM("asm/nonmatchings/main/800563F0", func_80056BA8);

/* Old-style definition: the u8 parameter lives in its home slot and is reloaded with lbu, as in the original. */
void func_800570B8(arg0, arg1)
s32 arg0;
u8 arg1;
{
    s32 t;

    t = arg1;
    func_80049A40(-0x90, 0x5B, 0x90, 0xE, 4, arg0, t);
    func_80049A40(0, 0x5B, 0x90, 0xE, 4, arg0, t);
    func_80049A40(-0x90, 0x69, 0x100, 0x10, 1, 0, 1);
}

INCLUDE_ASM("asm/nonmatchings/main/800563F0", func_8005715C);
