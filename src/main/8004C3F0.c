#include "common.h"
#include "game.h"

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", srn_tpage_show);

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

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", get_fnt);

/* FAKE: taking the address of the argument forces the home-slot spill (sw a0,0(sp)); real source unknown. T-0400 */
void func_8004DAC4(s32 arg0) {
    s32 *p = &arg0;
}

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", mod_set_gpu);

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", mod_set);

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", srn_set);

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
