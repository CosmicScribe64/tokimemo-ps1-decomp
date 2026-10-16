#include "common.h"
#include "ovl/TACO.h"

void func_80132220(void) {
    D_8015EDB4->unk74 = 0;
    D_8015EDB4->unk76 = 0;
    D_8015EDB4->unk78 = 0x800;
    D_8015EDB4[8].unk74 = 0;
    D_8015EDB4[8].unk76 = 0;
    D_8015EDB4[8].unk78 = 0x800;
    D_8015EDB4->unk2 = 0;
    D_8015EDB4[8].unk2 = 0;
    D_8015EDB4->unk3 = 0;
    D_8015EDB4->unk68 = 0;
    D_8015EDB4[8].unk68 = 0;
}

void func_801322AC(void) {
    func_801438F0(0x8019C800, 0, D_8015EDB4);
    D_8015EDB4->unk0[1] = 0x80;
    D_8015EDB4->unk6C = 0x400;
    D_8015EDB4->unk6E = 0x400;
    D_8015EDB4->unk70 = 0x400;
    func_801438F0(0x8019C800, 1, &D_8015EDB4[8]);
    D_8015EDB4[8].unk0[1] = 0x80;
    D_8015EDB4[8].unk6C = 0x400;
    D_8015EDB4[8].unk6E = 0x400;
    D_8015EDB4[8].unk70 = 0x400;
    D_8015EDB4->unk7A = 0;
    D_8015EDB4[8].unk7A = 0;
    D_8015EDB4->unk72 = -1;
    D_8015EDB4[8].unk72 = -1;
    D_8015EDB4->unk82 = -1;
    D_8015EDB4[8].unk82 = -1;
    D_8015EDB4->unk7C = 4;
    D_8015EDB4->unk7E = 3;
    D_8015EDB4->unk80 = 0;
    D_8015EDB4->unk84[1] = 0xFF;
    D_8015EDB4[8].unk84[1] = 0x80;
    func_80132220();
    D_8015EDD0 = 0;
    D_8015EDD4 = 0;
}

/* An int function with no return statement (K&R): with it the switch operands sit in $v0 as in the original; a returned result variable moves them to $a0. */
s32 func_8013240C() {
    /* FAKE: pad1..pad4 are unused locals that reserve the original's four extra frame words (m1 at sp+0x30, m2 at sp+0x2C); real source unknown. T-19025 */
    s32 pad1;
    s32 pad2;
    s32 pad3;
    s32 m1;
    s32 m2;
    s32 pad4;
    TcActor *p;
    s32 step;
    u32 r;

    p = D_8015EDB4;
    step = 0x20;
    if ((D_800E6280.unk_F80 & 0xA000) && (D_800E6280.unk_F80 & 0x5000)) {
        step = 0x2D;
    }
    if (D_800E6280.unk_F80 & 0x8000) {
        if (D_8015EDB4->unk74 > 0x800) {
            p->unk74 = p->unk74 + step * (0x1000 - p->unk74) / 2048;
        } else {
            p->unk74 = p->unk74 + step;
        }
    }
    if (D_800E6280.unk_F80 & 0x2000) {
        if (p->unk74 < -0x800) {
            p->unk74 = p->unk74 + step * (-0x1000 - p->unk74) / 2048;
        } else {
            p->unk74 = p->unk74 - step;
        }
    }
    if (D_800E6280.unk_F80 & 0x1000) {
        if (p->unk76 < -0x700) {
            p->unk76 = p->unk76 + step * (-0x800 - p->unk76) / 256;
        } else {
            p->unk76 = p->unk76 - step;
        }
    }
    if (D_800E6280.unk_F80 & 0x4000) {
        if (p->unk76 > 0x80) {
            p->unk76 = p->unk76 + step * (0x180 - p->unk76) / 256;
        } else {
            p->unk76 = p->unk76 + step;
        }
    }
    if (p->unk74 < -0x1000) {
        p->unk74 = -0x1000;
    }
    if (p->unk74 > 0x1000) {
        p->unk74 = 0x1000;
    }
    if (p->unk76 < -0x800) {
        p->unk76 = -0x800;
    }
    if (p->unk76 > 0x180) {
        p->unk76 = 0x180;
    }
    switch (D_8015EDDC % 3) {
    case 0:
        m1 = 5;
        m2 = 0xA;
        break;
    case 1:
        m1 = 4;
        m2 = 8;
        break;
    case 2:
        m1 = 1;
        m2 = 2;
        break;
    }
    if (p->unk3 == 0) {
        if (D_800E6280.unk_F88 & m1) {
            D_8015EDB4[8].unk2 += 1;
            p->unk2 = 0x81;
            p->unk3 = 0x7F;
        }
        if (D_800E6280.unk_F88 & m2) {
            D_8015EDB4[8].unk2 += 1;
            p->unk2 = 0x82;
            p->unk3 = 0x7F;
        }
    }
    if (p->unk2 & 1) {
        r = p->unk3;
        p->unk68 += r;
        p->unk74 += 8;
        if (r < 0xF) {
            if ((p->unk68 & 0xFFF) >= 0xFE1) {
                p->unk3 = 0;
                p->unk68 = 0;
                p->unk2 = 0;
            }
        } else {
            p->unk3 = p->unk3 - 1;
            if (p->unk3 < 0x40) {
                p->unk2 &= 0x7F;
            }
        }
    }
    if (p->unk2 & 2) {
        r = p->unk3;
        p->unk68 -= r;
        p->unk74 -= 8;
        if (r < 0xF) {
            if ((p->unk68 & 0xFFF) < 0x1F) {
                p->unk3 = 0;
                p->unk68 = 0;
                p->unk2 = 0;
            }
        } else {
            p->unk3 = p->unk3 - 1;
            if (p->unk3 < 0x40) {
                p->unk2 &= 0x7F;
            }
        }
    }
    if (p->unk2 & 4) {
        switch (D_8015EDB4->unk7A--) {
        case 2:
        case 10:
        case 14:
        case 15:
            func_801430B0();
            break;
        case 0:
            D_8015EDB4->unk2 &= 0xFB;
            break;
        }
    }
}

void func_801328B0(void) {
    /* FAKE: pad1..pad4 are unused locals that reserve the original's extra frame words (idx at sp+0x70, in/out vectors at sp+0x58/0x48, flags at sp+0x40, dist at sp+0x38); real source unknown. T-19025 */
    TcActor *act;
    u32 len;
    s32 pad1;
    s32 idx;
    s32 pad2;
    TcActor *q;
    VECTOR in;
    VECTOR out;
    s32 pad3;
    s32 flags;
    s32 pad4;
    s32 dist;

    dist = func_80140638(&idx);
    flags = 0;
    if (dist != 0) {
        if (!(D_800E6280.unk_10F8 & 7)) {
            flags = 0x20;
        } else if (!(D_800E6280.unk_10F8 & 0xF)) {
            flags = 0x20;
        }
    }
    act = &D_8015EDB4[8];
    D_8015EDB4[8].unk0[1] = 0x80;
    act->unk84[3] = 1;
    act->unk84[0] = 2;
    if (act->unk72 == 0) {
        func_80141170(1, flags);
    } else if (act->unk72 == 1) {
        func_80141290(1, flags);
    } else {
        func_80141050(1, flags);
    }
    if (act->unk82 == 0) {
        func_801404FC(1, flags);
    } else if (act->unk82 == 1) {
        func_80140364(1, flags);
    }
    if ((u32)dist < 8) {
        in.vx = D_8015EDB4->unk74 - act->unk74;
        in.vy = D_8015EDB4->unk76 - act->unk76;
        in.vz = 0;
        len = func_800AA460(&in, &out);
        switch (act->unk2) {
        default:
            act->unk2 = 0;
        case 0:
            act->unk0[1] = 0x80;
            act->unk2 += 1;
        case 1:
            if (len >= 0x10001) {
                act->unk74 += out.vx / 103;
                act->unk76 += out.vy / 103;
            }
            break;
        case 2:
            act->unk2 += 1;
        case 3:
            if (len >= 0x10001) {
                act->unk74 += out.vy / 103;
                act->unk76 += out.vx / 103;
            }
            break;
        case 4:
            act->unk0[1] = 0x80;
            act->unk2 += 1;
        case 5:
            in.vx = D_8015EDB4->unk74 - act->unk74;
            in.vy = D_8015EDB4->unk76 - act->unk76 + 0x80;
            in.vz = 0;
            len = func_800AA460(&in, &out);
            if (len >= 0x641) {
                act->unk74 += out.vy / 103;
                act->unk76 += out.vx / 103;
            }
            break;
        case 6:
            act->unk0[1] = 0x90;
            act->unk7C = 3 - (func_800AE0D0() & 7);
            act->unk7E = 3 - (func_800AE0D0() & 7);
            act->unk2 += 1;
        case 7:
            if (D_8015EDB4->unk78 + 0x21 < act->unk78) {
                act->unk80 = -0x20;
            }
            if (act->unk78 < 0x29) {
                act->unk80 = 0x20;
            }
            break;
        case 8:
            act->unk0[1] = 0x90;
            in.vx = 0xF - (func_800AE0D0() & 0x1F);
            in.vy = 0xF - (func_800AE0D0() & 0x1F);
            in.vz = 0xF - (func_800AE0D0() & 0x1F);
            func_800AA460(&in, &out);
            act->unk7C = out.vx / 103;
            act->unk7E = out.vy / 103;
            act->unk80 = out.vz / 103;
            act->unk2 += 1;
        case 9:
            if (!(func_800AE0D0() & 0x7F)) {
                act->unk2 = 8;
            }
            break;
        }
    } else {
        q = &D_8015EDB4[idx];
        in.vx = q->unk74 - act->unk74;
        in.vy = q->unk76 - act->unk76;
        in.vz = 0;
        func_800AA460(&in, &out);
        act->unk74 += out.vx / 103;
        act->unk76 += out.vy / 103;
    }
    act->unk78 += act->unk80;
    if (act->unk74 > 0x1000) {
        act->unk74 = 0x1000;
    }
    if (act->unk74 < -0x1000) {
        act->unk74 = -0x1000;
    }
    if (act->unk76 > 0x180) {
        act->unk76 = 0x180;
    }
    if (act->unk76 < -0x800) {
        act->unk76 = -0x800;
    }
    if (!(func_800AE0D0() & 0x3F)) {
        act->unk80 = 0xF - (func_800AE0D0() & 0x1F);
    }
    if (D_8015EDB4->unk78 + 0x200 < act->unk78) {
        act->unk80 = -(func_800AE0D0() & 0xF);
    }
    if (act->unk78 < -0x1000) {
        act->unk80 = func_800AE0D0() & 0xF;
    }
    act->unk6C = 0x400;
    act->unk6E = 0x400;
    act->unk70 = 0x400;
}

void func_80132FE8(void) {
    /* FAKE: pad1..pad3 are unused locals that reserve the original's three frame words above flags (flags at sp+0x30, actor at sp+0x2C, btn at sp+0x28); real source unknown. flags is left unset on the unk2 & 0x40 path and passed to func_80141DE0 as is, as in the original. T-19025 */
    s32 pad1;
    s32 pad2;
    s32 pad3;
    s32 flags;
    TcActor *actor;
    /* FAKE: btn copies D_800E6280.unk_F88 in both arms (the first arm reads it after the flag tests) and all six mask tests use it; it stays live in $v0 across the D_8015EDE4 tests so those take $a0 as in the original. Matches, real source unknown. T-19025 */
    s32 btn;

    actor = D_8015EDB4;
    if (actor->unk2 & 0x40) {
    } else {
        func_8013240C();
        func_801328B0();
        func_80140000();
        func_80140DF0();
        flags = 0;
        if (D_8015EDD8 != 0) {
            if ((D_800E6280.unk_F80 & 0x20) && (D_8015EDE0 % 3 == 0) && !(D_800E6280.unk_10F8 & 3)) {
                flags = 0x20;
            }
            if ((D_800E6280.unk_F80 & 0x40) && (D_8015EDE0 % 3 == 1) && !(D_800E6280.unk_10F8 & 3)) {
                flags |= 0x20;
            }
            if ((D_800E6280.unk_F80 & 0x80) && (D_8015EDE0 % 3 == 2) && !(D_800E6280.unk_10F8 & 3)) {
                flags |= 0x20;
            }
            btn = D_800E6280.unk_F88;
        } else {
            btn = D_800E6280.unk_F88;
            if (btn) {} /* FAKE: empty test of btn; without it the three btn masks that uopt inserts at the end of the first arm come out as 0x20, 0x80, 0x40, the original has 0x80, 0x40, 0x20. T-19025 */
            if ((btn & 0x20) && (D_8015EDE0 % 3 == 0)) {
                flags = 0x20;
            }
            if ((btn & 0x40) && (D_8015EDE0 % 3 == 1)) {
                flags |= 0x20;
            }
            if ((btn & 0x80) && (D_8015EDE0 % 3 == 2)) {
                flags |= 0x20;
            }
        }
        if ((btn & 0x40) && (D_8015EDE4 % 3 == 0)) {
            flags |= 0x40;
        }
        if ((btn & 0x80) && (D_8015EDE4 % 3 == 1)) {
            flags |= 0x40;
        }
        if ((btn & 0x20) && (D_8015EDE4 % 3 == 2)) {
            flags |= 0x40;
        }
        if (actor->unk72 == 0) {
            func_80141170(0, flags);
        } else if (actor->unk72 == 1) {
            func_80141290(0, flags);
        } else {
            func_80141050(0, flags);
        }
        if (actor->unk82 == 0) {
            func_801404FC(0, flags);
        } else if (actor->unk82 == 1) {
            func_80140364(0, flags);
        }
    }
    func_80141DE0(flags);
    func_8013B990();
    func_8013D348();
    func_8013D3F4();
    func_8013DDE4();
    func_8013C6D4();
    func_8013E4FC();
}

void func_80133374(void) {
    if (D_8015EDCC < 3U) {
        D_8015EDEC -= 1;
    }
    if (D_8015EDEC == 0) {
        func_80042908(0x2D);
    }
    if (D_800E6280.unk_F88 & 0x800) {
        func_80042908(0x2D);
    }
    if (D_8015EDB4->unk84[1] == 0) {
        func_80042908(0x2D);
    }
}

void func_80133410(s32 arg0) {
    s32 i;

    if (arg0 == 0) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 16; i++) D_8015E270[i].unk13 &= ~0x20;
    } else {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 16; i++) D_8015E3B0[i].unk13 &= ~0x20;
    }
}

void func_801334BC(void) {
    s32 i;

    D_80160460 = 0;
    D_80160464 = 0; for (i = 0; i < 16; i++) { /* FAKE: store and loop head on one source line, as1 then schedules the store after the pointer setup */
        D_8015E270[i].unk13 = 0;
        D_8015E3B0[i].unk13 = 0;
        D_8015E4F0[i].unk13 = 0;
        D_8015E630[i].unk13 = 0;
        D_8015E770[i].unk13 = 0;
    }
}

void func_8013356C(void) {
    s32 i;

    for (i = 0; i < 16; i++) {
        D_8015EB30[i].unk13 = 0;
    }
}
