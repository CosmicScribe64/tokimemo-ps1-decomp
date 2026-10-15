void func_80137264(void *arg0, s32 arg1) {
    u8 sp40[11];
    s32 temp_lo;
    s32 var_s1;
    s32 var_s3;
    u32 temp_hi;

    *(RepairBytes11 *)sp40 = *(RepairBytes11 *)&D_8015F428;
    var_s3 = arg1;
    if (arg1 >= 0x05F5E100) {
        var_s3 = 0x05F5E0FF;
    }
    if (var_s3 >= 0x989680) {
        func_800AE0F0(arg0, &D_8015D7DC);
    } else if (var_s3 >= 0xF4240) {
        func_800AE0F0(arg0, " ");
    } else if (var_s3 >= 0x186A0) {
        func_800AE0F0(arg0, "  ");
    } else if (var_s3 >= 0x2710) {
        func_800AE0F0(arg0, "   ");
    } else if (var_s3 >= 0x3E8) {
        func_800AE0F0(arg0, "    ");
    } else if (var_s3 >= 0x64) {
        func_800AE0F0(arg0, "     ");
    } else if (var_s3 >= 0xA) {
        func_800AE0F0(arg0, "      ");
    } else {
        func_800AE0F0(arg0, "       ");
    }
    var_s1 = 0x989680;
    if (var_s3 == 0) {
        func_800AE100(arg0, "0");
        var_s1 = 0x989680;
    }
    do {
        temp_lo = var_s3 / var_s1;
        if (temp_lo != 0) {
            temp_hi = (u32) temp_lo % 10U;
            switch (temp_hi) {
            case 0:
                func_800AE100(arg0, "0");
                break;
            case 1:
                func_800AE100(arg0, "1");
                break;
            case 2:
                func_800AE100(arg0, "2");
                break;
            case 3:
                func_800AE100(arg0, "3");
                break;
            case 4:
                func_800AE100(arg0, "4");
                break;
            case 5:
                func_800AE100(arg0, "5");
                break;
            case 6:
                func_800AE100(arg0, "6");
                break;
            case 7:
                func_800AE100(arg0, "7");
                break;
            case 8:
                func_800AE100(arg0, "8");
                break;
            case 9:
                func_800AE100(arg0, "9");
                break;
            }
        }
        var_s1 = var_s1 / 10;
    } while (var_s1 > 0);
}
