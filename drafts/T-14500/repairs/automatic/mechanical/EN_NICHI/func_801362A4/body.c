s32 func_801362A4(void) {
    s32 sp30;
    s32 sp2C;
    u32 temp_v0;
    u32 temp_v0_2;
    u8 var_a0;
    u8 var_a2;
    u8 var_a3;

    func_80136A2C();
    func_8013556C();
    func_80132ADC();
    func_801338EC();
    switch (D_800E738A) {
    case 0:
        func_80136700();
    default:
        break;
    case 1:
        if ((D_80139B00 == 0) && (D_80139B04 == 0)) {
            func_80044750(0x204);
        } else {
            func_80044750(0x202);
        }
        if (D_80139B00 >= 0x64) {
            D_80139B00 = 0x63;
        }
        if (D_80139B04 >= 0x64) {
            D_80139B04 = 0x63;
        }
        func_8004284C();
        break;
    case 2:
        temp_v0 = D_800E7384 + 1;
        D_800E7384 = temp_v0;
        if ((temp_v0 % 15U) == 0) {
            var_a2 = D_800E738D;
            var_a3 = var_a2;
            if (D_80139B00 < (s32) var_a2) {
                var_a3 = (u8) D_80139B00;
            }
            if (D_80139B04 < (s32) var_a2) {
                var_a2 = (u8) D_80139B04;
            }
            var_a0 = var_a3;
            if (var_a3 == D_80139B00) {
                if (var_a2 == D_80139B04) {
                    sp2C = (s32) var_a2;
                    sp30 = (s32) var_a3;
                    func_80044750(0x505);
                    func_80136904(D_80139B00, D_80139B04);
                    func_8004284C();
                }
                var_a0 = var_a3;
            }
            func_80136904((s32) var_a0, (s32) var_a2);
            if (D_800E738D != 0) {
                func_80044750(0x505);
            }
            D_800E738D += 1;
        }
        if (D_800E7208 & 0x40) {
            func_80136904(D_80139B00, D_80139B04);
            if ((D_80139B00 == 0) && (D_80139B04 == 0)) {
                func_8004284C();
            } else {
                func_8004284C();
                func_8004284C();
                func_8004284C();
            }
        }
        break;
    case 3:
        if ((D_80139B00 == 0) && (D_80139B04 == 0)) {
            switch (D_800E738D) {
            case 0:
                if (func_800460EC() == 2) {
                    D_800E738D += 1;
                }
                break;
            case 1:
                func_80046290(0x01000007, 0x03000015, 0xE);
                func_80044750(0x300);
                D_800E738D += 1;
                break;
            case 2:
                if (func_800460EC() == 4) {
                    D_800E738D += 1;
                }
                break;
            case 3:
                if (func_800460EC() == 2) {
                    func_80044750(0x505);
                    func_80136904(1, 0);
                    func_8004284C();
                }
                break;
            }
            temp_v0_2 = D_800E7384 + 1;
            D_800E7384 = temp_v0_2;
            if (temp_v0_2 >= 0x169U) {
                func_80044750(0x7F);
                func_8004284C();
                func_8004284C();
            }
            if (D_800E7208 & 0x800) {
                func_80044750(0x7F);
                func_8004284C();
                func_8004284C();
            }
        } else {
            func_8004284C();
        }
        break;
    case 4:
        D_800E7384 += 1;
        if (D_800E7208 & 0x60) {
            func_80044750(0x7F);
            func_8004284C();
        }
        break;
    case 5:
        func_80132C4C();
        if (D_8013921C == 0) {
            func_80042808();
        }
        break;
    }
}
