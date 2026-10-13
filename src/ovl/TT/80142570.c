#include "common.h"
#include "ovl/TT.h"

/* NON_MATCHING: T-11000, register-only (~60 lines, identical instruction stream incl. 4x unroll): original skips v1/a0, i in a1, 0x40/0x70/0x20 in a2/a3/t0; IDO uses v1, a0, a1, a2 (phantom a0, T-0018) */
#ifdef NON_MATCHING
void func_80142570(void) {
    u8 *p;
    s32 i;

    func_80133AD0(0x60C);
    p = D_80158A98;
    i = 0;
    do {
        i++;
        if (*p != 0 && *(u16 *)(p + 4) == 0x40) {
            *(u16 *)(p + 4) = 0x70;
            *(s16 *)(p + 0xA) = 0;
            *(s16 *)(p + 8) = 0;
            *(s16 *)(p + 6) = 0;
            p[0xF] = 0;
            p[0xE] = 0;
            p[0xD] = 0;
        }
        p += 0x78;
    } while (i < 0x40);
    p = D_80158A9C;
    for (i = 0; i < 0x20; i++) {
        if (*p != 0 && *(u16 *)(p + 4) == 0x40) {
            *(u16 *)(p + 4) = 0x70;
            *(s16 *)(p + 0xA) = 0;
            *(s16 *)(p + 8) = 0;
            *(s16 *)(p + 6) = 0;
            p[0xF] = 0;
            p[0xE] = 0;
            p[0xD] = 0;
        }
        p += 0x78;
    }
}
#else
INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142570", func_80142570);
#endif

void func_801426F0(void) {
    s32 i;
    u8 *p = D_80158A8C;
    u8 *r = D_80158AA4 + 0xC80;

    p[0] = 0;
    r[0xC8] = 0;
    r[0x64] = 0;
    r[0] = 0;
    r += 0x12C;
    for (i = 3; i < 0x1F; i++) {
        *r = 0;
        r += 0x64;
    }
}

void func_80142740(void) {
    u8 *p = D_80158A8C;
    s32 i;

    for (i = 0; i < 0x18; i++) {
        p[0x310 + i] = 0;
    }
}

void func_80142774(void) {
    u8 *p = D_80158A8C;
    s32 i;

    for (i = 0; i < 12; i++) {
        *(s32 *)(p + 0x218 + i * 4) = 0;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142570", func_801427A8);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142570", func_80142AF0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142570", func_80142C70);
