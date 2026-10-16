s32 func_80053F04(void) {
    D_800E6280.unk_1120 += 1;
    if (D_800E6280.unk_035 != 0) {
        switch ((u8) D_800E6280.unk_111C) {
        case 0x0:
            D_800E6280.unk_1115 = 0;
            Sw_Clear();
            if (_card_info(0) == 1) {
                func_80053CC0();
            }
            break;
        case 0x1:
            func_80053D24(0x20);
            break;
        case 0x2:
            if (_card_load(0) == 1) {
                func_80053CC0();
            }
            break;
        case 0x3:
            func_80053D24(0x40);
            break;
        case 0x4:
            D_800E6280.unk_035 = 0;
            D_800E6280.unk_1115 = 1;
            break;
        case 0x20:
            func_800AD950(0);
            Hw_Clear();
            if (_card_clear(0) == 0) {
                func_80053CAC(0);
            } else {
                Hw_Test();
                switch (D_800E6280.unk_1115) {
                default:
                case 0x0:
                case 0x81:
                    func_80053CAC(0);
                    break;
                case 0x88:
                    func_80053CAC(0x30);
                    break;
                }
            }
            break;
        case 0x30:
            if (_card_load(0) == 1) {
                func_80053CC0();
            }
            break;
        case 0x31:
            func_80053D24(0x40);
            break;
        case 0x32:
            D_800E6280.unk_035 = 0;
            D_800E6280.unk_1115 = 1;
            break;
        case 0x40:
            D_800E6280.unk_035 = 0;
            D_800E6280.unk_1115 = 0xF1;
            break;
        }
    }
    return (s32) D_800E6280.unk_1115;
}
