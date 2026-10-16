s32 load_string(u8 *arg0) {
    s32 sp34;
    s32 *var_v0;
    u32 temp_t0;
    u8 *temp_s1;
    u8 *temp_v1;
    u8 *var_s1;
    u8 temp_a0;
    u8 temp_v0;

    var_s1 = arg0;
    sp34 = (s32) text_glyph_count;
loop_1:
    temp_a0 = *var_s1;
    temp_s1 = var_s1 + 1;
    temp_v1 = temp_s1;
    var_s1 = temp_s1 + 1;
    if ((temp_a0 == 0) || (temp_v0 = *temp_v1, (temp_v0 == 0))) {
        return text_glyph_count - sp34;
    }
    temp_t0 = (temp_v0 + (temp_a0 << 8)) & 0xFFFF;
    if (temp_t0 < (u16) font_range_table) {
        printf("<kanji.c>error(No-sjis)");
        return text_glyph_count - sp34;
    }
    var_v0 = &D_800B4030;
loop_7:
    if (temp_t0 < (u16) (*(s32 *)var_v0)) {
        vload_font(*(u16 *)((unsigned char *)var_v0 + (-2)) + (temp_t0 - *(u16 *)((unsigned char *)var_v0 + (-4))));
    } else {
        var_v0 += 4;
        if (var_v0 == &D_800B40E4) {

        } else {
            goto loop_7;
        }
    }
    goto loop_1;
}
