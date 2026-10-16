s32 func_80159260(void) {
    TcPos sp58;
    TcPos sp50;
    s32 sp44;
    u32 temp_hi;

    func_800AE120(D_801604C0);
    sp44 = func_800AE0D0();
    sp50.x = ((func_800AE0D0() % 2500) + (sp44 % 1500)) - 0x7D0;
    sp44 = func_800AE0D0();
    sp50.y = ((func_800AE0D0() % 2000) + (sp44 % 1000)) - 0x7D0;
    sp50.z = (func_800AE0D0() % 5000) - 0x61A8;
    sp58.x = (func_800AE0D0() % 200) - 0x64;
    sp58.y = (func_800AE0D0() % 200) - 0x64;
    temp_hi = func_800AE0D0() % 5U;
    switch (temp_hi) {
    case 0:
        func_8015ACCC(2, 0x20, sp58, sp50, D_801604E4, 4, 0x10);
        return;
    case 1:
        func_8015ACCC(2, 0x20, sp58, sp50, D_801604E4, 4, 0x10);
        return;
    case 2:
        func_8015ACCC(2, 0x20, sp58, sp50, D_801604E4, 4, 0);
        return;
    case 3:
        func_8015ACCC(2, 0x20, sp58, sp50, D_801604E4, 4, 0);
        return;
    case 4:
        func_8015ACCC(2, 0x20, sp58, sp50, D_801604E4, 4, 0);
        /* fallthrough */
    default:
        return;
    }
}
