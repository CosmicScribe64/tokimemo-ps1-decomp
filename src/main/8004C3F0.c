#include "common.h"
#include "game.h"
#include "psx_types.h"

/* Adds five POLY_FT4 quads to the ordering table: three 256-pixel-wide pages (at x, x + 0x100 and
 * x + 0x200, scrolled left by the low byte of arg4) and two 1-pixel-wide columns between them,
 * textured from VRAM x 0x3C0 (the first one goes to ordering-table entry arg8 - 1). arg0 selects
 * the texture page bits, arg1 the colour mode, arg2 the semi-transparency; arg5 is unused, arg6/arg7
 * are the clut position and arg8 the ordering-table entry.
 * Matching notes (T-25104): the repeated expressions stay inline; IDO keeps them as common
 * subexpressions and spills them to its own stack slots across the calls.
 * FAKE (layout): T-25104, the colour, UV and XY stores of each quad are one comma statement each, in
 * the field order of the libgpu setRGB0/setUV4/setXY4 macros (likely the original's macros): as1
 * schedules by source line, and this ucode order gives the original's store order and decides which
 * common subexpression gets v0 or v1. One statement per field misses 57 rows. */
void srn_tpage_show(u8 arg0, u8 arg1, u8 arg2, RECT *arg3, s32 arg4, s32 arg5, s16 arg6, s16 arg7, s32 arg8) {
    GameState *g = &D_800E6280;
    POLY_FT4 *poly;
    s32 divisor;
    s32 pageWidth;

    /* FAKE: T-25104, one line per case so that as1 stores pageWidth before divisor */
    switch (arg1) {
    case 0:
        divisor = 4; pageWidth = 0x40;
        break;
    case 1:
        divisor = 2; pageWidth = 0x80;
        break;
    case 2:
        divisor = 1; pageWidth = 0x100;
        break;
    }

    /* left page */
    poly = (POLY_FT4 *)GetWorkBase(0x28, D_8011ECA0);
    func_8009F040(poly);
    if (arg2 != 0) {
        SetSemiTrans(poly, 1);
    } else {
        SetSemiTrans(poly, 0);
    }
    SetShadeTex(poly, 0);
    poly->r0 = g->unk_03A, poly->g0 = g->unk_03A, poly->b0 = g->unk_03A;
    poly->u0 = 0, poly->v0 = (arg0 & 0x20) * 4,
        poly->u1 = 0xFF, poly->v1 = (arg0 & 0x20) * 4,
        poly->u2 = 0, poly->v2 = (arg0 & 0x20) * 4 + arg3->h,
        poly->u3 = 0xFF, poly->v3 = (arg0 & 0x20) * 4 + arg3->h;
    poly->x0 = g->unk_014[D_8011ECA0] + -(arg4 & 0xFF) + arg3->x, poly->y0 = g->unk_018[D_8011ECA0] + arg3->y,
        poly->x1 = g->unk_014[D_8011ECA0] + -(arg4 & 0xFF) + arg3->x + 0xFF, poly->y1 = g->unk_018[D_8011ECA0] + arg3->y,
        poly->x2 = g->unk_014[D_8011ECA0] + -(arg4 & 0xFF) + arg3->x, poly->y2 = g->unk_018[D_8011ECA0] + arg3->y + arg3->h,
        poly->x3 = g->unk_014[D_8011ECA0] + -(arg4 & 0xFF) + arg3->x + 0xFF, poly->y3 = g->unk_018[D_8011ECA0] + arg3->y + arg3->h;
    poly->tpage = func_8009ECB0(arg1, arg2 & 3, ((arg0 & 0xF) << 6) + (arg4 & 0x7F00) / divisor, (arg0 & 0x10) * 0x10 + (arg0 & 0x20) * 4);
    poly->clut = func_8009ED74(arg6, arg7);
    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + arg8 * 4], poly);

    /* 1-pixel column after the left page */
    poly = (POLY_FT4 *)GetWorkBase(0x28, D_8011ECA0);
    func_8009F040(poly);
    if (arg2 != 0) {
        SetSemiTrans(poly, 1);
    } else {
        SetSemiTrans(poly, 0);
    }
    SetShadeTex(poly, 0);
    poly->r0 = g->unk_03A, poly->g0 = g->unk_03A, poly->b0 = g->unk_03A;
    poly->u0 = arg4 / 256 * 4 + 3, poly->v0 = 0,
        poly->u1 = arg4 / 256 * 4 + 4, poly->v1 = 0,
        poly->u2 = arg4 / 256 * 4 + 3, poly->v2 = arg3->h,
        poly->u3 = arg4 / 256 * 4 + 4, poly->v3 = arg3->h;
    poly->x0 = g->unk_014[D_8011ECA0] + (0xFF - (arg4 & 0xFF)) + arg3->x, poly->y0 = g->unk_018[D_8011ECA0] + arg3->y,
        poly->x1 = g->unk_014[D_8011ECA0] + (0xFF - (arg4 & 0xFF)) + arg3->x + 1, poly->y1 = g->unk_018[D_8011ECA0] + arg3->y,
        poly->x2 = g->unk_014[D_8011ECA0] + (0xFF - (arg4 & 0xFF)) + arg3->x, poly->y2 = g->unk_018[D_8011ECA0] + arg3->y + arg3->h,
        poly->x3 = g->unk_014[D_8011ECA0] - (arg4 & 0xFF) + arg3->x + 0x100, poly->y3 = g->unk_018[D_8011ECA0] + arg3->y + arg3->h;
    poly->tpage = func_8009ECB0(arg1, arg2 & 3, 0x3C0, 0);
    poly->clut = func_8009ED74(arg6, arg7);
    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + arg8 * 4 - 4], poly);

    /* middle page */
    poly = (POLY_FT4 *)GetWorkBase(0x28, D_8011ECA0);
    func_8009F040(poly);
    if (arg2 != 0) {
        SetSemiTrans(poly, 1);
    } else {
        SetSemiTrans(poly, 0);
    }
    SetShadeTex(poly, 0);
    poly->r0 = g->unk_03A, poly->g0 = g->unk_03A, poly->b0 = g->unk_03A;
    poly->u0 = 0, poly->v0 = (arg0 & 0x20) * 4,
        poly->u1 = 0xFF, poly->v1 = (arg0 & 0x20) * 4,
        poly->u2 = 0, poly->v2 = (arg0 & 0x20) * 4 + arg3->h,
        poly->u3 = 0xFF, poly->v3 = (arg0 & 0x20) * 4 + arg3->h;
    poly->x0 = g->unk_014[D_8011ECA0] + -(arg4 & 0xFF) + arg3->x + 0x100, poly->y0 = g->unk_018[D_8011ECA0] + arg3->y,
        poly->x1 = g->unk_014[D_8011ECA0] + -(arg4 & 0xFF) + arg3->x + 0x1FF, poly->y1 = g->unk_018[D_8011ECA0] + arg3->y,
        poly->x2 = g->unk_014[D_8011ECA0] + -(arg4 & 0xFF) + arg3->x + 0x100, poly->y2 = g->unk_018[D_8011ECA0] + arg3->y + arg3->h,
        poly->x3 = g->unk_014[D_8011ECA0] + -(arg4 & 0xFF) + arg3->x + 0x1FF, poly->y3 = g->unk_018[D_8011ECA0] + arg3->y + arg3->h;
    poly->tpage = func_8009ECB0(arg1, arg2 & 3, ((arg0 & 0xF) << 6) + (arg4 & 0x7F00) / divisor + pageWidth, (arg0 & 0x10) * 0x10 + (arg0 & 0x20) * 4);
    poly->clut = func_8009ED74(arg6, arg7);
    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + arg8 * 4], poly);

    /* 1-pixel column after the middle page */
    poly = (POLY_FT4 *)GetWorkBase(0x28, D_8011ECA0);
    func_8009F040(poly);
    if (arg2 != 0) {
        SetSemiTrans(poly, 1);
    } else {
        SetSemiTrans(poly, 0);
    }
    SetShadeTex(poly, 0);
    poly->r0 = g->unk_03A, poly->g0 = g->unk_03A, poly->b0 = g->unk_03A;
    poly->u0 = arg4 / 256 * 4 + 7, poly->v0 = 0,
        poly->u1 = arg4 / 256 * 4 + 8, poly->v1 = 0,
        poly->u2 = arg4 / 256 * 4 + 7, poly->v2 = arg3->h,
        poly->u3 = arg4 / 256 * 4 + 8, poly->v3 = arg3->h;
    poly->x0 = g->unk_014[D_8011ECA0] + (0xFF - (arg4 & 0xFF)) + arg3->x + 0x100, poly->y0 = g->unk_018[D_8011ECA0] + arg3->y,
        poly->x1 = g->unk_014[D_8011ECA0] + (0xFF - (arg4 & 0xFF)) + arg3->x + 0x101, poly->y1 = g->unk_018[D_8011ECA0] + arg3->y,
        poly->x2 = g->unk_014[D_8011ECA0] + (0xFF - (arg4 & 0xFF)) + arg3->x + 0x100, poly->y2 = g->unk_018[D_8011ECA0] + arg3->y + arg3->h,
        poly->x3 = g->unk_014[D_8011ECA0] - (arg4 & 0xFF) + arg3->x + 0x200, poly->y3 = g->unk_018[D_8011ECA0] + arg3->y + arg3->h;
    poly->tpage = func_8009ECB0(arg1, arg2 & 3, 0x3C0, 0);
    poly->clut = func_8009ED74(arg6, arg7);
    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + arg8 * 4], poly);

    /* right page */
    poly = (POLY_FT4 *)GetWorkBase(0x28, D_8011ECA0);
    func_8009F040(poly);
    if (arg2 != 0) {
        SetSemiTrans(poly, 1);
    } else {
        SetSemiTrans(poly, 0);
    }
    SetShadeTex(poly, 0);
    poly->r0 = g->unk_03A, poly->g0 = g->unk_03A, poly->b0 = g->unk_03A;
    poly->u0 = 0, poly->v0 = (arg0 & 0x20) * 4,
        poly->u1 = 0xFF, poly->v1 = (arg0 & 0x20) * 4,
        poly->u2 = 0, poly->v2 = (arg0 & 0x20) * 4 + arg3->h,
        poly->u3 = 0xFF, poly->v3 = (arg0 & 0x20) * 4 + arg3->h;
    poly->x0 = g->unk_014[D_8011ECA0] + -(arg4 & 0xFF) + arg3->x + 0x200, poly->y0 = g->unk_018[D_8011ECA0] + arg3->y,
        poly->x1 = g->unk_014[D_8011ECA0] + -(arg4 & 0xFF) + arg3->x + 0x2FF, poly->y1 = g->unk_018[D_8011ECA0] + arg3->y,
        poly->x2 = g->unk_014[D_8011ECA0] + -(arg4 & 0xFF) + arg3->x + 0x200, poly->y2 = g->unk_018[D_8011ECA0] + arg3->y + arg3->h,
        poly->x3 = g->unk_014[D_8011ECA0] + -(arg4 & 0xFF) + arg3->x + 0x2FF, poly->y3 = g->unk_018[D_8011ECA0] + arg3->y + arg3->h;
    poly->tpage = func_8009ECB0(arg1, arg2 & 3, ((arg0 & 0xF) << 6) + (arg4 & 0x7F00) / divisor + pageWidth * 2, (arg0 & 0x10) * 0x10 + (arg0 & 0x20) * 4);
    poly->clut = func_8009ED74(arg6, arg7);
    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + arg8 * 4], poly);
}

/* Matching notes (T-17015): the count and order of the 15 declared locals set the frame size and the
 * spilled home slots, so do not add or drop one. The masked table word is written twice ((*src & 0xFFFF)
 * in the test and in the call) because any local for it changes the registers; uopt merges the two.
 * Only the error path returns a value, the normal path falls off the end (as in the original). */
s32 srn_vram_set(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    u32 *src;
    s32 left;
    s32 w0;
    s32 h0;
    s32 tw;
    s32 th;
    s32 ox;
    s32 oy;
    s32 cw;
    s32 ch;
    s32 cols;
    s32 rows;
    s32 row;
    s32 col;
    SrnRec28 *rec;

    func_8009C674(0);
    rec = (SrnRec28 *)(D_80121750 + arg0 * 0x28);
    w0 = rec->unk_0E;
    h0 = rec->unk_0F;
    tw = rec->unk_10;
    th = rec->unk_11;
    ox = rec->unk_12;
    oy = rec->unk_13;
    cw = rec->unk_14;
    ch = rec->unk_15;
    src = rec->unk_08;
    if (h0 == 0 || w0 == 0) {
        printf("ERROR");
        return -1;
    }
    left = rec->unk_1C;
    row = 0;
    cols = cw * tw / w0;
    rows = ch * th / h0;
    if (rows > 0) {
        do {
            col = 0;
            if (cols > 0) {
                do {
                    if ((col - 1) * tw + ox * w0 < arg2 && (row - 1) * th + oy * h0 < arg3) {
                        if ((*src & 0xFFFF) != 0) {
                            mod_trans_vram(0, *src & 0xFFFF, arg1, tw * col, th * row);
                        }
                    }
                    left--;
                    src++;
                    if (left == 0) {
                        row = rows;
                        col = cols;
                    }
                    col++;
                } while (col < cols);
            }
            row++;
        } while (row < rows);
    }
    if ((u8)D_800B3DB0 != 0) {
        move_255_line(arg1, arg1 + 0xB, 0xF);
    }
}

void move_255_line(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;

    for (i = arg0; i <= arg1; i++) {
        MoveSquare((i * 0x40 + 0x3F) & 0xFFFF, 0, 1, 0xFF, ((arg2 & 0xF) << 6) + i - arg0, ((arg2 & 0x10) * 0x10) & 0xFFFF);
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", mod_trans_vram);

/* FAKE: the integer return type without a return preserves the original v0
 * lifetime and exit scheduling; callers discard the result. T-24601. */
s32 get_fnt(u8 *output, u8 *source, s32 glyph, s32 divisor, s32 width, u8 flags) {
    /* FAKE: unknown original locals occupy 20 bytes above the temporary and
     * four below it, placing the 256-byte copy at sp+0x8C in a 0x1A0 frame.
     * Neither pad is read or written. T-24601. */
    s32 pad_above[5];
    u8 pixels[256];
    s32 pad_below;
    s32 tiles_per_row;
    s32 row;
    s32 row_offset;
    s32 mirror_row;

    row = 0;
    if (width > 0) {
        tiles_per_row = 0x80 / width;
        row_offset = 0;
        do {
            memcpy((row_offset / divisor) + output, ((s32) ((glyph % tiles_per_row) * width) / divisor) + source + ((s32) ((((glyph / tiles_per_row) * width) + row) << 7) / divisor), width / divisor);
            row += 1;
            row_offset += width;
        } while (row != width);
    }
    if (flags & 1) {
        memcpy(pixels, output, 0x100);
        for (mirror_row = 0; mirror_row < width; mirror_row++) {
            memcpy(output + (width - mirror_row - 1) * width, pixels + mirror_row * width, width);
        }
    }
    if (flags & 2) {
        memcpy(pixels, output, 0x100);
        for (mirror_row = 0; mirror_row < width; mirror_row++) {
            output[width * mirror_row + 4] = pixels[width * mirror_row + 3];
            output[width * mirror_row + 5] = pixels[width * mirror_row + 2];
            output[width * mirror_row + 6] = pixels[width * mirror_row + 1];
            output[width * mirror_row + 7] = pixels[width * mirror_row];
            output[width * mirror_row] = pixels[width * mirror_row + 7];
            output[width * mirror_row + 1] = pixels[width * mirror_row + 6];
            output[width * mirror_row + 2] = pixels[width * mirror_row + 5];
            output[width * mirror_row + 3] = pixels[width * mirror_row + 4];
        }
    }
}

/* FAKE: taking the address of the argument forces the home-slot spill (sw a0,0(sp)); real source unknown. T-0400 */
void func_8004DAC4(s32 arg0) {
    s32 *p = &arg0;
}

#ifdef NON_MATCHING
/* NON_MATCHING: T-25104, 4 rows (T-25009: 17); registers, order and the other stack slots match. Left:
 * the record pointer p is spilled at sp+0x68 instead of sp+0x6C. Here p is a copy-propagated expression
 * whose spill temporary is the first one below the declared locals; the original has one more frame
 * word above the hoisted temporaries (drafts/T-25104/main/mod_set_gpu/NOTES.md).
 * search_tpage_multi is viewed as returning u16 here (the original compares the raw result and keeps
 * the narrowed copy). */
u16 search_tpage_multi();
void mod_set_gpu(s32 idx, s32 frame, s32 x, s32 y, s32 cx, s32 cy, s32 pri) {
    s32 padA; /* FAKE: T-25006, padA and padB put row and mode at the original stack homes */
    s32 padB;
    u32 row;
    SPRT_8 *s;
    u32 col;
    u32 *data;
    u32 uv;
    u32 page;
    u32 clut;
    u16 tpage;
    u16 last;
    s32 bright;
    s32 mode;
    s32 ypos;
    u8 rows;
    u8 cols;
    ModRec14 *p;

    bright = func_8004901C();
    p = (ModRec14 *)(D_801217A0 + idx * 0x14);
    switch (p->unk_08) {
    case 0:
        mode = 1;
        break;
    case 1:
        mode = 2;
        break;
    case 2:
        mode = 2;
        break;
    case 3:
        mode = 1;
        break;
    case 4:
        mode = 1;
        break;
    case 5:
        mode = 1;
        break;
    default:
        mode = 1;
        break;
    }
    last = 0xFFFF;
    rows = p->unk_0D;
    cols = p->unk_0C;
    data = rows * frame * cols + p->unk_04;
    row = 0;
    if (rows != 0) {
        do {
            col = 0;
            if (cols != 0) {
                do {
                    uv = (*data & 0xFF0000) >> 16;
                    page = (*data & 0xFF000000) >> 24;
                    clut = (*data & 0xFF00) >> 8;
                    s = (SPRT_8 *)GetWorkBase(0x10, D_8011ECA0);
                    func_8009F07C(s);
                    SetSemiTrans(s, 0);
                    s->x0 = x + 0xA0 + col * 8;
                    ypos = row * 8 + y + 0x168;
                    s->y0 = ypos - D_8011ECA0 * 0xF0;
                    /* FAKE: T-25006, one line so as1 keeps the u0/v0 order */
                    s->u0 = (uv & 0xF) * 8; s->v0 = ((uv & 0xF0) >> 4) * 8;
                    s->b0 = bright;
                    s->g0 = bright;
                    s->r0 = bright;
                    tpage = search_tpage_multi(p->unk_00[page], mode);
                    if (last != tpage) {
                        if (last != 0xFFFF) {
                            goto_tpage((last & 0xF) << 6, (last & 0x10) << 4, pri + 1, 0, 0);
                        }
                        last = tpage;
                    }
                    s->clut = func_8009ED74(cx + clut * 16, cy);
                    AddPrim(D_800E8CA0 + (D_8011ECA0 << 10) + pri * 4 + 4, s);
                    col++;
                    data++;
                    cols = p->unk_0C;
                } while (col < cols);
                rows = p->unk_0D;
            }
            row++;
        } while (row < rows);
    }
    goto_tpage((last & 0xF) << 6, (last & 0x10) << 4, pri + 1, 0, 0);
}
#else
INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", mod_set_gpu);
#endif

/* Matching notes (T-25104): sp is the sprite handed to func_80098B80 (GsSortSprite); the u16 priority
 * of its prototype converts pri at each call. Each tile field is masked and shifted in one statement:
 * the split form (mask, then shift) gives uv and clut more uses, and clut then outranks the x position
 * and col for the saved registers. The y position is written row * w + y with a signed row. */
void mod_set(s32 idx, s32 frame, s32 x, s32 y, s32 cx, s32 cy, s32 pri) {
    s32 padA[4]; /* FAKE: T-25104, padA, padB and padC put row, sp and mode at the original stack homes */
    s32 row;
    Rec24 sp;
    ModRec14 *p;
    u32 col;
    u32 *data;
    u32 word;
    u32 uv;
    u32 clut;
    s32 bright;
    u8 rows;
    u8 cols;
    u8 w;
    s32 padB[7];
    s32 mode;
    s32 padC[1];

    bright = func_8004901C();
    p = (ModRec14 *)(D_801217A0 + idx * 0x14);
    /* FAKE: T-25104, one line per case so that as1 stores mode before the attribute in cases 3 to 5 */
    switch (p->unk_08) {
    case 0: sp.unk_00 = 0x01000000; mode = 1; break;
    case 1: mode = 2; sp.unk_00 = 0; break;
    case 2: mode = 2; sp.unk_00 = 0; break;
    case 3: sp.unk_00 = 0x01000000; mode = 1; break;
    case 4: sp.unk_00 = 0x01000000; mode = 1; break;
    case 5: sp.unk_00 = 0x01000000; mode = 1; break;
    default: sp.unk_00 = 0x01000000; mode = 1; break;
    }
    rows = p->unk_0D;
    cols = p->unk_0C;
    w = p->unk_0E[0];
    sp.unk_0A = p->unk_0E[1];
    sp.unk_18 = 0;
    sp.unk_1A = 0;
    sp.unk_14 = bright;
    sp.unk_15 = bright;
    sp.unk_16 = bright;
    sp.unk_20 = 0;
    sp.unk_1C = 0x1000;
    sp.unk_1E = 0x1000;
    row = 0;
    sp.unk_08 = w;
    data = rows * frame * cols + p->unk_04;
    if (rows != 0) {
        do {
            col = 0;
            if (cols != 0) {
                do {
                    sp.unk_04 = x + col * w;
                    sp.unk_06 = row * w + y;
                    uv = (*data & 0xFF0000) >> 16;
                    word = (*data & 0xFF000000) >> 24;
                    clut = (*data & 0xFF00) >> 8;
                    sp.unk_0C = search_tpage_multi(p->unk_00[word], mode);
                    switch (w) {
                    case 8:
                        sp.unk_0E = (uv & 0xF) * 8;
                        sp.unk_0F = ((uv & 0xF0) >> 4) * 8;
                        break;
                    case 16:
                        sp.unk_0E = (uv & 7) * 16;
                        sp.unk_0F = ((uv & 0xF8) >> 4) * 16;
                        break;
                    default:
                        sp.unk_0E = (uv & 0xF) * 8;
                        sp.unk_0F = ((uv & 0xF0) >> 4) * 8;
                        break;
                    }
                    sp.unk_10 = cx + clut * 16;
                    sp.unk_12 = cy;
                    func_80098B80(&sp, (u8 *)&D_800E8C70 + D_8011ECA0 * 0x14, pri);
                    col++;
                    data++;
                    cols = p->unk_0C;
                } while (col < cols);
                rows = p->unk_0D;
            }
            row++;
        } while (row < rows);
    }
}

/* Matching notes (T-25006): the record fields are read through the global array (SRN_REC), not a
 * pointer, the row and column positions are written as expressions (uopt strength-reduces them into the
 * two induction variables), and cols, rows are computed before wid, hgt, left so cols wins $fp. */
s32 srn_set(s32 arg0) {
    SrnRec28 *rec; /* FAKE: T-25006, unused; its stack slot keeps the frame at 0xC0 and the spill homes */
    s32 w0;
    s32 h0;
    s32 tw;
    s32 th;
    s32 ox;
    s32 oy;
    s32 cw;
    s32 ch;
    s32 xoff;
    s32 rows;
    s32 yoff;
    u32 *src;
    s32 row;
    s32 left;
    s32 col;
    s32 wid;
    s32 hgt;
    s32 x;
    s32 idx;
    s32 cols;
    w0 = SRN_REC(arg0).unk_0E;
    h0 = SRN_REC(arg0).unk_0F;
    tw = SRN_REC(arg0).unk_10;
    th = SRN_REC(arg0).unk_11;
    ox = SRN_REC(arg0).unk_12;
    oy = SRN_REC(arg0).unk_13;
    cw = SRN_REC(arg0).unk_14;
    ch = SRN_REC(arg0).unk_15;
    xoff = SRN_REC(arg0).unk_02;
    yoff = SRN_REC(arg0).unk_06;
    src = SRN_REC(arg0).unk_08;
    if (h0 == 0 || w0 == 0) {
        return -1;
    }
    cols = cw * tw / w0;
    rows = ch * th / h0;
    wid = SRN_REC(arg0).unk_16;
    hgt = SRN_REC(arg0).unk_18;
    left = SRN_REC(arg0).unk_1C;
    for (row = 0; row < rows; row++) {
        if (th * row + yoff + h0 * oy >= -0x77) {
            for (col = 0; col < cols; col++) {
                if (th * row + yoff >= 0x79) {
                    return 0;
                }
                x = tw * col + xoff;
                if (tw * col + xoff + ox * w0 >= -0x9F) {
                    if ((*src & 0xFFFF) != 0) {
                        idx = *src & 0xFFFF;
                        mod_set_gpu(0, idx, x, th * row + yoff, wid, hgt, SRN_REC(0).unk_0C);
                    }
                }
                left--;
                src++;
                if (left == 0) {
                    return 0;
                }
            }
        }
    }
}


void srn_init(s32 arg0, u32 *arg1, s32 arg2) {
    u8 *p;

    p = D_80121750 + arg0 * 0x28;
    p[0xD] = (arg1[0] & 0xFF00) >> 8;
    *(s16 *)(p + 2) = 0;
    *(s16 *)(p + 6) = 0;
    p[0xC] = 0;
    *(u32 **)(p + 8) = arg1 + 5;
    p[0xE] = (arg1[1] & 0xFF000000) >> 24;
    p[0xF] = (arg1[1] & 0xFF0000) >> 16;
    p[0x10] = (arg1[1] & 0xFF00) >> 8;
    /* FAKE: the no-op "& 0xFF" consumes one IDO temp number, as in func_8004E44C. T-4070 */
    p[0x11] = arg1[1] & 0xFF;
    p[0x12] = (arg1[2] & 0xFF000000) >> 24;
    p[0x13] = (arg1[2] & 0xFF0000) >> 16;
    p[0x14] = (arg1[2] & 0xFF00) >> 8;
    p[0x15] = arg1[2] & 0xFF; /* FAKE: same no-op mask as above. T-4070 */
    *(s16 *)(p + 0x16) = 0;
    *(s16 *)(p + 0x18) = 0x1E0;
    *(u32 *)(p + 0x1C) = arg1[4];
    *(u32 *)(p + 0x20) = arg1[3];
    *(u8 **)(p + 0x24) = D_801217A0 + arg2 * 0x14;
}

void func_8004E44C(s32 arg0, u32 *arg1, s32 arg2) {
    u8 *p;

    p = D_801217A0 + arg0 * 0x14;
    *(s32 *)p = arg2;
    *(u32 **)(p + 4) = arg1 + 4;
    p[8] = (arg1[0] & 0xFF000000) >> 24;
    *(s16 *)(p + 0xA) = (arg1[0] & 0xFFFF) >> 24;
    p[0xC] = (arg1[1] & 0xFF000000) >> 24;
    p[0xD] = (arg1[1] & 0xFF0000) >> 16;
    p[0xE] = (arg1[1] & 0xFF00) >> 8;
    /* FAKE: the no-op "& 0xFF" consumes one IDO temp number, as in the original. T-2090 */
    p[0xF] = arg1[1] & 0xFF;
    *(s16 *)(p + 0x10) = ((u8 *)arg1)[8];
    *(s16 *)(p + 0x12) = ((u8 *)arg1)[0xC];
}
