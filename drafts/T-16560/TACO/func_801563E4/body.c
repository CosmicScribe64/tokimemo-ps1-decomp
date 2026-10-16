void func_801563E4(s32 arg0) {
    TcPos sp58;
    TcPos sp50;
    TcPos sp48;
    s32 sp40;
    s32 temp_hi;
    s32 temp_hi_2;
    s32 temp_hi_3;
    s32 temp_hi_4;

    func_800AE120(D_801604C0);
    if (arg0 == 0) {
        sp40 = func_800AE0D0();
        sp50.x = ((func_800AE0D0() % 4000) + D_8015EDB4->unk74 + (sp40 % 2000)) - 0xBB8;
        sp40 = func_800AE0D0();
        sp50.y = ((func_800AE0D0() % 2500) + D_8015EDB4->unk76 + (sp40 % 1500)) - 0x7D0;
        sp50.z = (func_800AE0D0() % 20000) + 0x8000;
        temp_hi = func_800AE0D0() % 30;
        sp48.y = 0x800;
        sp48.x = 0;
        sp48.z = 0;
        sp58.x = temp_hi + 0x28;
        sp58.y = (s16) (func_800AE0D0() % 2);
        if (D_801604C0 < 0xF0) {
            func_8015ACCC(7, 7, sp58, sp50, sp48, 5, 0x50);
            return;
        }
        func_8015ACCC(7, 7, sp58, sp50, sp48, 5, 0);
        return;
    }
    sp50.y = -0x384;
    sp50.z = -0x6590;
    sp48.y = 0x800;
    sp48.x = 0;
    sp48.z = 0;
    temp_hi_2 = func_800AE0D0() % 20;
    sp50.x = 0;
    sp58.x = temp_hi_2 + 0x1E;
    sp58.y = (s16) (func_800AE0D0() % 2);
    D_8015EDB4[func_8015ACCC(7, 7, sp58, sp50, sp48, 0x14, 0)].unk84[0] = 4;
    temp_hi_3 = func_800AE0D0() % 20;
    sp50.x = -0x3E8;
    sp58.x = temp_hi_3 + 0x1E;
    sp58.y = (s16) (func_800AE0D0() % 2);
    D_8015EDB4[func_8015ACCC(7, 7, sp58, sp50, sp48, 0x14, 0)].unk84[0] = 4;
    temp_hi_4 = func_800AE0D0() % 20;
    sp50.x = 0x3E8;
    sp58.x = temp_hi_4 + 0x1E;
    sp58.y = (s16) (func_800AE0D0() % 2);
    D_8015EDB4[func_8015ACCC(7, 7, sp58, sp50, sp48, 0x14, 0)].unk84[0] = 4;
}
