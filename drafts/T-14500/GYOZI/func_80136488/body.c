void func_80136488(void) {
    s16 var_s0;

    D_801461A0 = 0;
    func_80136220();
    func_8004EE18(D_80146170, 0x11, 1, 2, 0);
    for (var_s0 = 0; var_s0 < 3; var_s0++) {
        func_800549E8(var_s0 + 0x60);
        ((GyoziInitSprite44 *)D_80129F40)[var_s0 + 0x60].unk_01 = 0xA;
        ((GyoziInitSprite44 *)D_80129F40)[var_s0 + 0x60].unk_02 = 0;
        ((GyoziInitSprite44 *)D_80129F40)[var_s0 + 0x60].unk_38 = 0x01000000;
        ((GyoziInitSprite44 *)D_80129F40)[var_s0 + 0x60].unk_03 = 4;
        ((GyoziInitSprite44 *)D_80129F40)[var_s0 + 0x60].unk_0C = D_80146158;
        ((GyoziInitSprite44 *)D_80129F40)[var_s0 + 0x60].unk_10 = D_80146160;
        ((GyoziInitSprite44 *)D_80129F40)[var_s0 + 0x60].unk_34 = D_80146150;
        ((GyoziInitSprite44 *)D_80129F40)[var_s0 + 0x60].unk_16 = 0;
        ((GyoziInitSprite44 *)D_80129F40)[var_s0 + 0x60].unk_18 = 0;
        ((GyoziInitSprite44 *)D_80129F40)[var_s0 + 0x60].unk_06 = 1;
        ((GyoziInitSprite44 *)D_80129F40)[var_s0 + 0x60].unk_43 = 0x11;
        ((GyoziInitSprite44 *)D_80129F40)[var_s0 + 0x60].unk_26 = -0xA0;
        ((GyoziInitSprite44 *)D_80129F40)[var_s0 + 0x60].unk_2A = -0x78;
        ((GyoziInitSprite44 *)D_80129F40)[var_s0 + 0x60].unk_07 = 0x80;
        ((GyoziInitSprite44 *)D_80129F40)[var_s0 + 0x60].unk_05 = 0;
        ((GyoziInitSprite44 *)D_80129F40)[var_s0 + 0x60].unk_04 = 0;
        ((GyoziInitSprite44 *)D_80129F40)[var_s0 + 0x60].unk_14 = D_80146168;
    }
    ((GyoziInitSprite44 *)D_80129F40)[0x60].unk_26 = 0;
    ((GyoziInitSprite44 *)D_80129F40)[0x61].unk_16 = 3;
    ((GyoziInitSprite44 *)D_80129F40)[0x62].unk_16 = 4;
    func_8004EE18(D_80146174, 0x12, 0, 0, 2);
    func_800549E8(0x63);
    ((GyoziInitSprite44 *)D_80129F40)[0x63].unk_01 = 0xD;
    ((GyoziInitSprite44 *)D_80129F40)[0x63].unk_02 = 0;
    ((GyoziInitSprite44 *)D_80129F40)[0x63].unk_38 = 0;
    ((GyoziInitSprite44 *)D_80129F40)[0x63].unk_03 = 4;
    ((GyoziInitSprite44 *)D_80129F40)[0x63].unk_0C = D_8014615C;
    ((GyoziInitSprite44 *)D_80129F40)[0x63].unk_10 = D_80146164;
    ((GyoziInitSprite44 *)D_80129F40)[0x63].unk_34 = D_80146154;
    ((GyoziInitSprite44 *)D_80129F40)[0x63].unk_14 = D_8014616C;
    ((GyoziInitSprite44 *)D_80129F40)[0x63].unk_16 = 0;
    ((GyoziInitSprite44 *)D_80129F40)[0x63].unk_18 = 0;
    ((GyoziInitSprite44 *)D_80129F40)[0x63].unk_06 = 1;
    ((GyoziInitSprite44 *)D_80129F40)[0x63].unk_43 = 0;
    ((GyoziInitSprite44 *)D_80129F40)[0x63].unk_26 = -0xA0;
    ((GyoziInitSprite44 *)D_80129F40)[0x63].unk_2A = -0x78;
    ((GyoziInitSprite44 *)D_80129F40)[0x63].unk_07 = 0x80;
    ((GyoziInitSprite44 *)D_80129F40)[0x63].unk_05 = 0x81;
    ((GyoziInitSprite44 *)D_80129F40)[0x63].unk_04 = 0;
    func_8004DE1C();
}
