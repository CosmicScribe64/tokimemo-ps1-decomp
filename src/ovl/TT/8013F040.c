#include "common.h"
#include "ovl/TT.h"

typedef struct TtAnimRecord14 {
    /* 0x00 */ u8 pad[0x12];
    /* 0x12 */ u8 frame;
    /* 0x13 */ u8 pad13;
} TtAnimRecord14; /* size 0x14 */

typedef struct TtAnimScene366 {
    /* 0x000 */ u8 pad[0x40];
    /* 0x040 */ TtAnimRecord14 frames[10];
    /* 0x108 */ u8 pad108[0x25C];
    /* 0x364 */ u16 timer;
} TtAnimScene366; /* accessed prefix size 0x366; T-10030 */

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

void func_8013F878(void) {
    TtAnimScene366 *scene;

    scene = (TtAnimScene366 *)D_80158A8C;
    scene->frames[0].frame = ((scene->timer >> 2) & 1) + 14;
    scene->frames[1].frame = ((scene->timer >> 2) & 1) + 14;
    if ((scene->timer & 7) == 7) {
        scene->frames[2].frame++;
        if (scene->frames[2].frame >= 52U) {
            scene->frames[2].frame = 46;
        }
        scene->frames[3].frame = scene->frames[2].frame;
        scene->frames[4].frame++;
        if (scene->frames[4].frame >= 46U) {
            scene->frames[4].frame = 40;
        }
        scene->frames[5].frame = scene->frames[4].frame;
        scene->frames[6].frame++;
        if (scene->frames[6].frame >= 40U) {
            scene->frames[6].frame = 34;
        }
        scene->frames[7].frame = scene->frames[6].frame;
    }
    if ((scene->timer & 15) == 15) {
        if (scene->frames[8].frame & 1) {
            scene->frames[8].frame--;
        } else {
            scene->frames[8].frame++;
        }
    }
    scene->frames[9].frame = (scene->timer >> 4) & 1;
    scene->timer++;
}

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

/* FAKE: the one/period locals and declaration order reproduce the original
 * constant allocation and stack layout; original source unknown. T-9520. */
void func_8013FF14(void) {
    u16 mode;
    u8 *other;
    s32 one;
    s32 angle;
    u32 period;
    TtSceneView *p;
    TtPoint14 point;

    one = 1;
    period = 384;
    mode = ((TtSceneView *) D_80158A8C)->unk_08;
    other = D_80158A74;
    p = (TtSceneView *) D_80158A8C;
    switch (mode) {
    case 0:
        p->unk_366 = 0;
        p->unk_08 = 0x40;
        break;
    case 0x40:
        if ((u16) p->unk_366 % period == 192U) {
            func_80134470(p->unk_14, p->unk_16 + 8, 0x80, 0x30, 0xAA);
        }
        if ((p->unk_366 & 7) == 7) {
            point.x = p->unk_14 << 16;
            point.y = (p->unk_16 + 8) << 16;
            angle = func_801320F0((s32) &point, (s32) (other + 0x14));
            func_80133E30(p->unk_14, p->unk_16 + 8, 0x18, p->unk_3A6 * p->unk_3A4 + angle, 0xA0);
            p->unk_3A4 += 0x40;
            if ((u16) p->unk_3A4 >= 0x301U) {
                p->unk_3A4 = 0;
            }
            if ((u16) p->unk_3A6 == one) {
                p->unk_3A6 = -one;
            } else {
                p->unk_3A6 = one;
            }
        }
        if ((u16) p->unk_366 >= period) {
            p->unk_08 = 0xA0;
        }
        break;
    case 0xA0:
        p->unk_06 = one;
        p->unk_08 = 0;
        break;
    }
    p->unk_366++;
}

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
