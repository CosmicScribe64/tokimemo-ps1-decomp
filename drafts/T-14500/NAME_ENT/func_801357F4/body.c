void func_801357F4(void) {
    u8 cur;

    func_8006BA40();
    func_80133D94();
    func_80133E54();
    func_80133FC4();
    func_80132B78(0);
    if (D_8014CCC8 == 3) {
        func_80134110();
        func_801341A8();
    }
    func_80135650();
    if (D_800E6280.unk_F88 & 0x40) {
        cur = D_8014CC70;
        if (cur == 0xC) {
            D_8014CC70 = 2;
        } else if (cur != 0) {
            D_8014CC70 = cur - 1;
        }
    } else if (D_800E6280.unk_F88 & 0x20) {
        if (D_8014CC70 < 3 && D_8014CCC0 >= 0 && D_8014CCC4 >= 0) {
            func_80134248(0);
            func_80134B18();
        }
        if (D_8014CCC8 == 3) {
            func_801338DC(1);
            func_8004EA98();
            func_80134B18();
        } else if (D_8014CCC8 == 2) {
            func_80133510(D_8014CCE4);
            func_8004EA98();
            func_80134B18();
        }
        if (D_8014CCCC == 0) {
            D_8014CCC8 = 0;
            func_801330E8(D_8014CCE4);
            func_8004EA98();
            func_80134B18();
        }
        if (D_8014CCCC == 1) {
            D_8014CCC8 = 1;
            func_801332FC(D_8014CCE4);
            func_8004EA98();
            func_80134B18();
        }
        if (D_8014CCCC == 2) {
            D_8014CCC8 = 2;
            func_80133510(D_8014CCE4);
            func_8004EA98();
            func_80134B18();
        }
        if (D_8014CCCC == 3) {
            D_8014CCC8 = 3;
            func_801338DC(1);
            func_8004EA98();
            func_80134B18();
        }
        if (D_8014CCCC == 4) {
            D_8014CCC8 = 4;
            func_80134B94(0);
            func_80134B18();
            func_80044750(0x501);
            func_8004284C();
        }
    } else if (D_800E6280.unk_F88 & 0x800) {
        func_80134B94(0);
        func_80134B18();
        func_80044750(0x501);
        func_8004284C();
    }
}
