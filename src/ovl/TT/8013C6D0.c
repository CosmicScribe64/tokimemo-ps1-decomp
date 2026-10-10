#include "common.h"
#include "ovl/TT.h"

void func_8013C6D0(void) {
    u8 *q = D_80158A6C;
    u8 *p = D_80158AA4;

    p[0] = 1;
    p[1] = 0;
    *(s16 *)(p + 0x44) = 0;
    *(s16 *)(p + 0x14) = -0x60;
    *(s16 *)(p + 0x16) = q[0xA] * 16 + 0x60;
    *(s16 *)(p + 0x3C) = 8;
    *(s16 *)(p + 0x3E) = 8;
    p[0x46] = 0;
    p[0x47] = 0x18;
    *(s16 *)(p + 0x40) = 0xA;
    *(s16 *)(p + 0x42) = 0x3C0F;
    p[0x3B] = 0x65;
    *(s16 *)(p + 0x10) = 0;
}

void func_8013C740(void) {
    *(s16 *)(D_80158AA4 + 0x16) = D_80158A6C[0xA] * 16 + 0x60;
}

void func_8013C764(u16 arg0, u16 arg1) {
    u8 *p = D_80158A60;
    u8 v = 0;

    if (arg0 != 0) {
        if (arg0 & 0x20) {
            v = 5;
        } else if (arg0 & 0x40) {
            v = 6;
        } else if (arg0 & 0x10) {
            v = 4;
        } else if (arg0 & 0x80) {
            v = 7;
        } else if (arg0 & 4) {
            v = 2;
        } else if (arg0 & 8) {
            v = 3;
        } else if (!(arg0 & 1) && (arg0 & 2)) {
            v = 1;
        }
        if (arg1 == 0) {
            p[0x44] = v;
        } else {
            p[0x45] = v;
        }
    }
}

u16 func_8013C818(u8 arg0, s8 *arg1) {
    u16 ret;

    switch (arg0) {
    case 5:
        ret = 0x1E;
        *arg1 = 0;
        break;
    case 6:
        ret = 0x1F;
        *arg1 = 0;
        break;
    case 4:
        ret = 0x20;
        *arg1 = 0;
        break;
    case 7:
        ret = 0x21;
        *arg1 = 0;
        break;
    case 2:
        ret = 0x22;
        *arg1 = 0xF;
        break;
    case 3:
        ret = 0x23;
        *arg1 = 0xF;
        break;
    case 0:
        ret = 0x24;
        *arg1 = 0xF;
        break;
    case 1:
        ret = 0x25;
        *arg1 = 0xF;
        break;
    }
    return ret;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013C6D0", func_8013C8DC);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013C6D0", func_8013CA0C);

void func_8013CB58(void) {
    u8 *p = D_80158A64;

    switch (*(u16 *)(p + 4)) {
    case 0x0:
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
        func_8013C8DC();
        *(u16 *)(p + 4) = 0x40;
        break;
    case 0x40:
        func_8013CA0C();
        break;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013C6D0", func_8013CC24);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013C6D0", func_8013CDC0);

void func_8013D060(void) {
    u8 *p = D_80158A64;

    switch (*(u16 *)(p + 2)) {
    case 0x0:
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
        p[0x11] = 0;
        p[0x1C] = 0;
        p[0x13] = 0;
        p[0x14] = 0;
        p[0x15] = 0;
        func_8013CC24();
        *(u16 *)(p + 2) = 0x40;
        break;
    case 0x40:
        func_8013CDC0();
        break;
    case 0x50:
        func_8013CB58();
        break;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013C6D0", func_8013D150);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013C6D0", func_8013D2E0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013C6D0", func_8013D3C0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013C6D0", func_8013D508);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013C6D0", func_8013D73C);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013C6D0", func_8013D988);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013C6D0", func_8013DE10);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013C6D0", func_8013E1E8);
