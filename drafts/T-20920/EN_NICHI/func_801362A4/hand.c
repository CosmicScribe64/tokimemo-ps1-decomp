s32 func_801362A4(void) {
    s32 x;
    s32 y;

    func_80136A2C();
    func_8013556C();
    func_80132ADC();
    func_801338EC();
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80136700();
        break;
    case 1:
        if (D_80139B00 == 0 && D_80139B04 == 0) {
            func_80044750(0x204);
        } else {
            func_80044750(0x202);
        }
        if (D_80139B00 >= 100) {
            D_80139B00 = 99;
        }
        if (D_80139B04 >= 100) {
            D_80139B04 = 99;
        }
        func_8004284C();
        break;
    case 2:
        D_800E6280.unk_1104.u++;
        if (D_800E6280.unk_1104.u % 15 == 0) {
            x = y = D_800E6280.unk_110D;
            if (D_80139B00 < x) {
                x = D_80139B00;
            }
            if (D_80139B04 < y) {
                y = D_80139B04;
            }
            if (x == D_80139B00 && y == D_80139B04) {
                func_80044750(0x505);
                func_80136904(D_80139B00, D_80139B04);
                func_8004284C();
            }
            func_80136904(x, y);
            if (D_800E6280.unk_110D != 0) {
                func_80044750(0x505);
            }
            D_800E6280.unk_110D++;
        }
        if (D_800E6280.unk_F88 & 0x40) {
            func_80136904(D_80139B00, D_80139B04);
            if (D_80139B00 == 0 && D_80139B04 == 0) {
                func_8004284C();
            } else {
                func_8004284C();
                func_8004284C();
                func_8004284C();
            }
        }
        break;
    case 3:
        if (D_80139B00 == 0 && D_80139B04 == 0) {
            switch (D_800E6280.unk_110D) {
            case 0:
                if (func_800460EC() == 2) {
                    D_800E6280.unk_110D++;
                }
                break;
            case 1:
                func_80046290(0x01000007, 0x03000015, 0xE);
                func_80044750(0x300);
                D_800E6280.unk_110D++;
                break;
            case 2:
                if (func_800460EC() == 4) {
                    D_800E6280.unk_110D++;
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
            D_800E6280.unk_1104.u++;
            if (D_800E6280.unk_1104.u >= 361) {
                func_80044750(0x7F);
                func_8004284C();
                func_8004284C();
            }
            if (D_800E6280.unk_F88 & 0x800) {
                func_80044750(0x7F);
                func_8004284C();
                func_8004284C();
            }
        } else {
            func_8004284C();
        }
        break;
    case 4:
        D_800E6280.unk_1104.u++;
        if (D_800E6280.unk_F88 & 0x60) {
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
