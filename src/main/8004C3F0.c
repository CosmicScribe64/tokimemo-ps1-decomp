#include "common.h"
#include "game.h"

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

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", mod_set_gpu);

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", mod_set);

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
