void glyph_expand_1bpp(s32 arg0, void *arg1) {
    ? spB0;
    u8 sp20;
    s32 temp_t0;
    s32 temp_t2;
    s32 temp_t3;
    s32 temp_t3_2;
    s32 temp_t7;
    s32 var_a2_2;
    s32 var_a2_3;
    s32 var_a2_4;
    s32 var_v0;
    u8 *var_a2;
    u8 *var_v1;
    u8 *var_v1_2;
    u8 temp_t4;
    u8 temp_t4_2;
    u8 temp_t7_2;
    void *temp_a1;
    void *temp_a1_2;
    void *var_a1;

    var_a1 = arg1;
    var_a2 = (arg0 * 0x12) + font_glyphs;
    var_v1 = &sp20;
    do {
        var_v1 += 8;
        *(s8 *)((unsigned char *)var_v1 + (-8)) = (s8) ((u32) (*var_a2 & 0x80) >> 7);
        *(s8 *)((unsigned char *)var_v1 + (-7)) = (s8) ((u32) (*var_a2 & 0x40) >> 6);
        temp_t4 = *var_a2;
        var_a2 += 1;
        *(s8 *)((unsigned char *)var_v1 + (-6)) = (s8) ((u32) (temp_t4 & 0x20) >> 5);
        *(s8 *)((unsigned char *)var_v1 + (-5)) = (s8) ((u32) (*(u8 *)((unsigned char *)var_a2 + (-1)) & 0x10) >> 4);
        *(s8 *)((unsigned char *)var_v1 + (-4)) = (s8) ((u32) (*(u8 *)((unsigned char *)var_a2 + (-1)) & 8) >> 3);
        *(s8 *)((unsigned char *)var_v1 + (-3)) = (s8) ((u32) (*(u8 *)((unsigned char *)var_a2 + (-1)) & 4) >> 2);
        *(s8 *)((unsigned char *)var_v1 + (-2)) = (s8) ((u32) (*(u8 *)((unsigned char *)var_a2 + (-1)) & 2) >> 1);
        *(s8 *)((unsigned char *)var_v1 + (-1)) = (s8) (*(u8 *)((unsigned char *)var_a2 + (-1)) & 1);
    } while ((u32) var_v1 < (u32) &spB0);
    var_v0 = 0;
    var_v1_2 = &sp20;
    do {
        var_a1->unk0 = 0;
        var_a2_2 = 0;
loop_4:
        temp_t3 = *var_v1_2 << var_a2_2;
        var_a2_2 += 4;
        var_a1->unk0 = (s32) (var_a1->unk0 + temp_t3);
        var_v1_2 += 1;
        if (var_a2_2 < 0x20) {
            goto loop_4;
        }
        temp_a1 = var_a1 + 4;
        *(s32 *)((unsigned char *)var_a1 + (4)) = 0;
        var_a2_3 = 0;
loop_6:
        temp_t7 = *var_v1_2 << var_a2_3;
        var_a2_3 += 4;
        *(s32 *)((unsigned char *)var_a1 + (4)) = (s32) (*(s32 *)((unsigned char *)var_a1 + (4)) + temp_t7);
        var_v1_2 += 1;
        if (var_a2_3 < 0x10) {
            goto loop_6;
        }
        temp_a1_2 = temp_a1 + 4;
        *(s32 *)((unsigned char *)temp_a1 + (4)) = 0;
        var_a2_4 = 0;
loop_8:
        temp_t2 = *var_v1_2 << var_a2_4;
        var_a2_4 += 4;
        *(s32 *)((unsigned char *)temp_a1 + (4)) = (s32) (*(s32 *)((unsigned char *)temp_a1 + (4)) + temp_t2);
        var_v1_2 += 1;
        if (var_a2_4 < 0x20) {
            goto loop_8;
        }
        *(s32 *)((unsigned char *)temp_a1_2 + (4)) = 0;
        temp_t4_2 = var_v1_2->unk0;
        var_v0 += 1;
        *(s32 *)((unsigned char *)temp_a1_2 + (4)) = (s32) temp_t4_2;
        temp_t7_2 = *(u8 *)((unsigned char *)var_v1_2 + (1));
        var_v1_2 += 4;
        temp_t0 = temp_t4_2 + (temp_t7_2 * 0x10);
        *(s32 *)((unsigned char *)temp_a1_2 + (4)) = temp_t0;
        var_a1 = temp_a1_2 + 8;
        temp_t3_2 = temp_t0 + (*(u8 *)((unsigned char *)var_v1_2 + (-2)) << 8);
        *(s32 *)((unsigned char *)var_a1 + (-4)) = temp_t3_2;
        *(s32 *)((unsigned char *)var_a1 + (-4)) = (s32) (temp_t3_2 + (*(u8 *)((unsigned char *)var_v1_2 + (-1)) << 0xC));
    } while (var_v0 != 6);
}
