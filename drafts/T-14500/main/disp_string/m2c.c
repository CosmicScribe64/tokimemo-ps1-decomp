extern u8 D_800B3DC6;
extern s16 D_800B3F68;
extern u8 D_800E62BA;
extern u8 text_glyph_advance;
extern s16 text_glyph_count;

void disp_string(void) {
    s32 temp_s6;
    s32 temp_v0_2;
    s32 temp_v0_4;
    s32 var_fp;
    s32 var_s4;
    s8 temp_s2;
    s8 temp_t0;
    u32 var_s7;
    u8 temp_v0;
    u8 temp_v0_3;
    u8 temp_v0_5;
    u8 var_s5;
    void *var_s1;

    var_s5 = D_800B3DC6;
    if (var_s5 == 0) {
        var_s5 = 1;
    }
    var_fp = 1;
    var_s4 = 0;
    var_s7 = 0;
    if ((text_glyph_count > 0) && (D_800B3F68 > 0)) {
        var_s1 = (0 * 8) + text_list;
loop_5:
        if (var_s7 >= (u8) *(u8 *)((unsigned char *)var_s1 + (5))) {
            var_s7 = 0;
            var_s1 += 8;
        }
        if (*(s8 *)((unsigned char *)var_s1 + (7)) == 1) {
            temp_v0 = *(u8 *)((unsigned char *)var_s1 + (6));
            if (var_s5 != temp_v0) {
                var_s5 = temp_v0;
                var_fp = 1;
                if (temp_v0 == 0) {
                    var_s5 = 1;
                }
            }
            temp_s6 = var_s5 * 4;
            temp_v0_2 = GetWorkBase(0x10, D_8011ECA0);
            func_8009F090(temp_v0_2);
            SetSemiTrans(temp_v0_2, 0);
            SetShadeTex(temp_v0_2, 0);
            *(u8 *)((unsigned char *)temp_v0_2 + (4)) = (u8) D_800E62BA;
            *(u8 *)((unsigned char *)temp_v0_2 + (5)) = (u8) D_800E62BA;
            *(u8 *)((unsigned char *)temp_v0_2 + (6)) = (u8) D_800E62BA;
            *(s16 *)((unsigned char *)temp_v0_2 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + var_s1->unk0 + (var_s7 * text_glyph_advance));
            temp_t0 = (var_s4 & 0x78) * 2;
            temp_s2 = ((var_s4 & 7) * 0x10) + 0x80;
            *(s16 *)((unsigned char *)temp_v0_2 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + *(s16 *)((unsigned char *)var_s1 + (2)));
            *(s8 *)((unsigned char *)temp_v0_2 + (12)) = temp_s2;
            *(s8 *)((unsigned char *)temp_v0_2 + (13)) = temp_t0;
            temp_v0_3 = *(u8 *)((unsigned char *)var_s1 + (4));
            *(s16 *)((unsigned char *)temp_v0_2 + (14)) = func_8009ED74(((temp_v0_3 & 8) * 4) + 0x100, (temp_v0_3 & 7) + 0x1F8);
            AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + temp_s6], temp_v0_2);
            temp_v0_4 = GetWorkBase(0x10, D_8011ECA0);
            func_8009F090(temp_v0_4);
            SetSemiTrans(temp_v0_4, 0);
            SetShadeTex(temp_v0_4, 0);
            *(s8 *)((unsigned char *)temp_v0_4 + (4)) = 0;
            *(s8 *)((unsigned char *)temp_v0_4 + (5)) = 0;
            *(s8 *)((unsigned char *)temp_v0_4 + (6)) = 0;
            *(s16 *)((unsigned char *)temp_v0_4 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + var_s1->unk0 + (var_s7 * text_glyph_advance) + 1);
            *(s8 *)((unsigned char *)temp_v0_4 + (13)) = temp_t0;
            *(s16 *)((unsigned char *)temp_v0_4 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + *(s16 *)((unsigned char *)var_s1 + (2)) + 1);
            *(s8 *)((unsigned char *)temp_v0_4 + (12)) = temp_s2;
            temp_v0_5 = *(u8 *)((unsigned char *)var_s1 + (4));
            *(s16 *)((unsigned char *)temp_v0_4 + (14)) = func_8009ED74(((temp_v0_5 & 8) * 4) + 0x110, (temp_v0_5 & 7) + 0x1F8);
            AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + temp_s6], temp_v0_4);
            if (var_s4 == 0x7F) {
                var_fp = 0;
                goto_tpage(0x300, 0x100, (s32) var_s5, 0, 0);
            } else if (var_s4 == 0xFF) {
                var_fp = 0;
                goto_tpage(0x340, 0x100, (s32) var_s5, 0, 0);
            } else if (var_fp != 0) {
                var_fp = 0;
                if (var_s4 >= 0x100) {
                    goto_tpage(0x380, 0x100, (s32) var_s5, 0, 0);
                } else if (var_s4 >= 0x80) {
                    goto_tpage(0x340, 0x100, (s32) var_s5, 0, 0);
                } else {
                    goto_tpage(0x300, 0x100, (s32) var_s5, 0, 0);
                }
            }
        }
        var_s4 += 1;
        var_s7 += 1;
        if ((var_s4 < text_glyph_count) && (var_s4 < D_800B3F68)) {
            goto loop_5;
        }
    }
    if (var_s4 >= 0x100) {
        goto_tpage(0x380, 0x100, (s32) var_s5, 0, 0);
        return;
    }
    if (var_s4 >= 0x80) {
        goto_tpage(0x340, 0x100, (s32) var_s5, 0, 0);
        return;
    }
    goto_tpage(0x300, 0x100, (s32) var_s5, 0, 0);
}
