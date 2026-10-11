#include "common.h"
#include "ovl/TT.h"

void func_80133970(void) {
    s32 i;
    s32 j;

    func_80044750(0x7F);
    func_800451E0(0x80162000);
    func_80044750(0x11);
    func_80044750(0x22);
    func_80044750(0x21);
    func_80044750(0x2F);
    i = 0;
    do {
        func_80079B34();
        func_800AD950(0);
    } while (i++ < 0x10000 && func_80044F94(0) == 0);
    func_80044750(0x7F);
    func_800451E0(0x8019A000);
    func_80044750(0x12);
    func_80044750(0x23);
    func_80044750(0x24);
    func_80044750(0x2E);
    j = 0;
    do {
        func_80079B34();
        func_800AD950(0);
    } while (j++ < 0x10000 && func_80044F94(1) == 0);
}

void func_80133A60(void) {
    s32 i;

    func_80079B34();
    func_800451E0(0x8019A000);
    func_80044750(0x12);
    i = 0;
    do {
        func_80079B34();
        func_800AD950(0);
    } while (i++ < 0x10000 && func_80044F94(1) == 0);
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_80133AD0);

void func_80133B3C(void) {
    u8 *p = D_80158A60;
    s32 i;

    for (i = 0; i < 0x40; i++) {
        p[0x56 + i] = 0;
    }
}

void func_80133B70(void) {
    s32 i;
    u8 *p;

    i = 0;
    p = D_80158AA0;
    for (; i < 0x80; i++) {
        *p = 0;
        p += 0x68;
    }
}

u8 *func_80133BA4(u8 *arg0, u8 *arg1) {
    while (arg0 < arg1) {
        if (*arg0 == 0) {
            return arg0;
        }
        arg0 += 0x68;
    }
    return 0;
}

void func_80133BE0(u8 *arg0, u8 *arg1) {
    s32 dx = *(s32 *)(arg1 + 0x20) - *(s32 *)(arg0 + 0x20);
    s32 dy = *(s32 *)(arg1 + 0x24) - *(s32 *)(arg0 + 0x24);

    func_8014D260(dy >> 16, dx >> 16);
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_80133C1C);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_80133D14);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_80133E30);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_80133FB0);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_80134178);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_801342CC);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_80134470);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_80134630);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_80134768);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_80134874);

void func_80134924(u8 *arg0) {
    s16 x;
    s16 y;

    *(s32 *)(arg0 + 0x20) += *(s32 *)(arg0 + 0x28);
    *(s32 *)(arg0 + 0x24) += *(s32 *)(arg0 + 0x2C);
    *(s16 *)(arg0 + 0x14) = *(s16 *)(arg0 + 0x22);
    *(s16 *)(arg0 + 0x16) = *(s16 *)(arg0 + 0x26);
    x = *(s16 *)(arg0 + 0x16);
    if (x >= 0x101 || x < -0x10 || (y = *(s16 *)(arg0 + 0x14)) < -0x88 || y >= 0x89) {
        *(s16 *)(arg0 + 4) = 0;
        *(s16 *)(arg0 + 0xA) = 0;
        *(s16 *)(arg0 + 8) = 0;
        *(s16 *)(arg0 + 6) = 0;
        arg0[0xF] = 0;
        arg0[0xE] = 0;
        arg0[0xD] = 0;
        arg0[0] = 0;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_801349B0);

void func_80134ADC(TtActor68 *p) {
    TtSceneView *scene;
    u16 state;

    scene = (TtSceneView *)D_80158A8C;
    if (p->unk_58 == 15) {
        p->unk_62 = 0;
        p->unk_04 = 0x40;
        p->unk_0A = 0;
        p->unk_08 = 0;
        p->unk_06 = 0;
        p->unk_0F = 0;
        p->unk_0E = 0;
        p->unk_0D = 0;
    }
    state = p->unk_04;
    switch (state) {
    case 0x20:
        /* FAKE: empty comparison reserves a2 for the shared 0x20 constant
         * while state stays in v1; real source unknown. T-9760 */
        if ((!p->unk_04) && (!p->unk_04)) {}
        p->unk_20 += (scene->unk_14 - p->unk_60) << 16;
        p->unk_60 = scene->unk_14;
        break;
    case 0x40:
        p->unk_62 += 4;
        if (p->unk_62 >= 0x20) {
            p->unk_62 = 0x20;
            p->unk_2C = 0x40000;
            p->unk_04 = 0x50;
            p->unk_0A = 0;
            p->unk_08 = 0;
            p->unk_06 = 0;
            p->unk_0F = 0;
            p->unk_0E = 0;
            p->unk_0D = 0;
        }
        break;
    }
    p->unk_58++;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_80134BBC);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_80134D0C);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_80134E20);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_80135148);
