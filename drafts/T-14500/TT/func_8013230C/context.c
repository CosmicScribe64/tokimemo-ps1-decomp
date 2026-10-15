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

void func_8013230C(u8 arg0, s32 arg1) {
    u8 *sp34;
    s16 temp_t7;
    s16 temp_t7_2;
    s16 temp_t8;
    s16 var_a0;
    s16 var_a0_2;
    s32 temp_hi;
    s32 temp_t8_2;
    s32 var_a1;
    s32 var_a1_2;
    s32 var_v1;
    s32 var_v1_2;
    s32 var_v1_3;
    s32 var_v1_4;
    s32 var_v1_5;
    s32 var_v1_6;
    u8 *temp_v0;
    u8 *temp_v0_2;
    u8 *var_t1;
    u8 *var_v0;
    u8 temp_a3;

    temp_a3 = arg0 & 0xFF;
    sp34 = D_80158A64;
    var_t1 = D_80158AB0;
    var_v1 = 0;
    var_v0 = D_80158AA4 + 0x1900;
    do {
        (*(s32 *)var_t1) = 0x1000;
        *(s16 *)((unsigned char *)var_t1 + (2)) = 0x1000;
        var_v0 += 0x64;
        *(s16 *)((unsigned char *)var_t1 + (4)) = (s16) (((var_v1 / 5) * 0x48) - 0x5C);
        temp_hi = var_v1 % 5;
        var_v1 += 1;
        *(s16 *)((unsigned char *)var_t1 + (6)) = (s16) ((temp_hi * 0x48) - 0x48);
        *(s8 *)((unsigned char *)var_v0 + (-100)) = 1;
        *(s8 *)((unsigned char *)var_v0 + (-99)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (-32)) = 8;
        *(s8 *)((unsigned char *)var_v0 + (-41)) = 0x80;
        *(u8 **)((unsigned char *)var_v0 + (-28)) = var_t1;
        *(s8 *)((unsigned char *)var_v0 + (-23)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-22)) = 1;
        *(s8 *)((unsigned char *)var_v0 + (-42)) = 0x40;
        *(s8 *)((unsigned char *)var_v0 + (-43)) = 0x40;
        *(s8 *)((unsigned char *)var_v0 + (-44)) = 0x40;
        *(s8 *)((unsigned char *)var_v0 + (-20)) = 0;
        temp_t8 = *(s16 *)((unsigned char *)var_t1 + (4));
        var_t1 += 0x10;
        *(s16 *)((unsigned char *)var_v0 + (-80)) = (s16) (temp_t8 - 0x24);
        *(s16 *)((unsigned char *)var_v0 + (-78)) = (s16) (*(s16 *)((unsigned char *)var_t1 + (-10)) - 0x24);
        *(s8 *)((unsigned char *)var_v0 + (-30)) = 0x38;
        *(s8 *)((unsigned char *)var_v0 + (-29)) = 0xB8;
        *(s8 *)((unsigned char *)var_v0 + (-21)) = (s8) (arg1 & 0xFF);
        *(s16 *)((unsigned char *)var_v0 + (-40)) = 0x48;
        *(s16 *)((unsigned char *)var_v0 + (-38)) = 0x48;
        *(s16 *)((unsigned char *)var_v0 + (-36)) = 0xA;
        *(s16 *)((unsigned char *)var_v0 + (-34)) = (s16) ((temp_a3 & 0xF) + (((temp_a3 >> 4) + 0xF0) << 6));
        *(s8 *)((unsigned char *)var_v0 + (-24)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (-84)) = 0xD;
    } while (var_v1 < 0x19);
    var_v1_2 = 0;
    do {
        (*(s32 *)var_v0) = 1;
        *(s8 *)((unsigned char *)var_v0 + (1)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (68)) = 8;
        if (var_v1_2 & 1) {
            *(s16 *)((unsigned char *)var_v0 + (20)) = 0x58;
        } else {
            *(s16 *)((unsigned char *)var_v0 + (20)) = -0x60;
        }
        temp_t8_2 = var_v1_2 * 8;
        if ((var_v1_2 >> 1) != 0) {
            *(s16 *)((unsigned char *)var_v0 + (22)) = 0xB8;
        } else {
            *(s16 *)((unsigned char *)var_v0 + (22)) = 0x30;
        }
        var_v1_2 += 1;
        var_v0 += 0x64;
        *(s16 *)((unsigned char *)var_v0 + (-40)) = 8;
        *(s16 *)((unsigned char *)var_v0 + (-38)) = 8;
        *(s8 *)((unsigned char *)var_v0 + (-30)) = (s8) (temp_t8_2 + 0x40);
        *(s8 *)((unsigned char *)var_v0 + (-29)) = 0x18;
        *(s16 *)((unsigned char *)var_v0 + (-36)) = 0xA;
        *(s16 *)((unsigned char *)var_v0 + (-34)) = 0x3C0D;
        *(s8 *)((unsigned char *)var_v0 + (-41)) = 0x65;
        *(s16 *)((unsigned char *)var_v0 + (-84)) = 0;
    } while (var_v1_2 < 4);
    arg0 = temp_a3;
    var_a1 = 0;
    var_a0 = 0x30;
    do {
        (*(s32 *)var_v0) = 1;
        *(s8 *)((unsigned char *)var_v0 + (1)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (68)) = 8;
        *(s16 *)((unsigned char *)var_v0 + (20)) = -0x58;
        if (var_a0 == 0x30) {
            var_v1_3 = 1;
        } else {
            var_v1_3 = 2;
        }
        *(s16 *)((unsigned char *)var_v0 + (22)) = (s16) (var_v1_3 + var_a1 + 0x30);
        *(s16 *)((unsigned char *)var_v0 + (60)) = 0xB0;
        *(s16 *)((unsigned char *)var_v0 + (62)) = 1;
        *(s8 *)((unsigned char *)var_v0 + (56)) = 0xF8;
        *(s8 *)((unsigned char *)var_v0 + (57)) = 0xD8;
        *(s8 *)((unsigned char *)var_v0 + (58)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (16)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (59)) = 0x61;
        temp_v0 = var_v0 + 0x64;
        *(s8 *)((unsigned char *)var_v0 + (100)) = 1;
        *(s8 *)((unsigned char *)temp_v0 + (1)) = 0;
        *(s16 *)((unsigned char *)temp_v0 + (68)) = 8;
        *(s16 *)((unsigned char *)temp_v0 + (20)) = -0x58;
        if (var_a0 == 0x30) {
            var_v1_4 = 2;
        } else {
            var_v1_4 = 1;
        }
        *(s16 *)((unsigned char *)temp_v0 + (122)) = var_a0;
        var_a0 += 0x8C;
        temp_t7 = var_v1_4 + var_a1 + 0x30;
        var_a1 += 0x8C;
        var_v0 = temp_v0 + 0x12C;
        *(s16 *)((unsigned char *)var_v0 + (-278)) = temp_t7;
        *(s16 *)((unsigned char *)var_v0 + (-240)) = 0xB0;
        *(s16 *)((unsigned char *)var_v0 + (-238)) = 1;
        *(s8 *)((unsigned char *)var_v0 + (-244)) = 0xF8;
        *(s8 *)((unsigned char *)var_v0 + (-243)) = 0xA8;
        *(s8 *)((unsigned char *)var_v0 + (-242)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (-284)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-241)) = 0x61;
        *(s8 *)((unsigned char *)var_v0 + (-200)) = 1;
        *(s8 *)((unsigned char *)var_v0 + (-199)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (-132)) = 8;
        *(s16 *)((unsigned char *)var_v0 + (-180)) = -0x58;
        *(s16 *)((unsigned char *)var_v0 + (-140)) = 0xB0;
        *(s16 *)((unsigned char *)var_v0 + (-138)) = 4;
        *(s8 *)((unsigned char *)var_v0 + (-144)) = 0xC0;
        *(s8 *)((unsigned char *)var_v0 + (-143)) = 0x70;
        *(s8 *)((unsigned char *)var_v0 + (-142)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (-184)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-141)) = 0x61;
    } while (var_a0 < 0x148);
    var_a1_2 = 0;
    var_a0_2 = -0x60;
    do {
        (*(s32 *)var_v0) = 1;
        *(s8 *)((unsigned char *)var_v0 + (1)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (68)) = 8;
        if (var_a0_2 == -0x60) {
            var_v1_5 = 1;
        } else {
            var_v1_5 = 2;
        }
        *(s16 *)((unsigned char *)var_v0 + (20)) = (s16) ((var_v1_5 + var_a1_2) - 0x60);
        *(s16 *)((unsigned char *)var_v0 + (22)) = 0x38;
        *(s16 *)((unsigned char *)var_v0 + (60)) = 1;
        *(s16 *)((unsigned char *)var_v0 + (62)) = 0x80;
        *(s8 *)((unsigned char *)var_v0 + (56)) = 0xF8;
        *(s8 *)((unsigned char *)var_v0 + (57)) = 0xD8;
        *(s8 *)((unsigned char *)var_v0 + (58)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (16)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (59)) = 0x61;
        temp_v0_2 = var_v0 + 0x64;
        *(s8 *)((unsigned char *)var_v0 + (100)) = 1;
        *(s8 *)((unsigned char *)temp_v0_2 + (1)) = 0;
        *(s16 *)((unsigned char *)temp_v0_2 + (68)) = 8;
        if (var_a0_2 == -0x60) {
            var_v1_6 = 2;
        } else {
            var_v1_6 = 1;
        }
        *(s16 *)((unsigned char *)temp_v0_2 + (120)) = var_a0_2;
        var_a0_2 += 0xBC;
        temp_t7_2 = (var_v1_6 + var_a1_2) - 0x60;
        var_a1_2 += 0xBC;
        var_v0 = temp_v0_2 + 0x12C;
        *(s16 *)((unsigned char *)var_v0 + (-280)) = temp_t7_2;
        *(s16 *)((unsigned char *)var_v0 + (-278)) = 0x38;
        *(s16 *)((unsigned char *)var_v0 + (-240)) = 1;
        *(s16 *)((unsigned char *)var_v0 + (-238)) = 0x80;
        *(s8 *)((unsigned char *)var_v0 + (-244)) = 0xF8;
        *(s8 *)((unsigned char *)var_v0 + (-243)) = 0xA8;
        *(s8 *)((unsigned char *)var_v0 + (-242)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (-284)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-241)) = 0x61;
        *(s8 *)((unsigned char *)var_v0 + (-200)) = 1;
        *(s8 *)((unsigned char *)var_v0 + (-199)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (-132)) = 8;
        *(s16 *)((unsigned char *)var_v0 + (-178)) = 0x38;
        *(s16 *)((unsigned char *)var_v0 + (-140)) = 4;
        *(s16 *)((unsigned char *)var_v0 + (-138)) = 0x80;
        *(s8 *)((unsigned char *)var_v0 + (-144)) = 0xC0;
        *(s8 *)((unsigned char *)var_v0 + (-143)) = 0x70;
        *(s8 *)((unsigned char *)var_v0 + (-142)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (-184)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-141)) = 0x61;
    } while (var_a0_2 != 0x118);
    *(s16 *)((unsigned char *)var_v0 + (20)) = -0x5E;
    *(s8 *)((unsigned char *)temp_v0_2 + (300)) = 1;
    *(s8 *)((unsigned char *)var_v0 + (1)) = 0;
    *(s16 *)((unsigned char *)var_v0 + (22)) = 0x32;
    *(s16 *)((unsigned char *)var_v0 + (60)) = 0xBC;
    *(s16 *)((unsigned char *)var_v0 + (62)) = 0x8C;
    *(s8 *)((unsigned char *)var_v0 + (58)) = 0;
    *(s8 *)((unsigned char *)var_v0 + (57)) = 0;
    *(s8 *)((unsigned char *)var_v0 + (56)) = 0;
    *(s16 *)((unsigned char *)var_v0 + (16)) = 0xC;
    *(s8 *)((unsigned char *)var_v0 + (59)) = 0x63;
    switch (arg0) {
    case 12:
        *(s8 *)((unsigned char *)sp34 + (19)) = 0x7B;
        *(s8 *)((unsigned char *)sp34 + (20)) = 0xD5;
        *(s8 *)((unsigned char *)sp34 + (21)) = 0x94;
        return;
    case 13:
        *(s8 *)((unsigned char *)sp34 + (19)) = 0;
        *(s8 *)((unsigned char *)sp34 + (20)) = 0xCD;
        *(s8 *)((unsigned char *)sp34 + (21)) = 0xEE;
        return;
    case 14:
        *(s8 *)((unsigned char *)sp34 + (19)) = 0xFF;
        *(s8 *)((unsigned char *)sp34 + (20)) = 0x9C;
        *(s8 *)((unsigned char *)sp34 + (21)) = 0xCD;
        return;
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
