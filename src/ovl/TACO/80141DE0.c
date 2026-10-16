#include "common.h"
#include "ovl/TACO.h"

/* No return at the end: the paths that leave func_80154014() or func_801423B0() non-zero return their
 * result in $v0 (implicit int, K&R style). T-19024 */
s32 func_80141DE0(s32 arg0) {
    u32 idx;
    u32 temp;
    s32 i;
    s32 dx;
    s32 dy;
    s32 dz;
    Tc14 *p;

    if (func_80142220(arg0) == 0) {
        return 0;
    }
    if (func_80142220(arg0) != 4 && func_80142220(arg0) != 3) {
        if (func_80142220(arg0) == 1) {
            if (!(D_800E6280.unk_10F8 & 3) && D_80160464 >= 0x41) {
                func_8013CE78();
                func_801335A0(1, 0x505);
            }
        } else if (!(D_8015E770[0].unk12 & 3)) {
            /* FAKE: the temporary splits the first definition of idx so that uopt colours idx $v1 and the byte
             * $a0, as in the original; real source unknown. T-19024 */
            temp = D_8015E770[0].unk12 >> 2;
            idx = temp;
            if (D_8015E770[0].unk12 > 0x80) {
                func_8013CFD8();
                func_801335A0(1, 0x504);
                idx = D_8015E770[0].unk12 >> 2;
            }
            if ((s32) idx < 0x10) {
                p = &D_8015E770[idx];
                p->unk13 = 0x90;
                p->unk0 = D_8015EDB4->unk74;
                p->unk2 = D_8015EDB4->unk76 + 0x10;
                p->unk4 = D_8015EDB4->unk78 - 0x64;
                p->unk8 = 0;
                p->unkA = 0;
                p->unkC = -0x20;
                p->unk12 = 0;
            }
            p = &D_8015E770[idx];
            if (p->unk12 > 0x10 && D_8015E770[0].unk12 < 0x80) {
                p->unkC = -0x100;
            }
            dx = D_8015EDB4[0].unk74 - D_8015EDB4[8].unk74;
            dy = D_8015EDB4[0].unk76 - D_8015EDB4[8].unk76;
            dz = D_8015EDB4[0].unk78 - D_8015EDB4[8].unk78 - 0x200;
            D_8015EDB4[8].unk74 += dx / 2;
            D_8015EDB4[8].unk76 += dy / 2;
            D_8015EDB4[8].unk78 += dz / 2;
        }
    }
    if (D_80160460 != 0) {
        if (func_80154014() == 0) {
            D_8015EDB4->unk2 &= 0xFF3F;
            func_8013CE78();
            return func_801420DC();
        }
    } else if (func_801423B0() == 0) {
        D_8015EDB4->unk2 &= 0xFF3F;
        for (i = 0; i < 16; i++) {
            D_8015E770[i].unk13 = 0;
        }
        func_8013CFD8();
        return func_801420DC();
    }
}

s16 func_801420DC() {
    if (D_8015EDB4->unk80 == 3 || D_8015EDB4->unk80 == 4) {
        D_8015EDB4->unk80 = 0;
        return D_8015EDB4->unk80;
    }
    if (D_8015EDB4->unk7E == 3 || D_8015EDB4->unk7E == 4) {
        D_8015EDB4->unk7E = 0;
        return D_8015EDB4->unk7E;
    }
    if (D_8015EDB4->unk7C == 3 || D_8015EDB4->unk7C == 4) {
        D_8015EDB4->unk7C = 0;
        return D_8015EDB4->unk7C;
    }
    return 0;
}

s16 func_801421AC() {
    if (D_8015EDB4->unk80 == 3 || D_8015EDB4->unk80 == 4) {
        return D_8015EDB4->unk80;
    }
    if (D_8015EDB4->unk7E == 3 || D_8015EDB4->unk7E == 4) {
        return D_8015EDB4->unk7E;
    }
    if (D_8015EDB4->unk7C == 3 || D_8015EDB4->unk7C == 4) {
        return D_8015EDB4->unk7C;
    }
    return 0;
}

s32 func_80142220(s32 arg0) {
    Tc14 *p;

    for (p = D_8015E770; p < &D_8015E770[16]; p++) {
        if (p->unk13 & 0x80) {
            return -1;
        }
    }
    if (D_80160460 != 0) {
        return 1;
    }
    if (D_8015EDB4->unk2 & 0x80) {
        return 0;
    }
    if (arg0 & 0x40) {
        if (func_801421AC() == 3) {
            D_8015EDB4->unk2 |= 0xC0;
            func_801334BC();
            func_8013356C();
            D_80160460 = 1;
            return 3;
        }
        if (func_801421AC() == 4) {
            D_8015EDB4->unk2 |= 0xC0;
            func_801334BC();
            func_8013356C();
            D_8015E770[0].unk13 = 0x90;
            D_8015E770[0].unk0 = D_8015EDB4->unk74;
            D_8015E770[0].unk2 = D_8015EDB4->unk76;
            D_8015E770[0].unk4 = D_8015EDB4->unk78 - 0xC8;
            D_8015E770[0].unk8 = 0;
            D_8015E770[0].unkA = 0;
            D_8015E770[0].unkC = -8;
            D_8015E770[0].unk12 = 0;
            func_801335A0(1, 0x505);
            return 4;
        }
    }
    return 0;
}

s32 func_801423B0(void) {
    Tc14 *var_s0;
    s32 var_s1;
    u8 temp_v0;

    var_s0 = D_8015E770;
    var_s1 = 0;
loop_1:
    temp_v0 = var_s0->unk12;
    if (temp_v0 >= 0xFFU) {
        var_s0->unk13 = 0;
        return 0;
    }
    if (temp_v0 >= 0x80U) {
        var_s0->unkC = -0x400;
    }
    if (var_s0->unk4 < -0x7D00) {
        var_s0->unk13 = 0;
    }
    if (var_s0->unk13 & 0x80) {
        var_s0->unk12++;
        var_s0->unk0 += var_s0->unk8;
        var_s0->unk2 += var_s0->unkA;
        var_s0->unk4 += var_s0->unkC;
        /* A saved byte or extra mask changes IDO's reload here (T-9560). */
        func_801424A8(var_s1, var_s0->unk12);
    }
    var_s1 += 1;
    var_s0 += 1;
    if (var_s1 == 0x10) {
        return 1;
    }
    goto loop_1;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80141DE0", func_801424A8);
