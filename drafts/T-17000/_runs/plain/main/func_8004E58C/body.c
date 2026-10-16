void func_8004E58C(void) {
    s16 sp3A;
    s16 sp38;
    s16 sp36;
    s16 sp34;
    u8 *var_v0;

    LoadSquare(0x100U, 0x1F8U, 2U, 8U, &D_800B3FAC);
    LoadSquare(0x120U, 0x1F8U, 2U, 8U, &D_800B3FCC);
    LoadSquare(0x110U, 0x1F8U, 2U, 8U, &D_800B3FEC);
    LoadSquare(0x130U, 0x1F8U, 2U, 8U, &D_800B400C);
    LoadSquare(0x320U, 0x1FFU, 0x20U, 1U, &D_800B3F6C);
    LoadSquare(0x360U, 0x1FFU, 0x20U, 1U, &D_800B3F6C);
    func_8004E500(2);
    sp34 = 0x102;
    sp36 = 0x1F8;
    sp38 = 2;
    sp3A = 8;
    func_8009C7F8((RECT *) &sp34, 0x80, 0x80, 0x80);
    var_v0 = text_list;
    do {
        var_v0 += 0x20;
        *(s16 *)((unsigned char *)var_v0 + (-24)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (-22)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-20)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-19)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-18)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-17)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (-16)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (-14)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-12)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-11)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-10)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-9)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (-8)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (-6)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-4)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-3)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-2)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-1)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (-32)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (-30)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-28)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-27)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-26)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-25)) = 0;
    } while (var_v0 != &text_list_count);
    text_glyph_count = 0;
    text_glyph_count = 0;
    text_glyph_count = 0;
    text_glyph_count = 0;
    if ((text_glyph_advance != 0xE) && (text_glyph_advance != 0x10)) {
        text_glyph_advance = 0xE;
    }
}
