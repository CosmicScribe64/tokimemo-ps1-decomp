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

void func_801430D4(void) {
    RECT r;
    s32 pad[2]; /* FAKE: unused home below r puts the RECT at sp+0x30 in the 56-byte frame. T-11520 */

    func_80142DEC(0x80162000, 0xBECA, 0x54900);
    func_80142D40((u8 *)0x80162000, 0, 0xF0);
    func_80142D40((u8 *)0x80164000, 0x100, 0xF0);
    func_80142DA8((void *)0x80166000, 0x280, 0);
    func_80142DA8((void *)0x80168000, 0x280, 0x80);
    func_80142DA8((void *)0x8016A000, 0x2A0, 0);
    func_80142DA8((void *)0x8016C000, 0x2A0, 0x80);
    func_80142DA8((void *)0x8016E000, 0x2C0, 0);
    func_80142DA8((void *)0x80170000, 0x2C0, 0x80);
    func_80142DA8((void *)0x80172000, 0x2E0, 0);
    func_80142DA8((void *)0x80174000, 0x380, 0);
    func_80142DA8((void *)0x80176000, 0x380, 0x80);
    func_80142DA8((void *)0x80178000, 0, 0x100);
    func_80142DA8((void *)0x8017A000, 0, 0x180);
    func_80142DA8((void *)0x8017C000, 0x20, 0x100);
    func_80142DA8((void *)0x8017E000, 0x20, 0x180);
    func_80142DA8((void *)0x80180000, 0x40, 0x100);
    func_80142DA8((void *)0x80182000, 0x40, 0x180);
    func_80142DA8((void *)0x80184000, 0x60, 0x100);
    func_80142DA8((void *)0x80186000, 0x60, 0x180);
    func_80142DA8((void *)0x80188000, 0x80, 0x100);
    func_80142DA8((void *)0x8018A000, 0x80, 0x180);
    func_80142DA8((void *)0x8018C000, 0xA0, 0x100);
    func_80142DA8((void *)0x8018E000, 0xA0, 0x180);
    func_80142DA8((void *)0x80190000, 0xC0, 0x100);
    func_80142DA8((void *)0x80192000, 0xC0, 0x180);
    func_80142DA8((void *)0x80194000, 0xE0, 0x100);
    func_80142DA8((void *)0x80196000, 0xE0, 0x180);
    func_80142DA8((void *)0x80198000, 0x100, 0x100);
    func_80142DA8((void *)0x8019A000, 0x100, 0x180);
    func_80142DA8((void *)0x8019C000, 0x120, 0x100);
    func_80142DA8((void *)0x8019E000, 0x120, 0x180);
    func_80142DA8((void *)0x801A0000, 0x140, 0x100);
    func_80142DA8((void *)0x801A2000, 0x140, 0x180);
    func_80142DA8((void *)0x801A4000, 0x200, 0x100);
    func_80142DA8((void *)0x801A6000, 0x200, 0x180);
    func_80142DA8((void *)0x801A8000, 0x260, 0x100);
    func_80142DA8((void *)0x801AA000, 0x260, 0x180);
    func_80142DA8((void *)0x801AC000, 0x280, 0x100);
    func_80142DA8((void *)0x801AE000, 0x280, 0x180);
    func_80142DA8((void *)0x801B0000, 0x2A0, 0x100);
    func_80142DA8((void *)0x801B2000, 0x240, 0x100);
    func_80142DA8((void *)0x801B4000, 0x240, 0x180);
    /* The four RECT fields on one line: IDO then stores h, w, y, x (line-based scheduling). T-11520 */
    r.x = 0x3C0; r.y = 0x180; r.w = 0x18; r.h = 0x30;
    func_8009C884(&r, (void *)0x801B6000);
    r.x = 0x3C0; r.y = 0x1FF; r.w = 0x10; r.h = 1;
    func_8009C884(&r, D_80156788);
    func_8009C674(0);
    func_80142DEC(0x80010000, 0xC08B, 0xE20);
    func_80142DEC(0x80162000, 0xC06B, 0xFED0);
    func_80142DEC(0x80012000, 0xD6D4, 0x2420);
    func_80142DEC(0x8001C000, 0xD678, 0x69E4);
    func_80142DEC(0x80172000, 0xD685, 0x27D60);
    func_80142DEC(0x80016000, 0xC8A8, 0x2420);
    func_80142DEC(0x8002E800, 0xC857, 0x69E4);
    func_80142DEC(0x8019A000, 0xC85B, 0x27D60);
    func_80142DEC(0x8001A000, 0xC067, 0xE20);
    func_80142DEC(0x801C2000, 0xC032, 0x1A680);
}

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

void func_801436BC(void) {
    u8 *p;
    u8 *q;
    s32 n;

    p = D_80158A74;
    *p = 0;
    *(s16 *)(p + 2) = 0;
    p[12] = 0;
    *(s16 *)(p + 4) = 0;
    /* FAKE: chained zero stores; their reloads keep dead ranges (no code) that push the hoisted 2 and D_80151A50 into t0/t1 as in the original. T-11850 */
    p[13] = p[14] = p[15] = 0;
    *(u16 *)(p + 6) = *(u16 *)(p + 8) = *(u16 *)(p + 10) = 0; /* FAKE: u16 view, see above. T-11850 */
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
    *(u16 *)(p + 6) = *(u16 *)(p + 8) = *(u16 *)(p + 10) = 0; /* FAKE: chained, see above. T-11850 */
    p[13] = p[14] = p[15] = 0;
    q = D_80158A7C;
    for (n = 5; n < 15; n += 5) {
        q[62] = n;
        q += 0x60;
        q[-96] = 0;
        *(s16 *)(q - 94) = 0;
        *(s16 *)(q - 92) = 0;
        q[-84] = 0;
        *(u16 *)(q - 90) = *(u16 *)(q - 88) = *(u16 *)(q - 86) = 0; /* FAKE: chained, see above. T-11850 */
        q[-83] = q[-82] = q[-81] = 0;
        *(s16 *)(q - 80) = 2;
        *(u8 **)(q - 40) = D_80151A50;
        q[-36] = 0;
        q[-35] = 0;
        q[-32] = 0x60;
    }
}

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

void func_80143A44(void) {
    s32 i;
    s32 j;
    u8 *pad;
    u8 *p;
    s32 found;
    u8 *r;
    u8 *b;
    u8 *a;
    u8 *q;

    p = D_80158A74;
    pad = D_80158A60 + 0x20;
    if (*(u16 *)pad & 0x8000) {
        *(s32 *)(p + 0x28) = -1;
        p[0x59] = 0;
        if (p[0x58] < 8) {
            p[0x58]++;
        }
    } else if (*(u16 *)pad & 0x2000) {
        *(s32 *)(p + 0x28) = 1;
        p[0x58] = 0;
        if (p[0x59] < 8) {
            p[0x59]++;
        }
    } else {
        *(s32 *)(p + 0x28) = 0;
        if (p[0x58] != 0) {
            p[0x58]--;
        }
        if (p[0x59] != 0) {
            p[0x59]--;
        }
    }
    if (*(u16 *)pad & 0x1000) {
        *(s32 *)(p + 0x2C) = -1;
    } else if (*(u16 *)pad & 0x4000) {
        *(s32 *)(p + 0x2C) = 1;
    } else {
        *(s32 *)(p + 0x2C) = 0;
    }
    if (*(s32 *)(p + 0x28) != 0 && *(s32 *)(p + 0x2C) != 0) {
        *(s32 *)(p + 0x28) *= p[0x54] * 0x6000;
        *(s32 *)(p + 0x2C) *= p[0x54] * 0x6000;
    } else {
        *(s32 *)(p + 0x28) *= p[0x54] << 15;
        *(s32 *)(p + 0x2C) *= p[0x54] << 15;
    }
    if (*(u16 *)(pad + 4) & 1) {
        r = D_80158A80;
        for (i = 0; i < 2; i++) {
            if (p[0x55] == 0xA) {
                if (r[0] == 0 && r[0xA8] == 0 && r[0x150] == 0) {
                    func_80133AD0(0x608);
                    r[0] = 1;
                    *(u16 *)(r + 4) = 0x20;
                    r[0xD] = 0;
                    r[0xE] = 0;
                    r[0xF] = 0;
                    *(s16 *)(r + 6) = 0;
                    *(s16 *)(r + 8) = 0;
                    *(s16 *)(r + 0xA) = 0;
                    r[0xA8] = 1;
                    *(u16 *)(r + 0xAC) = 0x20;
                    r[0x150] = 1;
                    *(u16 *)(r + 0x154) = 0x20;
                    break;
                }
            } else if (r[0] == 0) {
                func_80133AD0(0x608);
                r[0] = 1;
                *(u16 *)(r + 4) = 0x20;
                r[0xD] = 0;
                r[0xE] = 0;
                r[0xF] = 0;
                *(s16 *)(r + 6) = 0;
                *(s16 *)(r + 8) = 0;
                *(s16 *)(r + 0xA) = 0;
                break;
            }
            r += 0x54;
        }
        if (p[0x53] != 0) {
            r = D_80158A80 + 6 * 0x54;
            for (i = 6; i < 8; i++) {
                if (r[0] == 0) {
                    r[0] = 1;
                    *(u16 *)(r + 4) = 0x20;
                    *(s16 *)(r + 0xA) = 0;
                    *(s16 *)(r + 8) = 0;
                    *(s16 *)(r + 6) = 0;
                    r[0xF] = 0;
                    r[0xE] = 0;
                    r[0xD] = 0;
                    break;
                }
                r += 0x54;
            }
            r = D_80158A80 + 8 * 0x54;
            for (i = 8; i < 10; i++) {
                if (r[0] == 0) {
                    r[0] = 1;
                    *(u16 *)(r + 4) = 0x20;
                    *(s16 *)(r + 0xA) = 0;
                    *(s16 *)(r + 8) = 0;
                    *(s16 *)(r + 6) = 0;
                    r[0xF] = 0;
                    r[0xE] = 0;
                    r[0xD] = 0;
                    break;
                }
                r += 0x54;
            }
            r = D_80158A80 + 10 * 0x54;
            for (i = 10; i < 12; i++) {
                if (r[0] == 0) {
                    r[0] = 1;
                    *(u16 *)(r + 4) = 0x20;
                    *(s16 *)(r + 0xA) = 0;
                    *(s16 *)(r + 8) = 0;
                    *(s16 *)(r + 6) = 0;
                    r[0xF] = 0;
                    r[0xE] = 0;
                    r[0xD] = 0;
                    break;
                }
                r += 0x54;
            }
        }
    }
    if (*(u16 *)(pad + 4) & 2) {
        /* FAKE: both loads on one line; IDO then schedules the lui of D_80158A88 first and the lw of D_80158A84 first. T-11840 */
        b = D_80158A88; a = D_80158A84;
        found = 0;
        for (i = 0; i < 2; i++) {
            if (b[0] == 0 && a[0] == 0 && (p[0x5C] == 0 || p[0x5D] == 0)) {
                func_80133AD0(0x609);
                b[0] = 1;
                *(u16 *)(b + 4) = 0x20;
                b[0xD] = 0;
                b[0xE] = 0;
                b[0xF] = 0;
                *(s16 *)(b + 6) = 0;
                *(s16 *)(b + 8) = 0;
                *(s16 *)(b + 0xA) = 0;
                a[0] = 1;
                a[1] = 1;
                *(u16 *)(a + 0x3E) = 1;
                a[0x3D] = p[0x60];
                *(u16 *)(a + 4) = 0x20;
                a[0xD] = 0;
                a[0xE] = 0;
                a[0xF] = 0;
                *(s16 *)(a + 6) = 0;
                *(s16 *)(a + 8) = 0;
                *(s16 *)(a + 0xA) = 0;
                found = a[0];
                break;
            }
            b += 0x58;
            a += 0x58;
        }
        if (p[0x53] != 0) {
            for (j = 1; j < 4; j++) {
                /* FAKE: one line, as above (temp numbering t5 = D_80158A88, loads in the other order). T-11840 */
                q = D_80158A88 + j * 0xB0; a = D_80158A84 + j * 0xB0;
                for (i = 0; i < 2; i++) {
                    if (q[0] == 0 && a[0] == 0 && (p[0x5C] == 0 || p[0x5D] == 0)) {
                        q[0] = 1;
                        *(u16 *)(q + 4) = 0x20;
                        /* FAKE: the u16 and byte chains keep a dead range in $a1 (no code), so 2, j * 0xB0 and p land in a2/a3/t0. T-11840 */
                        *(u16 *)(q + 6) = *(u16 *)(q + 8) = *(u16 *)(q + 0xA) = 0;
                        q[0xD] = q[0xE] = q[0xF] = 0;
                        a[0] = 1;
                        a[1] = 1;
                        *(u16 *)(a + 0x3E) = 1;
                        *(u16 *)(a + 4) = 0x20;
                        a[0xD] = 0;
                        a[0xE] = 0;
                        a[0xF] = 0;
                        *(s16 *)(a + 6) = 0;
                        *(s16 *)(a + 8) = 0;
                        *(s16 *)(a + 0xA) = 0;
                        found = 1;
                        a[0x3D] = p[0x60];
                        break;
                    }
                    q += 0x58;
                    a += 0x58;
                }
            }
        }
        if (found) {
            if (p[0x60] != 0) {
                p[0x5D] = 1;
            } else {
                p[0x5C] = 1;
            }
            p[0x60]++;
            p[0x60] &= 1;
        }
    }
}

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

void func_801441BC(void) {
    s32 i;
    u8 *g;
    u8 *h;
    u8 *r;
    u8 *p;
    u8 *q;
    u16 v;

    p = D_80158A74;
    q = D_80158A7C;
    g = D_80158A64;
    h = D_80158A6C;
    switch (*(u16 *)(p + 4)) {
    case 0x20:
        func_801437F0(h, g);
        if (p[0xD] != 0) {
            p[0xD] = 0;
            *(s16 *)(p + 0x62) = 0;
            func_80146BE0();
            func_80148DB0();
            *(u16 *)(p + 4) = 0x30;
            *(s16 *)(p + 0xA) = 0;
            *(s16 *)(p + 8) = 0;
            *(s16 *)(p + 6) = 0;
            p[0xF] = 0;
            p[0xE] = 0;
            p[0xD] = 0;
        }
        break;
    case 0x30:
        func_80143A44();
        v = *(u16 *)(p + 0x62);
        if (v >= 0x5A) {
            p[0x52] = 0;
            *(u16 *)(p + 0x50) &= ~2;
            *(u16 *)(p + 4) = 0x40;
            *(s16 *)(p + 0xA) = 0;
            *(s16 *)(p + 8) = 0;
            *(s16 *)(p + 6) = 0;
            p[0xF] = 0;
            p[0xE] = 0;
            p[0xD] = 0;
        } else {
            *(u16 *)(p + 0x62) = v + 1;
        }
        break;
    case 0x40:
        func_80143A44();
        break;
    case 0x60:
        *(s32 *)(p + 0x28) = 0;
        *(s32 *)(p + 0x2C) = 0x10000;
        func_801478D0(p + 0x38);
        func_801478D0(q + 0x38);
        if (p[0x40] & 1) {
            *(s32 *)(p + 0x28) = 0;
            *(s32 *)(p + 0x2C) = 0;
            *(u16 *)(p + 4) = 0x61;
        }
        break;
    case 0x61:
        if (D_80158A6C[0xC] == 0) {
            g[0x12] = 0;
            *(u16 *)g = 5;
            *(s16 *)(g + 4) = 0;
        } else {
            h[0xC] -= 1;
            func_80144CA8();
        }
        func_801436BC();
        p[0] = 0;
        *(s32 *)(p + 0x20) = 0;
        *(s32 *)(p + 0x24) = 0xF80000;
        *(s32 *)(p + 0x28) = 0;
        *(s32 *)(p + 0x2C) = 0;
        *(s16 *)(p + 0x14) = 0;
        *(s16 *)(p + 0x16) = 0xF8;
        *(u16 *)(p + 4) = 0x20;
        *(s16 *)(p + 0xA) = 0;
        *(s16 *)(p + 8) = 0;
        *(s16 *)(p + 6) = 0;
        p[0xF] = 0;
        p[0xE] = 0;
        p[0xD] = 0;
        break;
    case 0xA0:
        func_80143978();
        p[0x3E] = 0;
        q[0x3E] = 5;
        q[0x9E] = 0xA;
        if (p[0xD] != 0) {
            /* u16, not s16: the 4 then joins the a2 range of the later == 4 tests (li before the beqz here, and
             * the empty b/nop join blocks after the inner ifs of cases 0x20, 0x30 and 0x60). T-11840 */
            *(u16 *)g = 4;
            *(s16 *)(g + 6) = 0;
            *(s16 *)(g + 4) = 0;
            *(s16 *)(g + 2) = 0;
            g[0xB] = 0;
            g[0xA] = 0;
            g[9] = 0;
        }
        break;
    }
    func_80143FE8();
    if (*(u16 *)(p + 4) < 0x60) {
        if (p[0x58] == 0 && p[0x59] == 0) {
            p[0x57] = 0;
        } else if (p[0x58] >= 7) {
            p[0x57] = 2;
        } else if (p[0x58] >= 3) {
            p[0x57] = 1;
        } else if (p[0x59] >= 7) {
            p[0x57] = 4;
        } else if (p[0x59] >= 3) {
            p[0x57] = 3;
        }
        p[0x3E] = p[0x57];
        /* Byte before the product: sets the operand order of the addu and the t-register numbering. T-11840 */
        q[0x3E] = p[0x5A] + p[0x57] * 10 + 5;
        q[0x9E] = p[0x5B] + p[0x57] * 10 + 10;
        if (p[0x5C] != 0) {
            if (D_801567B0[p[0x5A]] < ++p[0x5E]) {
                p[0x5E] = 0;
                if (++p[0x5A] >= 5) {
                    p[0x5A] = 0;
                    p[0x5C] = 0;
                }
            }
            r = D_80158A84;
            for (i = 0; i < 8; i++) {
                if (r[0] != 0 && r[0x3D] == 0 && p[0x5A] == 4 && p[0x5E] == 1) {
                    r[1] = 0;
                    *(s16 *)(r + 0x3E) = 0;
                }
                r += 0x58;
            }
        }
        if (p[0x5D] != 0) {
            if (D_801567B0[p[0x5B]] < ++p[0x5F]) {
                p[0x5F] = 0;
                if (++p[0x5B] >= 5) {
                    p[0x5B] = 0;
                    p[0x5D] = 0;
                }
            }
            r = D_80158A84;
            for (i = 0; i < 8; i++) {
                if (r[0] != 0 && r[0x3D] == 1 && p[0x5B] == 4 && p[0x5F] == 1) {
                    r[1] = 0;
                    *(s16 *)(r + 0x3E) = 0;
                }
                r += 0x58;
            }
        }
    }
}

/* FAKE: the first test reads D_80158A74 through an array view of the pointer block at D_80158A60, so uopt does not
 * merge it with p; the original loads p straight into s2 and reloads the global for the test (T-0018 promo). T-11620 */
void func_80144854(void) {
    u8 *p;
    u8 *q;
    u16 k;
    u8 n;

    p = D_80158A74;
    q = D_80158A7C;
    if (p[0] != 0) {
        if (!(*(u16 *)(((u8 **)&D_80158A60)[5] + 0x50) & 2) || !(*(u16 *)(D_80158A60 + 8) & 1)) {
            k = *(u16 *)(D_80158A74 + 4);
            if (k >= 0x60 && k < 0x64) {
                if (!(D_80158A74[0x40] & 1)) {
                    func_80147A18(p + 0x14, p + 0x38, *(u16 *)(p + 0x10));
                }
                if (!(q[0x40] & 1)) {
                    func_80147A18(q + 0x14, q + 0x38, *(u16 *)(p + 0x10));
                }
            } else {
                n = D_80158A74[0x57];
                if (n == 0) {
                    func_80147A18(p + 0x14, q + 0x38, *(u16 *)(q + 0x10));
                    q += 0x60;
                    func_80147A18(p + 0x14, q + 0x38, *(u16 *)(q + 0x10));
                    func_80147A18(p + 0x14, p + 0x38, *(u16 *)(p + 0x10));
                } else if (n < 3) {
                    func_80147A18(p + 0x14, q + 0x98, *(u16 *)(q + 0x10));
                    func_80147A18(p + 0x14, p + 0x38, *(u16 *)(p + 0x10));
                    func_80147A18(p + 0x14, q + 0x38, *(u16 *)(q + 0x10));
                } else {
                    func_80147A18(p + 0x14, q + 0x38, *(u16 *)(q + 0x10));
                    func_80147A18(p + 0x14, p + 0x38, *(u16 *)(p + 0x10));
                    func_80147A18(p + 0x14, q + 0x98, *(u16 *)(q + 0x10));
                }
            }
        }
    }
}

void func_80144A04(u8 *arg0) {
    u8 *p;
    u8 *w;
    s32 i;
    u8 *q;

    p = D_80158A74;
    switch (*(u16 *)(arg0 + 4)) {
    case 0x40:
    case 0x60:
        switch (arg0[0x58]) {
        case 0:
            func_80133AD0(0x60A);
            arg0[0] = 0;
            break;
        case 1:
            func_80133AD0(0x605);
            func_8013BE08(arg0, 0x28);
            p[0x55] = 9;
            D_80158A78[0x92] = 9;
            *(s32 *)(arg0 + 0x34) = 0;
            *(s32 *)(arg0 + 0x2C) = 0;
            *(s32 *)(arg0 + 0x28) = 0;
            *(s16 *)(arg0 + 0x62) = 0;
            *(u16 *)(arg0 + 4) = 0x90;
            /* FAKE: chained stores; the reloads keep dead ranges in v0/v1 (no code, as the function is void), so the shared 9 lands in a0 as in the original. T-11850 */
            arg0[0xD] = arg0[0xE] = arg0[0xF] = 0;
            *(u16 *)(arg0 + 6) = *(u16 *)(arg0 + 8) = *(u16 *)(arg0 + 0xA) = 0; /* FAKE: u16 view, see above. T-11850 */
            break;
        case 2:
            func_80133AD0(0x602);
            func_8013BE08(arg0, 0x29);
            p[0x54] += 1;
            *(s32 *)(arg0 + 0x34) = 0;
            *(s32 *)(arg0 + 0x2C) = 0;
            *(s32 *)(arg0 + 0x28) = 0;
            *(s16 *)(arg0 + 0x62) = 0;
            *(u16 *)(arg0 + 4) = 0x90;
            arg0[0xD] = 0;
            arg0[0xE] = 0;
            arg0[0xF] = 0;
            *(s16 *)(arg0 + 6) = 0;
            *(s16 *)(arg0 + 8) = 0;
            *(s16 *)(arg0 + 0xA) = 0;
            break;
        case 3:
            func_80133AD0(0x604);
            if (p[0x53] == 0) {
                func_8013BE08(arg0, 0x2A);
                p[0x53] = 1;
                q = D_80158A78;
                for (i = 0; i != 3; i++, q += 0xA4) {
                    func_80147FD0(q);
                }
            }
            *(s32 *)(arg0 + 0x34) = 0;
            *(s32 *)(arg0 + 0x2C) = 0;
            *(s32 *)(arg0 + 0x28) = 0;
            *(s16 *)(arg0 + 0x62) = 0;
            *(u16 *)(arg0 + 4) = 0x90;
            *(s16 *)(arg0 + 0xA) = 0;
            *(s16 *)(arg0 + 8) = 0;
            *(s16 *)(arg0 + 6) = 0;
            arg0[0xF] = 0;
            arg0[0xE] = 0;
            arg0[0xD] = 0;
            break;
        case 6:
            func_80133AD0(0x603);
            func_8013BE08(arg0, 0x2B);
            p[0x54] -= 1;
            if (p[0x54] == 0) {
                p[0x54] = 1;
            }
            *(s32 *)(arg0 + 0x34) = 0;
            *(s32 *)(arg0 + 0x2C) = 0;
            *(s32 *)(arg0 + 0x28) = 0;
            *(s16 *)(arg0 + 0x62) = 0;
            *(u16 *)(arg0 + 4) = 0x90;
            *(s16 *)(arg0 + 0xA) = 0;
            *(s16 *)(arg0 + 8) = 0;
            *(s16 *)(arg0 + 6) = 0;
            arg0[0xF] = 0;
            arg0[0xE] = 0;
            arg0[0xD] = 0;
            break;
        }
        break;
    case 0x20:
        break;
    case 0x70:
        if (D_80158A74[0x52] == 0) {
            w = D_80158A6C;
            if (*(u16 *)((u8 *)D_80158A70 + 2) < 0x401) {
                *(s32 *)(w + 4) = 0x8000;
                *(s32 *)w = 0x8000;
            }
            func_80144094();
            *(u16 *)(p + 4) = 0x60;
            *(s16 *)(p + 0xA) = 0;
            *(s16 *)(p + 8) = 0;
            *(s16 *)(p + 6) = 0;
            p[0xF] = 0;
            p[0xE] = 0;
            p[0xD] = 0;
            arg0[0] = 0;
        }
        break;
    }
}

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

void func_80144CF0(void) {
    u8 *a;
    u8 *p;
    s32 i;

    a = D_80158A74;
    p = D_80158A78;
    *(u16 *)(D_80158A6C + 8) = 0x8CA0;
    a[0x53] = 1;
    a[0x55] = 9;
    p[0x92] = 9;
    for (i = 0; i != 3; i++) {
        func_80147FD0(p);
        p += 0xA4;
    }
}
