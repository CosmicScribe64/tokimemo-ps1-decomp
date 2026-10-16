s32 func_80159878(s32 arg0) {
    TcPos sp50;
    TcPos sp48;

    func_800AE120(D_801604C0);
    sp48.x = 0;
    sp48.z = 0;
    sp50.z = -0x4E20;
    switch (arg0) {
    case 0:
    case 2:
        sp50.x = 0x1B58;
        sp48.y = 0x400;
        sp50.y = -0x4B0;
        func_8015ACCC(3, 0x21, D_801604D4, sp50, sp48, 8, 0);
        sp50.y = -0xC8;
        func_8015ACCC(3, 0x21, D_801604D4, sp50, sp48, 8, 0);
        sp50.x = -0x1B58;
        sp48.y = 0xC00;
        sp50.y = -0x2BC;
        if (arg0 == 0) {
            func_8015ACCC(3, 0x21, D_801604D4, sp50, sp48, 0xA, 0xE0);
            return;
        }
        func_8015ACCC(3, 0x21, D_801604D4, sp50, sp48, 0xA, 0);
        return;
    case 1:
        /* fallthrough */
    case 3:
        sp50.x = -0x1B58;
        sp48.y = 0xC00;
        sp50.y = -0x4B0;
        if (arg0 == 3) {
            func_8015ACCC(3, 0x21, D_801604D4, sp50, sp48, 8, 0x50);
        } else {
            func_8015ACCC(3, 0x21, D_801604D4, sp50, sp48, 8, 0);
        }
        sp50.y = -0xC8;
        if (arg0 == 1) {
            func_8015ACCC(3, 0x21, D_801604D4, sp50, sp48, 8, 0x50);
        } else {
            func_8015ACCC(3, 0x21, D_801604D4, sp50, sp48, 8, 0);
        }
        sp50.x = 0x1B58;
        sp48.y = 0x400;
        sp50.y = -0x2BC;
        if (arg0 == 3) {
            func_8015ACCC(3, 0x21, D_801604D4, sp50, sp48, 0xA, 0x80);
            return;
        }
        func_8015ACCC(3, 0x21, D_801604D4, sp50, sp48, 8, 0);
        return;
    }
}
