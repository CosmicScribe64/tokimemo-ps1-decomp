void func_80137890(void) {
    switch ((u8) D_800E6280.unk_1109) {
    case 0x0:
        func_80066C08(2);
        hizuke_disp_switch(0);
        message_window_show();
        func_80042808();
        return;
    case 0x1:
        switch (D_80122EB8) {
        case 2:
            switch (D_800E6280.unk_03E) {
            case 0x5F:
                func_80042908(0xF0);
                return;
            case 0x60:
                func_80042908(0xF1);
                return;
            case 0x61:
                func_80042908(0xF2);
                return;
            }
            break;
        case 3:
            func_80042908(0xF3);
            return;
        case 5:
            func_80042908(0xF4);
            return;
        case 0:
            func_80042908(0xF5);
            return;
        case 1:
            switch (D_800E6280.unk_03E) {
            case 0x5F:
                func_80042908(0xF6);
                return;
            case 0x60:
                func_80042908(0xF7);
                return;
            case 0x61:
                func_80042908(0xF8);
                return;
            }
            break;
        default:
            return;
        }
        break;
    case 0x10:
        if ((u32) ((u16) D_800E6280.unk_000 >> 0xC) < 6U) {
            main_state_set(0x70);
        } else {
            main_state_set(0x71);
        }
        func_80042908((s32) D_80122EB0);
        func_80042940((s32) D_80122EB4);
        return;
    case 0xF0:
        func_801362EC();
        return;
    case 0xF1:
        func_801351DC();
        return;
    case 0xF2:
        func_80135A2C();
        return;
    case 0xF3:
        func_801345BC();
        return;
    case 0xF4:
        func_80138ADC();
        return;
    case 0xF5:
        func_801322EC();
        return;
    case 0xF6:
        func_801336AC();
        return;
    case 0xF7:
        func_80137CAC();
        return;
    case 0xF8:
        func_80136B3C();
        break;
    }
}
