#include "common.h"
#include "ovl/TT.h"

u8 *func_8013BD10(u8 *arg0, u8 *arg1) {
    while (arg0 < arg1) {
        if (*arg0 == 0) {
            return arg0;
        }
        arg0 += 0x6C;
    }
    return 0;
}

/* i = 0 before p = D_80158A90 gives the original's registers ($v0 counter, $v1 pointer); T-4060 */
void func_8013BD4C(void) {
    s32 i;
    u8 *p;

    i = 0;
    p = D_80158A90;
    for (; i < 0x20; i++) {
        *p = 0;
        p += 0x6C;
    }
}

void func_8013BD80(void) {
    s32 i;
    u8 *p;

    i = 0;
    p = D_80158A90;
    for (; i < 0x20; i++) {
        *p = 0;
        *(s16 *)(p + 0x10) = 6;
        p += 0x6C;
    }
}

void func_8013BDC8(u8 *arg0) {
    arg0[0] = 1;
    *(s16 *)(arg0 + 2) = 0x20;
    *(s16 *)(arg0 + 0x52) = 0xA;
    *(s16 *)(arg0 + 0x54) = 0x3C84;
    *(s32 *)(arg0 + 0x38) = D_8014E34C;
    arg0[0x40] = 0x80;
    *(s16 *)(arg0 + 0x10) = 6;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013BD10", func_8013BE08);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013BD10", func_8013BE9C);

void func_8013BF68(u8 *arg0) {
    *(s32 *)(arg0 + 0x20) = 0;
    *(s32 *)(arg0 + 0x24) = 0x200000;
    *(s32 *)(arg0 + 0x28) = 0;
    *(s32 *)(arg0 + 0x2C) = 0;
    *(s32 *)(arg0 + 0x30) = 0;
    *(s32 *)(arg0 + 0x34) = 0xC00;
    *(s16 *)(arg0 + 0x14) = *(s16 *)(arg0 + 0x22);
    *(s16 *)(arg0 + 0x16) = *(s16 *)(arg0 + 0x26);
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013BD10", func_8013BF9C);

typedef struct {
    /* 0x00 */ u8 flag;
    /* 0x01 */ u8 pad1[0x13];
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ u8 pad18[8];
    /* 0x20 */ s32 unk20; /* 16.16 position */
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
} TtMover; /* partial */

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
} TtMoverHi; /* partial: the integer halves of the 16.16 positions at 0x20 and 0x24 */

void func_8013C048(TtMover *arg0) {
    arg0->unk28 += arg0->unk30;
    arg0->unk2C += arg0->unk34;
    arg0->unk20 += arg0->unk28;
    arg0->unk24 += arg0->unk2C;
    arg0->unk14 = ((TtMoverHi *)arg0)->unk22;
    arg0->unk16 = ((TtMoverHi *)arg0)->unk26;
    if (arg0->unk16 >= 0x101) {
        arg0->flag = 0;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013BD10", func_8013C0A8);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013BD10", func_8013C1FC);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013BD10", func_8013C41C);
