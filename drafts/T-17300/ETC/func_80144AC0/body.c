/* Port of the main clone func_8005C5BC (T-17300); only the state codes differ. */
void func_80144AC0(void) {
    s32 i;
    s32 d;
    FlagsWork24 *work;
    s16 x[16];
    s16 y[16];
    s16 w[16];
    s16 h[16];

    func_8004E58C();
    func_80066104("データセーブ");
    d = 5;
    for (i = 0; i < 14; i++) {
        x[i] = (i / d) * 32 - 124;
        y[i] = (i % d) * 16 - 64;
        w[i] = 32;
        h[i] = 16;
        set_kanji_string(x[i], y[i], 15, D_800B32CC[i], 0);
    }
    menu_set(1, 14, x, y, w, h);
    D_800E6280.unk_1094 = 0;
    k_sub_reset_point_set();
    work = (FlagsWork24 *)&D_800E7D10;
    for (i = 1; i != 15; i++) {
        if (work->unk_04[i] == 1) {
            break;
        }
    }
    if ((u8) D_800E8BEE != 0) {
        D_800E6280.unk_1094 = D_800E7D36 - 1;
        D_800E6280.unk_1118 = (s8) D_800E7D36;
        func_80042940(2);
        func_800696DC(0, 0);
        func_8005C7C0();
    } else if (i == 0xF) {
        func_8005C7C0();
        func_80042940(2);
    } else {
        D_800E6280.unk_1118 = i;
        func_80042940(0xC0);
    }
    func_8005C7FC();
}
