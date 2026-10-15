extern s32 D_8014E0C0;
extern ? D_8014EA18;
extern ? D_8014EA1C;
extern ? D_8014EA40;
extern ? D_8014EA44;
extern ? D_8014EA4C;

void func_80132FB0(s32 arg0, s32 arg1, ? arg2, s32 arg3) {
    s32 *temp_v0;
    s32 temp_a0;
    s32 temp_t9;

    D_8014E0C0 = 0;
    if (arg0 == 0x14) {
        switch (arg1) {
        case 1:
        case 2:
        case 3:
            if (D_80150054 == 0x61) {
                func_8005742C(0x40F4, 1);
                D_8014E0C0 = 1;
            } else {
                func_8005742C(*(*(&D_8014EA18 + (arg0 * 4)) + (arg1 * 4)), 1);
                D_8014E0C0 = 1;
            }
            break;
        case 4:
            if (D_80150054 == 0x61) {
                func_8005742C(0x467C, 1);
                D_8014E0C0 = 1;
            } else {
                func_8005742C(*(*(&D_8014EA18 + (arg0 * 4)) + (arg1 * 4)), 1);
                goto block_36;
            }
            break;
        case 17:
        case 18:
        case 19:
        case 20:
            if (D_80150054 == 0x60) {
                func_8005742C(0x4118, 1);
                D_8014E0C0 = 1;
            } else {
                func_8005742C(*(*(&D_8014EA18 + (arg0 * 4)) + (arg1 * 4)), 1);
                D_8014E0C0 = 1;
            }
            break;
        default:
            func_8005742C(*(*(&D_8014EA18 + (arg0 * 4)) + (arg1 * 4)), 1);
            D_8014E0C0 = 1;
            break;
        }
    } else {
        temp_v0 = (arg0 * 4) + &D_8014EA18;
        temp_t9 = arg1 * 4;
        temp_a0 = *(*temp_v0 + temp_t9);
        if (temp_a0 != -1) {
            if (temp_a0 == 0x42EB) {
                if ((arg3 == 2) || (arg3 == 3)) {
                    func_8005742C(0x42F4, 1);
                    D_8014E0C0 = 1;
                } else {
                    func_8005742C(temp_a0, 1);
                    goto block_36;
                }
            } else if (temp_a0 == 0x42FD) {
                if ((arg3 == 2) || (arg3 == 3)) {
                    func_8005742C(0x4306, 1);
                    D_8014E0C0 = 1;
                } else {
                    func_8005742C(temp_a0, 1);
                    D_8014E0C0 = 1;
                }
            } else if (temp_a0 == 0x430F) {
                if ((arg3 == 2) || (arg3 == 3)) {
                    func_8005742C(0x4318, 1);
                    D_8014E0C0 = 1;
                } else {
                    func_8005742C(temp_a0, 1);
                    D_8014E0C0 = 1;
                }
            } else if (((temp_v0 != &D_8014EA18) && (temp_v0 != &D_8014EA1C) && (temp_v0 != &D_8014EA44) && (temp_v0 != &D_8014EA40) && (temp_v0 != &D_8014EA4C)) || (temp_t9 != 0)) {
                func_8005742C(temp_a0, 1);
block_36:
                D_8014E0C0 = 1;
            }
        }
    }
}
