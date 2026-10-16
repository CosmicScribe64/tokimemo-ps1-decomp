void func_80144F70(void) {
    BunkakenBytes11 sp3C;
    s32 i;
    s32 temp_a0;
    s32 temp_a0_2;

    sp3C = D_80155F90;
    func_80083808();
    switch (D_80122EC0) {
    case 0:
        func_80044750(0xBF);
        func_80044890(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        func_80085E30(1, 0);
        for (i = 0; i < 11; i++) {
            D_800CA2B0[i] = D_800E6280.unk_1BC[i].unk_0C.f.b1;
        }
        func_8008585C();
        dec_bg_reset();
        message_window_init();
        k_reset(0);
        func_8006612C((s32) "文化祭");
        func_80065F34(0);
        func_8006764C(0);
        hizuke_init();
        icon_disp_switch(0);
        parameter_disp_switch(0);
        D_80122EA4 = 0xFF;
        D_80122E98 = 0;
        D_80122EC0 += 1;
        D_80122EBC = 0xFF;
        break;
    case 1:
        set_dec_bri(0);
        D_80122EC0 += 1;
        break;
    case 2:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            dec_bg_cd_read(0x4029, 1);
            D_80122EC0 += 1;
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            cd_read_sectors(5U, 0x801E8000, 0x49BA);
            func_80065F34(1);
            D_80122EC0 += 1;
        }
        break;
    case 4:
        normal_date_bg_fadein();
        func_80145500();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            func_80084E4C();
            func_800847B8(0U);
            func_801324E0();
            D_800CA148 = 0;
            D_800CA14C = 0;
            D_800CA160 = (s32) D_80155E90;
            D_800CA164 = (s32) D_80155E94;
            D_800CA168 = (s32) D_80155E98;
            D_80122EC0 += 1;
        }
        break;
    case 6:
        normal_date_speak();
        func_80145500();
        break;
    case 7:
        func_80145E40();
        break;
    case 8:
        if ((D_800E6280.unk_03F == (D_800E6280.unk_1BC[0].unk_10.w & 0xF)) && (D_800E6280.unk_040 == ((u32) (D_800E6280.unk_1BC[0].unk_10.w << 0x17) >> 0x1B)) && !(D_800E6280.unk_71E & 1) && ((D_80122EBC == 0) || (D_80122EBC == 0xB))) {
            D_80122EC0 = 9;
            D_800E6280.unk_1109 = 0;
            D_800E6280.unk_110A = 0;
        } else {
            temp_a0 = D_80122EBC;
            if ((D_800E6280.unk_03F == (D_800E6280.unk_0F8 & 0xF)) && (D_800E6280.unk_040 == ((u32) (D_800E6280.unk_0F8 << 0x17) >> 0x1B)) && !(D_800E6280.unk_71E & 4) && (temp_a0 >= 4) && (temp_a0 < 0xB) && ((D_80122EBC = temp_a0, ((func_8008667C(temp_a0) < 2) != 0)) || ((D_80122EBC == 0xB) && (func_8008667C(0) < 2)))) {
                D_80122EC0 = 0xA;
                D_800E6280.unk_1109 = 0;
                D_800E6280.unk_110A = 0;
            } else if (D_800E6280.unk_1109 == 0) {
                func_80044750(0xBF);
                func_800674B0();
                func_80042808();
            } else if (func_80044E8C() == 1) {
                func_80145550();
                temp_a0_2 = D_80122EBC;
                if (temp_a0_2 != 0xFF) {
                    D_80122EBC = temp_a0_2;
                    if (func_8008667C(temp_a0_2) < 4) {
                        func_80145A64();
                    }
                }
                for (i = 0; i < 0x40; i++) {
                    ((s32 *)D_80122DD0)[i] = 0;
                }
                func_8008585C();
                dec_bg_reset();
                set_dec_bri(0);
                func_80072B5C(1);
            }
        }
        break;
    case 9:
        func_80149144();
        break;
    case 10:
        func_8014A0E0();
        break;
    default:
        func_80046500();
        break;
    }
    if (D_80122E98 == 0) {
        Default_Disp();
    }
}
