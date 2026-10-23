#include "common.h"
#include "game.h"
#include "psx_types.h"

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", srn_tpage_show);

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
/* NON_MATCHING: T-25009, 17 rows (T-25006: 28); last now takes $s3, but clut (uopt priority 100) still
 * outranks the x counter and data, so clut sits in $s4 instead of $s7 and x/data shift; the p and ypos
 * spill slots are 4 bytes off (p is a spilled expression range, not its declared home).
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
                ypos = row * 8 + y + 0x168;
                do {
                    uv = *data & 0xFF0000;
                    page = *data & 0xFF000000;
                    clut = *data & 0xFF00;
                    uv >>= 16;
                    page >>= 24;
                    clut >>= 8;
                    s = (SPRT_8 *)GetWorkBase(0x10, D_8011ECA0);
                    func_8009F07C(s);
                    SetSemiTrans(s, 0);
                    s->x0 = x + 0xA0 + col * 8;
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
                    /* FAKE: T-25009, an extra read of last raises its priority above clut */
                    if (last) {}
                    s->clut = func_8009ED74(cx + clut * 16, cy);
                    AddPrim(D_800E8CA0 + (D_8011ECA0 << 10) + pri * 4 + 4, s);
                    data++;
                    col++;
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

#ifdef NON_MATCHING
/* NON_MATCHING: T-25104, 10 rows; clut takes $s1 before the x position and col (uopt priority
 * 133 against 103 and 78), so x/col/clut sit in s2/s3/s1 instead of s1/s2/s3. */
void func_80098B80(Rec24 *sp, void *ot, u16 pri);
void mod_set(s32 idx, s32 frame, s32 x, s32 y, s32 cx, s32 cy, s32 pri) {
    s32 padA[4];
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
                    uv = *data & 0xFF0000;
                    word = *data & 0xFF000000;
                    clut = *data & 0xFF00;
                    uv >>= 16;
                    word >>= 24;
                    clut >>= 8;
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
#else
INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", mod_set);
#endif

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
