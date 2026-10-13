#include "common.h"
#include "ovl/TT.h"

void func_80142D40(u8 *arg0, s16 arg1, s16 arg2) {
    RECT r;
    s32 i;

    /* FAKE: w and h on one source line; IDO then stores h before w (line-based scheduling). T-6070 */
    r.w = 0x100; r.h = 0x10;
    r.x = arg1;
    r.y = arg2;
    for (i = 0; i < 0x2000; i += 0x20) {
        *(s16 *)(arg0 + i) = 0;
    }
    func_8009C884(&r, arg0);
}

void func_80142DA8(void *arg0, s16 arg1, s16 arg2) {
    RECT r;

    /* FAKE: w and h on one source line; IDO then stores h before w (line-based scheduling). T-6070 */
    r.w = 0x20; r.h = 0x80;
    r.x = arg1;
    r.y = arg2;
    func_8009C884(&r, arg0);
}

void func_80142DEC(s32 arg0, s32 arg1, s32 arg2) {
    u8 pos[4];
    s32 sec;
    s32 min;
    s32 frame;

    frame = arg1 % 75;
    sec = arg1 / 75;
    pos[2] = (frame / 10) * 16 + frame % 10;
    min = sec / 60;
    sec = sec % 60;
    pos[0] = (min / 10) * 16 + min % 10;
    pos[1] = (sec / 10) * 16 + sec % 10;
    while (func_800879D0(2, pos, 0) == 0) {
    }
    while (func_80087954(1, 0) != 2) {
    }
    func_80087E6C((arg2 + 0x7FF) / 0x800, arg0, 0x80);
    while (func_80087E4C(1, 0) > 0) {
    }
}

void func_8014304C(s32 arg0, s32 arg1, s32 arg2) {
    D_80156784 = D_80156780;
    D_80156780 = func_800460DC();
    func_80045414(9, 0, 0);
    func_800462C8((arg2 + 0x7FF) / 2048, arg0, arg1);
}

u8 func_801430C4(void) {
    return D_80156784;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142D40", func_801430D4);

void func_80143570(void) {
    *D_80158A74 = 0;
}

void func_80143580(void) {
    s32 i;
    u8 *p;
    u8 *q;
    s8 v;

    q = D_80158A74;
    p = D_80158A78;
    v = 8;
    if (D_80158A64[0x22] != 0) {
        switch (D_80158A60[0x52]) {
        default:
            v = 8;
            q[0x53] = 0;
            q[0x54] = 4;
            q[0x55] = v;
            p[0x92] = v;
            break;
        case 1:
            v = 8;
            q[0x53] = 0;
            q[0x54] = 4;
            q[0x55] = v;
            p[0x92] = v;
            break;
        case 2:
            v = 9;
            q[0x53] = 0;
            q[0x54] = 4;
            q[0x55] = v;
            p[0x92] = v;
            break;
        case 3:
            v = 9;
            q[0x53] = 0;
            q[0x54] = 4;
            q[0x55] = v;
            p[0x92] = v;
            break;
        case 4:
            q[0x53] = 1;
            q[0x54] = 5;
            q[0x55] = 0xA;
            p[0x92] = 9;
            for (i = 0; i != 3; i++) {
                func_80147FD0(p);
                p += 0xA4;
            }
            break;
        }
    } else {
        q[0x53] = 0;
        q[0x54] = 4;
        q[0x55] = 8;
        p[0x92] = v;
    }
}

#ifdef NON_MATCHING
/* NON_MATCHING: T-10320, constants 2 and D_80151A50 sit in $a1/$a2, the original keeps them in $t0/$t1 (20 lines, nothing else differs) */
void func_801436BC(void) {
    u8 *p;
    u8 *q;
    s32 n;

    p = D_80158A74;
    *p = 0;
    *(s16 *)(p + 2) = 0;
    p[12] = 0;
    *(s16 *)(p + 4) = 0;
    p[13] = 0;
    p[14] = 0;
    p[15] = 0;
    *(s16 *)(p + 6) = 0;
    *(s16 *)(p + 8) = 0;
    *(s16 *)(p + 10) = 0;
    *(s16 *)(p + 16) = 2;
    *(u8 **)(p + 56) = D_80151A50;
    p[60] = 0;
    p[61] = 0;
    p[62] = 0;
    p[64] = 0x60;
    p[72] = 0;
    func_80143580();
    p[87] = 0;
    p[88] = 0;
    p[89] = 0;
    p[90] = 0;
    p[91] = 0;
    p[92] = 0;
    p[93] = 0;
    p[94] = 0;
    p[95] = 0;
    p[96] = 0;
    *(s16 *)(p + 4) = 0x20;
    *(s16 *)(p + 10) = 0;
    *(s16 *)(p + 8) = 0;
    *(s16 *)(p + 6) = 0;
    p[15] = 0;
    p[14] = 0;
    p[13] = 0;
    q = D_80158A7C;
    n = 5;
    do {
        q[62] = n;
        n += 5;
        q += 0x60;
        q[-96] = 0;
        *(s16 *)(q - 94) = 0;
        *(s16 *)(q - 92) = 0;
        q[-84] = 0;
        *(s16 *)(q - 86) = 0;
        *(s16 *)(q - 88) = 0;
        *(s16 *)(q - 90) = 0;
        q[-81] = 0;
        q[-82] = 0;
        q[-83] = 0;
        *(s16 *)(q - 80) = 2;
        *(u8 **)(q - 40) = D_80151A50;
        q[-36] = 0;
        q[-35] = 0;
        q[-32] = 0x60;
    } while (n != 0xF);
}
#else
INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142D40", func_801436BC);
#endif

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142D40", func_801437F0);

void func_80143970(void) {
}

void func_80143978(void) {
    u8 *p = D_80158A74;
    u8 *q = D_80158A6C;

    switch (*(u16 *)(p + 6)) {
    case 0:
        p[0x52] = 1;
        *(u16 *)(p + 0x50) |= 2;
        p[0x56] = 1;
        p[0x5A] = 0;
        p[0x5B] = 0;
        *(u16 *)(p + 6) = 0x40;
        break;
    case 0x40:
        *(s32 *)(p + 0x28) = 0;
        *(s32 *)(p + 0x2C) = 0xFFFC0000;
        if (*(s32 *)(p + 0x24) < (s32)0xFFF00000) {
            *(s32 *)(p + 0x2C) = 0;
            *(u16 *)(p + 6) = 0x50;
            *(u16 *)(q + 0x10) = 0;
        }
        break;
    case 0x50:
        if ((u32)*(u16 *)(D_80158A6C + 0x10) >= 0x5B) {
            *(u16 *)(p + 6) = 0;
            p[0xD] = 1;
        }
        *(u16 *)(q + 0x10) += 1;
        break;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142D40", func_80143A44);

void func_80143FE8(void) {
    u8 *p = D_80158A74;
    s32 v;

    *(s32 *)(p + 0x20) += *(s32 *)(p + 0x28);
    *(s32 *)(p + 0x24) += *(s32 *)(p + 0x2C);
    if (p[0x56] == 0) {
        if (*(s32 *)(p + 0x20) < (s32)0xFF880000) {
            *(s32 *)(p + 0x20) = -0x780000;
        } else if (*(s32 *)(p + 0x20) >= 0x780001) {
            *(s32 *)(p + 0x20) = 0x780000;
        }
        v = *(s32 *)(p + 0x24);
        if (v < 0x380000) {
            *(s32 *)(p + 0x24) = 0x380000;
        } else if (v >= 0xE00001) {
            *(s32 *)(p + 0x24) = 0xE00000;
        }
    }
    *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
    *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
}

typedef struct {
    s32 w[9];
} TtBlk36; /* a 0x24-byte struct copy: IDO copies it in a 3-trip loop of 3 words */

void func_80144094(void) {
    s32 pad; /* FAKE: unused local declared first puts sp38/sp34 at the original slots; real source unknown. T-9340 */
    u8 *sp38;
    s32 *sp34;
    u8 *p;
    u8 *q;

    sp38 = D_80158A6C;
    sp34 = D_80158A70;
    p = D_80158A74;
    q = D_80158A7C;
    func_80133AD0(0x610);
    func_80133AD0(0x607);
    if (*(u16 *)((u8 *)sp34 + 2) < 0x401) {
        *(s32 *)(sp38 + 4) = 0x8000;
        *(s32 *)sp38 = 0x8000;
    }
    p[0x52] = 1;
    p[0x56] = 1;
    p[0x53] = 0;
    *(s32 *)(p + 0x28) = 0;
    *(s32 *)(p + 0x2C) = 0x10000;
    *(u8 **)(p + 0x38) = D_80151A50;
    p[0x3C] = 0;
    p[0x3D] = 1;
    p[0x3E] = 0;
    p[0x40] = 0x80;
    func_801478D0(p + 0x38);
    *(TtBlk36 *)(q + 0x14) = *(TtBlk36 *)(p + 0x14);
    *(s32 *)(q + 0x28) = 0;
    *(s32 *)(q + 0x2C) = 0;
    *(u8 **)(q + 0x38) = D_80151A50;
    q[0x3C] = 0;
    q[0x3D] = 2;
    q[0x3E] = 0;
    q[0x40] = 0x80;
    func_801478D0(q + 0x38);
    *(s32 *)(q + 0x98) = 0;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142D40", func_801441BC);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142D40", func_80144854);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142D40", func_80144A04);

void func_80144CA8(void) {
    u8 *q;
    u8 *p;
    u32 i;

    q = D_80158A6C;
    for (i = 0, p = D_80158AA4 + 0x64; i < (u32)(2 - q[0xC]); i++) {
        *p = 0;
        p -= 0x64;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142D40", func_80144CF0);
