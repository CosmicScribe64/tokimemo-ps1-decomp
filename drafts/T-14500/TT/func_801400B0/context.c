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

void func_801400B0(void) {
    s32 sp6C;
    s32 sp68;
    s32 sp5C[1];
    s32 temp_t6;
    s32 temp_t7;
    s32 temp_v0_2;
    s32 temp_v1;
    s32 var_a2;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s0_3;
    s32 var_s0_4;
    s32 var_s0_5;
    s32 var_s1;
    s32 var_t7;
    s32 var_t8;
    s32 var_t9;
    u16 temp_t6_2;
    u16 temp_t8;
    u16 temp_t9;
    u16 temp_v0;
    u8 *temp_s0;
    u8 *temp_s2;
    u8 *var_a0;
    u8 *var_a0_2;
    u8 *var_a1;
    u8 *var_a1_2;
    u8 *var_v1;
    u8 *var_v1_2;
    u8 *temp_v0_3;
    u8 *temp_v1_2;
    u8 *temp_v1_3;

    temp_s2 = D_80158A8C;
    temp_v0 = *(u16 *)((unsigned char *)temp_s2 + (8));
    temp_s0 = D_80158A74;
    switch (temp_v0) {
    default:
        var_s0 = 0x10;
        if (temp_v0 != 0x81) {
            var_s0_2 = 0x10;
            if (temp_v0 != 0xA0) {

            } else {
                var_a1 = temp_s2 + 0x10;
                do {
                    var_s0_2 += 1;
                    var_a1 += 1;
                    *(s8 *)((unsigned char *)var_a1 + (783)) = 0;
                } while (var_s0_2 != 0x13);
                *(s32 *)((unsigned char *)temp_s2 + (1000)) = 0;
                *(s16 *)((unsigned char *)temp_s2 + (6)) = 0;
                *(u16 *)((unsigned char *)temp_s2 + (8)) = 0U;
                *(s32 *)((unsigned char *)temp_s2 + (996)) = (s32) *(s32 *)((unsigned char *)temp_s2 + (32));
            }
        } else {
            do {
                func_80142AF0(D_80155DF0, var_s0);
                var_s0 += 1;
            } while (var_s0 != 0x13);
            if ((u16) *(u16 *)((unsigned char *)temp_s2 + (870)) >= 0x40U) {
                *(s16 *)((unsigned char *)temp_s2 + (8)) = 0xA0;
            }
        }
        break;
    case 0x0:
        if (temp_v0 == 0) {
            func_80133AD0(0x512);
            *(u16 *)((unsigned char *)temp_s2 + (870)) = 0U;
            var_a2 = 0x140;
            var_v1 = temp_s2 + 0x140;
            var_a1_2 = temp_s2 + 0x10;
            var_a0 = temp_s2 + 0x20;
            do {
                *(u8 **)((unsigned char *)var_v1 + (56)) = D_80151A50;
                *(s8 *)((unsigned char *)var_v1 + (60)) = 0;
                *(s8 *)((unsigned char *)var_v1 + (61)) = 0xA;
                *(s8 *)((unsigned char *)var_v1 + (62)) = 0x10;
                *(s8 *)((unsigned char *)var_v1 + (64)) = 0x60;
                *(s8 *)((unsigned char *)var_v1 + (67)) = 0xF;
                *(s8 *)((unsigned char *)var_v1 + (72)) = 0;
                *(s8 *)((unsigned char *)var_a1_2 + (784)) = 1;
                var_a2 += 0x14;
                var_v1 += 0x14;
                var_a1_2 += 1;
                var_a0 += 2;
                *(s16 *)((unsigned char *)var_a0 + (678)) = 7;
                *(s16 *)((unsigned char *)var_a0 + (582)) = 0;
                *(s16 *)((unsigned char *)var_a0 + (630)) = 8;
            } while (var_a2 != 0x17C);
            *(s32 *)((unsigned char *)temp_s2 + (1004)) = 0x80000;
            *(s32 *)((unsigned char *)temp_s2 + (1008)) = -0x10000;
            *(s32 *)((unsigned char *)temp_s2 + (1012)) = 0x400;
            *(s32 *)((unsigned char *)temp_s2 + (1016)) = 0;
            *(s8 *)((unsigned char *)temp_s2 + (835)) = 0;
            *(s16 *)((unsigned char *)temp_s2 + (8)) = 0x40;
        }
        break;
    case 0x40:
        var_s0_3 = 0x10;
        if (*(s32 *)((unsigned char *)D_80158A8C + (1004)) >= 0x200001) {
            *(s32 *)((unsigned char *)temp_s2 + (1012)) = -0x400;
        } else {
            *(s32 *)((unsigned char *)temp_s2 + (1012)) = 0x400;
        }
        var_a0_2 = temp_s2 + 0x20;
        if ((*(s32 *)((unsigned char *)temp_s2 + (1004)) >= 0x200001) && (*(s32 *)((unsigned char *)temp_s2 + (1008)) <= 0)) {
            *(s32 *)((unsigned char *)temp_s2 + (1012)) = 0;
            *(s16 *)((unsigned char *)temp_s2 + (8)) = 0x50;
        }
        temp_v0_2 = *(s32 *)((unsigned char *)temp_s2 + (1008));
        temp_v1 = *(s32 *)((unsigned char *)temp_s2 + (1016)) + 0x4000;
        *(s32 *)((unsigned char *)temp_s2 + (1004)) = (s32) (*(s32 *)((unsigned char *)temp_s2 + (1004)) + temp_v0_2);
        temp_t7 = -temp_v1;
        *(s32 *)((unsigned char *)temp_s2 + (1016)) = temp_v1;
        *(s32 *)((unsigned char *)temp_s2 + (1008)) = (s32) (temp_v0_2 + *(s32 *)((unsigned char *)temp_s2 + (1012)));
        var_t8 = temp_t7 >> 0x10;
        if (temp_t7 < 0) {
            var_t8 = (s32) (temp_t7 + 0xFFFF) >> 0x10;
        }
        *(s16 *)((unsigned char *)temp_s2 + (618)) = (s16) var_t8;
        var_t9 = temp_v1 >> 0x10;
        if (temp_v1 < 0) {
            var_t9 = (s32) (temp_v1 + 0xFFFF) >> 0x10;
        }
        *(s16 *)((unsigned char *)temp_s2 + (620)) = (s16) var_t9;
        do {
            temp_t6 = *(s32 *)((unsigned char *)temp_s2 + (1004));
            var_s0_3 += 1;
            var_t7 = temp_t6 >> 0x10;
            if (temp_t6 < 0) {
                var_t7 = (s32) (temp_t6 + 0xFFFF) >> 0x10;
            }
            var_a0_2 += 2;
            *(s16 *)((unsigned char *)var_a0_2 + (630)) = (s16) var_t7;
        } while (var_s0_3 != 0x13);
        break;
    case 0x50:
        *(u16 *)((unsigned char *)temp_s2 + (870)) = 0U;
        *(s16 *)((unsigned char *)temp_s2 + (8)) = 0x51;
        break;
    case 0x51:
        if ((u16) *(u16 *)((unsigned char *)D_80158A8C + (870)) >= 0x20U) {
            *(u16 *)((unsigned char *)temp_s2 + (870)) = 0U;
            *(s16 *)((unsigned char *)temp_s2 + (8)) = 0x52;
        }
        break;
    case 0x52:
        *(u16 *)((unsigned char *)temp_s2 + (870)) = 0U;
        *(u16 *)((unsigned char *)temp_s2 + (980)) = 0U;
        *(u16 *)((unsigned char *)temp_s2 + (982)) = 1U;
        *(u16 *)((unsigned char *)temp_s2 + (984)) = 2U;
        *(s16 *)((unsigned char *)temp_s2 + (8)) = 0x53;
        break;
    case 0x53:
        if ((*(u8 *)((unsigned char *)D_80158A8C + (382)) == 0x12) && !(*(u16 *)((unsigned char *)D_80158A8C + (870)) & 7)) {
            func_80133AD0(0x513);
        }
        if ((*(u8 *)((unsigned char *)temp_s2 + (382)) == 0x12) && ((*(u16 *)((unsigned char *)temp_s2 + (870)) & 7) == 7)) {
            sp68 = (*(s16 *)((unsigned char *)(temp_s2 + (*(u16 *)((unsigned char *)temp_s2 + (980)) * 2)) + (616)) + *(s16 *)((unsigned char *)temp_s2 + (20))) << 0x10;
            sp6C = (*(s16 *)((unsigned char *)(temp_s2 + (*(u16 *)((unsigned char *)temp_s2 + (980)) * 2)) + (664)) + *(s16 *)((unsigned char *)temp_s2 + (22))) << 0x10;
            var_s0_4 = 0;
            var_s1 = func_801320F0((s32) &sp5C, (s32) (temp_s0 + 0x14));
            do {
                temp_v0_3 = temp_s2 + (*(u16 *)((unsigned char *)temp_s2 + (980)) * 2);
                func_80133E30(*(s16 *)((unsigned char *)temp_v0_3 + (616)) + *(s16 *)((unsigned char *)temp_s2 + (20)), *(s16 *)((unsigned char *)temp_v0_3 + (664)) + *(s16 *)((unsigned char *)temp_s2 + (22)), 0x18, var_s1, 0xA0);
                temp_v1_2 = temp_s2 + (*(u16 *)((unsigned char *)temp_s2 + (982)) * 2);
                func_80133E30(*(s16 *)((unsigned char *)temp_v1_2 + (616)) + *(s16 *)((unsigned char *)temp_s2 + (20)), *(s16 *)((unsigned char *)temp_v1_2 + (664)) + *(s16 *)((unsigned char *)temp_s2 + (22)), 0x18, var_s0_4, 0xA0);
                temp_v1_3 = temp_s2 + (*(u16 *)((unsigned char *)temp_s2 + (984)) * 2);
                func_80133E30(*(s16 *)((unsigned char *)temp_v1_3 + (616)) + *(s16 *)((unsigned char *)temp_s2 + (20)), *(s16 *)((unsigned char *)temp_v1_3 + (664)) + *(s16 *)((unsigned char *)temp_s2 + (22)), 0x18, var_s0_4 + 0x200, 0xA0);
                var_s0_4 += 0x400;
                var_s1 += 0x400;
            } while (var_s0_4 != 0x1000);
        }
        func_8013F988();
        temp_t9 = *(u16 *)((unsigned char *)temp_s2 + (980)) + 1;
        *(u16 *)((unsigned char *)temp_s2 + (980)) = temp_t9;
        if ((u32) (temp_t9 & 0xFFFF) >= 3U) {
            *(u16 *)((unsigned char *)temp_s2 + (980)) = 0U;
        }
        temp_t8 = *(u16 *)((unsigned char *)temp_s2 + (982)) + 1;
        *(u16 *)((unsigned char *)temp_s2 + (982)) = temp_t8;
        if ((u32) (temp_t8 & 0xFFFF) >= 3U) {
            *(u16 *)((unsigned char *)temp_s2 + (982)) = 0U;
        }
        temp_t6_2 = *(u16 *)((unsigned char *)temp_s2 + (984)) + 1;
        *(u16 *)((unsigned char *)temp_s2 + (984)) = temp_t6_2;
        if ((u32) (temp_t6_2 & 0xFFFF) >= 3U) {
            *(u16 *)((unsigned char *)temp_s2 + (984)) = 0U;
        }
        if ((u16) *(u16 *)((unsigned char *)temp_s2 + (870)) >= 0x180U) {
            *(s16 *)((unsigned char *)temp_s2 + (8)) = 0x80;
        }
        break;
    case 0x80:
        func_80133AD0(0x50A);
        *(s8 *)((unsigned char *)temp_s2 + (835)) = 1;
        var_s0_5 = 0x10;
        var_v1_2 = temp_s2 + 0x140;
        do {
            var_s0_5 += 1;
            var_v1_2 += 0x14;
            *(s8 *)((unsigned char *)var_v1_2 + (41)) = 4;
            *(s8 *)((unsigned char *)var_v1_2 + (42)) = 0x34;
            *(s8 *)((unsigned char *)var_v1_2 + (40)) = 0;
            *(s8 *)((unsigned char *)var_v1_2 + (44)) = 0x80;
        } while (var_s0_5 != 0x13);
        *(u16 *)((unsigned char *)temp_s2 + (870)) = 0U;
        *(s16 *)((unsigned char *)temp_s2 + (8)) = 0x81;
        break;
    }
    *(u16 *)((unsigned char *)temp_s2 + (870)) = (u16) (*(u16 *)((unsigned char *)temp_s2 + (870)) + 1);
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

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013F040", func_80140D84);
