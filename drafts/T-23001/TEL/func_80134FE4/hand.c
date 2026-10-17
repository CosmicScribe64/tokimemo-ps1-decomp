s32 func_80134FE4(void) {
    u8 zy;

    switch (D_800E6280.unk_110D) {
    case 0:
        k_sub_reset();
        set_c_girl(D_801405E4);
        D_800E6280.unk_1BC[D_801405E4].unk_0C.b[3] += 1;
        gnsx(girl_short_name_table[D_801405E4]);
        sndi(D_801406E0[D_801405E4], 0, D_801405E4);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        func_80052060((s32) D_80140710, (s32) D_80140770, (s32) D_801405E4, (s32) D_801405E4);
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(&D_800E6280.unk_0D4[0]);
        sndisp("えっと… Pって言いますけど。", 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 3:
        xa_wait();
        break;
    case 4:
        if (get_g_zyotai_s(D_801405E4) == 4) {
            D_800E6280.unk_1BC[D_801405E4].unk_06 = 0x10;
            func_80042940(3);
            if (D_801405E4 == 7) {
                D_800E6280.unk_110D = 0;
            } else {
                D_800E6280.unk_110D = 4;
            }
        } else {
            zy = get_g_zyotai_s(D_801405E4) & 0x7F;
            if (zy < 2U) {
                zy = D_801405E4;
                if (zy == 0) {
                    func_80042940(8);
                } else if ((zy == 7) && (D_800E6280.unk_1BC[zy].unk_0C.b[3] & 1)) {
                    func_80042940(9);
                } else if (zy == 9) {
                    func_80042940(0xA);
                } else {
                    func_80042940(4);
                }
            } else if (zy == 2) {
                zy = D_801405E4;
                if ((zy == 0xA) && (D_800E6280.unk_1BC[zy].unk_0C.b[3] & 1)) {
                    func_80042940(7);
                }
                func_80042940(5);
            } else {
                func_80042940(6);
            }
        }
        break;
    default:
        func_80042940(0x10);
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}
