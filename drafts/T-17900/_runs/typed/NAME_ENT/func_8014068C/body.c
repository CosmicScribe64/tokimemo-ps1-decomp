void func_8014068C(void) {
    u8 sp3B;
    u8 sp3A;
    s32 sp39[1];
    s32 sp2C;
    s32 var_v1;
    s32 var_v1_2;
    u8 temp_v0;
    u8 temp_v0_7;
    u8 temp_v1;
    u8 *temp_v0_2;
    u8 *temp_v0_3;
    u8 *temp_v0_4;
    u8 *temp_v0_5;
    u8 *temp_v0_6;

    temp_v1 = D_800E6280.unk_110D;
    switch (temp_v1) {
    case 0:
        dec_bg_reset();
        dec_bg_show_switch(2);
        func_80057640(0xF0);
        func_8006BD6C(0);
        func_80041584();
        D_800E6280.unk_F5E = func_8006C988((s32) &sp3A, (s32) &sp39, 1);
        sp3B = sp3A;
        func_800AE0B0("girl %x \n", sp3A);
        func_800AE0B0("i %x \n", sp3A);
        if (sp3B >= 0xDU) {
            temp_v0 = D_800E6280.unk_03E;
            if ((temp_v0 != 0x62) || (D_800E6280.unk_03F != 3) || (D_800E6280.unk_040 != 1)) {
                if ((temp_v0 != 0x5F) || (D_800E6280.unk_03F != 4) || (D_800E6280.unk_040 != 4)) {
                    func_80044750(5);
                    func_80044750(0xC1);
                }
            }
            D_800E6280.unk_110D = 4;
        } else if (sp3B == 0xB) {
            if (D_800E6280.unk_56C[0x4D] != 0) {
                temp_v0_2 = (sp3B * 0x10) + 8 + D_800B3B9C;
                func_80046290((*(s32 *)temp_v0_2), *(s32 *)((unsigned char *)temp_v0_2 + (4)), (s32) sp3B);
                func_80044750(0x300);
                D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
            } else {
                temp_v0_3 = (sp3B * 0x10) + D_800B3B9C;
                func_80046290((*(s32 *)temp_v0_3), *(s32 *)((unsigned char *)temp_v0_3 + (4)), (s32) sp3B);
                func_80044750(0x300);
                D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
            }
        } else if (sp3B == 0xC) {
            if ((((NameMetricWork *)&D_800E7D10)->flags[0xC] != 0) || (D_800E6280.unk_53C[0xC] != 0)) {
                if ((u8) D_800E6280.unk_1BC[0xC].unk_0C.b[3] >= 0x29U) {
                    var_v1 = 0;
                } else {
                    var_v1 = 1;
                }
                temp_v0_4 = (sp3B * 0x10) + (var_v1 * 8) + D_800B3B9C;
                func_80046290((*(s32 *)temp_v0_4), *(s32 *)((unsigned char *)temp_v0_4 + (4)), (s32) sp3B);
                func_800AE0B0("伊集院A");
                func_80044750(0x300);
                D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
            } else if ((u8) D_800E6280.unk_1BC[0xC].unk_0C.b[3] >= 0x29U) {
                temp_v0_5 = (sp3B * 0x10) + 8 + D_800B3B9C;
                func_80046290((*(s32 *)temp_v0_5), *(s32 *)((unsigned char *)temp_v0_5 + (4)), (s32) sp3B);
                func_80044750(0x300);
                func_800AE0B0("伊集院B");
                D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
            } else {
                func_800AE0B0("伊集院C");
                D_800E6280.unk_110D = 4;
            }
        } else {
            sp2C = (s32) sp3B;
            if (!(get_g_zyotai_s(sp3B) & 0x7F)) {
                var_v1_2 = 1;
            } else {
                var_v1_2 = 0;
            }
            temp_v0_6 = (sp3B * 0x10) + (var_v1_2 * 8) + D_800B3B9C;
            func_80046290((*(s32 *)temp_v0_6), *(s32 *)((unsigned char *)temp_v0_6 + (4)), sp2C);
            func_80044750(0x300);
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        }
        break;
    case 1:
        D_800E6280.unk_03A = 0x80;
        set_dec_bri(0x80);
        func_80041584();
        if (func_800460EC() & 4) {
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        }
        break;
    case 2:
        if ((func_800460EC() & 2) && ((temp_v0_7 = D_800E6280.unk_03E, D_800E6280.unk_110D = D_800E6280.unk_110D + 1, (temp_v0_7 != 0x62)) || (D_800E6280.unk_03F != 3) || (D_800E6280.unk_040 != 1)) && ((temp_v0_7 != 0x5F) || (D_800E6280.unk_03F != 4) || (D_800E6280.unk_040 != 4))) {
            func_80044750(5);
            func_80044750(0xC1);
        }
        break;
    case 3:
        D_800E6280.unk_110D = temp_v1 + 1;
        break;
    case 4:
        if ((u8) D_800E6280.unk_03A < 4U) {
            func_800AE0B0("A\n");
            D_800E6280.unk_037 = 0;
            D_800E6280.unk_036 = 0;
            D_800E6280.unk_038 = 0;
            func_8004284C();
        }
        break;
    }
    func_80140B20();
    func_800578F4(2);
    D_8011ECA4 = 1;
}
