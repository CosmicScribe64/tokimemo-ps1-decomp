#include "common.h"
#include "ovl/TT.h"

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013F040", func_8013F040);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013F040", func_8013F3B4);

void func_8013F588(void) {
    u8 *p = D_80158A8C;

    *(s32 *)(p + 0x20) = 0;
    *(s32 *)(p + 0x24) += 0x10000;
    *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
    *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
    if (*(s16 *)(p + 0x16) >= 0x40) {
        *(s32 *)(p + 0x24) = 0x400000;
        *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
        *(s16 *)(p + 4) = 0x30;
        *(s16 *)(p + 0xA) = 0;
        *(s16 *)(p + 8) = 0;
        *(s16 *)(p + 6) = 0;
        p[0xF] = 0;
        p[0xE] = 0;
        p[0xD] = 0;
    }
}

void func_8013F5F8(void) {
    u8 *p = D_80158A8C;

    func_80142AF0(D_80155E2C, 0);
    if (p[0x40] & 1) {
        *(u8 **)(p + 0x218) = D_80155D2C;
        *(s16 *)(p + 4) = 0x40;
    }
}

void func_8013F650(void) {
    u8 *p;
    u8 *q;
    s32 d;
    s32 a;

    p = D_80158A8C;
    q = D_80158A74;
    d = *(s32 *)(q + 0x20) - *(s32 *)(p + 0x3E4);
    a = d < 0 ? -d : d;
    if (a < 0x4000) {
        *(s32 *)(p + 0x3E4) = *(s32 *)(q + 0x20);
    } else if (d < 0) {
        *(s32 *)(p + 0x3E4) -= 0x4000;
    } else {
        *(s32 *)(p + 0x3E4) += 0x4000;
    }
    if (*(s32 *)(p + 0x3E4) < -0x400000) {
        *(s32 *)(p + 0x3E4) = -0x400000;
    } else if (*(s32 *)(p + 0x3E4) > 0x400000) {
        *(s32 *)(p + 0x3E4) = 0x400000;
    }
    *(s32 *)(p + 0x20) = *(s32 *)(p + 0x3E4) + (func_800A0070(*(s32 *)(p + 0x3E8)) << 9);
    *(s32 *)(p + 0x3E8) += 8;
    if (*(s32 *)(p + 0x3E8) > 0x1000) {
        *(s32 *)(p + 0x3E8) = 0;
    }
}

void func_8013F748(void) {
    u8 *p = D_80158A8C;
    s32 v = *(s32 *)(p + 0x20);

    if (v < (s32)0xFFE00000) {
        *(s32 *)(p + 0x20) += 0x10000;
        return;
    }
    if (v >= 0x200001) {
        *(s32 *)(p + 0x20) += 0xFFFF0000;
        return;
    }
    *(s16 *)(p + 6) = 2;
}

void func_8013F7B0(void) {
    switch (*(u16 *)(D_80158A8C + 0x30E)) {
    case 0:
        switch (*(u16 *)(D_80158A8C + 6)) {
        case 0:
            func_8013F650();
            return;
        case 1:
            func_8013F748();
            return;
        }
        break;
    case 1:
        switch (*(u16 *)(D_80158A8C + 6)) {
        case 0:
            func_8013F650();
            return;
        case 1:
            func_8013F748();
            return;
        }
        break;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013F040", func_8013F878);

/* FAKE: the loop counter and pointer are declared as parameters so that they get $a0/$a1 like the original; nothing reads the incoming values. T-4050 */
void func_8013F988(s32 i, u8 *p) {
    i = 0x140;
    p = D_80158A8C + 0x140;
    if ((*(u16 *)(D_80158A8C + 0x366) & 7) == 7) {
        do {
            i += 0x14;
            p[0x3E] += 1;
            if (p[0x3E] >= 0x16U) {
                p[0x3E] = 0x10;
            }
            p += 0x14;
        } while (i != 0x17C);
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013F040", func_8013F9E0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013F040", func_8013FBD0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013F040", func_8013FF14);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013F040", func_801400B0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013F040", func_80140624);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013F040", func_80140920);

void func_80140CBC(void) {
    switch (*(u16 *)(D_80158A8C + 0x30E)) {
    case 0:
        switch (*(u16 *)(D_80158A8C + 6)) {
        case 0:
            func_8013FF14();
            return;
        case 2:
            func_801400B0();
            return;
        }
        break;
    case 1:
        switch (*(u16 *)(D_80158A8C + 6)) {
        case 0:
            func_80140624();
            return;
        case 2:
            func_80140920();
            return;
        }
        break;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013F040", func_80140D84);
