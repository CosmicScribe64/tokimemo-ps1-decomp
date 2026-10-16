#include "common.h"
#include "ovl/TT.h"

#ifdef NON_MATCHING
/* NON_MATCHING: T-11820, 17 lines: the head (old D_80152AF4 value, *D_80158A6C, page) differs; the original loads the old value straight into v0 and keeps the page in the ugen temp t7, here uopt copies the old value through t7 into a0 and gives the page v0. The unrolled six-record loop (i + 1 compare) matches. Tried: old/t/v locals in all orders, compare against the globals, assignment in the condition. */
void func_80136830(void) {
    u8 *c;
    u8 *p;
    s32 i;
    s32 old;
    s32 v;

    c = D_80158A6C;
    v = *(s32 *)c;
    old = D_80152AF4;
    D_80152AF0 = old;
    D_80152AF4 = v / 0x8000;
    if (old != v / 0x8000) {
        func_80133AD0(0x611);
    }
    if (*(s32 *)c == 0) {
        for (i = 0, p = D_80158AA4 + 0xC8; i < 6; i++, p += 0x64) {
            *(s16 *)(p + 0x42) = 0x3C0A;
        }
    } else {
        for (i = 0, p = D_80158AA4 + 0xC8; i < 6; i++, p += 0x64) {
            if (*(s32 *)c / 0x8000 == i + 1) {
                *(s16 *)(p + 0x42) = 0x3C09;
            } else {
                *(s16 *)(p + 0x42) = 0x3C0A;
            }
        }
    }
}
#else
INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80136830", func_80136830);
#endif

void func_801369F4(void) {
    s32 *p;
    u8 *q;
    u8 k;

    q = D_80158A6C;
    p = D_80158A70;
    p[0] = 0;
    k = q[0xB];
    *(s32 *)q = 0x8000;
    *(s32 *)(q + 4) = 0x8000;
    switch (k) {
    default:
    case 0:
        p[1] = (s32)D_80151A80;
        p[2] = (s32)D_80151AC8;
        p[3] = (s32)func_80139B98;
        return;
    case 1:
        p[1] = (s32)D_80154520;
        p[2] = (s32)D_80154564;
        p[3] = (s32)func_801365C4;
        return;
    case 2:
        p[1] = (s32)D_80155D60;
        p[2] = (s32)D_80155D9C;
        p[3] = (s32)func_8013BB28;
        return;
    case 3:
        p[1] = (s32)D_801564E0;
        p[2] = (s32)D_80156534;
        p[3] = (s32)func_80140D84;
        break;
    }
}

void func_80136AD8(void) {
    u8 *a;
    u8 *b;
    u8 *c;

    a = D_80158A60;
    b = D_80158A64;
    c = D_80158A6C;
    func_8009C5E0(1);
    if (b[0x22] != 0) {
        if (a[0x52] == 0xFF) {
            u8 *p = D_80158AA8 + 0x9D8;

            *(s16 *)(D_80158AA8 + 0x9E2) = -0x24;
            *(s16 *)(p + 0xC) = 0x74;
            *(s16 *)(p + 0xE) = 9;
            p[2] = 2;
            p[0] = 1;
            *(s16 *)(p + 6) = 0;
            *(s16 *)(p + 0x10) = 0;
        }
        switch (a[0x52]) {
        case 0xFF:
        default:
            *(u8 **)(a + 0x4C) = D_80150A50;
            *(s16 *)(c + 8) = 0;
            c[0xB] = 0;
            c[0xC] = 0;
            c[0xD] = 0;
            break;
        case 1:
            *(u8 **)(a + 0x4C) = D_80152E20;
            *(s16 *)(c + 8) = 0;
            c[0xB] = 0;
            c[0xC] = 2;
            c[0xD] = 0;
            break;
        case 2:
            *(u8 **)(a + 0x4C) = D_80151AF0;
            *(s16 *)(c + 8) = 0xC3E;
            c[0xB] = 1;
            c[0xC] = 0;
            c[0xD] = 0;
            break;
        case 3:
            *(u8 **)(a + 0x4C) = D_80154590;
            *(s16 *)(c + 8) = 0x131F;
            c[0xB] = 2;
            c[0xC] = 0;
            c[0xD] = 0;
            break;
        case 4:
            *(u8 **)(a + 0x4C) = D_8014ED60;
            *(s16 *)(c + 8) = 0x235D;
            c[0xB] = 3;
            c[0xC] = 2;
            c[0xD] = 0;
            break;
        }
    } else {
        *(s16 *)(c + 8) = 0;
        c[0xB] = 0;
        c[0xC] = 2;
        c[0xD] = 0;
    }
}

void func_80136C64(void) {
    u8 *a;
    u8 *b;
    u8 *c;
    s32 *p;
    u8 *q;
    u8 *r;
    u8 *d;
    s32 i;
    u8 *e;
    s32 pad[2]; /* FAKE: the original frame has two more home slots (sp+0x38, sp+0x34) above t. T-11840 */
    s32 t;

    a = D_80158A60;
    b = D_80158A64;
    c = D_80158A6C;
    p = D_80158A70;
    q = D_80158AA8;
    d = D_80158A74;
    *p += *(s32 *)c;
    if (c) {} /* FAKE: an extra use of c raises its priority over b, so c takes v1 and b a2 as in the original; no code. T-11840 */
    switch (*(u16 *)(b + 2)) {
    case 0:
        if (b[0x22] != 0) {
            switch (a[0x52]) {
            case 1:
            case 2:
            case 3:
            case 4:
                t = func_800460DC();
                if (((t - func_801430C4() + 0x100) & 0xFF) < 5) {
                    return;
                }
                func_80133970();
                func_80044750(0x21);
                func_80044750(0x22);
                func_80044750(0x2F);
                func_80044750(4);
                func_80044750(0x201);
                break;
            default:
                func_80044750(0xB1);
                break;
            }
        } else {
            func_80044750(0x21);
            func_80044750(0x22);
            func_80044750(0x2F);
            func_80044750(4);
            func_80044750(0x201);
        }
        func_800AE080(c, 0x20);
        func_800AE080((u8 *)p, 0x20);
        *(s16 *)(a + 0x50) = 0;
        func_8014CBD0();
        func_80148210();
        func_80143570();
        func_8014A7E0();
        func_80146BE0();
        func_80148DB0();
        func_80147CE0();
        func_8014AF3C();
        func_80132F4C();
        func_80133B70();
        func_8013BD4C();
        func_801473BC();
        func_801426F0();
        if (b[0x22] != 0) {
            b[0x11] = 0;
        } else {
            b[0x11] = 1;
        }
        func_80147D08();
        func_8014A814();
        func_80148DE4();
        func_801436BC();
        func_8014AF70();
        func_80132F80();
        b[0x12] = 1;
        *(u8 **)(b + 0x18) = D_80156494;
        b[0x1C] = 2;
        func_80136AD8();
        func_801369F4();
        *(s16 *)(q + 0xA) = -0x70;
        *(s16 *)(q + 0xC) = 0x10;
        *(s16 *)(q + 0xE) = 0xD;
        q[2] = 1;
        q[0] = 1;
        *(s16 *)(q + 6) = 0;
        *(s16 *)(q + 0x10) = 0;
        q += 0x14;
        *(s16 *)(q + 0xA) = -0x48;
        *(s16 *)(q + 0xC) = 0x10;
        *(s16 *)(q + 0xE) = 0x27;
        q[2] = 1;
        q[0] = 1;
        *(s16 *)(q + 6) = 0;
        *(s16 *)(q + 0x10) = 0;
        q += 0x14;
        func_80148584(*(u16 *)(c + 8), 0x27);
        r = D_80158AA4;
        for (i = 0; i < 2; i++, r += 0x64) {
            r[0] = 1;
            r[1] = 0;
            *(s16 *)(r + 0x44) = 0;
            *(s16 *)(r + 0x16) = 0x18;
            *(s16 *)(r + 0x3C) = 0x10;
            *(s16 *)(r + 0x3E) = 0x10;
            r[0x46] = 0;
            r[0x47] = 0x70;
            *(s16 *)(r + 0x40) = 0xA;
            *(s16 *)(r + 0x42) = 0x3C0B;
            r[0x3B] = 0x65;
            *(s16 *)(r + 0x10) = 0;
            *(s16 *)(r + 0x14) = i * 0x10 - 0x70;
        }
        func_80144CA8();
        for (i = 0; i < 6; i++, r += 0x64) {
            r[0] = 1;
            r[1] = 0;
            *(s16 *)(r + 0x44) = 0;
            *(s16 *)(r + 0x14) = -0x78;
            *(s16 *)(r + 0x16) = 0xD8 - i * 8;
            *(s16 *)(r + 0x3E) = 8;
            *(s16 *)(r + 0x3C) = 0x10;
            r[0x46] = (i / 2) * 0x10 + 0x50;
            r[0x47] = (i & 1) * 8 + 0x30;
            *(s16 *)(r + 0x40) = 0xA;
            *(s16 *)(r + 0x42) = 0x3C09;
            r[0x3B] = 0x65;
            *(s16 *)(r + 0x10) = 0;
        }
        d[0x59] = 0;
        d[0x58] = 0;
        *(s16 *)(b + 2) = 0x40;
        break;
    case 1:
        func_80133A60();
        func_80044750(0x21);
        func_80044750(0x22);
        func_80044750(0x2F);
        func_80044750(4);
        func_80044750(0x201);
        func_8014A7E0();
        func_80146BE0();
        func_80148DB0();
        func_8014AF3C();
        func_80132F4C();
        func_80133B70();
        func_8013BD4C();
        func_801473BC();
        func_801426F0();
        b[0x11] = 1;
        d[0x57] = 0;
        /* Chained stores as in the original: the 2-chain costs one ugen temporary (t-register numbering below), the longer chains leave uopt ranges for the reloaded value in v1/a0, so d takes a1. T-11840 */
        d[0x58] = d[0x59] = 0;
        d[0x5A] = d[0x5B] = d[0x5C] = d[0x5D] = d[0x5E] = d[0x5F] = 0;
        *(s16 *)(d + 4) = 0x20;
        d[0xD] = d[0xE] = d[0xF] = 0;
        *(s16 *)(d + 6) = 0;
        *(s16 *)(d + 8) = 0;
        *(s16 *)(d + 0xA) = 0;
        func_8014A814();
        b[0x12] = 1;
        b[0x1C] = 2;
        func_801369F4();
        *(s16 *)(b + 2) = 0x40;
        break;
    case 0x40:
        func_80148584(*(u16 *)(c + 8), 0x27);
        if (*(u16 *)((u8 *)p + 2) > 0x400) {
            e = D_80158A8C;
            *(s16 *)(e + 4) = 0;
            e[0xC] = 0;
            *(s16 *)(e + 0xA) = 0;
            *(s16 *)(e + 8) = 0;
            *(s16 *)(e + 6) = 0;
            e[0xF] = 0;
            e[0xE] = 0;
            e[0xD] = 0;
            *(u16 *)(b + 2) = 0x50;
        }
        break;
    case 0x50:
        func_80148584(*(u16 *)(c + 8), 0x27);
        ((void (*)(void))p[3])();
        break;
    case 0xA0:
        func_80148584(*(u16 *)(c + 8), 0x27);
        *(s16 *)(d + 4) = 0xA0;
        break;
    }
    if (b[0x22] != 0 && a[0x52] == 0xFF) {
        q = D_80158AA8 + 0x9D8;
        if ((*(u16 *)(a + 8) >> 4) & 1) {
            *(u16 *)(q + 6) = 1;
        } else {
            *(s16 *)(q + 6) = 0;
        }
        if ((*(u16 *)(a + 0x14) & 0x800) || (*(u16 *)(a + 0x14) & 0x20)) {
            *(u16 *)b = 1;
            *(s16 *)(b + 6) = 0;
            *(s16 *)(b + 4) = 0;
            *(s16 *)(b + 2) = 0;
            b[0xB] = 0;
            b[0xA] = 0;
            b[9] = 0;
        }
    }
    func_801480BC();
    func_801441BC();
    func_8014AB48();
    func_80147074();
    func_80149108();
    func_8013C0A8();
    func_801475C8();
    func_8014C680();
    func_801333D4();
    func_80134BBC();
    func_8013D150();
    func_80136830();
    func_801455C8();
}
