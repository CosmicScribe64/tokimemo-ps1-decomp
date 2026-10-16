#include "common.h"
#include "ovl/TACO.h"

/* NON_MATCHING: T-19028, 15 differing lines; the original loads the flags word before the three position halfwords and fills the beqz delay slot with the z store, and loads all three vout words before the first division */
#ifdef NON_MATCHING
/* FAKE: pad_a, pad_e, pad_c and pad_d are unused locals; with div they reproduce the original frame (0x68: i at sp+0x60, vin at 0x4C, vout at 0x3C, h at 0x38, p at 0x2C); real locals unknown. T-19028 */
s32 func_8013DB80(TcPos pos, s32 speed, s32 flags) {
    s32 pad_a;
    s32 i;
    s32 pad_e;
    VECTOR vin;
    VECTOR vout;
    s16 h;
    s32 div;
    s32 pad_c;
    Tc14 *p;
    s32 pad_d;

    h = speed;
    if (h == 0) {
        h = 0x100;
    }
    p = D_8015EB30;
    for (i = 0; i < 16; i++) {
        if (!(p->unk13 & 0x80)) {
            break;
        }
        p++;
    }
    if (i >= 16) {
        return -1;
    }
    p->unk12 = 0;
    p->unk13 = 0x80;
    p->unk0 = pos.x;
    p->unk2 = pos.y;
    p->unk4 = pos.z;
    if (flags & 0x80) {
        vin.vx = D_8015EDB4->unk74 - pos.x;
        vin.vy = D_8015EDB4->unk76 - pos.y;
        vin.vz = D_8015EDB4->unk78 - pos.z;
        if (flags & 0x40) {
            vin.vx = vin.vx - (func_800AE0D0() & 0xFF) + 0x7F;
            vin.vy = vin.vy - (func_800AE0D0() & 0xFF) + 0x7F;
            vin.vz = vin.vz - (func_800AE0D0() & 0xFF) + 0x7F;
        }
        func_800AA460(&vin, &vout);
        div = 0x1000 / h + 1;
        p->unk8 = vout.vx / div;
        p->unkA = vout.vy / div;
        p->unkC = vout.vz / div;
    } else {
        p->unkC = h;
        p->unk8 = 0;
        p->unkA = 0;
    }
    p->unk11 = flags;
    return i;
}
#else
INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013DB80", func_8013DB80);
#endif

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

/* NON_MATCHING: T-19028, 18 differing lines; the original loads x, y and z first (lh, lh, lh) and stores in the jal delay slot; IDO interleaves lh/nop/sh */
#ifdef NON_MATCHING
s32 func_8013E424(pos, kind)
TcPos pos;
s32 kind;
{
    s32 pad3;
    s32 i;
    /* FAKE: 48 unused bytes around i reproduce the original frame (0x60, p at sp+0x28, i at sp+0x58); real locals unknown. T-19028 */
    Tc14 pad0;
    Tc14 pad1;
    s32 pad2;
    Tc14 *p;

    p = D_8015EC70;
    for (i = 0; i < 16; i++) {
        if (!(p->unk13 & 0x80)) {
            break;
        }
        p++;
    }
    if (i >= 16) {
        return -1;
    }
    p->unk13 = 0x80;
    p->unk0 = pos.x;
    p->unk2 = pos.y;
    p->unk4 = pos.z;
    p->unk12 = 0;
    p->unk8 = 0x3F - (func_800AE0D0() & 0x7F);
    p->unkA = 0x3F - (func_800AE0D0() & 0x7F);
    p->unkC = 0x50;
    p->unk11 = kind;
    return i;
}
#else
INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013DB80", func_8013E424);
#endif

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
            if (p->unk11 < 2) {
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
