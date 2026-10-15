#include "common.h"
#include "ovl/TT.h"

u8 *func_8013BD10(u8 *arg0, u8 *arg1) {
    while (arg0 < arg1) {
        if (*arg0 == 0) {
            return arg0;
        }
        arg0 += 0x6C;
    }
    return 0;
}

/* i = 0 before p = D_80158A90 gives the original's registers ($v0 counter, $v1 pointer); T-4060 */
void func_8013BD4C(void) {
    s32 i;
    u8 *p;

    i = 0;
    p = D_80158A90;
    for (; i < 0x20; i++) {
        *p = 0;
        p += 0x6C;
    }
}

void func_8013BD80(void) {
    s32 i;
    u8 *p;

    i = 0;
    p = D_80158A90;
    for (; i < 0x20; i++) {
        *p = 0;
        *(s16 *)(p + 0x10) = 6;
        p += 0x6C;
    }
}

void func_8013BDC8(u8 *arg0) {
    arg0[0] = 1;
    *(s16 *)(arg0 + 2) = 0x20;
    *(s16 *)(arg0 + 0x52) = 0xA;
    *(s16 *)(arg0 + 0x54) = 0x3C84;
    *(s32 *)(arg0 + 0x38) = D_8014E34C;
    arg0[0x40] = 0x80;
    *(s16 *)(arg0 + 0x10) = 6;
}

void func_8013BE08(u8 *arg0, u16 arg1) {
    *(s16 *)(arg0 + 2) = arg1;
    switch (arg1) {
    case 40:
        arg0[0x56] = 0;
        arg0[0x57] = 0;
        break;
    case 41:
        /* FAKE: both stores on one source line give the original's li/sb order (as1 schedules by line, T-7020) */
        arg0[0x56] = 0x40; arg0[0x57] = 0x10;
        break;
    case 42:
        arg0[0x56] = 0x40;
        arg0[0x57] = 0;
        break;
    case 43:
        arg0[0x57] = 0x20;
        arg0[0x56] = 0;
        break;
    case 44:
        arg0[0x57] = 0x10;
        arg0[0x56] = 0;
        break;
    }
    arg0[0x3C] = 0;
    *(s16 *)(arg0 + 0x52) = 0xB;
    *(s16 *)(arg0 + 0x54) = 0x3C81;
}

/* NON_MATCHING: T-10320, saved pointers swap $a2/$a3 (base vs arg0) and the constant stores schedule one slot apart, 23 lines */
void func_8013BE9C(u8 *arg0) {
    u8 *base;
    u8 *p;

    base = D_80158A90;
    func_80133AD0(0x606);
    p = func_8013BD10(base, base + 0xD80);
    *(s32 *)(p + 0x20) = *(s32 *)(arg0 + 0xC);
    *(s32 *)(p + 0x24) = *(s32 *)(arg0 + 0x10);
    *(s16 *)(p + 0x14) = *(s16 *)arg0;
    *(s32 *)(p + 0x30) = 0;
    *(s32 *)(p + 0x34) = 0;
    *(s32 *)(p + 0x2C) = 0;
    *(s32 *)(p + 0x28) = 0;
    p[0] = 1;
    *(s16 *)(p + 0x16) = *(s16 *)(arg0 + 2);
    p[0x40] = 0x80;
    *(s16 *)(p + 0x10) = 6;
    *(s16 *)(p + 0x62) = 0;
    *(s16 *)(p + 0x50) = 0;
    *(s16 *)(p + 4) = 0x90;
    p[0xD] = 0;
    p[0xE] = 0;
    p[0xF] = 0;
    *(s16 *)(p + 6) = 0;
    *(s16 *)(p + 8) = 0;
    *(s16 *)(p + 0xA) = 0;
    *(s32 *)(p + 0x38) = D_8014E34C;
    func_8013BE08(p, 0x2C);
}

void func_8013BF68(u8 *arg0) {
    *(s32 *)(arg0 + 0x20) = 0;
    *(s32 *)(arg0 + 0x24) = 0x200000;
    *(s32 *)(arg0 + 0x28) = 0;
    *(s32 *)(arg0 + 0x2C) = 0;
    *(s32 *)(arg0 + 0x30) = 0;
    *(s32 *)(arg0 + 0x34) = 0xC00;
    *(s16 *)(arg0 + 0x14) = *(s16 *)(arg0 + 0x22);
    *(s16 *)(arg0 + 0x16) = *(s16 *)(arg0 + 0x26);
}

/* FAKE: one-word struct copies compile to lw/sw through $at, as the original's copies do; real source unknown. T-9110 */
typedef struct {
    s32 w;
} TtVec1;

void func_8013BF9C(u8 *arg0) {
    u8 *p;

    p = func_8013BD10(D_80158A90, D_80158A90 + 0xD80);
    *(TtVec1 *)(p + 0x20) = *(TtVec1 *)(arg0 + 0x20);
    *(TtVec1 *)(p + 0x24) = *(TtVec1 *)(arg0 + 0x24);
    *(TtVec1 *)(p + 0x28) = *(TtVec1 *)(arg0 + 0x28);
    *(TtVec1 *)(p + 0x2C) = *(TtVec1 *)(arg0 + 0x2C);
    *(s32 *)(p + 0x30) = 0;
    *(s32 *)(p + 0x34) = 0;
    p[0x59] = 0;
    *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
    *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
    func_8013BDC8(p);
    *(s16 *)(p + 0x50) = 1;
    *(s16 *)(p + 4) = 0x20;
    *(s16 *)(p + 0xA) = 0;
    *(s16 *)(p + 8) = 0;
    *(s16 *)(p + 6) = 0;
    p[0xF] = 0;
    p[0xE] = 0;
    p[0xD] = 0;
}

typedef struct {
    /* 0x00 */ u8 flag;
    /* 0x01 */ u8 pad1[0x13];
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ u8 pad18[8];
    /* 0x20 */ s32 unk20; /* 16.16 position */
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
} TtMover; /* partial */

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
} TtMoverHi; /* partial: the integer halves of the 16.16 positions at 0x20 and 0x24 */

void func_8013C048(TtMover *arg0) {
    arg0->unk28 += arg0->unk30;
    arg0->unk2C += arg0->unk34;
    arg0->unk20 += arg0->unk28;
    arg0->unk24 += arg0->unk2C;
    arg0->unk14 = ((TtMoverHi *)arg0)->unk22;
    arg0->unk16 = ((TtMoverHi *)arg0)->unk26;
    if (arg0->unk16 >= 0x101) {
        arg0->flag = 0;
    }
}

void func_8013C0A8(void) {
    s32 i;
    u8 *q;
    u8 *p;

    q = D_80158A6C;
    p = D_80158A90;
    for (i = 0; i < 0x20; i++, p += 0x6C) {
        if (*p == 0) {
            continue;
        }
        switch (*(u16 *)(p + 4)) {
        case 0x40:
            break;
        case 0x90:
            *(u16 *)(p + 98) = *(u16 *)(p + 98) + 0x80;
            if (*(u16 *)(p + 98) > 0x1000) {
                *(u16 *)(p + 98) = 0x1000;
                *(u16 *)(p + 4) = 0x91;
            }
            break;
        case 0x91:
            *(s32 *)(p + 44) = *(s32 *)q;
            *(u16 *)(p + 90) = 0x3C;
            *(u16 *)(p + 4) = 0x92;
            break;
        case 0x92:
            *(s32 *)(p + 44) = *(s32 *)q;
            *(u16 *)(p + 90) = *(u16 *)(p + 90) - 1;
            if (*(u16 *)(p + 90) == 0) {
                *p = 0;
            }
            break;
        }
        func_8013C048(p);
        if (*(u16 *)(p + 4) < 0x90) {
            func_80132000(p + 0x38, p + 0x56, p + 0x57);
        }
    }
}

/* T-11540/T-11550: matched by passing the ordering-table entry as &((TtDrawBuffer80B8 *)buf)->ordering[idx] (as in func_8014CC04): the typed index gives the original addu buf, idx*4 operand order. */
void func_8013C1FC(void) {
    u8 *q;
    u8 *p;
    u8 *buf;
    u8 *v;
    s32 i;

    q = D_80158AB0;
    p = D_80158A90;
    i = 0;
    buf = *(s32 *)(D_80158A68 + 8) * 0x80B8 + D_80158AB4;
    do {
        if (*p != 0 && !(*(u16 *)(p + 0x50) & 1)) {
            v = *(u8 **)(buf + 4);
            *(u8 **)(buf + 4) += 0x30;
            *(s16 *)(q + 4) = *(s16 *)(p + 0x14);
            *(s16 *)(q + 6) = *(s16 *)(p + 0x16);
            v[0x29] = 0;
            v[0x2A] = 1;
            v[0x28] = p[0x3C];
            *(u16 *)(v + 0x16) = *(u16 *)(p + 0x52);
            *(u16 *)(v + 0xE) = *(u16 *)(p + 0x54);
            v[0x2C] = 0;
            if (*(u16 *)(p + 4) < 0x90) {
                *(s16 *)(v + 8) = *(s16 *)(p + 0x14) - 8;
                *(s16 *)(v + 0xA) = *(s16 *)(p + 0x16) - 8;
                v[0xC] = p[0x56];
                v[0xD] = p[0x57] + 0x80;
                v[0x2B] = 0;
                func_80141C6C(&((TtDrawBuffer80B8 *)buf)->ordering[*(u16 *)(p + 0x10)], v, q);
            } else {
                s16 t = *(s16 *)(p + 0x62);

                *(s16 *)(q + 2) = t;
                *(s16 *)q = t;
                *(s16 *)(v + 8) = *(s16 *)(p + 0x14) - 0x1F;
                *(s16 *)(v + 0xA) = *(s16 *)(p + 0x16) - 7;
                v[0xC] = p[0x56];
                v[0xD] = p[0x57];
                v[0x2B] = 1;
                *(s16 *)(v + 0x10) = 0x40;
                *(s16 *)(v + 0x12) = 0x10;
                *(u16 *)(v + 0x16) = 0xB;
                *(u16 *)(v + 0xE) = 0x3C81;
                v[0x28] = 0;
                func_80141EBC(&((TtDrawBuffer80B8 *)buf)->ordering[*(u16 *)(p + 0x10)], v, q);
            }
        }
        i++;
        p += 0x6C;
    } while (i != 0x20);
}

void func_8013C41C(u8 *p, s16 d) {
    u8 *c;
    u8 *e;

    c = D_80158A6C;
    e = D_80158A74;
    c[0xD]++;
    switch (*(u16 *)(p + 4)) {
    case 0x20:
        *(u16 *)(p + 4) = 0x40;
        *(s16 *)(p + 0xA) = 0;
        *(s16 *)(p + 8) = 0;
        *(s16 *)(p + 6) = 0;
        p[0xF] = 0;
        p[0xE] = 0;
        p[0xD] = 0;
    case 0x40:
        func_80133AD0(0x60B);
        if (c[0xD] >= 0x15) {
            c[0xD] = 1;
            if (++p[0x59] >= 2) {
                c[0xD] = 0;
                *(s32 *)(p + 0x38) = D_8014E350;
                p[0x40] = 0x80;
                *(u16 *)(p + 4) = 0x60;
                p[0xD] = 0;
                p[0xE] = 0;
                p[0xF] = 0;
                *(s16 *)(p + 6) = 0;
                *(s16 *)(p + 8) = 0;
                *(s16 *)(p + 0xA) = 0;
            }
        }
        p[0x58] = D_80155B10[c[0xD]];
        switch (p[0x58]) {
        case 1:
            if (e[0x55] == 9) {
                p[0x58] = 0;
            }
            break;
        case 2:
            if (e[0x54] >= 0xA) {
                p[0x58] = 0;
            }
            break;
        case 3:
            if (e[0x53] != 0) {
                p[0x58] = 0;
            }
            break;
        case 6:
            if (e[0x54] == 0) {
                p[0x58] = 0;
            }
            break;
        }
        if (c) { /* FAKE: an extra use of c lowers its priority below the p[0x58] value (c in a3, e in t0). T-11810 */
        }
        *(s16 *)(p + 0x54) = p[0x58] + 0x3C84;
        *(u16 *)(p + 0x50) &= 0xFFFE;
        break;
    case 0x60:
        func_80133AD0(0x60B);
        if (c[0xD] >= 4) {
            *(s16 *)(p + 0x54) = 0x3C86;
            *(s32 *)(p + 0x38) = D_8014E354;
            if (c) { /* FAKE: as above, stretches c over this block. T-11810 */
            }
            p[0x40] = 0xC0;
            *(s32 *)(p + 0x34) = 0;
            *(s32 *)(p + 0x2C) = 0x20000;
            *(s32 *)(p + 0x28) = 0;
            *(u16 *)(p + 4) = 0x70;
            p[0xD] = 0;
            p[0xE] = 0;
            p[0xF] = 0;
            *(s16 *)(p + 6) = 0;
            *(s16 *)(p + 8) = 0;
            *(s16 *)(p + 0xA) = 0;
            return;
        }
        break;
    case 0x70:
        func_80133AD0(0x60C);
        p[0] = 0;
        break;
    }
    *(s32 *)(p + 0x34) = 0x1000;
    *(s32 *)(p + 0x2C) = 0xFFFE8000;
    if (d < -1) {
        *(s32 *)(p + 0x28) = 0x1000;
    } else if (d >= 2) {
        *(s32 *)(p + 0x28) = -0x1000;
    } else {
        *(s32 *)(p + 0x28) = 0;
    }
}
