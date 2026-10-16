#include "common.h"
#include "ovl/TT.h"

void func_80132130(void) {
    u8 *p = D_80158AA4;

    p[0] = 1;
    p[1] = 0;
    *(s16 *)(p + 0x44) = 8;
    *(s16 *)(p + 0x14) = -0x38;
    *(s16 *)(p + 0x16) = 0x48;
    *(s16 *)(p + 0x3C) = 0x70;
    *(s16 *)(p + 0x3E) = 0x10;
    p[0x46] = 0;
    p[0x47] = 0x60;
    *(s16 *)(p + 0x40) = 0xA;
    *(s16 *)(p + 0x42) = 0x3C08;
    p[0x3B] = 0x65;
    *(s16 *)(p + 0x10) = 0;
}

void func_80132198(u8 arg0, u8 arg1, u8 arg2) {
    u8 *q;
    u8 *r;
    s32 i;
    u8 c; /* set by the switch for every i; IDO still loads and stores its stack home */

    q = D_80158A60;
    r = D_80158AA8;
    for (i = 0; i != 3; i++) {
        switch (i) {
        case 0:
            c = arg0;
            break;
        case 1:
            c = arg1;
            break;
        case 2:
            c = arg2;
            break;
        }
        *(s16 *)(r + 0xA) = -0x4C;
        *(s16 *)(r + 0xC) = i * 0x10 + 0x78;
        *(s16 *)(r + 0xE) = i + 0xE;
        r[2] = c;
        r[0] = 1;
        *(s16 *)(r + 6) = 8;
        *(s16 *)(r + 0x10) = 0;
        *(s16 *)(r + 0x1E) = -0x1C;
        *(s16 *)(r + 0x20) = i * 0x10 + 0x78;
        *(s16 *)(r + 0x22) = i + 0x28;
        r[0x16] = c;
        r[0x14] = 1;
        *(s16 *)(r + 0x1A) = 8;
        *(s16 *)(r + 0x24) = 0;
        r += 0x28;
        func_80148584(*(u16 *)(q + i * 6 + 0x30), i + 0x28);
        func_801486D8((s32)(q + i * 6 + 0x32), i + 0x2B);
        *(s16 *)(r + 0xA) = 0x34;
        *(s16 *)(r + 0xC) = i * 0x10 + 0x78;
        *(s16 *)(r + 0xE) = i + 0x2B;
        r[2] = c;
        r[0] = 1;
        *(s16 *)(r + 6) = 8;
        *(s16 *)(r + 0x10) = 0;
        r += 0x14;
    }
}

/* Fills the 25 grid records and the corner, frame and panel records after them, then the colour bytes
 * for scene arg0. The u16 stores of 8, 0xD and 0xC give the original register allocation. T-11840 */
void func_8013230C(u8 arg0, u8 arg1) {
    u8 *state;
    u8 *p;
    u8 *g;
    s32 i;

    state = D_80158A64;
    /* FAKE: p, g and the loop header on one line put the hoisted (arg0 >> 4) chain ahead of the p load (as1 order). T-11840 */
    p = D_80158AA4 + 0x1900; g = D_80158AB0; for (i = 0; i < 25; i++) {
        *(s16 *)(g + 4) = (i / 5) * 0x48 - 0x5C;
        *(s16 *)(g + 6) = (i % 5) * 0x48 - 0x48;
        *(s16 *)(g + 0) = 0x1000;
        *(s16 *)(g + 2) = 0x1000;
        p[0] = 1;
        p[1] = 0;
        *(u16 *)(p + 0x44) = 8;
        p[0x3B] = 0x80;
        *(u8 **)(p + 0x48) = g;
        p[0x4D] = 0;
        p[0x4E] = 1;
        p[0x3A] = 0x40;
        p[0x39] = 0x40;
        p[0x38] = 0x40;
        p[0x50] = 0;
        *(s16 *)(p + 0x14) = *(s16 *)(g + 4) - 0x24;
        *(s16 *)(p + 0x16) = *(s16 *)(g + 6) - 0x24;
        p[0x46] = 0x38; p[0x47] = 0xB8; /* FAKE: one line for the original li order. T-11840 */
        p[0x4F] = arg1;
        *(s16 *)(p + 0x3C) = 0x48;
        *(s16 *)(p + 0x3E) = 0x48;
        *(s16 *)(p + 0x40) = 0xA;
        *(s16 *)(p + 0x42) = (arg0 & 0xF) + (((arg0 >> 4) + 0xF0) << 6);
        p[0x4C] = 0;
        *(u16 *)(p + 0x10) = 0xD;
        p += 0x64;
        g += 0x10;
    }
    for (i = 0; i < 4; i++, p += 0x64) {
        p[0] = 1;
        p[1] = 0;
        *(u16 *)(p + 0x44) = 8;
        if (i & 1) {
            *(s16 *)(p + 0x14) = 0x58;
        } else {
            *(s16 *)(p + 0x14) = -0x60;
        }
        if (i >> 1) {
            *(s16 *)(p + 0x16) = 0xB8;
        } else {
            *(s16 *)(p + 0x16) = 0x30;
        }
        *(s16 *)(p + 0x3C) = 8;
        *(s16 *)(p + 0x3E) = 8;
        p[0x46] = (i << 3) + 0x40;
        p[0x47] = 0x18;
        *(s16 *)(p + 0x40) = 0xA;
        *(s16 *)(p + 0x42) = 0x3C0D;
        p[0x3B] = 0x65;
        *(s16 *)(p + 0x10) = 0;
    }
    for (i = 0; i < 2; i++, p += 0xC8) {
        p[0] = 1;
        p[1] = 0;
        *(u16 *)(p + 0x44) = 8;
        *(s16 *)(p + 0x14) = -0x58;
        *(s16 *)(p + 0x16) = (i == 0 ? 1 : 2) + i * 0x8C + 0x30;
        *(s16 *)(p + 0x3C) = 0xB0;
        *(s16 *)(p + 0x3E) = 1;
        p[0x38] = 0xF8;
        p[0x39] = 0xD8;
        p[0x3A] = 0;
        *(s16 *)(p + 0x10) = 0;
        p[0x3B] = 0x61;
        p += 0x64;
        p[0] = 1;
        p[1] = 0;
        *(u16 *)(p + 0x44) = 8;
        *(s16 *)(p + 0x14) = -0x58;
        *(s16 *)(p + 0x16) = (i == 0 ? 2 : 1) + i * 0x8C + 0x30;
        *(s16 *)(p + 0x3C) = 0xB0;
        *(s16 *)(p + 0x3E) = 1;
        p[0x38] = 0xF8;
        p[0x39] = 0xA8;
        p[0x3A] = 0;
        *(s16 *)(p + 0x10) = 0;
        p[0x3B] = 0x61;
        p += 0x64;
        p[0] = 1;
        p[1] = 0;
        *(u16 *)(p + 0x44) = 8;
        *(s16 *)(p + 0x14) = -0x58;
        *(s16 *)(p + 0x16) = i * 0x8C + 0x30;
        *(s16 *)(p + 0x3C) = 0xB0;
        *(s16 *)(p + 0x3E) = 4;
        p[0x38] = 0xC0;
        p[0x39] = 0x70;
        p[0x3A] = 0;
        *(s16 *)(p + 0x10) = 0;
        p[0x3B] = 0x61;
    }
    for (i = 0; i < 2; i++, p += 0xC8) {
        p[0] = 1;
        p[1] = 0;
        *(u16 *)(p + 0x44) = 8;
        *(s16 *)(p + 0x14) = (i == 0 ? 1 : 2) + i * 0xBC - 0x60;
        *(s16 *)(p + 0x16) = 0x38;
        *(s16 *)(p + 0x3C) = 1;
        *(s16 *)(p + 0x3E) = 0x80;
        p[0x38] = 0xF8;
        p[0x39] = 0xD8;
        p[0x3A] = 0;
        *(s16 *)(p + 0x10) = 0;
        p[0x3B] = 0x61;
        p += 0x64;
        p[0] = 1;
        p[1] = 0;
        *(u16 *)(p + 0x44) = 8;
        *(s16 *)(p + 0x14) = (i == 0 ? 2 : 1) + i * 0xBC - 0x60;
        *(s16 *)(p + 0x16) = 0x38;
        *(s16 *)(p + 0x3C) = 1;
        *(s16 *)(p + 0x3E) = 0x80;
        p[0x38] = 0xF8;
        p[0x39] = 0xA8;
        p[0x3A] = 0;
        *(s16 *)(p + 0x10) = 0;
        p[0x3B] = 0x61;
        p += 0x64;
        p[0] = 1;
        p[1] = 0;
        *(u16 *)(p + 0x44) = 8;
        *(s16 *)(p + 0x14) = i * 0xBC - 0x60;
        *(s16 *)(p + 0x16) = 0x38;
        *(s16 *)(p + 0x3C) = 4;
        *(s16 *)(p + 0x3E) = 0x80;
        p[0x38] = 0xC0;
        p[0x39] = 0x70;
        p[0x3A] = 0;
        *(s16 *)(p + 0x10) = 0;
        p[0x3B] = 0x61;
    }
    p[0] = 1;
    p[1] = 0;
    *(s16 *)(p + 0x14) = -0x5E;
    *(s16 *)(p + 0x16) = 0x32;
    *(s16 *)(p + 0x3C) = 0xBC;
    *(s16 *)(p + 0x3E) = 0x8C;
    p[0x3A] = 0;
    p[0x39] = 0;
    p[0x38] = 0;
    *(u16 *)(p + 0x10) = 0xC;
    p[0x3B] = 0x63;
    switch (arg0) {
    case 12:
        state[0x13] = 0x7B;
        state[0x14] = 0xD5;
        state[0x15] = 0x94;
        break;
    case 13:
        state[0x13] = 0;
        state[0x14] = 0xCD;
        state[0x15] = 0xEE;
        break;
    case 14:
        state[0x13] = 0xFF;
        state[0x14] = 0x9C;
        state[0x15] = 0xCD;
        break;
    }
}

void func_80132810(void) {
    s32 i;
    TtGridCore *state;
    TtGrid10 *grid;
    TtGrid64 *tiles;
    state = (TtGridCore *)D_80158A64;
    i = 0;
    tiles = (TtGrid64 *)(D_80158AA4 + 0x1900);
    grid = (TtGrid10 *)D_80158AB0;
    for (; i < 25; i++) {
        grid->x = (i / 5) * 0x48 - state->offset - 0x5C;
        grid->y = state->offset + (i % 5) * 0x48 - 0x48;
        tiles->x = grid->x - 0x24;
        tiles->y = grid->y - 0x24;
        tiles++;
        grid++;
    }
    if (++state->offset >= 0x48U) {
        state->offset = 0;
    }
}

void func_80132B88(void) {
    u8 *p;
    u8 *q;
    s32 i;

    p = D_80158A64;
    p[0x1D] += 4;
    p[0x1E] = p[0x1F] = p[0x1D];
    if (p[0x1D] >= 0x80U) {
        q = D_80158AA8;
        for (i = 0; i < 0x80; i++) {
            q += 0x14;
            *(s16 *)(q - 0xE) = 0;
        }
        q = D_80158AA4;
        for (i = 0; i < 0x80; i++) {
            *(s16 *)(q + 0x44) = 0;
            q += 0x64;
        }
        *(s16 *)(p + 2) = 0x40;
    }
    func_80132810();
}

void func_80132C24(void) {
    u8 *p;
    u8 *q;
    s32 i;

    p = D_80158A64;
    if (p[0x1D] == 0x80U) {
        i = 0;
        q = D_80158AA8;
        for (; i < 0x80; i++) {
            q += 0x14;
            *(s16 *)(q - 0xE) = 8;
        }
        q = D_80158AA4;
        for (i = 0; i < 0x80; i++) {
            *(s16 *)(q + 0x44) = 8;
            q += 0x64;
        }
    }
    p[0x1D] -= 4;
    p[0x1E] = p[0x1F] = p[0x1D];
    if (p[0x1D] == 0) {
        p[9] = 1;
    }
    func_80132810();
}

void func_80132CCC(void) {
    u8 *scene = D_80158A60;
    /* FAKE (T-9780): retain the unused selector home between the saved pointers. */
    u16 pad_selector;
    u8 *control;
    TtButtons06 *buttons;
    u16 flags;
    TtCore30 *state = (TtCore30 *)D_80158A64;

    control = D_80158A6C;
    buttons = (TtButtons06 *)(scene + 0x10);
    switch (state->unk_02) {
    case 0:
        func_80044750(0x22);
        func_80044750(0x2F);
        func_80044750(4);
        func_80044750(0x204);
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
        state->unk_11 = 0;
        state->unk_1C = 1;
        state->unk_1D = 0;
        state->unk_1E = 0;
        state->unk_1F = 0;
        func_80132130();
        func_80132198(2, 6, 4);
        func_8013230C(0xD, 0);
        state->unk_2E = 0;
        if (state->unk_22 != 0) {
            state->unk_26 = 0x12CU;
        } else {
            state->unk_26 = 0xE10U;
        }
        state->unk_02 = 0x20;
        break;
    case 0x20:
        func_80132B88((u8 *)buttons);
        break;
    case 0x40:
        func_80132810((u8 *)buttons);
        if ((state->unk_26 -= 1) == 0) {
            state->unk_02 = 0x80;
        }
        if ((state->unk_22 == 0) && ((flags = buttons->unk_04, ((flags & 0x20) != 0)) || (flags & 0x800))) {
            state->unk_1D = 0x80;
            state->unk_1E = 0x80;
            state->unk_1F = 0x80;
            state->unk_02 = 0x80;
        }
        break;
    case 0x80:
        func_80044750(0xB1);
        func_80132C24();
        break;
    }
    if (state->unk_09 != 0) {
        if (scene[83] == 0) {
            if (control[15] != 0) {
                state->unk_00 = 8;
            } else {
                state->unk_00 = 1;
            }
        } else {
            state->unk_00 = 1;
        }
        state->unk_06 = 0;
        state->unk_04 = 0;
        state->unk_02 = 0;
        state->unk_0B = 0;
        state->unk_0A = 0;
        state->unk_09 = 0U;
    }
}
