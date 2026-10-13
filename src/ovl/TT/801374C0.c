#include "common.h"
#include "ovl/TT.h"

/* Fields used by TT's phase update. T-9720. */
typedef struct TtPhase3AE {
    /* 0x000 */ u8 pad000[0x52];
    /* 0x052 */ u8 frame;
    /* 0x053 */ u8 pad053[0x357];
    /* 0x3AA */ u16 phase;
    /* 0x3AC */ u16 step;
} TtPhase3AE; /* size 0x3AE */

/* T-11000 near miss: frame, spill slots, hoisted constants and stores match; only the saved-register numbering of a, &pt and D_80155C64 is rotated (original s2, s3, s4; here s4, s2, s3), 32 line pairs. */
#ifdef NON_MATCHING
void func_801374C0(u8 *arg0) {
    u8 *a;
    u8 *p;
    s32 j;
    s32 n;
    /* FAKE: unused homes (pa/pm) place pt at sp+0x68 and the spilled q, i at sp+0x54/0x58 in the 152-byte frame. T-11000. */
    s32 pa[7];
    s16 pt[2];
    s32 pm[3];
    s32 i;
    u8 *q;

    a = D_80158A74;
    i = 0;
    q = arg0;
    do {
        pt[0] = *(s16 *)(q + 0x1E);
        pt[1] = *(s16 *)(q + 0x9E);
        if (a[0x52] == 0 && a[0] != 0 && func_80145508(a + 0x14, D_80155B30, (u8 *)pt, D_80155C64) != 0) {
            func_80144094();
            *(u16 *)(a + 4) = 0x60;
            *(s16 *)(a + 10) = 0;
            *(s16 *)(a + 8) = 0;
            *(s16 *)(a + 6) = 0;
            a[15] = 0;
            a[14] = 0;
            a[13] = 0;
        }
        p = D_80158A80;
        for (j = 0, n = 12; j != n; j++, p += 0x54) {
            if (p[0] != 0 && *(u16 *)(p + 4) == 0x40 && func_80145508(p + 0x14, &D_80155B36, (u8 *)pt, D_80155C64) != 0) {
                *(u16 *)(p + 4) = 0x80;
                *(s16 *)(p + 10) = 0;
                *(s16 *)(p + 8) = 0;
                *(s16 *)(p + 6) = 0;
                p[15] = 0;
                p[14] = 0;
                p[13] = 0;
            }
        }
        i += 4;
        q += 4;
    } while (i != 0x20);
}
#else
INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801374C0", func_801374C0);
#endif

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801374C0", func_80137654);

void func_80137878(void) {
    u8 *c;
    u8 *b;
    /* FAKE: unused homes retain the observed frame. T-11000. */
    s32 frame_pad[8];
    s16 pt[2];
    u8 *p;
    s32 i;
    s32 n;

    c = D_80158A8C;
    pt[0] = *(s16 *)(c + 0x14) + *(s16 *)(c + 0x254);
    b = D_80158A64;
    pt[1] = *(s16 *)(c + 0x16) + *(s16 *)(c + 0x284);
    c[0xBA] = 0;
    p = D_80158A80;
    for (i = 0, n = 12; i != n; i++, p += 0x54) {
        if (p[0] != 0 && *(u16 *)(p + 4) == 0x40 && func_80145508(p + 0x14, &D_80155B36, (u8 *)pt, &D_80155C70) != 0) {
            if (*(u16 *)(c + 0x30E) == 2) {
                b[0x25] = b[0x25] + 1;
                if (b[0x24] != 0) {
                    if (!(b[0x25] & 3)) {
                        if (!(b[0x25] & 7)) {
                            func_801342CC(*(s16 *)(p + 0x14), *(s16 *)(p + 0x16), 0x100, 0x20, 0xA0);
                        } else {
                            func_80133FB0(*(s16 *)(p + 0x14), *(s16 *)(p + 0x16), 0x100, 0x20, 0xA0);
                        }
                    }
                }
                func_80133AD0(0x507);
                if (i >= 6) {
                    *(s16 *)(c + 0x30C) -= 1;
                } else if (*(u16 *)(p + 2) == 9) {
                    *(s16 *)(c + 0x30C) -= 4;
                } else {
                    *(s16 *)(c + 0x30C) -= 2;
                }
            } else {
                func_80133AD0(0x506);
            }
            c[0xBA] = 1;
            *(u16 *)(p + 4) = 0x80;
            p[13] = 0;
            p[14] = 0;
            p[15] = 0;
            *(s16 *)(p + 6) = 0;
            *(s16 *)(p + 8) = 0;
            *(s16 *)(p + 10) = 0;
        }
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801374C0", func_80137A78);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801374C0", func_80137C20);

void func_80138178(void) {
    u8 *p = D_80158A8C;

    *(s32 *)(p + 0x20) = 0;
    *(s32 *)(p + 0x24) += 0x10000;
    *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
    *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
    if (*(s16 *)(p + 0x16) >= 0x50) {
        *(s32 *)(p + 0x24) = 0x500000;
        *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
        *(s16 *)(p + 0xA) = 0;
        *(s16 *)(p + 8) = 0;
        *(s16 *)(p + 6) = 0;
        p[0xF] = 0;
        p[0xE] = 0;
        p[0xD] = 0;
        *(s16 *)(p + 4) = 0x30;
    }
}

void func_801381E8(void) {
    u8 *p = D_80158A8C;

    func_80142AF0(D_80152C54, 0);
    if (p[0x40] & 1) {
        *(u8 **)(p + 0x218) = D_80155C58;
        *(s16 *)(p + 4) = 0x40;
    }
}

void func_80138240(void) {
    u8 *b = D_80158A8C;
    u8 *a = D_80158A74;
    s32 d = *(s16 *)(a + 0x14) - *(s16 *)(b + 0x14);
    s32 ad;
    s16 y;

    if (d < 0) {
        ad = -d;
    } else {
        ad = d;
    }
    if (ad < 2) {
        *(s32 *)(b + 0x20) = *(s16 *)(a + 0x14) << 16;
    } else if (d < 0) {
        *(s32 *)(b + 0x20) -= 0x4000;
    } else {
        *(s32 *)(b + 0x20) += 0x4000;
    }
    y = *(s16 *)(b + 0x22);
    if (y >= 0x51) {
        *(s32 *)(b + 0x20) = 0x500000;
        y = *(s16 *)(b + 0x22);
    } else if (y < -0x50) {
        *(s32 *)(b + 0x20) = 0xFFB00000;
        y = *(s16 *)(b + 0x22);
    }
    *(s16 *)(b + 0x14) = y;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801374C0", func_80138300);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801374C0", func_80138808);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801374C0", func_80138B90);

void func_80138D10(void) {
    switch (*(u16 *)(D_80158A8C + 0x30E)) {
    case 1:
        func_80142AF0(D_80152CA4, 6);
        break;
    case 2:
        func_80142AF0(D_80152CB0, 6);
        break;
    case 3:
        func_80142AF0(D_80152CC4, 6);
        break;
    }
}

void func_80138D94(void) {
    u8 *base;
    s32 i;

    base = D_80158A8C;
    for (i = 2; i < 6; i++) {
        if (base[i + 0x351] != 0) {
            func_80142AF0(D_80152CD0, i);
        } else {
            func_80142AF0(D_80152CE4, i);
        }
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801374C0", func_80138E28);

void func_8013923C(void) {
    TtPhase3AE *work;
    u16 step;
    u16 phase;

    work = (TtPhase3AE *)D_80158A8C;
    phase = work->phase += work->step;
    if (phase >= 0x800U) {
        phase = work->phase = 0;
    }
    step = work->step;
    work->frame = (s8) ((phase >> 8) + 1);
    if (step < 0x100U) {
        work->step = (u16) (step + 1);
        return;
    }
    work->step = 0x100U;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801374C0", func_801392A0);

void func_801396C4(void) {
    func_80138300();
}

void func_801396E4(void) {
    u8 *p = D_80158A8C;
    u8 *q;

    if ((u32)*(u16 *)(p + 0x366) >= 0x18) {
        q = D_80158AAC;
        func_80133D14(*(s16 *)(q + 0x3A), *(s16 *)(q + 0xBA), 0x18, 0xA8);
        q += 0xEFC;
        func_80133D14(*(s16 *)(q + 0x3A), *(s16 *)(q + 0xBA), 0x18, 0xA8);
        *(u16 *)(p + 0x366) = 0;
        return;
    }
    *(u16 *)(p + 0x366) += 1;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801374C0", func_80139770);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/801374C0", func_80139B98);
