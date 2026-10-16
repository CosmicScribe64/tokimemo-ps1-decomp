#include "common.h"
#include "ovl/TACO.h"

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013DB80", func_8013DB80);

void func_8013DDE4(void) {
    Tc14 *p;
    s32 i;

    /* FAKE: init and `do` share a source line (as1 schedules by line); the second base is spelled from the
     * end label so that IDO does not share the first loop's address. Same bytes as the original. T-10320 */
    p = D_8015EB30; do {
        p->unk12 = p->unk12 + 1;
        if (p->unk12 == 0xFF) {
            p->unk13 = 0;
        }
        if (p->unk4 >= 0x2711) {
            p->unk13 = 0;
        }
        if (p->unk13 & 0x80) {
            p->unk0 = p->unk0 + p->unk8;
            p->unk2 = p->unk2 + p->unkA;
            p->unk4 = p->unk4 + p->unkC;
        }
        p++;
    } while (p < D_8015EC70);
    p = (Tc14 *)((u8 *)D_8015EC70 - 0x140);
    i = 0;
    do {
        if (p->unk13 & 0x80) {
            func_8013DEE4(i);
        }
        i++;
        p++;
    } while (i != 0x10);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013DB80", func_8013DEE4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013DB80", func_8013E424);

void func_8013E4FC(void) {
    Tc14 *p;
    s32 i;

    /* FAKE: the loop entry is written on one source line; as1 schedules by line and this orders the
     * end-pointer and base setup as the original does. T-10320 */
    p = D_8015EC70; do { p->unk12 += 1; if (p->unk12 == 0xFF) { p->unk12 = 0;
            p->unk13 += 1;
        }
        if (p->unk13 == 0x85) {
            p->unk13 = 0;
        }
        if (p->unk4 >= 0x2711) {
            p->unk13 = 0;
        }
        if (p->unk13 & 0x80) {
            p->unk0 += p->unk8;
            p->unk2 += p->unkA;
            p->unk4 += p->unkC;
            if (p->unk8 > 0) {
                p->unk8 -= 1;
            }
            if (p->unkA > 0) {
                p->unkA -= 1;
            }
            if (p->unk8 < 0) {
                p->unk8 += 1;
            }
            if (p->unkA < 0) {
                p->unkA += 1;
            }
            if (!(p->unk13 & 4)) {
                if (p->unk0 > 0x800) {
                    p->unk0 = 0x800;
                }
                if (p->unk0 < -0x800) {
                    p->unk0 = -0x800;
                }
                if (p->unk2 > 0x80) {
                    p->unk2 = 0x80;
                }
                if (p->unk2 < -0x700) {
                    p->unk2 = -0x700;
                }
                if (p->unk4 > D_8015EDB4->unk78 + 0x10) {
                    if (p->unkC > 0) {
                        p->unkC = p->unkC * -1;
                    }
                }
                if (p->unk4 < -0x1000) {
                    if (p->unkC < 0) {
                        p->unkC = p->unkC * -1;
                    }
                }
            }
        }
        p++;
    } while (p < (Tc14 *)&D_8015EDB0);
    /* FAKE: the base is spelled from the end label so that IDO does not share the first loop's address; same bytes. T-10320 */
    p = (Tc14 *)((u8 *)&D_8015EDB0 - 0x140);
    i = 0;
    do {
        if (p->unk13 & 0x80) {
            if (p->padE[3] < 2) {
                func_8013E778(i);
            } else {
                func_8013EAA4(i);
            }
        }
        i++;
        p++;
    } while (i != 0x10);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013DB80", func_8013E778);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013DB80", func_8013EAA4);
