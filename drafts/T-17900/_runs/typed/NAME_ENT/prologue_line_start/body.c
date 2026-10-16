void prologue_line_start(void) {
    NameCommandPair8 *temp_v1;
    u8 temp_v0;
    u8 var_v0;

    k_sub_reset();
    temp_v0 = D_8014DA04[(u8) D_8014D180];
    if ((temp_v0 & 0xF) == 0xF) {
        gnsx(D_800E6280.unk_0D4);
    } else if (temp_v0 == 0x1E) {
        gnsx("女子Ａ");
    } else if (temp_v0 == 0x2E) {
        gnsx("女子Ｂ");
    } else {
        gnsx(girl_short_name_table[temp_v0]);
    }
    sndisp(D_8014D958[(u8) D_8014D180], 0, (s32) D_8014DA04[(u8) D_8014D180]);
    var_v0 = D_8014DA04[(u8) D_8014D180];
    if ((var_v0 & 0xF) != 0xF) {
        temp_v1 = &D_8014DA30[(u8) D_8014D180];
        func_80046290(temp_v1->unk_00, temp_v1->unk_04, (s32) var_v0);
        func_80044750(0x300);
        var_v0 = D_8014DA04[(u8) D_8014D180];
    }
    D_800E6280.unk_F5F = var_v0;
    D_8014D188 = 0;
    k_sub_disp_start(1);
}
