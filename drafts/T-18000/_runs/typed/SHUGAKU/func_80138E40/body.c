void func_80138E40(void) {
    s32 sp30;
    s32 sp2C;
    s32 temp_v1;
    u8 temp_v0;

    if ((D_800E6280.unk_F5F != 4) || (D_8013C980 != 0x36)) {
        normal_date_bg_fadeout();
        return;
    }
    temp_v1 = D_800E6280.unk_1104.w;
    sp30 = (s32) D_800E6280.unk_110A;
    if ((temp_v1 == 0) && (D_800E6280.unk_110D == 0)) {
        D_800CA2DC = 0x10;
    }
    temp_v0 = D_800E6280.unk_110D;
    if (temp_v0 == 0) {
        D_800E6280.unk_1104.w = temp_v1 + 1;
    }
    sp2C = (s32) temp_v0;
    if (temp_v0 == 0) {
        func_801386C4();
    } else if (temp_v0 == 1) {
        normal_date_girl_in();
    } else if (temp_v0 == 2) {
        func_801386C4();
    } else if (temp_v0 == 3) {
        normal_date_girl_out();
    } else {
        normal_date_bg_fadeout();
    }
    if (sp30 != D_800E6280.unk_110A) {
        if (sp2C < 4) {
            D_800E6280.unk_110A = (u8) sp30;
            D_800E6280.unk_110D = (sp2C + 1) & 0xFF;
            return;
        }
        D_800CA2DC = D_8013C970;
        D_800E6280.unk_110D = (sp2C + 1) & 0xFF;
    }
}
