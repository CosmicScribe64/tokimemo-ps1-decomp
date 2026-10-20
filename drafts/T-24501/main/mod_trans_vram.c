typedef struct ModRec14 {
    /* 0x00 */ u8 **glyphs;
    /* 0x04 */ u32 *tiles;
    /* 0x08 */ u8 format;
    /* 0x09 */ u8 unk_09;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ u8 cols;
    /* 0x0D */ u8 rows;
    /* 0x0E */ u8 cell_size;
    /* 0x0F */ u8 unk_0F;
    /* 0x10 */ s16 unk_10;
    /* 0x12 */ s16 unk_12;
} ModRec14; /* size 0x14 */

void mod_trans_vram(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    u32 var_a0;
    /* 256-byte extent proved by get_fnt writes and 0x100-byte mirror reads.
     * Word-aligned storage view; original array element type is unknown. */
    u32 sp88[64];
    u8 sp87;
    s32 sp78;
    ModRec14 *sp74;
    u32 *var_s2;
    s32 temp_t0;
    s32 var_a1;
    s32 var_s1;
    u32 var_s0;
    u8 var_v0;
    u8 var_v1;
    ModRec14 *temp_t8;

    temp_t8 = (ModRec14 *)(D_801217A0 + arg0 * 0x14);
    sp74 = temp_t8;
    if (temp_t8->cell_size == 8) {
        var_a1 = 8;
    } else {
        sp78 = 0x10;
        var_a1 = 0x10;
    }
    switch (sp74->format) {
    case 0:
        sp87 = 1;
        break;
    case 1:
        sp87 = 2;
        break;
    case 2:
        sp87 = 2;
        break;
    case 3:
        sp87 = 1;
        break;
    case 4:
        sp87 = 1;
        break;
    case 5:
        sp87 = 1;
        break;
    default:
        sp87 = 1;
        break;
    }
    var_a0 = 0;
    var_v1 = sp74->rows;
    var_v0 = sp74->cols;
    var_s2 = sp74->tiles + var_v1 * arg1 * var_v0;
    if (var_v1 != 0) {
        do {
            var_s0 = 0;
            if (var_v0 != 0) {
                var_s1 = 0;
                sp78 = var_a1;
                do {
                    temp_t0 = *var_s2;
                    get_fnt(sp88, sp74->glyphs[(u32)(temp_t0 & 0xFF000000) >> 24], (s32) ((u32) (temp_t0 & 0xFF0000) >> 0x10), (s32) sp87, sp78, temp_t0 & 0xFF);
                    LoadSquare((((arg2 & 0xF) << 6) + ((u32) (arg3 / 2) / sp87) + ((u32) (var_s1 / 2) / sp87)) & 0xFFFF, (((arg2 & 0x10) * 0x10) + ((arg2 & 0x20) * 4) + arg4 + (var_a0 * var_a1)) & 0xFFFF & 0xFFFF, ((u32) (var_a1 / 2) / sp87) & 0xFFFF & 0xFFFF, var_a1 & 0xFFFF & 0xFFFF, sp88);
                    var_v0 = sp74->cols;
                    var_s0 += 1;
                    var_s2 += 1;
                    var_s1 += sp78;
                } while (var_s0 < var_v0);
                var_v1 = sp74->rows;
                var_a1 = sp78;
            }
            var_a0 += 1;
        } while (var_a0 < var_v1);
    }
}
