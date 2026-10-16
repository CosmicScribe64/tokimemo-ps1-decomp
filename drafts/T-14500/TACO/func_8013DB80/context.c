#include "common.h"
#include "ovl/TACO.h"

s32 func_8013DB80(s16 arg0, s16 arg1, s16 arg2, s32 arg3) {
    s32 sp44;
    s32 sp40;
    s32 sp60;
    s32 sp54;
    s32 sp50;
    s32 sp4C;
    s32 sp3C;
    s16 sp3A;
    Tc14 *sp2C;
    Tc14 *var_v1;
    s32 temp_a0;
    s32 var_a2;
    s32 var_at;

    sp3A = arg2;
    if (arg2 == 0) {
        sp3A = 0x100;
    }
    var_v1 = D_8015EB30;
    var_a2 = 0;
loop_3:
    var_at = var_a2 < 0x10;
    if (var_v1->unk13 & 0x80) {
        var_a2 += 1;
        var_v1 += 0x14;
        if (var_a2 == 0x10) {
            var_at = var_a2 < 0x10;
        } else {
            goto loop_3;
        }
    }
    if (var_at == 0) {
        return -1;
    }
    var_v1->unk12 = 0;
    var_v1->unk13 = 0x80;
    var_v1->unk0 = arg0;
    var_v1->unk2 = arg0;
    var_v1->unk4 = arg1;
    if (arg3 & 0x80) {
        sp4C = D_8015EDB4->unk74 - arg0;
        sp50 = D_8015EDB4->unk76 - arg0;
        sp54 = D_8015EDB4->unk78 - arg1;
        if (arg3 & 0x40) {
            sp2C = var_v1;
            sp60 = var_a2;
            sp4C = (sp4C - (func_800AE0D0() & 0xFF)) + 0x7F;
            sp50 = (sp50 - (func_800AE0D0() & 0xFF)) + 0x7F;
            sp54 = (sp54 - (func_800AE0D0() & 0xFF)) + 0x7F;
        }
        sp2C = var_v1;
        sp60 = var_a2;
        func_800AA460((s32) &sp4C, (s32) &sp3C);
        var_v1 = sp2C;
        var_a2 = sp60;
        temp_a0 = (0x1000 / sp3A) + 1;
        var_v1->unk8 = (s16) (sp3C / temp_a0);
        var_v1->unkA = (s16) (sp40 / temp_a0);
        var_v1->unkC = (s16) (sp44 / temp_a0);
    } else {
        var_v1->unk8 = 0;
        var_v1->unkA = 0;
        var_v1->unkC = sp3A;
    }
    var_v1->padE[3] = (u8) arg3;
    return var_a2;
}

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
    } while (p < &D_8015EC70);
    p = (Tc14 *)((u8 *)&D_8015EC70 - 0x140);
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
    p = &D_8015EC70; do { p->unk12 += 1; if (p->unk12 == 0xFF) { p->unk12 = 0;
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
