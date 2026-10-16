void func_80045414(s32 arg0, s32 arg1, u8 *arg2) {
    s8 sp38;
    s32 var_a3;
    u32 temp_t6;

    temp_t6 = arg0 & 0xFF;
    var_a3 = arg1;
    if ((u32) arg1 >= 0x4AB51U) {
        var_a3 = 0x4AB50;
    }
    switch (temp_t6) {
    case 0:
        func_80045318(0, 0, 0, var_a3);
        return;
    case 9:
        func_80045318(9, 0, 0, var_a3);
        return;
    case 6:
        D_800B3D6C = var_a3;
        func_80045318(6, var_a3, (s32) arg2, var_a3);
        return;
    case 14:
        func_80045318(0xE, 0, (s32) arg2, var_a3);
        return;
    case 13:
        if (var_a3 == 0) {
            func_80045318(0xD, 0, (s32) arg2, var_a3);
            return;
        }
        arg1 = var_a3;
        func_80045318(0xD, 0, (s32) arg2, var_a3);
        func_80045318(2, arg1, 0);
        func_80045318(0x1B, arg1, 0);
        D_800B3D6C = arg1;
        return;
    case 2:
        func_80045318(2, var_a3, 0, var_a3);
        return;
    case 21:
        arg1 = var_a3;
        func_80045318(2, var_a3, 0, var_a3);
        func_80045318(0x15, arg1, 0);
        return;
    case 10:
        func_80045318(0xA, var_a3, 0, var_a3);
        return;
    case 3:
        sp38 = 5;
        arg1 = var_a3;
        func_80045318(0xE, 0, (s32) &sp38, var_a3);
        func_80045318(2, arg1, 0);
        func_80045318(3, 0, 0);
        /* fallthrough */
    default:
        return;
    }
}
