#include "common.h"
#include "ovl/TT.h"

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80136830", func_80136830);

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

/* T-11000 near miss: 0xFF compare constant is in $t0 in the original, $at here; all later constants shift one register (about 40 lines). */
#ifdef NON_MATCHING
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
#else
INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80136830", func_80136AD8);
#endif

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80136830", func_80136C64);
