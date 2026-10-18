typedef struct ModRec14 {
    /* 0x00 */ u8 **unk_00;
    /* 0x04 */ u32 *unk_04;
    /* 0x08 */ u8 unk_08;
    /* 0x09 */ u8 pad_09;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ u8 unk_0C;
    /* 0x0D */ u8 unk_0D;
    /* 0x0E */ u8 unk_0E;
    /* 0x0F */ u8 unk_0F;
    /* 0x10 */ s16 unk_10;
    /* 0x12 */ s16 unk_12;
} ModRec14;
void mod_trans_vram(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    u32 sp194;
    u8 sp88[256];
    u8 sp87;
    s32 sp78;
    ModRec14 *sp74;
    s32 *var_s2;
    s32 temp_t0;
    s32 var_a1;
    s32 var_s1;
    u32 var_a0;
    u32 var_s0;
    u8 temp_t3;
    u8 var_v0;
    u8 var_v1;
    ModRec14 *temp_t8;

    temp_t8 = (ModRec14 *)(D_801217A0 + arg0 * 0x14);
    sp74 = temp_t8;
    if (temp_t8->unk_0E == 8) {
        var_a1 = 8;
    } else {
        sp78 = 0x10;
        var_a1 = 0x10;
    }
    temp_t3 = sp74->unk_08;
    switch (temp_t3) {
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
    var_v1 = sp74->unk_0D;
    var_v0 = sp74->unk_0C;
    var_s2 = sp74->unk_04 + var_v1 * arg1 * var_v0;
    if (var_v1 != 0) {
        do {
            var_s0 = 0;
            if (var_v0 != 0) {
                var_s1 = 0;
                sp194 = var_a0;
                sp78 = var_a1;
                do {
                    temp_t0 = *var_s2;
                    get_fnt((s32) &sp88, sp74->unk_00[(temp_t0 & 0xFF000000) >> 24], (s32) ((u32) (temp_t0 & 0xFF0000) >> 0x10), (s32) sp87, sp78, temp_t0 & 0xFF);
                    LoadSquare((((arg2 & 0xF) << 6) + ((u32) (arg3 / 2) / sp87) + ((u32) (var_s1 / 2) / sp87)) & 0xFFFF, (((arg2 & 0x10) * 0x10) + ((arg2 & 0x20) * 4) + arg4 + (var_a0 * var_a1)) & 0xFFFF & 0xFFFF, ((u32) (var_a1 / 2) / sp87) & 0xFFFF & 0xFFFF, var_a1 & 0xFFFF & 0xFFFF, &sp88);
                    var_v0 = sp74->unk_0C;
                    var_s0 += 1;
                    var_s2 += 1;
                    var_s1 += sp78;
                } while (var_s0 < var_v0);
                var_v1 = sp74->unk_0D;
                var_a0 = sp194;
                var_a1 = sp78;
            }
            var_a0 += 1;
        } while (var_a0 < var_v1);
    }
}
