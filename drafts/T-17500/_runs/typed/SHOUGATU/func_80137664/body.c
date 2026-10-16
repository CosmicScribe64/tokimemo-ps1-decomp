void func_80137664(void) {
    s32 var_a1;
    s32 var_a2;
    u8 temp_t9;
    u8 temp_v1;

    D_800B3D60 = 0;
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    back_clear_switch(1);
    tpage_buf_clear();
    func_8004E58C();
    hizuke_init();
    message_window_init();
    func_8008585C();
    temp_t9 = D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01;
    D_800E6280.unk_F5F = temp_t9;
    D_80144E14 = (u8) D_80122CD0;
    D_80144E10 = temp_t9;
    temp_v1 = D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01;
    if ((temp_v1 == 2) || (var_a1 = 0xC043, (temp_v1 == 6))) {
        var_a1 = 0xC063;
        var_a2 = 0xC045;
    } else {
        var_a2 = 0xC01D;
    }
    func_80044890(1, var_a1, var_a2, D_800B3688[temp_v1], D_800B36C8[temp_v1], D_800B3708[temp_v1]);
    if (func_80044E8C() == 1) {
        func_80042808();
    }
    D_800E6280.unk_1BC[D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01].unk_06 = 0x46;
    func_80042808();
}
