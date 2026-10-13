#define MAIN_API_OVERRIDE_func_80044750 /* unprototyped call preserves u16 argument conversion */
#include "common.h"
#include "ovl/TT.h"

s32 func_80044750();

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

void func_80133AD0(arg0)
u16 arg0;
{
    TtState96 *data;
    u16 index;
    data = (TtState96 *)D_80158A60;
    index = arg0 & 0x1F;
    index |= (arg0 & 0x100) >> 3;
    if (data->unk_56[index] == 0) {
        func_80044750(arg0);
        data->unk_56[index] = 1;
    }
}


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

/* The caller consumes the angle left in v0 by func_8014D260.
 * Keep the fallthrough return: an explicit return through its void API
 * is unavailable, and this definition matches the original helper. T-10020. */
s32 func_80133BE0(u8 *arg0, u8 *arg1) {
    s32 dx = *(s32 *)(arg1 + 0x20) - *(s32 *)(arg0 + 0x20);
    s32 dy = *(s32 *)(arg1 + 0x24) - *(s32 *)(arg0 + 0x24);

    func_8014D260(dy >> 16, dx >> 16);
}

void func_80133C1C(o, kind, idx)
    TtActor68 *o;
    u16 kind;
    s32 idx;
{
    o->unk_00 = 1;
    o->unk_02 = kind;
    o->unk_10 = 1;
    o->unk_38 = (void *)(&D_8014E34C)[idx];
    o->unk_40 = 0x80;
    switch (idx) {
    case 18:
    case 19:
        *(s16 *)((u8 *)o + 0x52) = 0xA;
        *(s16 *)((u8 *)o + 0x54) = 0x3C8B;
        o->unk_4C = (u8 *)&D_80155B30[3];
        return;
    case 20:
        *(s16 *)((u8 *)o + 0x52) = 0xA;
        *(s16 *)((u8 *)o + 0x54) = 0x3C8C;
        o->unk_4C = (u8 *)&D_80155B30[3];
        return;
    case 21:
        *(s16 *)((u8 *)o + 0x52) = 0x12;
        *(s16 *)((u8 *)o + 0x54) = 0x3CD3;
        o->unk_4C = (u8 *)&D_80155B30[3];
        return;
    case 22:
        *(s16 *)((u8 *)o + 0x52) = 0x12;
        *(s16 *)((u8 *)o + 0x54) = 0x3CD4;
        o->unk_4C = (u8 *)&D_80155B30[2];
        return;
    case 23:
        *(s16 *)((u8 *)o + 0x52) = 0x12;
        *(s16 *)((u8 *)o + 0x54) = 0x3CD5;
        o->unk_4C = (u8 *)&D_80155B30[2];
    }
}

void func_80133D14(s16 x, s16 y, s32 speed, u16 kind) {
    TtActor68 *object;
    u8 *source;
    s32 angle;

    source = D_80158A74;
    object = (TtActor68 *)func_80133BA4(D_80158AA0, D_80158AA0 + 0x3400);
    if (object != 0) {
        object->unk_20 = x << 16;
        object->unk_24 = y << 16;
        angle = func_80133BE0((u8 *)object, source);
        object->unk_28 = func_800A0140(angle) * speed;
        object->unk_2C = func_800A0070(angle) * speed;
        switch (kind) {
        case 0xA0:
            func_80133C1C(object, 0xA0, 0x12);
            break;
        case 0xA8:
            func_80133C1C(object, 0xA8, 0x15);
            break;
        }
        object->unk_04 = 0x40;
        object->unk_0A = 0;
        object->unk_08 = 0;
        object->unk_06 = 0;
        object->unk_0F = 0;
        object->unk_0E = 0;
        object->unk_0D = 0;
    }
}

void func_80133E30(s16 x, s16 y, s32 speed, s32 angle, u16 kind) {
    TtActor68 *object;

    object = (TtActor68 *)func_80133BA4(D_80158AA0, D_80158AA0 + 0x3400);
    if (object != 0) {
        object->unk_20 = x << 16;
        object->unk_24 = y << 16;
        object->unk_28 = func_800A0140(angle) * speed;
        object->unk_2C = func_800A0070(angle) * speed;
        switch (kind) {
        case 0xA0:
            func_80133C1C(object, 0xA0, 0x12);
            break;
        case 0xA1:
            func_80133C1C(object, 0xA1, 0x13);
            break;
        case 0xA8:
            func_80133C1C(object, 0xA8, 0x15);
            break;
        case 0xA9:
            func_80133C1C(object, 0xA9, 0x16);
            break;
        case 0xAC:
            object->unk_60 = angle + 0x800;
            object->unk_58 = 0;
            func_80133C1C(object, 0xAC, 0x14);
            object->unk_4C = (u8 *)&D_80155B30[2];
            break;
        }
        object->unk_04 = 0x40;
        object->unk_0A = 0;
        object->unk_08 = 0;
        object->unk_06 = 0;
        object->unk_0F = 0;
        object->unk_0E = 0;
        object->unk_0D = 0;
    }
}

/* ANSI view of the K&R definition above: later callers convert the kind argument at the call (andi), T-11000 */
void func_80133C1C(TtActor68 *, u16, s32);

void func_80133FB0(s16 x, s16 y, s32 spread, s32 speed, u16 kind) {
    TtActor68 *object;
    u8 *source;
    s32 angle;
    s32 i;

    object = (TtActor68 *)D_80158AA0;
    source = D_80158A74;
    for (i = 0; i < 3; i++) {
        object = (TtActor68 *)func_80133BA4((u8 *)object, (u8 *)object + 0x3400);
        if (object == 0) {
            break;
        }
        object->unk_20 = x << 16;
        object->unk_24 = y << 16;
        angle = func_80133BE0((u8 *)object, source);
        switch (i) {
        case 1:
            angle = angle - spread;
            break;
        case 2:
            angle = angle + spread;
            break;
        }
        object->unk_28 = func_800A0140(angle) * speed;
        object->unk_2C = func_800A0070(angle) * speed;
        switch (kind) {
        case 0xA0:
            func_80133C1C(object, 0xA0, 0x12);
            break;
        case 0xA2:
            func_80133C1C(object, 0xA2, 0x14);
            break;
        case 0xA8:
            func_80133C1C(object, 0xA8, 0x15);
            break;
        case 0xAA:
            func_80133C1C(object, 0xAA, 0x17);
            break;
        }
        object->unk_04 = 0x40;
        object->unk_0A = 0;
        object->unk_08 = 0;
        object->unk_06 = 0;
        object->unk_0F = 0;
        object->unk_0E = 0;
        object->unk_0D = 0;
    }
}

void func_80134178(s16 x, s16 y, s32 spread, s32 speed, u16 kind) {
    TtActor68 *object;
    s32 angle;
    s32 i;

    object = (TtActor68 *)D_80158AA0;
    for (i = 0; i < 3; i++) {
        object = (TtActor68 *)func_80133BA4((u8 *)object, (u8 *)object + 0x3400);
        if (object == 0) {
            break;
        }
        object->unk_20 = x << 16;
        object->unk_24 = y << 16;
        angle = 0x400;
        switch (i) {
        case 1:
            angle = 0x400 - spread;
            break;
        case 2:
            angle = spread + 0x400;
            break;
        }
        object->unk_28 = func_800A0140(angle) * speed;
        object->unk_2C = func_800A0070(angle) * speed;
        switch (kind) {
        case 0xA8:
            func_80133C1C(object, 0xA8, 0x15);
            break;
        }
        object->unk_04 = 0x40;
        object->unk_0A = 0;
        object->unk_08 = 0;
        object->unk_06 = 0;
        object->unk_0F = 0;
        object->unk_0E = 0;
        object->unk_0D = 0;
    }
}

void func_801342CC(s16 x, s16 y, s32 spread, s32 speed, u16 kind) {
    TtActor68 *object;
    u8 *source;
    s32 half;
    s32 angle;
    s32 i;

    object = (TtActor68 *)D_80158AA0;
    i = 0;
    source = D_80158A74;
    for (;;) {
        object = (TtActor68 *)func_80133BA4((u8 *)object, (u8 *)object + 0x3400);
        if (object == 0) {
            break;
        }
        object->unk_20 = x << 16;
        object->unk_24 = y << 16;
        half = spread / 2;
        angle = func_80133BE0((u8 *)object, source);
        if (i & 1) {
            angle = (angle + half) - ((i / 2 + 1) * spread);
        } else {
            angle = angle + half + (i / 2) * spread;
        }
        object->unk_28 = func_800A0140(angle) * speed;
        object->unk_2C = func_800A0070(angle) * speed;
        if (kind == 0xA0) {
            func_80133C1C(object, 0xA0, 0x12);
        }
        i++;
        object->unk_04 = 0x40;
        object->unk_0A = 0;
        object->unk_08 = 0;
        object->unk_06 = 0;
        object->unk_0F = 0;
        object->unk_0E = 0;
        object->unk_0D = 0;
        if (i == 4) {
            break;
        }
    }
}

/* NON_MATCHING: T-11000, 1 line: addu s1,v0,t6 in the original, addu s1,t6,v0 here (even-index branch); no operand order or += form changes it */
#ifdef NON_MATCHING
void func_80134470(s16 x, s16 y, s32 spread, s32 speed, u16 kind) {
    TtActor68 *object;
    u8 *source;
    s32 angle;
    s32 base;
    s32 i;

    object = (TtActor68 *)D_80158AA0;
    source = D_80158A74;
    for (i = 0; i < 9; i++) {
        object = (TtActor68 *)func_80133BA4((u8 *)object, (u8 *)object + 0x3400);
        if (object == 0) {
            break;
        }
        object->unk_20 = x << 16;
        object->unk_24 = y << 16;
        base = func_80133BE0((u8 *)object, source);
        angle = base;
        if (i != 0) {
            if (i & 1) {
                angle = base - ((i / 2 + 1) * spread);
            } else {
                angle = base + (i / 2) * spread;
            }
        }
        object->unk_28 = func_800A0140(angle) * speed;
        object->unk_2C = func_800A0070(angle) * speed;
        switch (kind) {
        case 0xA0:
            func_80133C1C(object, 0xA0, 0x12);
            break;
        case 0xAA:
            func_80133C1C(object, 0xAA, 0x17);
            break;
        }
        object->unk_04 = 0x40;
        object->unk_0A = 0;
        object->unk_08 = 0;
        object->unk_06 = 0;
        object->unk_0F = 0;
        object->unk_0E = 0;
        object->unk_0D = 0;
    }
}
#else
INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_80134470);
#endif

void func_80134630(s16 x, s16 y, s32 speed, u16 kind) {
    TtActor68 *object;
    s32 i;

    object = (TtActor68 *)D_80158AA0;
    for (i = 0; i < 2; i++) {
        object = (TtActor68 *)func_80133BA4((u8 *)object, (u8 *)object + 0x3400);
        if (object == 0) {
            break;
        }
        object->unk_20 = ((x - i * 0x30) + 0x18) << 16;
        object->unk_24 = y << 16;
        object->unk_28 = 0;
        object->unk_2C = speed << 16;
        switch (kind) {
        case 0xA0:
            func_80133C1C(object, 0xA0, 0x12);
            break;
        case 0xA2:
            func_80133C1C(object, 0xA2, 0x14);
            break;
        }
        object->unk_04 = 0x40;
        object->unk_0A = 0;
        object->unk_08 = 0;
        object->unk_06 = 0;
        object->unk_0F = 0;
        object->unk_0E = 0;
        object->unk_0D = 0;
    }
}

/* NON_MATCHING: T-10510, 30 lines: object in v0 / constants 1 and 0 in a2 and a0 in the original */
#ifdef NON_MATCHING
void func_80134768(s16 x, s16 y, s32 speed, s32 angle) {
    TtActor68 *object;

    object = (TtActor68 *)func_80133BA4(D_80158AA0, D_80158AA0 + 0x3400);
    if (object != 0) {
        object->unk_00 = 1;
        object->unk_02 = 0xAB;
        object->unk_04 = 0x40;
        object->unk_08 = 0;
        object->unk_06 = 0;
        object->unk_0A = 0;
        object->unk_0F = 0;
        object->unk_0E = 0;
        object->unk_0D = 0;
        object->unk_10 = 1;
        object->unk_20 = x << 16;
        object->unk_24 = y << 16;
        object->unk_28 = func_800A0140(angle) * speed;
        object->unk_2C = func_800A0070(angle) * speed;
        object->unk_38 = D_80151A50;
        object->unk_3C = 0;
        object->unk_3D = 9;
        object->unk_3E = 0x14;
        object->unk_40 = 0x60;
        object->unk_41 = 0;
        object->unk_4C = (u8 *)&D_80155B30[5];
    }
}
#else
INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_80134768);
#endif

#ifdef NON_MATCHING
/* NON_MATCHING: T-10720, constant 1 is in v1, original has it in a1 (T-0018 phantom registers, same as func_80134768); 4 reg lines plus sched */
void func_80134874(x, y)
s16 x;
s16 y;
{
    TtActor68 *object;
    u8 *source;

    source = D_80158A8C;
    object = (TtActor68 *)func_80133BA4(D_80158AA0, D_80158AA0 + 0x3400);
    if (object != 0) {
        object->unk_00 = 1;
        object->unk_02 = 0xAD;
        object->unk_04 = 0x20;
        object->unk_0A = 0;
        object->unk_08 = 0;
        object->unk_06 = 0;
        object->unk_0F = 0;
        object->unk_0E = 0;
        object->unk_0D = 0;
        object->unk_10 = 1;
        object->unk_20 = x << 16;
        object->unk_28 = 0;
        object->unk_24 = y << 16;
        object->unk_2C = 0;
        object->unk_58 = 0;
        object->unk_4C = (u8 *)&D_80155B30[5];
        object->unk_60 = *(s16 *)(source + 0x14);
    }
}
#else
INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80133970", func_80134874);
#endif

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
