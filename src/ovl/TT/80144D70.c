#define MAIN_API_OVERRIDE_func_800AE080 /* implicit int declaration, as matched: with the void prototype the reload of ctl after the call in func_80144D70 takes v0, not v1. T-11840 */
#include "common.h"
#include "ovl/TT.h"

/* T-11840: matched. r is the selector of both switches and holds the func_800460DC result; with pool assigned again in case 0xA0 this raises r above pool in the uopt order, so the selectors take v1 and pool a3. The head order (scene, g, ctl, pool, stream, buttons, list) sets the ugen temporary order. */
void func_80144D70(void) {
    u8 *scene;
    TtCore30 *g;
    u8 *ctl;
    s32 *stream;
    TtButtons06 *buttons;
    TtPoolDraw14 *pool;
    u8 *list;
    u32 r;

    scene = D_80158A60;
    g = (TtCore30 *)D_80158A64;
    ctl = D_80158A6C;
    pool = (TtPoolDraw14 *)(D_80158AA8 + 0x140);
    stream = D_80158A70;
    buttons = (TtButtons06 *)(scene + 0x10);
    list = D_80158AA4 + 0xC80;
    r = g->unk_02;
    switch (r) {
    case 0x0:
        func_80044750(0x22);
        func_80044750(0x2F);
        func_8014AF3C();
        func_80132F4C();
        func_80133B70();
        g->unk_11 = 0;
        if (ctl[0xB] < 3) {
            func_80044750(4);
            func_80044750(0x202);
            r = ctl[0xB];
            switch (r) {
            case 0:
                *(s16 *)(scene + 0xA4) = 1;
                func_8014304C(0x80016000, 0xC869, 0x2420);
                func_8014304C(0x8002E800, 0xC817, 0x69E4);
                func_8014304C(0x8019A000, 0xC81A, 0x27D60);
                break;
            case 1:
                *(s16 *)(scene + 0xA4) = 2;
                func_8014304C(0x80016000, 0xC8BD, 0x2420);
                func_8014304C(0x8002E800, 0xC86D, 0x69E4);
                func_8014304C(0x8019A000, 0xC871, 0x27D60);
                break;
            case 2:
                *(s16 *)(scene + 0xA4) = 3;
                func_8014304C(0x80016000, 0xC916, 0x2420);
                func_8014304C(0x8002E800, 0xC8C1, 0x69E4);
                func_8014304C(0x8019A000, 0xC8C6, 0x27D60);
                break;
            }
            func_8014304C(0x8001A000, 0xBFD1, 0xE20);
            func_8014304C(0x801C2000, 0xBF9C, 0x1A680);
            pool->x = -0x38;
        } else {
            func_80044750(4);
            func_80044750(0x200);
            pool->x = -0x20;
        }
        pool->y = 0x74;
        pool->text = 0xB;
        pool->unk_02 = 4;
        pool->unk_00 = 1;
        pool->flags = 0;
        pool->ot = 0;
        pool++;
        if (ctl[0xB] < 3) {
            pool->x = 8;
        } else {
            pool->x = 0x10;
        }
        pool->y = 0x74;
        pool->text = 0xC;
        pool->unk_02 = 4;
        pool->unk_00 = 1;
        pool->flags = 0;
        pool->ot = 0;
        pool++;
        if (ctl[0xB] < 3) {
            list[0] = 1;
            list[1] = 0;
            *(s16 *)(list + 0x44) = 0;
            *(s16 *)(list + 0x14) = -0xC;
            *(s16 *)(list + 0x16) = 0x6C;
            *(s16 *)(list + 0x3C) = 0x10;
            *(s16 *)(list + 0x3E) = 0x10;
            list[0x46] = ctl[0xB] * 0x10 + 0x10;
            list[0x47] = 0x20;
            *(s16 *)(list + 0x40) = 0xA;
            *(s16 *)(list + 0x42) = 0x3C08;
            list[0x3B] = 0x65;
            *(s16 *)(list + 0x10) = 0;
        } else {
            pool->x = -0x40;
            pool->y = 0x74;
            pool->text = 0xA;
            pool->unk_02 = 4;
            pool->unk_00 = 1;
            pool->flags = 0;
            pool->ot = 0;
        }
        *(u16 *)&g->unk_2E = 0x80;
        g->unk_1D = 0x80;
        g->unk_1E = 0x80;
        g->unk_1F = 0x80;
        g->unk_1C = 3;
        *(s32 *)(ctl + 4) = 0;
        func_80148DB0();
        g->unk_02 = 0x40;
        break;
    case 0x40:
        if ((*(u16 *)&g->unk_2E -= 4) == 0) {
            *(s32 *)ctl = 0;
            g->unk_26 = 0;
            g->unk_02 = 0x50;
        }
        g->unk_1D = *(u16 *)&g->unk_2E;
        g->unk_1E = *(u16 *)&g->unk_2E;
        g->unk_1F = *(u16 *)&g->unk_2E;
        if (*(s32 *)(ctl + 4) < *(s32 *)ctl) {
            *(s32 *)ctl -= 0x400;
        } else {
            *(s32 *)ctl += 0x200;
        }
        if ((*(s32 *)ctl - *(s32 *)(ctl + 4) < 0 ? -(*(s32 *)ctl - *(s32 *)(ctl + 4)) : *(s32 *)ctl - *(s32 *)(ctl + 4)) < 0x1000) {
            *(s32 *)ctl = *(s32 *)(ctl + 4);
        }
        break;
    case 0x50:
        if (D_80158A6C[0xB] < 3) {
            if (((TtCore30 *)D_80158A64)->unk_26 >= 0x191) {
                g->unk_02 = 0xA0;
            } else {
                g->unk_26++;
            }
        } else if (((TtCore30 *)D_80158A64)->unk_26 >= 0x1AF) {
            g->unk_02 = 0xA0;
        } else {
            g->unk_26++;
        }
        if (g->unk_22 == 0 && (buttons->unk_04 & 0x820)) {
            g->unk_02 = 0xA0;
        }
        break;
    case 0xA0:
        if (D_80158A6C[0xB] < 3) {
            r = func_800460DC();
            if ((s32)((r - func_801430C4() + 0x100) & 0xFF) < 5) {
                break;
            }
        }
        pool = (TtPoolDraw14 *)(D_80158AA8 + 0x140);
        pool->unk_00 = 0;
        pool++;
        pool->unk_00 = 0;
        list[0] = 0;
        func_800AE080(stream, 0x20);
        if ((u8)++ctl[0xB] < 4) {
            g->unk_00 = 2;
            g->unk_02 = 1;
        } else {
            ctl[0xF] = 1;
            if (g->unk_22 != 0) {
                g->unk_00 = 8;
            } else {
                g->unk_00 = 6;
            }
            g->unk_06 = 0;
            g->unk_04 = 0;
            g->unk_02 = 0;
            g->unk_0B = 0;
            g->unk_0A = 0;
            g->unk_09 = 0;
        }
        break;
    }
}

/* T-11800: matched. T-10820 dead compare locals reserve a0 in both loops; d2, assigned before the first loop and tested by an empty if inside it (FAKE), also reserves a1 there, so the hoisted 0x40/0x70 take a2/a3 as in the original. */
void func_80145370(void) {
    s32 i;
    u8 *p;
    int d1;
    int d2;

    i = 0;
    p = D_80158A98;
    d2 = (int)D_80158AA0; /* FAKE: with the empty if (d2) {} below, d2 lives through the first loop and reserves a1, so the hoisted 0x70 takes a3 as in the original; no code is emitted. T-11800 */
    for (; i < 0x40; i++) {
        /* FAKE: dead compare locals with an else branch (decomp-permuter); they only reserve registers so that the hoisted constants sit in the original's registers. T-10820 */
        if (p[0] != 0 && (d1 = (*(u16 *)(p + 4) == 0x40))) {
            *(u16 *)(p + 4) = 0x70;
            *(s16 *)(p + 0xA) = 0;
            *(s16 *)(p + 8) = 0;
            *(s16 *)(p + 6) = 0;
            p[0xF] = 0;
            if (d2) { /* FAKE: see d2 above. T-11800 */
            }
            p[0xE] = 0;
            p[0xD] = 0;
        } else {
            d1 = 0;
        }
        p += 0x78;
    }
    i = 0;
    p = D_80158AA0;
    for (; i < 0x80; i++) {
        if (p[0] != 0 && (d2 = (*(u16 *)(p + 4) == 0x40))) {
            p[0] = 0;
            *(s16 *)(p + 0xA) = 0;
            *(s16 *)(p + 8) = 0;
            *(s16 *)(p + 6) = 0;
            p[0xF] = 0;
            p[0xE] = (*(u16 *)(p + 4) == 0x40) * 0;
            p[0xD] = 0;
        } else {
            d2 = 0;
        }
        p += 0x68;
    }
}

void func_801454D8(u8 *arg0) {
    D_80158A74[0x55] = 0xA;
    func_8013BE9C(arg0 + 0x14);
}

s32 func_80145508(u8 *arg0, u8 *arg1, u8 *arg2, u8 *arg3) {
    s16 x1 = *(s16 *)arg1 + *(s16 *)arg0;
    s16 y1 = *(s16 *)(arg1 + 2) + *(s16 *)(arg0 + 2);
    s16 x2 = *(s16 *)arg3 + *(s16 *)arg2;
    s16 y2 = *(s16 *)(arg3 + 2) + *(s16 *)(arg2 + 2);
    s32 dx = x1 - x2;
    s32 dy;
    s32 a;

    if (dx < 0) {
        a = -dx;
    } else {
        a = dx;
    }
    dy = y1 - y2;
    if ((u32)a < (u32)(arg3[4] + arg1[4])) {
        if (dy < 0) {
            a = -dy;
        } else {
            a = dy;
        }
        if ((u32)a < (u32)(arg3[5] + arg1[5])) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80144D70", func_801455C8);
