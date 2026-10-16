void func_80132FF8(void) {
    s32 var_at;
    u8 temp_v0;
    u8 temp_v0_2;
    u8 var_v0;
    u8 var_v0_2;

    func_80083808();
    var_v0 = D_800E6280.unk_1109;
    if (((var_v0 != 2) && (var_v0 != 5) && (var_v0 != 9) && (var_v0 != 0xA) && (var_v0 != 0xC) && (var_v0 != 0xF) && (var_v0 != 0x11) && (var_v0 != 0x14) && (var_v0 != 0x16) && (var_v0 != 0x19) && (var_v0 != 0x1D) && (var_v0 != 0x1F)) || (D_800E6280.unk_110A == 0) || ((u8) D_80139AC0 != 0)) {
        if ((var_v0 != 0x1E) || (var_at = var_v0 < 0x21U, ((u8) D_80139AC0 != 0))) {
            func_80065B0C(1);
            var_v0 = D_800E6280.unk_1109;
            goto block_17;
        }
    } else {
block_17:
        var_at = var_v0 < 0x21U;
    }
    if (var_at != 0) {
        switch (var_v0) {
        case 0:
            func_801335F0();
            break;
        case 1:
            func_801337B4();
            break;
        case 2:
            func_80133C10();
            break;
        case 3:
            vacation_day_init();
            break;
        case 4:
            func_80133A14();
            break;
        case 5:
            func_80138078();
            break;
        case 6:
            func_801335A0();
            break;
        case 7:
            vacation_day_init();
            break;
        case 8:
            func_80133A14();
            break;
        case 9:
            func_80134120();
            break;
        case 10:
            func_80138078();
            break;
        case 11:
            func_801335A0();
            break;
        case 12:
            func_80135678();
            break;
        case 13:
            vacation_day_init();
            break;
        case 14:
            func_80133A14();
            break;
        case 15:
            func_80138078();
            break;
        case 16:
            func_801335A0();
            break;
        case 17:
            func_80135678();
            break;
        case 18:
            vacation_day_init();
            break;
        case 19:
            func_80133A14();
            break;
        case 20:
            func_80138078();
            break;
        case 21:
            func_801335A0();
            break;
        case 22:
            func_80135678();
            break;
        case 23:
            vacation_day_init();
            break;
        case 24:
            func_80133A14();
            break;
        case 25:
            func_80138078();
            break;
        case 26:
            func_801335A0();
            break;
        case 27:
            vacation_day_init();
            break;
        case 28:
            func_80133A14();
            break;
        case 29:
            func_80136D58();
            break;
        case 30:
            func_80138078();
            break;
        case 31:
            func_80136018();
            break;
        case 32:
            func_801338EC();
            break;
        }
    } else {
        func_80046500();
    }
    temp_v0 = D_800E6280.unk_1109;
    if ((temp_v0 == 3) || (temp_v0 == 7) || (temp_v0 == 0xD) || (temp_v0 == 0x12) || (temp_v0 == 0x17) || (temp_v0 == 0x1B)) {
        func_80065B0C(1);
    }
    parameter_show();
    check_k_scroll();
    k_disp_inc2();
    func_80066334();
    message_window_show();
    hizuke_show();
    func_80047560();
    temp_v0_2 = D_800E6280.unk_1109;
    if (((temp_v0_2 == 2) || (temp_v0_2 == 5) || (temp_v0_2 == 9) || (temp_v0_2 == 0xA) || (temp_v0_2 == 0xC) || (temp_v0_2 == 0xF) || (temp_v0_2 == 0x11) || (temp_v0_2 == 0x14) || (temp_v0_2 == 0x16) || (temp_v0_2 == 0x19) || (temp_v0_2 == 0x1D) || (temp_v0_2 == 0x1F)) && (D_800E6280.unk_110A != 0) && ((u8) D_80139AC0 == 0)) {
        func_80066C08(2);
        func_80083A10();
    } else if ((temp_v0_2 == 0x1E) && ((u8) D_80139AC0 == 0)) {
        func_80066C08(2);
        func_80083A10();
    } else if ((u8) D_80139AC0 == 1) {
        func_800578F4(0);
        if ((D_80122D2C == 0) && (D_80122D04 == 0) && ((D_800E6280.unk_F80 & 0xF000F000) || (D_80122D30 == 1))) {
            ((DisplayActor44 *)D_8011ECD0)->unk_03 = ((DisplayActor44 *)D_8011ECD0)->unk_03 | 0x80;
        }
        if (((DisplayActor44 *)D_8011ECD0)->unk_03 & 0x80) {
            func_8006BA40();
        }
        func_80066C08(1);
    } else {
        func_800578F4(0);
        func_80066C08(1);
        if ((D_80122D2C == 0) && (D_80122D04 == 0) && ((D_800E6280.unk_F80 & 0xF000F000) || (D_80122D30 == 1))) {
            ((DisplayActor44 *)D_8011ECD0)->unk_03 = ((DisplayActor44 *)D_8011ECD0)->unk_03 | 0x80;
        }
        var_v0_2 = ((DisplayActor44 *)D_8011ECD0)->unk_03;
        if (D_80122D04 != 0) {
            var_v0_2 &= 0x7F;
        }
        ((DisplayActor44 *)D_8011ECD0)->unk_03 = var_v0_2;
        if (var_v0_2 & 0x80) {
            func_8006BA40();
        }
    }
    func_80047560();
}
