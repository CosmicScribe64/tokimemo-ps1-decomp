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

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013F040", func_8013F650);

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

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013F040", func_8013F988);

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
