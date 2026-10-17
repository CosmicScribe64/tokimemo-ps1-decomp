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

void func_8013F040(void) {
    u8 *c;
    u8 *a;
    s32 pad[9]; /* FAKE: unused, places pt, box and the spilled q at the original's frame offsets. T-11860 */
    s16 pt[2];
    TtCollision6 box;
    s32 i;
    s32 j;
    u8 *p;
    u8 *q;

    c = D_80158A8C;
    a = D_80158A74;
    if (c[0] == 0) {
        return;
    }
    for (i = 0; i < 0x18; i++) {
        c[i * 0x14 + 0x42] = 0;
    }
    if (c[0x342] == 0 && func_801427A8() != 0) {
        func_80133AD0(0x503);
    }
    for (i = 0x10; i < 0x13; i++) {
        if (c[i + 0x310] != 0) {
            c[i * 0x14 + 0x42] = 0;
        }
    }
    if (c[0x343] == 0) {
        for (i = 0x10, q = c + 0x10; i != 0x13; i++, q++) {
            if (q[0x310] != 0) {
                pt[0] = *(s16 *)(c + 0x14) + ((s16 *)(c + 0x248))[i];
                pt[1] = *(s16 *)(c + 0x16) + ((s16 *)(c + 0x278))[i];
                if (a[0x52] == 0 && a[0] != 0 && func_80145508(a + 0x14, D_80155B30, pt, &D_80155D44) != 0) {
                    func_80144094();
                    *(s16 *)(a + 4) = 0x60;
                    *(s16 *)(a + 10) = 0;
                    *(s16 *)(a + 8) = 0;
                    *(s16 *)(a + 6) = 0;
                    a[15] = 0;
                    a[14] = 0;
                    a[13] = 0;
                }
                for (j = 0, p = D_80158A80; j != 12; j++, p += 0x54) {
                    /* Separate tests number the hoisted constants 12, 0x40, &D_80155B36, 0x80 as in the original. */
                    if (p[0] == 0) {
                        continue;
                    }
                    if (*(u16 *)(p + 4) != 0x40) {
                        continue;
                    }
                    if (func_80145508(p + 0x14, &D_80155B36, pt, &D_80155D44) == 0) {
                        continue;
                    }
                    *(u16 *)(p + 4) = 0x80;
                    *(s16 *)(p + 10) = 0;
                    *(s16 *)(p + 8) = 0;
                    *(s16 *)(p + 6) = 0;
                    p[15] = 0;
                    p[14] = 0;
                    p[13] = 0;
                }
            }
        }
    }
    if (c[0x344] == 0) {
        c[0x20E] = 0;
        if (a[0] != 0) {
            pt[0] = *(s16 *)(c + 0x14) + *(s16 *)(c + 0x276);
            pt[1] = *(s16 *)(c + 0x16) + *(s16 *)(c + 0x2A6);
            box.x = 0;
            box.y = 0;
            box.w = *(s16 *)(c + 0x210) * 0x30 / 4096;
            box.h = *(s16 *)(c + 0x212) * 0x30 / 4096;
            if (a[0x52] == 0 && a[0] != 0 && func_80145508(a + 0x14, D_80155B30, pt, &box) != 0) {
                func_80144094();
                *(s16 *)(a + 4) = 0x60;
                *(s16 *)(a + 10) = 0;
                *(s16 *)(a + 8) = 0;
                *(s16 *)(a + 6) = 0;
                a[15] = 0;
                a[14] = 0;
                a[13] = 0;
            }
        }
    }
}

void func_8013F3B4(void) {
    u8 *p;
    s32 i;

    p = D_80158A8C;
    func_80142740();
    func_80142774();
    p[0] = 1;
    p[0x342] = 0;
    *(s32 *)(p + 0x20) = 0;
    *(s32 *)(p + 0x24) = 0xFFC00000;
    *(s16 *)(p + 0x14) = 0;
    *(s16 *)(p + 0x16) = -0x40;
    *(s32 *)(p + 0x28) = 0;
    *(s32 *)(p + 0x2C) = 0;
    *(s16 *)(p + 0x30C) = 0xE0;
    *(s16 *)(p + 0x30E) = 0;
    for (i = 0; i < 11; i++) {
        ((TtAnim14 *)(p + 0x38))[i].frames = D_80151A50;
        ((TtAnim14 *)(p + 0x38))[i].unk_04 = D_80155E14[i];
        ((TtAnim14 *)(p + 0x38))[i].unk_05 = 0xA;
        ((TtAnim14 *)(p + 0x38))[i].unk_06 = D_80155E20[i];
        ((TtAnim14 *)(p + 0x38))[i].unk_08 = 0x60;
        ((TtAnim14 *)(p + 0x38))[i].unk_0B = 0xF;
        ((TtAnim14 *)(p + 0x38))[i].unk_10 = 0;
        p[i + 0x310] = 1;
        ((s16 *)(p + 0x2A8))[i] = 9;
        ((s16 *)(p + 0x248))[i] = 0;
        ((s16 *)(p + 0x278))[i] = 0;
    }
    *(s32 *)(p + 0x3E4) = *(s32 *)(p + 0x20);
    *(s16 *)(p + 0x2AA) = 8;
    *(s16 *)(p + 0x2AC) = 8;
    *(s16 *)(p + 0x3A4) = 0;
    *(s16 *)(p + 0x3A6) = 0;
    *(s32 *)(p + 0x3E8) = 0;
    *(s32 *)(p + 0x3EC) = 0;
    *(s32 *)(p + 0x3F0) = 0;
    *(s32 *)(p + 0x3F4) = 0;
    *(s32 *)(p + 0x3F8) = 0;
    *(s16 *)(p + 0x364) = 0;
    *(s16 *)(p + 0x366) = 0;
    *(s16 *)(p + 0x368) = 0;
    p[0x343] = 1;
    p[0x344] = 1;
    *(u8 **)(p + 0x228) = D_80155D32;
    *(u8 **)(p + 0x22C) = D_80155D38;
    *(u8 **)(p + 0x230) = D_80155D3E;
}

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

void func_8013F9E0(void) {
    u8 *p;
    s32 i;
    s16 v;

    p = D_80158A8C;
    switch (*(u16 *)(p + 6)) {
    case 0:
        if (*(s32 *)(D_80158A8C + 0x24) >= 0x600000) {
            *(s32 *)(p + 0x24) = 0x600000;
            *(s16 *)(p + 6) = 0x20;
        } else {
            *(s32 *)(p + 0x24) += 0x8000;
        }
        break;
    case 0x20:
        for (i = 0xB; i < 0xD; i++) {
            ((TtAnim14 *)(p + 0x38))[i].frames = D_80151A50;
            ((TtAnim14 *)(p + 0x38))[i].unk_04 = 0;
            ((TtAnim14 *)(p + 0x38))[i].unk_05 = 0xA;
            ((TtAnim14 *)(p + 0x38))[i].unk_08 = 0x60;
            ((TtAnim14 *)(p + 0x38))[i].unk_0B = 0xF;
            ((TtAnim14 *)(p + 0x38))[i].unk_10 = 0;
            p[i + 0x310] = 1;
            ((s16 *)(p + 0x2A8))[i] = 8;
            ((s16 *)(p + 0x248))[i] = 0;
            ((s16 *)(p + 0x278))[i] = -0x78;
        }
        p[0x11A] = 0x18;
        p[0x12E] = 0xC;
        *(s16 *)(p + 6) = 0x40;
        break;
    case 0x40:
        v = *(s16 *)(D_80158A8C + 0x28E);
        if (v >= -0x30) {
            func_80133AD0(0x502);
            *(s16 *)(p + 0x28E) = -0x30;
            *(s16 *)(p + 0x290) = -0x30;
            *(s16 *)(p + 6) = 0x50;
            break;
        }
        if (v >= -0x40) {
            if (*(u8 *)(D_80158A8C + 0xF2) == 0x16) {
                p[0xF2] = 0x3C;
            } else if (*(u8 *)(D_80158A8C + 0xF2) == 0x17) {
                p[0xF2] = 0x3D;
            }
        }
        *(s16 *)(p + 0x28E) += 1;
        *(s16 *)(p + 0x290) = *(s16 *)(p + 0x28E);
        break;
    case 0x50:
        func_80142AF0(D_80155E48, 0xB);
        if (p[0x11C] & 1) {
            *(s16 *)(p + 4) = 0x80;
            *(s16 *)(p + 6) = 0;
        }
        break;
    }
}

void func_8013FBD0(void) {
    u8 *c;
    s32 i;
    s16 *t;
    s16 j;
    u8 *e;
    u16 k;

    k = *(u16 *)(D_80158A8C + 8);
    c = D_80158A8C;
    switch (k) {
    case 0:
        func_80133AD0(0x501);
        *(u16 *)(c + 0x364) = 0;
        *(u16 *)(c + 0x3D4) = 0;
        for (i = 3; i < 9; i++) {
            ((TtSlot14 *)(c + 0x38))[i].unk_00 = D_80151A50;
            ((TtSlot14 *)(c + 0x38))[i].unk_04 = 0;
            ((TtSlot14 *)(c + 0x38))[i].unk_05 = 4;
            ((TtSlot14 *)(c + 0x38))[i].unk_08 = 0x80;
            ((TtSlot14 *)(c + 0x38))[i].unk_0A = 0;
            ((TtSlot14 *)(c + 0x38))[i].unk_10 = 0;
            ((s16 *)(c + 0x2A8))[i] = 6;
        }
        ((s16 *)(c + 0x248))[3] = -0x26;
        ((s16 *)(c + 0x278))[3] = 0xF;
        ((s16 *)(c + 0x248))[4] = 0x26;
        ((s16 *)(c + 0x278))[4] = 0xF;
        ((s16 *)(c + 0x248))[5] = -0x2C;
        ((s16 *)(c + 0x278))[5] = 0xA;
        ((s16 *)(c + 0x248))[6] = 0x2C;
        ((s16 *)(c + 0x278))[6] = 0xA;
        ((s16 *)(c + 0x248))[7] = -0x30;
        ((s16 *)(c + 0x278))[7] = 0;
        ((s16 *)(c + 0x248))[8] = 0x30;
        ((s16 *)(c + 0x278))[8] = 0;
        for (i = 3; i < 9; i++) {
            func_80142AF0(D_80155DF0, i);
        }
        for (i = 0x10; i != 0x13; i++) {
            ((TtSlot14 *)(c + 0x38))[i].unk_00 = D_80151A50;
            ((TtSlot14 *)(c + 0x38))[i].unk_04 = 0;
            ((TtSlot14 *)(c + 0x38))[i].unk_05 = 4;
            ((TtSlot14 *)(c + 0x38))[i].unk_0A = 0;
            ((TtSlot14 *)(c + 0x38))[i].unk_10 = 0;
            c[0x310 + i] = 0;
            ((s16 *)(c + 0x2A8))[i] = 8;
            ((s16 *)(c + 0x248))[i] = 0;
            ((s16 *)(c + 0x278))[i] = 0;
        }
        *(u16 *)(c + 8) = 0x40;
        break;
    case 0x40:
        t = D_80155E74;
        e = D_80158A8C;
        if (t[*(u16 *)(e + 0x3D4)] != -1 && t[*(u16 *)(e + 0x3D4)] == *(u16 *)(e + 0x364)) {
            func_80133AD0(0x501);
            j = t[*(u16 *)(c + 0x3D4) + 4];
            c[j + 0x310] = 1;
            c[j * 0x14 + 0x40] = 0x80;
            *(s16 *)(c + j * 2 + 0x248) = t[*(u16 *)(c + 0x3D4) + 1];
            *(s16 *)(c + j * 2 + 0x278) = t[*(u16 *)(c + 0x3D4) + 2];
            *(s16 *)(c + j * 2 + 0x2A8) = t[*(u16 *)(c + 0x3D4) + 3];
            *(u16 *)(c + 0x3D4) += 5;
        }
        for (i = 3; i < 9; i++) {
            func_80142AF0(D_80155DF0, i);
        }
        for (i = 0x10; i < 0x13; i++) {
            func_80142AF0(D_80150950, i);
        }
        if (*(u16 *)(c + 0x364) == 0x8F) {
            for (i = 0; i != 0x10; i++) {
                c[0x310 + i] = 0;
            }
        }
        if (*(u16 *)(c + 0x364) == 0xA6) {
            for (i = 0; i != 0x18; i++) {
                c[0x310 + i] = 0;
            }
            *(s16 *)(c + 8) = 0;
            *(s16 *)(c + 4) = 0xA0;
        }
        break;
    }
    *(u16 *)(c + 0x364) += 1;
}

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

/* T-11840: matched once the 7 stored to +0x2A8[i] is u16 (it then shares v0 with the (timer & 7) == 7 test), the sums are written base + table[index] (uopt loads the table entry first), and two unused words size the frame. */
void func_801400B0(void) {
    u16 mode;
    u8 *other;
    u8 *p;
    s32 angle;
    s32 pad[2]; /* FAKE: unused, sizes the frame (0x88, point at sp+0x5C). T-11840 */
    TtPoint14 point;
    s32 i;

    p = D_80158A8C;
    mode = *(u16 *)(p + 8);
    other = D_80158A74;
    switch (mode) {
    case 0:
        func_80133AD0(0x512);
        *(u16 *)(p + 0x366) = 0;
        for (i = 0x10; i < 0x13; i++) {
            ((TtAnim14 *)(p + 0x38))[i].frames = D_80151A50;
            ((TtAnim14 *)(p + 0x38))[i].unk_04 = 0;
            ((TtAnim14 *)(p + 0x38))[i].unk_05 = 0xA;
            ((TtAnim14 *)(p + 0x38))[i].unk_06 = 0x10;
            ((TtAnim14 *)(p + 0x38))[i].unk_08 = 0x60;
            ((TtAnim14 *)(p + 0x38))[i].unk_0B = 0xF;
            ((TtAnim14 *)(p + 0x38))[i].unk_10 = 0;
            p[i + 0x310] = 1;
            ((u16 *)(p + 0x2A8))[i] = 7;
            ((s16 *)(p + 0x248))[i] = 0;
            ((s16 *)(p + 0x278))[i] = 8;
        }
        *(s32 *)(p + 0x3EC) = 0x80000;
        *(s32 *)(p + 0x3F0) = -0x10000;
        *(s32 *)(p + 0x3F4) = 0x400;
        *(s32 *)(p + 0x3F8) = 0;
        p[0x343] = 0;
        *(u16 *)(p + 8) = 0x40;
        break;
    case 0x40:
        if (*(s32 *)(D_80158A8C + 0x3EC) > 0x200000) {
            *(s32 *)(p + 0x3F4) = -0x400;
        } else {
            *(s32 *)(p + 0x3F4) = 0x400;
        }
        if (*(s32 *)(p + 0x3EC) > 0x200000 && *(s32 *)(p + 0x3F0) <= 0) {
            *(s32 *)(p + 0x3F4) = 0;
            *(u16 *)(p + 8) = 0x50;
        }
        *(s32 *)(p + 0x3F8) += 0x4000;
        *(s32 *)(p + 0x3EC) += *(s32 *)(p + 0x3F0);
        *(s32 *)(p + 0x3F0) += *(s32 *)(p + 0x3F4);
        *(s16 *)(p + 0x26A) = -*(s32 *)(p + 0x3F8) / 0x10000;
        *(s16 *)(p + 0x26C) = *(s32 *)(p + 0x3F8) / 0x10000;
        for (i = 0x10; i < 0x13; i++) {
            ((s16 *)(p + 0x278))[i] = *(s32 *)(p + 0x3EC) / 0x10000;
        }
        break;
    case 0x50:
        *(u16 *)(p + 0x366) = 0;
        *(u16 *)(p + 8) = 0x51;
        break;
    case 0x51:
        if (*(u16 *)(D_80158A8C + 0x366) >= 0x20) {
            *(u16 *)(p + 0x366) = 0;
            *(u16 *)(p + 8) = 0x52;
        }
        break;
    case 0x52:
        *(u16 *)(p + 0x366) = 0;
        *(u16 *)(p + 0x3D4) = 0;
        *(u16 *)(p + 0x3D6) = 1;
        *(u16 *)(p + 0x3D8) = 2;
        *(u16 *)(p + 8) = 0x53;
        break;
    case 0x53:
        if (*(u8 *)(D_80158A8C + 0x17E) == 0x12 && !(*(u16 *)(D_80158A8C + 0x366) & 7)) {
            func_80133AD0(0x513);
        }
        if (p[0x17E] == 0x12 && (*(u16 *)(p + 0x366) & 7) == 7) {
            point.x = (*(s16 *)(p + 0x14) + ((s16 *)(p + 0x268))[*(u16 *)(p + 0x3D4)]) << 16;
            point.y = (*(s16 *)(p + 0x16) + ((s16 *)(p + 0x298))[*(u16 *)(p + 0x3D4)]) << 16;
            angle = func_801320F0((s32) &point, (s32) (other + 0x14));
            for (i = 0; i != 0x1000; i += 0x400) {
                func_80133E30(*(s16 *)(p + 0x14) + ((s16 *)(p + 0x268))[*(u16 *)(p + 0x3D4)],
                              *(s16 *)(p + 0x16) + ((s16 *)(p + 0x298))[*(u16 *)(p + 0x3D4)], 0x18, angle + i, 0xA0);
                func_80133E30(*(s16 *)(p + 0x14) + ((s16 *)(p + 0x268))[*(u16 *)(p + 0x3D6)],
                              *(s16 *)(p + 0x16) + ((s16 *)(p + 0x298))[*(u16 *)(p + 0x3D6)], 0x18, i, 0xA0);
                func_80133E30(*(s16 *)(p + 0x14) + ((s16 *)(p + 0x268))[*(u16 *)(p + 0x3D8)],
                              *(s16 *)(p + 0x16) + ((s16 *)(p + 0x298))[*(u16 *)(p + 0x3D8)], 0x18, i + 0x200, 0xA0);
            }
        }
        func_8013F988();
        *(u16 *)(p + 0x3D4) += 1;
        if (*(u16 *)(p + 0x3D4) >= 3) {
            *(u16 *)(p + 0x3D4) = 0;
        }
        *(u16 *)(p + 0x3D6) += 1;
        if (*(u16 *)(p + 0x3D6) >= 3) {
            *(u16 *)(p + 0x3D6) = 0;
        }
        *(u16 *)(p + 0x3D8) += 1;
        if (*(u16 *)(p + 0x3D8) >= 3) {
            *(u16 *)(p + 0x3D8) = 0;
        }
        if (*(u16 *)(p + 0x366) >= 0x180) {
            *(u16 *)(p + 8) = 0x80;
        }
        break;
    case 0x80:
        func_80133AD0(0x50A);
        p[0x343] = 1;
        for (i = 0x10; i < 0x13; i++) {
            ((TtAnim14 *)(p + 0x38))[i].unk_05 = 4;
            ((TtAnim14 *)(p + 0x38))[i].unk_06 = 0x34;
            ((TtAnim14 *)(p + 0x38))[i].unk_04 = 0;
            ((TtAnim14 *)(p + 0x38))[i].unk_08 = 0x80;
        }
        *(u16 *)(p + 0x366) = 0;
        *(u16 *)(p + 8) = 0x81;
        break;
    case 0x81:
        for (i = 0x10; i < 0x13; i++) {
            func_80142AF0(D_80155DF0, i);
        }
        if (*(u16 *)(p + 0x366) >= 0x40) {
            *(u16 *)(p + 8) = 0xA0;
        }
        break;
    case 0xA0:
        for (i = 0x10; i < 0x13; i++) {
            p[i + 0x310] = 0;
        }
        *(s32 *)(p + 0x3E8) = 0;
        *(u16 *)(p + 6) = 0;
        *(u16 *)(p + 8) = 0;
        *(s32 *)(p + 0x3E4) = *(s32 *)(p + 0x20);
        break;
    }
    *(u16 *)(p + 0x366) += 1;
}

/* T-11520/T-11550: matched once the case-0x40 tables are read as x[i]/y[i] in counted loops (i < 5, i < 4): uopt strength-reduces them to the same pointer walk; with i shared by those loops (likely a lower priority for i) the angle loop keeps 0x900 (n) in s1 and angle + i in s0 and spills i, as in the original. The stack layout needs n and i declared after the TtPoint14. */
void func_80140624(void) {
    u16 mode;
    u8 *other;
    TtSceneView *p;
    s32 *x;
    s32 *y;
    s32 angle;
    TtPoint14 point;
    s32 n;
    s32 i;

    mode = ((TtSceneView *) D_80158A8C)->unk_08;
    other = D_80158A74;
    p = (TtSceneView *) D_80158A8C;
    switch (mode) {
    case 0:
        p->unk_366 = 0;
        p->unk_08 = 0x10;
        break;
    case 0x10:
        if (((TtSceneView *) D_80158A8C)->unk_366 >= 0x3C) {
            p->unk_08 = 0x40;
        }
        break;
    case 0x40:
        if (p->unk_366 < 0x80) {
            if (((TtSceneView *) other)->unk_16 < 0x78) {
                p->unk_366 = 0x80;
            }
            if ((p->unk_366 & 0x1F) == 0x1F) {
                func_80133AD0(0x500);
                if (((u8 *) p)[0x106] == 0) {
                    x = D_80155EC8;
                    y = D_80155EDC;
                    for (i = 0; i < 5; i++) {
                        func_80134874(p->unk_14 - x[i], y[i] + p->unk_16);
                        func_80134874(x[i] + p->unk_14, y[i] + p->unk_16);
                    }
                } else {
                    x = D_80155EF0;
                    y = D_80155F04;
                    for (i = 0; i < 4; i++) {
                        func_80134874(p->unk_14 - x[i], y[i] + p->unk_16);
                        func_80134874(x[i] + p->unk_14, y[i] + p->unk_16);
                    }
                }
            }
        } else if (p->unk_366 < 0x100) {
            if ((p->unk_366 & 1) == 1) {
                if ((p->unk_366 & 3) == 3) {
                    func_80133AD0(0x50D);
                }
                func_80133E30(p->unk_14, p->unk_16 + 8, 0x30, ((p->unk_366 & 0x7F) << 6) - 0x400, 0xA1);
            }
        } else if ((p->unk_366 & 0x3F) == 0x3F) {
            func_80133AD0(0x50B);
            point.x = p->unk_14 << 16;
            point.y = (p->unk_16 + 8) << 16;
            angle = func_801320F0((s32) &point, (s32) (other + 0x14));
            angle &= 0x7F;
            for (i = -0x100, n = 0x900; i != n; i += 0x80) {
                func_80133E30(p->unk_14, p->unk_16 + 8, 0x48, angle + i, 0xA0);
            }
        }
        if (p->unk_366 >= 0x180) {
            p->unk_08 = 0xA0;
        }
        break;
    case 0xA0:
        p->unk_06 = 1;
        p->unk_08 = 0;
        break;
    }
    p->unk_366++;
}

/* T-11540/T-11550: matched once case 0x52 reads +0x3EC/+0x3F0 back from memory for the /0x10000 stores (the vx/vy copies took a register and spilled one more word) and pt is declared after vy, which puts it at sp+56. */
void func_80140920(void) {
    u8 *b;
    u8 *c;
    u8 *a;
    u16 mode;
    s32 t; /* FAKE: unused, needed for the frame size. T-11510 */
    s32 vx; /* FAKE: unused, needed for the frame layout (pt at sp+56). T-11550 */
    s32 vy; /* FAKE: unused, needed for the frame layout (pt at sp+56). T-11550 */
    TtPoint14 pt;
    s16 x;
    s16 y;
    c = D_80158A8C;
    mode = *(u16 *)(c + 8);
    b = D_80158A60;
    a = D_80158A74;
    switch (mode) {
    case 0:
        func_80133AD0(0x510);
        *(u8 **)(c + 0x204) = D_80151A50;
        c[0x208] = 0;
        c[0x209] = 4;
        c[0x20A] = 0x41;
        c[0x20C] = 0x60;
        c[0x20F] = 0xF;
        c[0x214] = 1;
        c[0x327] = 1;
        *(s16 *)(c + 0x2D6) = 0xA;
        *(s16 *)(c + 0x276) = 0;
        *(s16 *)(c + 0x2A6) = 8;
        c[0x20E] = 0;
        *(s16 *)(c + 0x210) = 0;
        *(s16 *)(c + 0x212) = 0;
        *(s32 *)(c + 0x3EC) = 0x80000;
        c[0x344] = 0;
        *(s16 *)(c + 8) = 0x40;
        return;
    case 0x40:
        if (*(u16 *)(D_80158A60 + 8) & 3) {
            c[0x327] = 1;
        } else {
            c[0x327] = 0;
        }
        c[0x20E] = 0;
        *(s16 *)(c + 0x210) += 0x20;
        *(s16 *)(c + 0x212) += 0x20;
        *(s32 *)(c + 0x3EC) += 0x8000;
        *(s16 *)(c + 0x2A6) = *(s32 *)(c + 0x3EC) / 0x10000;
        if (*(s16 *)(c + 0x212) >= 0x1000) {
            *(s16 *)(c + 8) = 0x50;
            c[0x214] = 0;
            *(u16 *)(c + 0x368) = 0;
            return;
        }
        return;
    case 0x50:
        if (*(u16 *)(D_80158A60 + 8) & 3) {
            c[0x327] = 1;
        } else {
            c[0x327] = 0;
        }
        c[0x20E] = 0;
        if (*(u16 *)(c + 0x368) >= 0x3C) {
            *(s16 *)(c + 8) = 0x51;
            return;
        }
        *(u16 *)(c + 0x368) += 1;
        return;
    case 0x51:
        func_80133AD0(0x60C);
        if (*(u16 *)(b + 8) & 3) {
            c[0x327] = 1;
        } else {
            c[0x327] = 0;
        }
        c[0x20E] = 0;
        pt.x = *(s16 *)(c + 0x14) << 16;
        pt.y = (*(s16 *)(c + 0x16) + 0x48) << 16;
        D_80155F18 = func_801320F0((s32) &pt, (s32) (a + 0x14));
        *(s32 *)(c + 0x3EC) = *(s16 *)(c + 0x276) << 16;
        *(s32 *)(c + 0x3F0) = *(s16 *)(c + 0x2A6) << 16;
        *(s16 *)(c + 8) = 0x52;
        return;
    case 0x52:
        if (*(u16 *)(D_80158A60 + 8) & 3) {
            c[0x327] = 1;
        } else {
            c[0x327] = 0;
        }
        c[0x20E] = 0;
        *(s32 *)(c + 0x3EC) += func_800A0140(D_80155F18) * 0x1C;
        *(s32 *)(c + 0x3F0) += func_800A0070(D_80155F18) * 0x1C;
        *(s16 *)(c + 0x276) = *(s32 *)(c + 0x3EC) / 0x10000;
        *(s16 *)(c + 0x2A6) = *(s32 *)(c + 0x3F0) / 0x10000;
        x = *(s16 *)(c + 0x14) + *(s16 *)(c + 0x276);
        y = *(s16 *)(c + 0x16) + *(s16 *)(c + 0x2A6);
        if (y >= 0x139 || y < -0x48 || x < -0xC8 || x >= 0xC9) {
            c[0x327] = 0;
            *(s16 *)(c + 8) = 0xA0;
            return;
        }
        break;
    case 0xA0:
        c[0x344] = 1;
        *(s32 *)(c + 0x3E8) = 0;
        *(s16 *)(c + 6) = 0;
        *(u16 *)(c + 8) = 0;
        *(s32 *)(c + 0x3E4) = *(s32 *)(c + 0x20);
        break;
    }
}

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

void func_80140D84(void) {
    u8 *s;
    u8 *t;
    u8 *p;
    u8 *e;

    p = D_80158A8C;
    s = D_80158A64;
    e = D_80158A74;
    switch (*(u16 *)(p + 4)) {
    case 0:
        func_80142570();
        if (*(u16 *)(s + 4) == 0) {
            func_80044750(0xB1);
            func_800451E0(0x8019A000);
            func_80044750(0x12);
            func_80044750(0x23);
            func_80044750(0x2E);
            func_80044750(0x21);
            func_80044750(4);
            func_80044750(0x200);
        }
        *(s16 *)(s + 0x32) = 0;
        *(s16 *)(s + 0x34) = 0x1000;
        func_8013F3B4();
        *(s16 *)(p + 4) = 0x20;
        break;
    case 0x20:
        if (*(s32 *)D_80158A6C == 0) {
            s[0x1C] = 0x10;
        }
        func_8013F588();
        func_8013F878();
        break;
    case 0x30:
        if (*(s32 *)D_80158A6C == 0) {
            s[0x1C] = 0x10;
        }
        func_8013F5F8();
        func_8013F878();
        break;
    case 0x40:
        if (*(s32 *)D_80158A6C == 0) {
            s[0x1C] = 0x10;
        }
        func_8013F7B0();
        func_8013F878();
        func_80140CBC();
        if (*(s16 *)(p + 0x30C) < 0) {
            switch (*(u16 *)(p + 0x30E)) {
            case 0:
                if (*(u16 *)(p + 6) == 2) {
                    if (*(u16 *)(p + 8) < 0x80) {
                        *(u16 *)(p + 8) = 0x80;
                    }
                } else {
                    *(u16 *)(p + 0x30E) = 0x10;
                    *(u16 *)(p + 0x366) = 0;
                    *(u16 *)(p + 8) = 0;
                }
                break;
            case 1:
                p[0x327] = 0;
                p[0x344] = 1;
                func_80142774();
                /* FAKE: zero-store chains (TT house idiom); their dead reloads keep the constant 1 in a0. T-11860 */
                *(u16 *)(p + 6) = *(u16 *)(p + 8) = *(u16 *)(p + 0xA) = 0;
                p[0xD] = p[0xE] = p[0xF] = 0;
                *(s16 *)(p + 4) = 0x50;
                break;
            case 0x10:
                if (*(u16 *)(p + 0x366) >= 0x3C) {
                    *(s16 *)(p + 0x30C) = 0x100;
                    *(u16 *)(p + 0x30E) = 1;
                } else {
                    *(u16 *)(p + 0x366) += 1;
                }
                break;
            }
        }
        break;
    case 0x50:
        func_8013F9E0();
        func_8013F878();
        break;
    case 0x80:
        func_80044750(0xC1);
        e[0x52] = 1;
        s[0x1C] = 0x11;
        func_8013FBD0();
        break;
    case 0xA0:
        func_80044750(0xB1);
        p[0] = 0;
        s[0x12] = 0;
        t = (u8 *)D_80158A70;
        *(u16 *)(t + 0x1E) = 1;
        *(s16 *)(s + 2) = 0xA0;
        break;
    }
    func_8013F040();
}
