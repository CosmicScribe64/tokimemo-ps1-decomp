void prologue_line_start(void) {
    NameCommandPair8 *temp_v1;
    u8 temp_v0;
    u8 var_v0;

    func_8004EAAC();
    temp_v0 = D_8014DA04[(u8) D_8014D180];
    if ((temp_v0 & 0xF) == 0xF) {
        func_80050DFC(D_800E6280.unk_0D4);
    } else if (temp_v0 == 0x1E) {
        func_80050DFC("女子Ａ");
    } else if (temp_v0 == 0x2E) {
        func_80050DFC("女子Ｂ");
    } else {
        func_80050DFC(girl_short_name_table[temp_v0]);
    }
    func_80050E8C(D_8014D958[(u8) D_8014D180], 0, (s32) D_8014DA04[(u8) D_8014D180]);
    var_v0 = D_8014DA04[(u8) D_8014D180];
    if ((var_v0 & 0xF) != 0xF) {
        temp_v1 = &D_8014DA30[(u8) D_8014D180];
        func_80046290(temp_v1->unk_00, temp_v1->unk_04, (s32) var_v0);
        func_80044750(0x300);
        var_v0 = D_8014DA04[(u8) D_8014D180];
    }
    D_800E6280.unk_F5F = var_v0;
    D_8014D188 = 0;
    func_8004EAD4(1);
}
