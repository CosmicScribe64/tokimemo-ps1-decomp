#include "common.h"
#include "ovl/TACO.h"

void func_80139170(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8013920C();
        return;
    case 1:
        func_80139AB0();
        return;
    case 2:
        func_80139BCC();
        return;
    case 3:
        func_80139B54();
        return;
    case 4:
        func_80139394();
        return;
    case 5:
        func_801393F4();
        return;
    }
}

void func_8013920C(void) {
    s32 previous;

    func_8013A764();
    func_8013AFDC();
    func_800450F4(0, 0x202);
    D_800E6280.unk_036 = 0;
    D_800E6280.unk_037 = 0;
    D_800E6280.unk_038 = 0;
    back_clear_switch(1);
    D_8015EDB0 = 0;
    /* FAKE: the scalar address view and shared source line retain the two
     * score-load registers and their order. Original spelling unknown. T-9530 */
    func_80059048(); previous = *(s32 *)&D_8015EDF8;
    if (previous < D_8015EDF0) {
        D_8015F4D8 = 0;
        D_8015EE00 = D_8015EDFC;
        D_8015EDF8 = D_8015EDF0;
        D_8015EDFC = previous;
        strncpy(D_8015EE0C, D_8015EE08, 4);
        strncpy(D_8015EE08, D_8015EE04, 4);
        strncpy(D_8015EE04, "   ", 4);
    } else if (D_8015EDFC < D_8015EDF0) {
        D_8015F4D8 = 1;
        D_8015EE00 = D_8015EDFC;
        D_8015EDFC = D_8015EDF0;
        strncpy(D_8015EE0C, D_8015EE08, 4);
        strncpy(D_8015EE08, "   ", 4);
    } else {
        D_8015F4D8 = 2;
        D_8015EE00 = D_8015EDF0;
        strncpy(D_8015EE0C, "   ", 4);
    }
    func_8004284C();
}

void func_80139394(void) {
    D_800E6280.unk_110D += 1;
    func_8013A790(0, 0);
    if ((u32)(D_800E6280.unk_110D * 3) >= 0x81U) {
        func_8004284C();
    }
}

void func_801393F4(void) {
    func_80042908(5);
    func_8004284C();
    func_8004284C();
}

/* Draws glyph arg0 ('0'-'9', 'A'-'Z', ". - ! @ * #", other: a fallback cell) as a POLY_FT4 centred
 * on D_800E6280.unk_014/unk_018[D_8011ECA0] plus (arg4, arg5), half sizes from arg6/arg7, clut row arg1, ordering-table
 * slot arg2, grey level arg3; arg8 == 3 selects the wide cell, arg8 == 1 anchors the glyph at its
 * bottom edge. Called without a prototype (K&R definition, the u8 parameter is narrowed at entry).
 * Matching notes (T-25112): every vertex coordinate is its own full expression with the offset
 * grouped first, `centre + (arg4 - dx)`: uopt then shares the centre load but not the sum, and the
 * repeated vertices become its copies (x1 and x2 stay in their stack homes) as in the original.
 * FAKE (layout): T-25112, each UV setup is one comma statement on one line, the expansion of the
 * libgpu setUVWH(p, u, v, w, 12) macro (likely the original's): the special glyphs need it for the
 * original's constant registers and store order. */
void func_80139424(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)
u8 arg0;
s32 arg1;
s32 arg2;
s32 arg3;
s32 arg4;
s32 arg5;
s32 arg6;
s32 arg7;
s32 arg8;
{
    POLY_FT4 *p;
    s32 dx;
    s32 dy;
    s32 x0, y0, x1, y1, x2, y2, x3, y3;
    s32 w;
    u8 letter;
    u8 *uv;
    s32 pad[2]; /* FAKE: two unused words give the original's 0x68 frame and home slots; real source unknown. T-25112 */

    /* FAKE: dead store; mentioning x3 first gives it the lower uopt bit, so it takes t5 and x0 ra
     * at their equal priority, as in the original. Real source unknown. T-25112 */
    x3 = 0;
    if (arg8 == 3) {
        w = 12;
    } else {
        w = 11;
    }
    p = (POLY_FT4 *)GetWorkBase(sizeof(POLY_FT4), D_8011ECA0);
    func_8009F040(p);
    SetShadeTex(p, 0);
    SetSemiTrans(p, 0);
    p->b0 = p->g0 = p->r0 = arg3;
    dx = (w * arg6) / 8192;
    x0 = D_800E6280.unk_014[D_8011ECA0] + (arg4 - dx);
    x3 = D_800E6280.unk_014[D_8011ECA0] + (arg4 + dx);
    x1 = D_800E6280.unk_014[D_8011ECA0] + (arg4 + dx);
    x2 = D_800E6280.unk_014[D_8011ECA0] + (arg4 - dx);
    if (arg8 != 1) {
        dy = (arg7 * 12) / 8192;
        y0 = D_800E6280.unk_018[D_8011ECA0] + (arg5 - dy);
        y1 = D_800E6280.unk_018[D_8011ECA0] + (arg5 - dy);
        y2 = D_800E6280.unk_018[D_8011ECA0] + (arg5 + dy);
        y3 = D_800E6280.unk_018[D_8011ECA0] + (arg5 + dy);
    } else {
        y0 = D_800E6280.unk_018[D_8011ECA0] + (arg5 - (arg7 * 12) / 4096);
        y1 = D_800E6280.unk_018[D_8011ECA0] + (arg5 - (arg7 * 12) / 4096);
        y2 = D_800E6280.unk_018[D_8011ECA0] + arg5;
        y3 = D_800E6280.unk_018[D_8011ECA0] + arg5;
    }
    p->tpage = func_8009ECB0(0, 0, 0x340, 0x180);
    p->x0 = x0;
    p->y0 = y0;
    p->x1 = x1;
    p->y1 = y1;
    p->x2 = x2;
    p->y2 = y2;
    p->x3 = x3;
    p->y3 = y3;
    if (arg0 >= '0' && arg0 <= '9') {
        uv = &D_8015F5A0[(u8)(arg0 - '0') * 2];
        p->u0 = uv[0] + 0x80, p->v0 = uv[1], p->u1 = uv[0] + 0x80 + w, p->v1 = uv[1], p->u2 = uv[0] + 0x80, p->v2 = uv[1] + 12, p->u3 = uv[0] + 0x80 + w, p->v3 = uv[1] + 12;
    } else if ((letter = arg0 - 'A') < 26) {
        uv = &D_8015F5B4[letter * 2];
        p->u0 = uv[0] + 0x80, p->v0 = uv[1], p->u1 = uv[0] + 0x80 + w, p->v1 = uv[1], p->u2 = uv[0] + 0x80, p->v2 = uv[1] + 12, p->u3 = uv[0] + 0x80 + w, p->v3 = uv[1] + 12;
    } else if (arg0 == '.') {
        p->u0 = D_8015F5E8 + 0x80, p->v0 = D_8015F5E9, p->u1 = D_8015F5E8 + 0x80 + w, p->v1 = D_8015F5E9, p->u2 = D_8015F5E8 + 0x80, p->v2 = D_8015F5E9 + 12, p->u3 = D_8015F5E8 + 0x80 + w, p->v3 = D_8015F5E9 + 12;
    } else if (arg0 == '-') {
        p->u0 = D_8015F5EA + 0x80, p->v0 = D_8015F5EB, p->u1 = D_8015F5EA + 0x80 + w, p->v1 = D_8015F5EB, p->u2 = D_8015F5EA + 0x80, p->v2 = D_8015F5EB + 12, p->u3 = D_8015F5EA + 0x80 + w, p->v3 = D_8015F5EB + 12;
    } else if (arg0 == '!') {
        p->u0 = D_8015F5EE + 0x80, p->v0 = D_8015F5EF, p->u1 = D_8015F5EE + 0x80 + w, p->v1 = D_8015F5EF, p->u2 = D_8015F5EE + 0x80, p->v2 = D_8015F5EF + 12, p->u3 = D_8015F5EE + 0x80 + w, p->v3 = D_8015F5EF + 12;
    } else if (arg0 == '@') {
        p->u0 = 0xA6, p->v0 = 0xD3, p->u1 = 0xA6 + w, p->v1 = 0xD3, p->u2 = 0xA6, p->v2 = 0xD3 + 12, p->u3 = 0xA6 + w, p->v3 = 0xD3 + 12;
    } else if (arg0 == '*') {
        p->u0 = 0xBE, p->v0 = 0xD3, p->u1 = 0xBE + w, p->v1 = 0xD3, p->u2 = 0xBE, p->v2 = 0xD3 + 12, p->u3 = 0xBE + w, p->v3 = 0xD3 + 12;
    } else if (arg0 == '#') {
        p->u0 = 0xEE, p->v0 = 0xD3, p->u1 = 0xEE + w, p->v1 = 0xD3, p->u2 = 0xEE, p->v2 = 0xD3 + 12, p->u3 = 0xEE + w, p->v3 = 0xD3 + 12;
    } else {
        p->u0 = D_8015F5EC + 0x80, p->v0 = D_8015F5ED, p->u1 = D_8015F5EC + 0x80 + w, p->v1 = D_8015F5ED, p->u2 = D_8015F5EC + 0x80, p->v2 = D_8015F5ED + 12, p->u3 = D_8015F5EC + 0x80 + w, p->v3 = D_8015F5ED + 12;
    }
    p->clut = func_8009ED74(arg1 * 16, 0x1EF);
    AddPrim(D_800E8CA0 + (D_8011ECA0 << 10) + arg2 * 4, (s32)p);
    dtd_on_tpage(0x340, 0x180, arg2, 0, 0);
}

void func_80139AB0(void) {
    if (D_8015F4F8 < 0x100) {
        D_8015F4F0 = D_8015F4F8 / 2;
    } else {
        D_8015F4F0 = 0x80;
    }
    func_8013A790(0, 0);
    func_80139FB8(0x41, 0, 0x280 - D_8015F4F8, 0);
    D_8015F4F8 += 4;
    if (D_8015F4F8 >= 0x1E1) {
        func_8004284C();
    }
}

void func_80139B54(void) {
    func_8013A790(0, 0);
    func_8013A580(D_8015F4F8);
    func_80139FB8(D_8015F4E0, 0, 0x280 - D_8015F4F8, 0);
    D_8015F4F8 -= 8;
    if (D_8015F4F8 < 0) {
        func_8004284C();
    }
}

void func_80139BCC(void) {
    s32 temp_lo;
    s32 scale;
    s32 right;
    s32 left;
    s32 pad;

    if (D_8015F4E4 < 0) {
        scale = D_8015F4E4 + 0x50;
    } else {
        scale = 0x50 - D_8015F4E4;
    }
    func_80139FB8(D_8015F4E0, D_8015F4E4, 0xA0, 0);
    temp_lo = scale * 0x7000 / 80;
    func_80139424(D_8015F4B0[D_8015F4E0 % 40], 6, 4, 0x80, 0, 0x3C, temp_lo, temp_lo, 1);
    func_8013A580(0x1E0);
    func_8013A790(0, 0);
    /* pad is tested again in the zero-speed block (five uses, so it is coloured before the masks),
       and left is assigned before right; both are needed for the original's register split (T-19021) */
    pad = D_800E6280.unk_F80;
    left = pad & 0x8000;
    right = pad & 0x2000;
    if (right != 0 && D_8015F4E8 < 0) {
        D_8015F4E8 = 1;
    }
    if (left != 0 && D_8015F4E8 > 0) {
        D_8015F4E8 = -1;
    }
    switch (D_8015F4E8) {
    case -2:
        D_8015F4E4 += 0xA;
        break;
    case -1:
        D_8015F4E4 += 5;
        break;
    case 1:
        D_8015F4E4 -= 5;
        break;
    case 2:
        D_8015F4E4 -= 0xA;
        break;
    }
    if (D_8015F4E4 == 0) {
        if (!(pad & 0xA000)) {
            D_8015F4E8 = 0;
        }
        if (pad & 0x2000) {
            D_8015F4E8 += 1;
        }
        if (pad & 0x8000) {
            D_8015F4E8 -= 1;
        }
        if (D_8015F4E8 >= 3) {
            D_8015F4E8 = 2;
        }
        if (D_8015F4E8 < -2) {
            D_8015F4E8 = -2;
        }
    }
    if (D_8015F4E4 >= 0x33) {
        D_8015F4E0 += 1;
        D_8015F4E4 = -0x32;
    }
    if (D_8015F4E4 < -0x32) {
        D_8015F4E4 = 0x32;
        D_8015F4E0 -= 1;
    }
    if (D_8015F4E0 >= 0x28) {
        D_8015F4E0 = 0;
    }
    if (D_8015F4E0 < 0) {
        D_8015F4E0 = 0x27;
    }
    if ((D_800E6280.unk_F88 & 0x20) && !(D_800E6280.unk_F88 & 0x40)) {
        if (D_8015F4E0 == 0x27) {
            /* the original passes the byte itself as the destination (a lost &); T-19021 */
            strncpy((void *)D_8015EE04[D_8015F4D8 * 4 + D_8015F4DC], "", 1);
            func_8004284C();
        } else if (D_8015F4DC != 3) {
            strncpy(&D_8015EE04[D_8015F4D8 * 4 + D_8015F4DC], &D_8015F4B0[D_8015F4E0], 1);
            D_8015F4DC += 1;
        }
    }
    if ((D_800E6280.unk_F88 & 0x800) && !(D_800E6280.unk_F88 & 0x40)) {
        strncpy((void *)D_8015EE04[D_8015F4D8 * 4 + D_8015F4DC], "", 1);
        func_8004284C();
    }
    if ((D_800E6280.unk_F88 & 0x40) && D_8015F4DC != 0) {
        strncpy(&D_8015EE04[D_8015F4D8 * 4 + D_8015F4DC - 1], " ", 1);
        D_8015F4DC -= 1;
    }
}

/* Draws the 40-entry ring around D_8015F4E0: the front entry, 19 pairs on both sides, then the back
   entry. Three source details set the original's registers (T-25112): one x for the front, the +i and
   the back entries (sine in s5), scale reused for the mirrored angle before it is reflected (v0/v1 split),
   and a for loop (a do/while puts 3 and 40 in the wrong s registers and 0x100000 in fp instead of
   the table address). */
void func_80139FB8(s32 arg0, s32 arg1, s32 radius, s32 arg3) {
    s32 pos;
    s32 d;
    s32 angle;
    s32 x;
    s32 x2;
    s32 z;
    s32 scale;
    s32 i;

    angle = (-D_8015F4E4 << 10) / 512;
    x = (rsin(angle) * radius) >> 12;
    z = (func_800A0140(angle) * radius) >> 12;
    if (z < -0x100) {
        scale = 0;
    } else {
        scale = ((z << 12) + 0x100000) / 96;
    }
    func_80139424(D_8015F4B0[D_8015F4E0 % 40], 2, 5, ((z << 5) + 0x2800) / 160, x, (z >> 2) + 50, scale, scale, 0);
    for (i = 1; i < 20; i++) {
        pos = i * 0x1000 / 40;
        d = pos - D_8015F4E4;
        if (d < 0x200) {
            angle = (d << 10) / 512;
        } else {
            angle = (d * 2 + 0x800) / 3;
        }
        x = (rsin(angle) * radius) >> 12;
        z = (func_800A0140(angle) * radius) >> 12;
        if (z < -0x100) {
            scale = 0;
        } else {
            scale = ((z << 12) + 0x100000) / 96;
        }
        func_80139424(D_8015F4B0[(D_8015F4E0 + i) % 40], 0, 5, ((z << 5) + 0x2800) / 160, x, (z >> 2) + 50, scale, scale, 0);
        d = pos + D_8015F4E4;
        if (d < 0x200) {
            scale = (d << 10) / 512;
        } else {
            scale = (d * 2 + 0x800) / 3;
        }
        angle = 0x1000 - scale;
        x2 = (rsin(angle) * radius) >> 12;
        z = (func_800A0140(angle) * radius) >> 12;
        if (z < -0x100) {
            scale = 0;
        } else {
            scale = ((z << 12) + 0x100000) / 96;
        }
        func_80139424(D_8015F4B0[(D_8015F4E0 - i + 40) % 40], 0, 5, ((z << 5) + 0x2800) / 160, x2, (z >> 2) + 50, scale, scale, 0);
    }
    d = 0x800 - D_8015F4E4;
    if (d < 0x200) {
        angle = (d << 10) / 512;
    } else {
        angle = (d * 2 + 0x800) / 3;
    }
    x = (rsin(angle) * radius) >> 12;
    z = (func_800A0140(angle) * radius) >> 12;
    if (z < -0x100) {
        scale = 0;
    } else {
        scale = ((z << 12) + 0x100000) / 96;
    }
    func_80139424(D_8015F4B0[(D_8015F4E0 + i) % 40], 0, 5, ((z << 5) + 0x2800) / 160, x, (z >> 2) + 50, scale, scale, 0);
}

/* The y offset stays an expression: a fourth named temporary adds a frame word that moves the four
   spilled call values down by 4 (T-19021). */
void func_8013A580(s32 arg0) {
    s32 k;
    s32 a;
    s32 b;
    s32 c;

    switch (D_8015F4D8) {
    case 0:
        k = 1;
        break;
    case 1:
        k = 4;
        break;
    case 2:
        k = 3;
        break;
    }
    a = (arg0 << 7) / 480;
    b = (arg0 * 0x7000) / 480;
    c = (b * 12) / 4096;
    func_80139424(D_8015EE04[D_8015F4D8 * 4], k, 3, a, -c, -0x28 - arg0 / 16, b, b, 0);
    func_80139424(D_8015EE04[D_8015F4D8 * 4 + 1], k, 3, a, 0, -0x28 - arg0 / 16, b, b, 0);
    func_80139424(D_8015EE04[D_8015F4D8 * 4 + 2], k, 3, a, c, -0x28 - arg0 / 16, b, b, 0);
}

void func_8013A764(void) {
    D_8015F4DC = 0;
    D_8015F4E0 = 0;
    D_8015F4E4 = 0;
    D_8015F4E8 = 0;
    D_8015F4F8 = 0;
}

void func_8013A790(s32 arg0, s32 arg1) {
    /* FAKE: preserve the original 0x68 frame while keeping next at sp+0x44. T-24502 */
    s32 pad[4];
    s32 divisor;
    s32 scroll;
    s32 count;
    s32 i;
    s32 next;

    divisor = 30000;
    scroll = 0;
    if (arg1 == 0) {
        if (D_800E6280.unk_F80 & 1) divisor = 10000;
        else if (D_800E6280.unk_F80 & 2) divisor = 50000;
        if (D_800E6280.unk_F80 & 4) scroll = 40;
        if (D_800E6280.unk_F80 & 8) scroll = -40;
    } else if (arg1 == 1) {
        if ((D_8015EDB4->unk2 & 1) == 1) scroll = 40;
        if ((D_8015EDB4->unk2 & 2) == 2) scroll = -40;
    } else if (arg1 == 2) {
        scroll = 40;
        divisor = 20000;
    } else if (arg1 == 3) {
        scroll = 40;
        divisor = 10000;
    }
    count = 0;
    if (scroll > 0) {
        for (i = 0; i < 100; i++) {
            D_8015F4EC[i].a += scroll;
            if (D_8015F4EC[i].a >= 4097) D_8015F4EC[i].a -= 4096;
        }
    }
    if (scroll < 0) {
        for (i = 0; i < 100; i++) {
            D_8015F4EC[i].a += scroll;
            if (D_8015F4EC[i].a < 0) D_8015F4EC[i].a += 4096;
        }
    }
    for (i = 0; i != 100; i++) {
        next = count + 1;
        if (D_8015F4EC[i].b == 0) {
            /* FAKE: | 0 keeps the divisor/count value in one spill web. T-24502 */
            if (rand() % (next | 0) == 0) {
                count = next;
                D_8015F4EC[i].a = rand() % 4096;
                if (arg0 == 0) D_8015F4EC[i].b = 240000;
                else D_8015F4EC[i].b = 1000;
            }
        } else count = next;
        if (arg0 == 0) {
            D_8015F4EC[i].b -= D_8015F4EC[i].b * 1000 / divisor;
            if (D_8015F4EC[i].b < 3001) D_8015F4EC[i].b = 0;
        } else {
            D_8015F4EC[i].b += D_8015F4EC[i].b * 1000 / divisor;
            /* FAKE: uopt counts this use, then ugen discards * 0; count gets s2 before the bound gets s3. T-24502 */
            if (D_8015F4EC[i].b >= (count * 0) + 240000) D_8015F4EC[i].b = 0;
        }
        if (D_8015F4EC[i].b != 0) func_8013AD64(D_8015F4EC[i].a, D_8015F4EC[i].b / 1000, -1, arg1);
    }
}


/* r_sh and g_sh are single-use named shifts: they reserve the two frame words above the four spilled values
   that the original has (T-19021). */
void func_8013AD64(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 r_sh;
    s32 g_sh;
    s32 r;
    s32 g;
    s32 b;
    s32 x;
    s32 y;
    u8 *p;

    r_sh = arg2 >> 16;
    g_sh = arg2 >> 8;
    r = r_sh & 0xFF;
    g = g_sh & 0xFF;
    b = arg2 & 0xFF;
    if (arg3 < 2) {
        if (arg1 < 0x80) {
            r = r * arg1 / 128;
            g = g * arg1 / 128;
            b = b * arg1 / 128;
        }
        r = r * D_8015F4F0 / 128;
        g = g * D_8015F4F0 / 128;
        b = b * D_8015F4F0 / 128;
    }
    p = GetWorkBase(0xC, D_8011ECA0);
    func_8009F0B8(p);
    SetSemiTrans(p, 0);
    p[4] = r;
    p[5] = g;
    p[6] = b;
    x = (arg1 * rsin(arg0) >> 12) + D_800E6280.unk_014[D_8011ECA0];
    if (arg3 == 0) {
        y = (arg1 * func_800A0140(arg0) >> 12) + D_800E6280.unk_018[D_8011ECA0] - 0x28;
    } else {
        y = (arg1 * func_800A0140(arg0) >> 12) + D_800E6280.unk_018[D_8011ECA0];
    }
    *(s16 *)(p + 8) = x;
    *(s16 *)(p + 0xA) = y;
    if (arg3 < 2) {
        AddPrim((D_8011ECA0 << 10) + 0x3F8 + D_800E8CA0, p);
        return;
    }
    AddPrim((D_8011ECA0 << 10) + 0x40 + D_800E8CA0, p);
}

void func_8013AFDC(void) {
    s32 i;

    srand(D_800E6280.unk_10F4);
    D_8015F4EC = (TacoRnd *)D_800E6280.unk_163C;
    D_8015F4F0 = 0;
    D_8015F4F4 = 0;
    for (i = 0; i < 0x64; i++) {
        if (i < 0x50) {
            if (rand() % 5 < 4) {
                D_8015F4EC[i].a = rand() % 4096;
                D_8015F4EC[i].b = i * 1000 * i / 30;
            } else {
                D_8015F4EC[i].b = 0;
            }
        } else {
            D_8015F4EC[i].b = 0;
        }
    }
}
