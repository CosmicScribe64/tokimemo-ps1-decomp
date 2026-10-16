/* T-19022 draft, 70 register rows differ (D_801405E4 range in t0, original v1); needs TEL.h: u8 *D_801406E0[], u32 D_80140710[], D_80140770[]. */
void func_80134FE4(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        k_sub_reset();
        set_c_girl(D_801405E4);
        D_800E6280.unk_1BC[D_801405E4].unk_0C.b[3] += 1;
        gnsx(girl_short_name_table[D_801405E4]);
        sndi(D_801406E0[D_801405E4], 0, D_801405E4);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        func_80052060(D_80140710, D_80140770, D_801405E4, D_801405E4);
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
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
            u8 x;
            u32 t;

            x = get_g_zyotai_s(D_801405E4);
            t = x & 0x7F;
            if (t < 2U) {
                if (D_801405E4 == 0) {
                    func_80042940(8);
                } else if (D_801405E4 == 7 && (D_800E6280.unk_1BC[D_801405E4].unk_0C.b[3] & 1)) {
                    func_80042940(9);
                } else if (D_801405E4 == 9) {
                    func_80042940(0xA);
                } else {
                    func_80042940(4);
                }
            } else if (t == 2) {
                if (D_801405E4 == 0xA && (D_800E6280.unk_1BC[D_801405E4].unk_0C.b[3] & 1)) {
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
