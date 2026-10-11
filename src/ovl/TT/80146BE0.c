#include "common.h"
#include "ovl/TT.h"

void func_80146BE0(void) {
    s32 i;
    u8 *p;

    i = 0;
    p = D_80158A84;
    for (; i < 8; i++) {
        *p = 0;
        p += 0x58;
    }
}

void func_80146C14(void) {
    s32 i;
    u8 *p;

    i = 0;
    p = D_80158A84;
    for (; i < 8; i++) {
        *(s16 *)(p + 0x10) = 3;
        *(s16 *)(p + 0x40) = 0xA;
        *(s16 *)(p + 0x42) = 0x3C81;
        p += 0x58;
    }
}

void func_80146C74(u8 *arg0, s32 arg1) {
    u8 *p = D_80158A74;

    if (arg1 < 2) {
        if (arg0[0x3D] != 0) {
            *(s16 *)(arg0 + 0x14) = *(s16 *)(p + 0x14) + 6;
        } else {
            *(s16 *)(arg0 + 0x14) = *(s16 *)(p + 0x14) - 6;
        }
        *(s32 *)(arg0 + 0x20) = *(s16 *)(arg0 + 0x14) << 16;
        *(s16 *)(arg0 + 0x16) = *(s16 *)(p + 0x16) - 0x10;
        *(s32 *)(arg0 + 0x24) = *(s16 *)(arg0 + 0x16) << 16;
        *(s32 *)(arg0 + 0x2C) = 0xFFFC0000;
        arg0[0x44] = 0xA0;
        arg0[0x45] = 0xD0;
        *(s16 *)(arg0 + 0x10) = 3;
        *(s16 *)(arg0 + 0x40) = 0xA;
        *(s16 *)(arg0 + 0x42) = 0x3C81;
        *(s16 *)(arg0 + 0x46) = 0;
        return;
    }
    p = D_80158A78 + (arg1 >> 1) * 0xA4 - 0xA4;
    if (arg0[0x3D] != 0) {
        *(s16 *)(arg0 + 0x14) = *(s16 *)(p + 0x14) + 6;
    } else {
        *(s16 *)(arg0 + 0x14) = *(s16 *)(p + 0x14) - 6;
    }
    *(s32 *)(arg0 + 0x20) = *(s16 *)(arg0 + 0x14) << 16;
    *(s16 *)(arg0 + 0x16) = *(s16 *)(p + 0x16) - 0x10;
    *(s32 *)(arg0 + 0x24) = *(s16 *)(arg0 + 0x16) << 16;
    *(s32 *)(arg0 + 0x2C) = 0xFFFC0000;
    arg0[0x44] = 0xA0;
    arg0[0x45] = 0xD0;
    *(s16 *)(arg0 + 0x10) = 3;
    *(s16 *)(arg0 + 0x40) = 0xA;
    *(s16 *)(arg0 + 0x42) = 0x3C81;
    *(s16 *)(arg0 + 0x46) = 0;
}

void func_80146DC8(u8 *arg0) {
    u32 v = (u16)++*(u16 *)(arg0 + 0x46);

    if (v >= 0x15) {
        arg0[0x44] = 0xC0;
    } else if (v >= 0xB) {
        arg0[0x44] = 0xB0;
    } else {
        arg0[0x44] = 0xA0;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80146BE0", func_80146E10);

void func_80147074(void) {
    u32 limit;
    s32 i;
    u16 mode;
    TtState58 *p;

    limit = 8;
    p = (TtState58 *)D_80158A84;
    i = 0;
    do {
        if (p->unk_00 != 0 && p->unk_01 == 0) {
            mode = p->unk_04;
            switch (mode) {
            case 0x20:
                func_80146C74((u8 *)p, i);
                p->unk_04 = 0x40U;
                p->unk_0A = 0;
                p->unk_08 = 0;
                p->unk_06 = 0;
                p->unk_0F = 0;
                p->unk_0E = 0;
                p->unk_0D = 0;
                break;
            case 0x40:
                func_80146DC8((u8 *)p);
                break;
            }
            func_80146E10((u8 *)p, i);
        }
        i++;
        p++;
    /* FAKE: narrowing the constant limit keeps 8 in s3 after 0x40. T-9520. */
    } while (i != (limit & 0xFF));
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80146BE0", func_80147150);
