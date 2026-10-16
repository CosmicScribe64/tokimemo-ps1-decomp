extern ? D_800B6CF8;

void func_80077E30(void) {
    magazine_base_show();
    switch (D_800E6280.unk_110D) {
    case 0:
        xa_wait();
        return;
    case 1:
        if (D_800B3CFC != 0) {
            D_800E6280.unk_110D += 1;
            return;
        }
        func_8004284C();
        return;
    case 2:
        k_sub_reset();
        k_sub_disp_start(0);
        gnsx(D_800E6280.unk_0D4);
        sndisp(*(&D_800B6CF8 + (func_80077CA8() * 4)), 0, 0x1F);
        D_800E6280.unk_110D += 1;
        return;
    case 3:
        xa_wait();
        return;
    default:
        func_8004284C();
        return;
    }
}
