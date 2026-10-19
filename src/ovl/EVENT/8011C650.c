#define MAIN_API_OVERRIDE_func_8004AFB8 /* T-24089: O.BIN entry returns s32 in $v0, reserving r2 across call */
#include "common.h"
#include "ovl/EVENT.h"

/* FAKE: O.BIN entry returns a status in $v0, reserving r2 across the call in func_8011C750. T-24089 */
s32 func_8004AFB8();
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/EVENT/data/EVENT/8011C650.data", D_801258C0);
INCLUDE_RODATA("asm/ovl/EVENT/data/EVENT/8011C650.data", D_801258C4);
INCLUDE_RODATA("asm/ovl/EVENT/data/EVENT/8011C650.data", D_801258C8);
INCLUDE_RODATA("asm/ovl/EVENT/data/EVENT/8011C650.data", D_801258CC);
INCLUDE_RODATA("asm/ovl/EVENT/data/EVENT/8011C650.data", D_801258D0);
INCLUDE_RODATA("asm/ovl/EVENT/data/EVENT/8011C650.data", D_801258D4);
INCLUDE_RODATA("asm/ovl/EVENT/data/EVENT/8011C650.data", D_801258D8);
INCLUDE_RODATA("asm/ovl/EVENT/data/EVENT/8011C650.data", D_801258DC);
INCLUDE_RODATA("asm/ovl/EVENT/data/EVENT/8011C650.data", D_801258E0);
/* T-22009: the four halfword arrays are defined here so IDO shares one lui $at between the stores of
 * func_8011EDFC. D_801258FC ends at 0x80125902; D_80125904 is the zero fill that follows it up to the last
 * word of the overlay file, D_80125FFC (0x10124FE6; no EVENT code references it). */
s16 D_801258E4[3] = { 0, 0, 0 };
s16 D_801258EC[3] = { 0, 0, 0 };
s16 D_801258F4[3] = { 0, 0, 0 };
s16 D_801258FC[3] = { 0, 0, 0 };
u8 D_80125904[0x6F8] = { 0 };
u32 D_80125FFC = 0x10124FE6;

void func_8011C650(void) {
    D_801258C0 = 0x801C6000;
    D_801258C4 = 0x801AE000;
    D_801258C8 = 0x801B2000;
    D_801258CC = 0x801B6000;
    D_801258D0 = 0x801BA000;
    D_801258D4 = 0x801BE000;
    D_801258D8 = 0x801C2000;
}

void func_8011C6C4(void) {
    switch (*(u32 *)&D_80125320) {
    case 0:
        func_8011DDB8();
        return;
    case 1:
        func_8011EDFC();
        return;
    case 2:
        func_8011D5C4();
        return;
    case 3:
        func_8011C750();
        return;
    case 4:
        func_8011C750();
        return;
    }
}

u16 func_8011C750(void) {
    EventMovieState *g;
    s16 i;
    s16 v;
    u8 x;
    RECT rect;
    s32 sp50;
    u32 sp4C;
    Rec24 *p;
    EventMovieWork *movie;
    u8 value;
    u32 color;
    u32 timer;

    g = (EventMovieState *)&D_800B1AF0;
    movie = (EventMovieWork *)D_800F1990;

    switch (g->unk_00) {
    case 0:
        D_800EED08 = 1;
        func_8011F530(D_800B1AE4);
        func_800433D0(0x207);
        movie->unk_06 = 0;
        movie->unk_02 = 0;
        movie->unk_04 = 0;
        movie->unk_00 = 0;
        movie->unk_19 = 0;
        movie->unk_18 = 0;
        D_80080710 = -1;
        movie->unk_0A = 0;
        movie->unk_1A = 0;
        func_800352B4(0);
        D_801258E0 = 2;
        g->unk_09 = 0;
        D_801258DC = 0; for (i = 0; i < 2; i++) { /* FAKE: one source line keeps the as1 store order (as in func_801348F0) T-24018 */
            ((Rec24 *)D_800EC190)[i + 4].unk_08 = (i << 6) + 0x80;
            ((Rec24 *)D_800EC190)[i + 4].unk_00 = 0x82000000;
            ((Rec24 *)D_800EC190)[i + 4].unk_0A = 0;
            ((Rec24 *)D_800EC190)[i + 4].unk_0E = 0;
            ((Rec24 *)D_800EC190)[i + 4].unk_0F = 0xA0;
            ((Rec24 *)D_800EC190)[i + 4].unk_10 = 0;
            ((Rec24 *)D_800EC190)[i + 4].unk_0C = (i * 2) + 0x15;
            ((Rec24 *)D_800EC190)[i + 4].unk_12 = 0x1E0;
            ((Rec24 *)D_800EC190)[i + 4].unk_14 = 0;
            ((Rec24 *)D_800EC190)[i + 4].unk_15 = 0;
            ((Rec24 *)D_800EC190)[i + 4].unk_16 = 0;
            ((Rec24 *)D_800EC190)[i + 4].unk_18 = 0;
            ((Rec24 *)D_800EC190)[i + 4].unk_1A = 0;
            ((Rec24 *)D_800EC190)[i + 4].unk_1C = 0x1000;
            ((Rec24 *)D_800EC190)[i + 4].unk_1E = 0x1000;
            ((Rec24 *)D_800EC190)[i + 4].unk_20 = 0;
            ((Rec24 *)D_800EC190)[i + 4].unk_04 = (i << 7) - 0xA0;
            ((Rec24 *)D_800EC190)[i + 4].unk_06 = -0x78;
            ((EventEffectTables *)D_800B0860)->unk_1031[i] = 0xB;
        }
        for (i = 0; i < 2; i++) {
            ((Rec24 *)D_800EC190)[i + 6].unk_08 = (i << 6) + 0x80;
            ((Rec24 *)D_800EC190)[i + 6].unk_00 = 0x82000000;
            ((Rec24 *)D_800EC190)[i + 6].unk_0A = 0xA0;
            ((Rec24 *)D_800EC190)[i + 6].unk_0E = 0;
            ((Rec24 *)D_800EC190)[i + 6].unk_0F = 0;
            ((Rec24 *)D_800EC190)[i + 6].unk_10 = 0;
            ((Rec24 *)D_800EC190)[i + 6].unk_0C = (i * 2) + 0xA;
            ((Rec24 *)D_800EC190)[i + 6].unk_12 = 0x1E0;
            ((Rec24 *)D_800EC190)[i + 6].unk_14 = 0;
            ((Rec24 *)D_800EC190)[i + 6].unk_15 = 0;
            ((Rec24 *)D_800EC190)[i + 6].unk_16 = 0;
            ((Rec24 *)D_800EC190)[i + 6].unk_18 = 0;
            ((Rec24 *)D_800EC190)[i + 6].unk_1A = 0;
            ((Rec24 *)D_800EC190)[i + 6].unk_1C = 0x1000;
            ((Rec24 *)D_800EC190)[i + 6].unk_1E = 0x1000;
            ((Rec24 *)D_800EC190)[i + 6].unk_20 = 0;
            ((Rec24 *)D_800EC190)[i + 6].unk_04 = (i << 7) - 0xA0;
            ((Rec24 *)D_800EC190)[i + 6].unk_06 = -0x78;
            ((EventEffectTables *)D_800B0860)->unk_1031[i + 2] = 0xB;
        }
        func_8002746C(0x433B, 1);
        g->unk_00 += 1;
        break;
    case 1:
        if (func_80015A44() & 1) {
            func_8002469C(0x140, 0x100, 0x180, 0xA0);
            rect.x = 0x140;
            rect.y = 0x100;
            rect.w = 0x180;
            rect.h = 0xA0;
            func_800651F8(&rect, (void *)0x80183000);
            g->unk_00 += 1;
        }
        break;
    case 2:
        func_8002746C(0x4344, 1);
        g->unk_00 += 1;
        break;
    case 3:
        if (func_80015A44() & 1) {
            func_8002469C(0x280, 0, 0x180, 0xA0);
            rect.x = 0x280;
            rect.y = 0;
            rect.w = 0x180;
            rect.h = 0xA0;
            func_800651F8(&rect, (void *)0x801A1000);
            g->unk_00 += 1;
        }
        break;
    case 4:
        func_8002746C(0x434E, 1);
        g->unk_00 += 1;
        break;
    case 5:
        if (func_80015A44() & 1) {
            func_8002469C(0x140, 0x100, 0x180, 0xA0);
            rect.x = 0x140;
            rect.y = 0x100;
            rect.w = 0x180;
            rect.h = 0xA0;
            func_800651F8(&rect, (void *)0x801BF000);
            g->unk_00 += 1;
        }
        break;
    case 6:
        func_8002746C(0x4357, 1);
        g->unk_00 += 1;
        break;
    case 7:
        if (func_80015A44() & 1) {
            func_8002469C(0x280, 0, 0x180, 0xA0);
            rect.x = 0x280;
            rect.y = 0;
            rect.w = 0x180;
            rect.h = 0xA0;
            func_800651F8(&rect, (void *)0x801DD000);
            g->unk_00 += 1;
        }
        break;
    case 8:
        for (i = 0; i < 4; i++) {
            ((Rec24 *)D_800EC190)[i + 4].unk_00 = 0x02000000;
        }
        g->unk_00 += 1;
        break;
    case 9:
        value = ((Rec24 *)D_800EC190)[4].unk_14;
        value += 2;
        color = (value & 0xFF);
        ((Rec24 *)D_800EC190)[7].unk_16 = color;
        ((Rec24 *)D_800EC190)[7].unk_15 = color;
        ((Rec24 *)D_800EC190)[7].unk_14 = color;
        ((Rec24 *)D_800EC190)[6].unk_16 = color;
        ((Rec24 *)D_800EC190)[6].unk_15 = color;
        ((Rec24 *)D_800EC190)[6].unk_14 = color;
        ((Rec24 *)D_800EC190)[5].unk_16 = color;
        ((Rec24 *)D_800EC190)[5].unk_15 = color;
        ((Rec24 *)D_800EC190)[5].unk_14 = color;
        ((Rec24 *)D_800EC190)[4].unk_16 = color;
        ((Rec24 *)D_800EC190)[4].unk_15 = color;
        ((Rec24 *)D_800EC190)[4].unk_14 = color;
        if (value == 0x80) {
            g->unk_00 += 1;
        }
        break;
    case 10:
        break;
    case 11:
        value = ((Rec24 *)D_800EC190)[4].unk_14;
        value -= 2;
        color = (value & 0xFF);
        ((Rec24 *)D_800EC190)[7].unk_16 = color;
        ((Rec24 *)D_800EC190)[7].unk_15 = color;
        ((Rec24 *)D_800EC190)[7].unk_14 = color;
        ((Rec24 *)D_800EC190)[6].unk_16 = color;
        ((Rec24 *)D_800EC190)[6].unk_15 = color;
        ((Rec24 *)D_800EC190)[6].unk_14 = color;
        ((Rec24 *)D_800EC190)[5].unk_16 = color;
        ((Rec24 *)D_800EC190)[5].unk_15 = color;
        ((Rec24 *)D_800EC190)[5].unk_14 = color;
        ((Rec24 *)D_800EC190)[4].unk_16 = color;
        ((Rec24 *)D_800EC190)[4].unk_15 = color;
        ((Rec24 *)D_800EC190)[4].unk_14 = color;
        if (value == 0) {
            for (i = 0; i < 4; i++) {
                ((Rec24 *)D_800EC190)[i + 4].unk_00 = 0x82000000;
            }
            g->unk_00 += 1;
        }
        break;
    case 12:
        sp50 = D_800B1AF6;
        sp4C = g->unk_00;
        func_8004AFB8();
        g->unk_00 = sp4C;
        if (sp50 != D_800B1AF6) {
            D_800B1AF6--;
            g->unk_00 += 1;
        }
        break;
    case 13:
    case 16:
    case 19:
    case 21:
    case 23:
        if (func_80015A44() & 1) {
            g->unk_00 += 1;
        }
        break;
    case 14:
        sp4C = g->unk_00;
        func_8004A9CC();
        D_800B1AF6 -= 1;
        g->unk_00 = sp4C + 1;
        break;
    case 15:
        switch (*(u32 *)&D_80125320) {
        case 0:
            func_800469F4(0x42AE);
            break;
        case 1:
            func_800469F4(0x42B7);
            break;
        case 2:
            func_800469F4(0x42C0);
            break;
        case 3:
            func_800469F4(0x42C9);
            break;
        case 4:
            func_800469F4(0x42D2);
            break;
        }
        g->unk_00 += 1;
        break;
    case 17:
        func_8002755C(1);
        func_80027438(1);
        g->unk_00 += 1;
        break;
    case 18:
        sp4C = g->unk_00;
        func_8011A62C();
        g->unk_06 -= 1;
        g->unk_00 = sp4C + 1;
        break;
    case 20:
        sp4C = g->unk_00;
        func_8011A7FC();
        g->unk_06 -= 1;
        g->unk_00 = sp4C + 1;
        break;
    case 22:
        sp4C = g->unk_00;
        func_8011A838();
        g->unk_06 -= 1;
        g->unk_00 = sp4C + 1;
        break;
    default:
        func_800352B4(1);
        D_800EED08 = 0;
        func_800433D0(0x206);
        func_80011DFC();
        break;
    }
    if (D_800B1AF0 < 9U || D_800B1AF0 >= 12U) {
        return 0;
    }
    p = ((Rec24 *)D_800EC190);
    v = p[4].unk_0E;
    v += D_801258E0;
    if (v < 0) {
        v = 0;
        D_801258E0 = (func_800789A0() % 4) + 2;
    }
    if (v >= 0x40) {
        v = 0x3F;
        D_801258E0 = -2 - (func_800789A0() % 4);
    }
    x = p[4].unk_0F;
    p[7].unk_0E = v;
    p[6].unk_0E = v;
    p[5].unk_0E = v;
    p[4].unk_0E = v;
    if (x != 0) {
        p[4].unk_0A += 0x14;
        p[5].unk_0A += 0x14;
        x -= 0x14;
        p[5].unk_0F -= 0x14;
    } else {
        p[4].unk_0A -= 0x14;
        p[4].unk_06 += 0x14;
        p[5].unk_0A -= 0x14;
        p[5].unk_06 += 0x14;
    }
    if (p[6].unk_0F != 0) {
        p[6].unk_0A += 0x14;
        p[7].unk_0A += 0x14;
        p[6].unk_0F -= 0x14;
        p[7].unk_0F -= 0x14;
    } else {
        p[6].unk_0A -= 0x14;
        p[6].unk_06 += 0x14;
        p[7].unk_0A -= 0x14;
        p[7].unk_06 += 0x14;
    }
    p[4].unk_0F = x;
    if ((x == 0 && (g->unk_09 == 1 || g->unk_09 == 3)) || (p[6].unk_0F == 0 && (g->unk_09 == 0 || g->unk_09 == 2))) {
        switch (g->unk_09) {
        case 0:
            for (i = 0; i < 2; i++) {
                ((Rec24 *)D_800EC190)[i + 4].unk_0A = 0;
                ((Rec24 *)D_800EC190)[i + 4].unk_0F = 0xA0;
                ((Rec24 *)D_800EC190)[i + 4].unk_06 = -0x78;
                ((Rec24 *)D_800EC190)[i + 4].unk_0C = (i * 2) + 0x15;
            }
            func_800137AC(0x140, 0x100, 0x180, 0xA0, 0x80183000);
            break;
        case 1:
            for (i = 0; i < 2; i++) {
                ((Rec24 *)D_800EC190)[i + 6].unk_0A = 0;
                ((Rec24 *)D_800EC190)[i + 6].unk_0F = 0xA0;
                ((Rec24 *)D_800EC190)[i + 6].unk_06 = -0x78;
                ((Rec24 *)D_800EC190)[i + 6].unk_0C = (i * 2) + 0xA;
            }
            func_800137AC(0x280, 0, 0x180, 0xA0, 0x801A1000);
            break;
        case 2:
            for (i = 0; i < 2; i++) {
                ((Rec24 *)D_800EC190)[i + 4].unk_0A = 0;
                ((Rec24 *)D_800EC190)[i + 4].unk_0F = 0xA0;
                ((Rec24 *)D_800EC190)[i + 4].unk_06 = -0x78;
                ((Rec24 *)D_800EC190)[i + 4].unk_0C = (i * 2) + 0x15;
            }
            func_800137AC(0x140, 0x100, 0x180, 0xA0, 0x801BF000);
            break;
        case 3:
            for (i = 0; i < 2; i++) {
                ((Rec24 *)D_800EC190)[i + 6].unk_0A = 0;
                ((Rec24 *)D_800EC190)[i + 6].unk_0F = 0xA0;
                ((Rec24 *)D_800EC190)[i + 6].unk_06 = -0x78;
                ((Rec24 *)D_800EC190)[i + 6].unk_0C = (i * 2) + 0xA;
            }
            func_800137AC(0x280, 0, 0x180, 0xA0, 0x801DD000);
            break;
        }
        if (++g->unk_09 >= 4) {
            g->unk_09 = 0;
        }
    }
    timer = (D_801258DC + 1) & 0xFFFF;
    D_801258DC = timer;
    if (timer == 0x12C) {
        D_800B1AF0 += 1;
    }
}

s32 func_8011D5C4(void) {
    /* FAKE: unused RECT reserves the observed frame words. T-24028. */
    RECT rect;
    EventMovieState *g;
    u32 sp30;
    s16 var_v1;
    u8 x;
    /* FAKE: second fade copy holds v1 in cases 6/7/9; dead stores in the tail block cvt_pass widening so uopt promotes D_801258DC to a3. T-24028, T-24089. */
    u32 y;
    EventMovieWork *movie;

    g = (EventMovieState *)&D_800B1AF0;
    movie = (EventMovieWork *)D_800F1990;

    switch (g->unk_00) {
    case 0:
        D_800EED08 = 1;
        func_800433D0(0x203);
        movie->unk_06 = 0;
        movie->unk_02 = 0;
        movie->unk_04 = 0;
        movie->unk_00 = 0;
        movie->unk_19 = 0;
        movie->unk_18 = 0;
        D_80080710 = -1;
        movie->unk_0A = 0;
        movie->unk_1A = 0;
        func_800352B4(0);
        /* FAKE: one source line keeps the as1 store order of the original. T-24028. */
        D_801258DC = 0; for (var_v1 = 0; var_v1 < 2; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_08 = (var_v1 << 6) + 0x80;
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_00 = 0x82000000;
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_0A = 0xa0;
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_0E = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_0F = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_10 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_0C = (var_v1 * 2) + 0x15;
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_12 = 0x1E0;
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_14 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_15 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_16 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_18 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_1A = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_1C = 0x1000;
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_1E = 0x1000;
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_20 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_04 = (var_v1 << 7) - 0xA0;
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_06 = -0x96;
            D_800B0860[var_v1 + 0x1037] = 0xC;
        }
        func_8002746C(0x4332, 1);
        g->unk_00 += 1;
        return 0;
    case 1:
        if (func_80015A44() & 1) {
            func_8002469C(0x140, 0x100, 0x140, 0xA0);
            g->unk_00 += 1;
        }
        return 0;
    case 2:
        func_800433D0(0x502);
        func_80015D28(0x31, 0x801AE000, 0xACA8);
        func_8011C650();
        g->unk_00 += 1;
        return 0;
    case 3:
    case 11:
    case 14:
    case 16:
    case 18:
        if (func_80015A44() & 1) {
            g->unk_00 += 1;
        }
        return 0;
    case 4:
        func_80012D64(D_801258C0, 0x11, 1, 0, 0);
        func_8004C46C(D_801258C4, D_801258C8, D_801258CC, D_801258D0, D_801258D4, D_801258D8);
        g->unk_00 += 1;
        return 0;
    case 5:
        for (var_v1 = 0; var_v1 < 8; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_00 &= 0x7FFFFFFF;
        }
        func_800433D0(0x503);
        g->unk_00 += 1;
        return 0;
    case 6:
        x = (((Rec24 *)D_800EC190)[4].unk_14 += 1); y = ((Rec24 *)D_800EC190)[4].unk_15 = x; ((Rec24 *)D_800EC190)[4].unk_16 = y; /* FAKE: one source line preserves store order. T-24028. */
        for (var_v1 = 1; var_v1 < 8; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_14 = ((Rec24 *)D_800EC190)[var_v1 + 4].unk_15 = ((Rec24 *)D_800EC190)[var_v1 + 4].unk_16 = ((Rec24 *)D_800EC190)[4].unk_14;
        }
        if (((Rec24 *)D_800EC190)[4].unk_14 == 0x40) {
            g->unk_00 += 1;
        }
        return 0;
    case 7:
        x = (((Rec24 *)D_800EC190)[4].unk_14 += 1); y = ((Rec24 *)D_800EC190)[4].unk_15 = x; ((Rec24 *)D_800EC190)[4].unk_16 = y; /* FAKE: one source line preserves store order. T-24028. */
        for (var_v1 = 1; var_v1 < 8; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_14 = ((Rec24 *)D_800EC190)[var_v1 + 4].unk_15 = ((Rec24 *)D_800EC190)[var_v1 + 4].unk_16 = ((Rec24 *)D_800EC190)[4].unk_14;
        }
        if (((Rec24 *)D_800EC190)[4].unk_14 == 0x80) {
            g->unk_00 += 1;
        }
        break;
    case 8:
        break;
    case 9:
        x = (((Rec24 *)D_800EC190)[4].unk_14 -= 2); y = ((Rec24 *)D_800EC190)[4].unk_15 = x; ((Rec24 *)D_800EC190)[4].unk_16 = y; /* FAKE: one source line preserves store order. T-24028. */
        for (var_v1 = 1; var_v1 < 8; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_14 = ((Rec24 *)D_800EC190)[var_v1 + 4].unk_15 = ((Rec24 *)D_800EC190)[var_v1 + 4].unk_16 = ((Rec24 *)D_800EC190)[4].unk_14;
        }
        if (((Rec24 *)D_800EC190)[4].unk_14 == 0) {
            for (var_v1 = 0; var_v1 < 8; var_v1++) {
                ((Rec24 *)D_800EC190)[var_v1 + 4].unk_00 |= 0x80000000;
            }
            g->unk_00 += 1;
        }
        break;
    case 10:
        switch (*(u32 *)&D_80125320) {
        case 0:
            func_800469F4(0x42AE);
            break;
        case 1:
            func_800469F4(0x42B7);
            break;
        case 2:
            func_800469F4(0x42C0);
            break;
        case 3:
            func_800469F4(0x42C9);
            break;
        case 4:
            func_800469F4(0x42D2);
            break;
        }
        g->unk_00 += 1;
        return 0;
    case 12:
        func_8002755C(1);
        func_80027438(1);
        g->unk_00 += 1;
        return 0;
    case 13:
        sp30 = g->unk_00;

        func_8011A62C();
        D_800B1AF6 -= 1;
        g->unk_00 = sp30;
        g->unk_00++;
        return 0;
    case 15:
        sp30 = g->unk_00;

        func_8011A7FC();
        D_800B1AF6 -= 1;
        g->unk_00 = sp30;
        g->unk_00++;
        return 0;
    case 17:
        sp30 = g->unk_00;

        func_8011A838();
        D_800B1AF6 -= 1;
        g->unk_00 = sp30;
        g->unk_00++;
        return 0;
    default:
        func_80011DFC();
        D_800EED08 = 0;
        func_800433D0(0x206);
        func_800352B4(1);
        return 0;
    }
    D_801258DC += (y = 1);
    if (D_801258DC % (y = 19U) == 0x12) {
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 10].unk_06 += 1;
        }
        ((Rec24 *)D_800EC190)[var_v1 + 10].unk_0A -= 1;
    }
    if (D_801258DC % (y = 5U) == 4) {
        for (var_v1 = 0; var_v1 < 6; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_06 += 1;
        }
        for (var_v1 = 3; var_v1 < 6; var_v1++) {
            if (((Rec24 *)D_800EC190)[var_v1 + 4].unk_0A != 0) {
                ((Rec24 *)D_800EC190)[var_v1 + 4].unk_0A -= 1;
            } else {
                ((Rec24 *)D_800EC190)[var_v1 + 1].unk_0A -= 1;
            }
        }
    }
    if (D_801258DC == (y = 0x12C)) {
        g->unk_00 += 1;
    }
}

void func_8011DDB8(void) {
    EventMovieState *g;
    s16 var_v1;
    s32 sp54;
    u32 sp50;
    RECT rect;
    EventMovieWork *movie;
    /* FAKE: unused view reserves the observed 0x60 frame slot (as in DATE func_80147958). T-24097 */
    EventMovieWork *header;
    u8 value;
    u32 color;

    g = (EventMovieState *)&D_800B1AF0;
    movie = (EventMovieWork *)D_800F1990;

    switch (g->unk_00) {
    case 0:
        D_800EED08 = 1;
        movie->unk_06 = 0;
        movie->unk_02 = 0;
        movie->unk_04 = 0;
        movie->unk_00 = 0;
        movie->unk_19 = 0;
        movie->unk_18 = 0;
        D_80080710 = -1;
        movie->unk_0A = 0;
        movie->unk_1A = 0;
        func_800352B4(0);
        D_801258DC = 0;
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_08 = (var_v1 << 6) + 0x80;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_00 = 0x82000000;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_0A = 0xA0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_0E = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_0F = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_10 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_0C = (var_v1 * 2) + 0x15;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_12 = 0x1E0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_14 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_15 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_16 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_18 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_1A = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_1C = 0x1000;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_1E = 0x1000;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_20 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_04 = (var_v1 << 7) - 0xA0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_06 = -0x78;
            ((EventEffectTables *)D_800B0860)->unk_1031[var_v1] = 0xB;
        }
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_08 = (var_v1 << 6) + 0x80;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_00 = 0x82000000;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_0A = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_0E = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_0F = 0xA0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_10 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_0C = (var_v1 * 2) + 0xA;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_12 = 0x1E0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_14 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_15 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_16 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_18 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_1A = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_1C = 0x1000;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_1E = 0x1000;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_20 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_04 = (var_v1 << 7) - 0xA0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_06 = -0x78;
            ((EventEffectTables *)D_800B0860)->unk_1031[var_v1 + 2] = 0xB;
        }
        func_8002746C(0x4321, 1);
        g->unk_00 += 1;
        return;
    case 1:
    case 4:
    case 7:
    case 10:
    case 13:
    case 16:
    case 19:
    case 27:
    case 41:
    case 44:
    case 47:
    case 49:
        if (func_80015A44() & 1) {
            g->unk_00 += 1;
            return;
        }
        return;
    case 2:
        func_8002469C(0x140, 0x100, 0x140, 0xA0);
        rect.x = 0x140;
        rect.y = 0x100;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_800651F8(&rect, (void *)0x80183000);
        g->unk_00 += 1;
        return;
    case 3:
        func_8002746C(0x431A, 1);
        g->unk_00 += 1;
        return;
    case 5:
        func_8002469C(0x140, 0x100, 0x140, 0xA0);
        rect.x = 0x140;
        rect.y = 0x100;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_800651F8(&rect, (void *)0x8019C000);
        g->unk_00 += 1;
        return;
    case 6:
        func_8002746C(0x4313, 1);
        g->unk_00 += 1;
        return;
    case 8:
        func_8002469C(0x140, 0x100, 0x140, 0xA0);
        rect.x = 0x140;
        rect.y = 0x100;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_800651F8(&rect, (void *)0x801B5000);
        g->unk_00 += 1;
        return;
    case 9:
        func_8002746C(0x430C, 1);
        g->unk_00 += 1;
        return;
    case 11:
        func_8002469C(0x140, 0x100, 0x140, 0xA0);
        rect.x = 0x140;
        rect.y = 0x100;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_800651F8(&rect, (void *)0x801CE000);
        g->unk_00 += 1;
        return;
    case 12:
        func_8002746C(0x4305, 1);
        g->unk_00 += 1;
        return;
    case 14:
        func_8002469C(0x140, 0, 0x140, 0xA0);
        g->unk_00 += 1;
        return;
    case 15:
        func_8002746C(0x42ED, 1);
        g->unk_00 += 1;
        return;
    case 17:
        func_8002469C(0x140, 0x100, 0x140, 0xA0);
        g->unk_00 += 1;
        return;
    case 18:
        func_8002746C(0x42F5, 1);
        g->unk_00 += 1;
        return;
    case 20:
        func_8002469C(0x280, 0, 0x140, 0xA0);
        g->unk_00 += 1;
        return;
    case 21:
        for (var_v1 = 0; var_v1 < 4; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_00 &= 0x7FFFFFFF;
        }
        g->unk_00 += 1;
        return;
    case 22:
        value = ((Rec24 *)D_800EC190)[4].unk_14;
        value += 2;
        color = (value & 0xFF);
        ((Rec24 *)D_800EC190)[7].unk_16 = color;
        ((Rec24 *)D_800EC190)[7].unk_15 = color;
        ((Rec24 *)D_800EC190)[7].unk_14 = color;
        ((Rec24 *)D_800EC190)[6].unk_16 = color;
        ((Rec24 *)D_800EC190)[6].unk_15 = color;
        ((Rec24 *)D_800EC190)[6].unk_14 = color;
        ((Rec24 *)D_800EC190)[5].unk_16 = color;
        ((Rec24 *)D_800EC190)[5].unk_15 = color;
        ((Rec24 *)D_800EC190)[5].unk_14 = color;
        ((Rec24 *)D_800EC190)[4].unk_16 = color;
        ((Rec24 *)D_800EC190)[4].unk_15 = color;
        ((Rec24 *)D_800EC190)[4].unk_14 = color;
        if (value == 0x80) {
            g->unk_00 += 1;
            func_800433D0(0x500);
        }
        break;
    case 23:
        if (!(D_801258DC & 7)) {
            func_8011ED50(4);
        }
        if (++D_801258DC == 0x140) {
            D_801258DC = 0;
            D_800B1AF0 += 1;
        }
        break;
    case 24:
        g->unk_00 += 1;
        return;
    case 25:
        func_80012D40(0);
        for (var_v1 = 0; var_v1 < 4; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_00 |= 0x80000000;
        }
        g->unk_00 += 1;
        return;
    case 26:
        func_8002746C(0x42FD, 1);
        g->unk_00 += 1;
        return;
    case 28:
        func_8002469C(0x140, 0x100, 0x140, 0xA0);
        rect.x = 0x140;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_80065234(&rect, 0x280, 0);
        g->unk_00 += 1;
        func_80012D40(1);
        return;
    case 29:
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_00 = 0x02000000;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_0A = 0xA0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_0F = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_06 = -0x78;
        }
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_00 = 0x02000000;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_0A = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_0F = 0xA0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_06 = -0x78;
        }
        D_801258DC = 0;
        func_800433D0(0x501);
        g->unk_00 += 1;
        return;
    case 30:
    case 32:
    case 34:
        func_8011ED50(8);
        if (++D_801258DC == 0x14) {
            g->unk_00 += 1;
            return;
        }
        return;
    case 31:
        rect.x = 0x280;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_80065234(&rect, 0x140, 0x100);
        func_800137AC(0x280U, 0U, 0x140U, 0xA0U, 0x801CE000);
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_0A = 0xA0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_0F = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_06 = -0x78;
        }
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_0A = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_0F = 0xA0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_06 = -0x78;
        }
        D_801258DC = 0;
        g->unk_00 += 1;
        return;
    case 33:
        rect.x = 0x280;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_80065234(&rect, 0x140, 0x100);
        func_800137AC(0x280U, 0U, 0x140U, 0xA0U, 0x801B5000);
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_0A = 0xA0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_0F = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_06 = -0x78;
        }
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_0A = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_0F = 0xA0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_06 = -0x78;
        }
        D_801258DC = 0;
        g->unk_00 += 1;
        return;
    case 35:
        rect.x = 0x280;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_80065234(&rect, 0x140, 0x100);
        func_800137AC(0x280U, 0U, 0x140U, 0xA0U, 0x8019C000);
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_0A = 0xA0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_0F = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_06 = -0x78;
        }
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_0A = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_0F = 0xA0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_06 = -0x78;
        }
        D_801258DC = 0;
        g->unk_00 += 1;
        return;
    case 36:
        func_8011ED50(8);
        if ((u16) D_801258DC >= 8U) {
            value = ((Rec24 *)D_800EC190)[4].unk_14;
            value -= 4;
            color = (value & 0xFF);
            ((Rec24 *)D_800EC190)[7].unk_16 = color;
            ((Rec24 *)D_800EC190)[7].unk_15 = color;
            ((Rec24 *)D_800EC190)[7].unk_14 = color;
            ((Rec24 *)D_800EC190)[6].unk_16 = color;
            ((Rec24 *)D_800EC190)[6].unk_15 = color;
            ((Rec24 *)D_800EC190)[6].unk_14 = color;
            ((Rec24 *)D_800EC190)[5].unk_16 = color;
            ((Rec24 *)D_800EC190)[5].unk_15 = color;
            ((Rec24 *)D_800EC190)[5].unk_14 = color;
            ((Rec24 *)D_800EC190)[4].unk_16 = color;
            ((Rec24 *)D_800EC190)[4].unk_15 = color;
            ((Rec24 *)D_800EC190)[4].unk_14 = color;
        }
        if (++D_801258DC == 0x14) {
            g->unk_00 += 1;
            return;
        }
        return;
    case 37:
        rect.x = 0x280;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_80065234(&rect, 0x140, 0x100);
        func_800137AC(0x280U, 0U, 0x140U, 0xA0U, 0x80183000);
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_0A = 0xA0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_0F = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_06 = -0x78;
        }
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_0A = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_0F = 0xA0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_06 = -0x78;
        }
        D_801258DC = 0;
        g->unk_00 += 1;
        return;
    case 38:
        func_8011ED50(8);
        value = ((Rec24 *)D_800EC190)[4].unk_14;
        value -= 4;
        color = (value & 0xFF);
        ((Rec24 *)D_800EC190)[7].unk_16 = color;
        ((Rec24 *)D_800EC190)[7].unk_15 = color;
        ((Rec24 *)D_800EC190)[7].unk_14 = color;
        ((Rec24 *)D_800EC190)[6].unk_16 = color;
        ((Rec24 *)D_800EC190)[6].unk_15 = color;
        ((Rec24 *)D_800EC190)[6].unk_14 = color;
        ((Rec24 *)D_800EC190)[5].unk_16 = color;
        ((Rec24 *)D_800EC190)[5].unk_15 = color;
        ((Rec24 *)D_800EC190)[5].unk_14 = color;
        ((Rec24 *)D_800EC190)[4].unk_16 = color;
        ((Rec24 *)D_800EC190)[4].unk_15 = color;
        ((Rec24 *)D_800EC190)[4].unk_14 = color;

        if (value < 4U) {
            g->unk_00 += 1;
            return;
        }
        return;
    case 39:
        for (var_v1 = 0; var_v1 < 4; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_00 |= 0x80000000;
        }
        g->unk_00 += 1;
        return;
    case 40:
        sp54 = g->unk_06;
        sp50 = g->unk_00;
        func_8004AFB8();
        g->unk_00 = sp50;
        if (sp54 != g->unk_06) {
            g->unk_06--;
            g->unk_00 += 1;
        }
        return;
    case 42:
        sp50 = g->unk_00;
        func_8004A9CC();
        g->unk_06 -= 1;
        g->unk_00 = sp50 + 1;
        return;
    case 43:
        switch (*(u32 *)&D_80125320) {
        case 0:
            func_800469F4(0x42AE);
            break;
        case 1:
            func_800469F4(0x42B7);
            break;
        case 2:
            func_800469F4(0x42C0);
            break;
        case 3:
            func_800469F4(0x42C9);
            break;
        case 4:
            func_800469F4(0x42D2);
            break;
        }
        g->unk_00 += 1;
        return;
    case 45:
        func_8002755C(1);
        func_80027438(1);
        g->unk_00 += 1;
        return;
    case 46:
        sp50 = g->unk_00;
        func_8011A62C();
        g->unk_06 -= 1;
        g->unk_00 = sp50 + 1;
        return;
    case 48:
        sp50 = g->unk_00;
        func_8011A7FC();
        g->unk_06 -= 1;
        g->unk_00 = sp50 + 1;
        return;
    case 50:
        sp50 = g->unk_00;
        func_8011A838();
        g->unk_06 -= 1;
        g->unk_00 = sp50 + 1;
        return;
    default:
        func_80011DFC();
        D_800EED08 = 0;
        func_800433D0(0x206);
        func_800352B4(1);
        return;
    }
}

/* T-10040: the implicit result view matches DATE func_80148924. */
s32 func_8011ED50(s16 arg0) {
    ((Rec24 *)D_800EC190)[4].unk_06 += arg0;
    ((Rec24 *)D_800EC190)[5].unk_06 += arg0;
    ((Rec24 *)D_800EC190)[4].unk_0A -= arg0;
    ((Rec24 *)D_800EC190)[5].unk_0A -= arg0;
    ((Rec24 *)D_800EC190)[6].unk_0A += arg0;
    ((Rec24 *)D_800EC190)[7].unk_0A += arg0;
    ((Rec24 *)D_800EC190)[6].unk_0F -= arg0;
    ((Rec24 *)D_800EC190)[7].unk_0F -= arg0;
}

void func_8011EDFC(void) {
    s16 *temp_v0;
    s16 *var_a0;
    s16 temp_v0_2;
    s16 var_a1;
    s16 var_s3;
    s32 temp_s1;
    s32 temp_s1_3;
    u8 temp_s0;
    u8 temp_s0_2;

    switch (((EventTransitionState *)&D_800B1AF0)->unk_00) {
    case 0:
        D_800EED08 = 1;
        func_8011F530(D_800B1AE4);
        func_800433D0(0x202);
        D_801258DC = 0;
        func_800352B4(0);
        func_800469F4(0x432A);
        D_801258E4[0] = -0x50;
        D_801258E4[1] = 0;
        D_801258E4[2] = 0x64;
        D_801258EC[0] = -0x28;
        D_801258EC[1] = 0;
        D_801258EC[2] = -0x3C;
        ((EventTransitionState *)&D_800B1AF0)->unk_00 += 1;
        D_801258F4[0] = (func_800789A0() % 3) - 1;
        D_801258F4[1] = (func_800789A0() % 3) - 1;
        D_801258F4[2] = (func_800789A0() % 3) - 1;
        D_801258FC[0] = (func_800789A0() % 3) - 1;
        D_801258FC[1] = (func_800789A0() % 3) - 1;
        D_801258FC[2] = (func_800789A0() % 3) - 1;
        break;
    case 1:
        if (func_80015A44() & 1) {
            ((EventTransitionState *)&D_800B1AF0)->unk_00 += 1;
            break;
        }
        break;
    case 2:
        func_8002755C(1);
        func_80027438(1);
        ((EventTransitionState *)&D_800B1AF0)->unk_00 += 1;
        break;
    case 3:
        temp_s0 = ((EventTransitionState *)&D_800B1AF0)->unk_06;
        temp_s1 = ((EventTransitionState *)&D_800B1AF0)->unk_00;
        func_80046480();
        ((EventTransitionState *)&D_800B1AF0)->unk_00 = temp_s1;
        if (temp_s0 != ((EventTransitionState *)&D_800B1AF0)->unk_06) {
            ((EventTransitionState *)&D_800B1AF0)->unk_00 = temp_s1 + 1;
            func_800433D0(0x201);
        }
        ((EventTransitionState *)&D_800B1AF0)->unk_06 = temp_s0;
        break;
    case 4:
        var_s3 = 0;
        do {
            if (((u32) D_800B1AE4 % 30U) == (func_800789A0() % 30)) {
                (D_801258F4)[var_s3] = (func_800789A0() % 3) - 1;
            }

            if (((u32) D_800B1AE4 % 30U) == (func_800789A0() % 30)) {
                (D_801258FC)[var_s3] = (func_800789A0() % 3) - 1;
            }

            if ((D_801258F4[var_s3] == 0) && (D_801258FC[var_s3] == 0)) {
                if (func_800789A0() % 2) {
                    D_801258F4[var_s3] = ((func_800789A0() % 2) * 2) - 1;
                } else {
                    D_801258FC[var_s3] = ((func_800789A0() % 2) * 2) - 1;
                }
            }
            temp_v0 = &(D_801258E4)[var_s3];
            *temp_v0 += D_801258F4[var_s3];
            var_a0 = &(D_801258EC)[var_s3];
            *var_a0 += D_801258FC[var_s3];
            var_a1 = *temp_v0;
            if (var_a1 < -0x95) {
                D_801258F4[var_s3] = 1;
            }
            if (var_a1 >= 0x96) {
                D_801258F4[var_s3] = -1;
            }
            temp_v0_2 = *var_a0;
            if (temp_v0_2 < -0x6D) {
                D_801258FC[var_s3] = 1;
            }

            if (temp_v0_2 >= 0x1E) {
                D_801258FC[var_s3] = -1;

            }
            var_s3++;
        } while (var_s3 < 3);
        func_8001BB10(D_801258E4[0], D_801258EC[0], 0xA, 2, 2, 0x808080, 1, 1);
        func_8001BB10(D_801258E4[1], D_801258EC[1], 0xA, 2, 2, 0x804040, 1, 1);
        func_8001BB10(D_801258E4[2], D_801258EC[2], 0xA, 2, 2, 0x800000, 1, 1);
        if (D_801258DC++ == 0x12C) {
            ((EventTransitionState *)&D_800B1AF0)->unk_00 += 1;
            func_800433D0(0x202);
            break;
        }
        break;
    case 5:
        temp_s0_2 = ((EventTransitionState *)&D_800B1AF0)->unk_06;
        temp_s1_3 = ((EventTransitionState *)&D_800B1AF0)->unk_00;
        func_80046598();
        ((EventTransitionState *)&D_800B1AF0)->unk_00 = temp_s1_3;
        if (temp_s0_2 != ((EventTransitionState *)&D_800B1AF0)->unk_06) {
            ((EventTransitionState *)&D_800B1AF0)->unk_00 = temp_s1_3 + 1;
        }
        ((EventTransitionState *)&D_800B1AF0)->unk_06 = temp_s0_2;
        break;
    case 6:
        func_80027438(1);
        ((EventTransitionState *)&D_800B1AF0)->unk_00 += 1;
        break;
    default:
        func_800433D0(0x206);
        D_800EED08 = 0;
        func_800352B4(1);
        func_80011DFC();
        break;
    }
}
