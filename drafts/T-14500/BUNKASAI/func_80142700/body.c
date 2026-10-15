void func_80142700(void) {
    s32 sp3C[3];
    ? *var_v0;
    ? *var_v1;
    u32 temp_t1;
    u32 temp_t4;
    u32 temp_t5;
    u32 temp_t6;
    u8 *var_v0_2;

    sp3C.unk0 = (s32) D_80160520.unk0;
    *(s32 *)((unsigned char *)(&sp3C) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80160520) + (4));
    *(? *)((unsigned char *)(&sp3C) + (7)) = M2C_LWL(*(? *)((unsigned char *)(&D_80160520) + (10)));
    func_80083808();
    switch (D_80122EC0) {
    case 0:
        func_80044750(0xBF);
        func_80044890(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        func_80085E30(1, 0);
        func_8008585C();
        *D_800CA2B0 = (u8) ((u32) (D_800E6448 << 0x1E) >> 0x1F);
        D_800CA2B1 = (s8) ((u32) (D_800E6480 << 0x1E) >> 0x1F);
        var_v0 = &D_800E6328;
        var_v1 = &D_800CA2B3;
        D_800CA2B2 = (s8) ((u32) (D_800E64B8 << 0x1E) >> 0x1F);
        do {
            temp_t5 = (u32) (*(s32 *)((unsigned char *)var_v0 + (456)) << 0x1E) >> 0x1F;
            temp_t4 = (u32) (*(s32 *)((unsigned char *)var_v0 + (624)) << 0x1E) >> 0x1F;
            temp_t1 = (u32) (*(s32 *)((unsigned char *)var_v0 + (568)) << 0x1E) >> 0x1F;
            temp_t6 = (u32) (*(s32 *)((unsigned char *)var_v0 + (512)) << 0x1E) >> 0x1F;
            var_v0 += 0xE0;
            *(s8 *)((unsigned char *)var_v1 + (1)) = (s8) temp_t6;
            *(s8 *)((unsigned char *)var_v1 + (2)) = (s8) temp_t1;
            *(s8 *)((unsigned char *)var_v1 + (3)) = (s8) temp_t4;
            var_v1 += 4;
            *(s8 *)((unsigned char *)var_v1 + (-4)) = (s8) temp_t5;
        } while (var_v0 != &D_800E64E8);
        dec_bg_reset(&D_800E64E8);
        message_window_init();
        k_reset(0);
        func_8006612C((s32) "文化祭");
        func_80065F34(0);
        func_8006764C(0);
        hizuke_init();
        icon_disp_switch(0);
        parameter_disp_switch(0);
        D_80122EBC = 0xFF;
        D_80122EA4 = 0xFF;
        D_80122E98 = 0;
        D_80122EC0 += 1;
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
        func_80142B98();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            func_80084E4C();
            func_800847B8(0U);
            func_801323A8();
            D_800CA148 = 0;
            D_800CA14C = 0;
            D_800CA160 = (s32) D_801604F0;
            D_800CA164 = (s32) D_801604F4;
            D_800CA168 = (s32) D_801604F8;
            D_80122EC0 += 1;
        }
        break;
    case 6:
        normal_date_speak();
        func_80142B98();
        break;
    case 7:
        func_80150930();
        break;
    case 8:
        if ((D_800E62BF == (D_800E644C & 0xF)) && (D_800E62C0 == ((u32) (D_800E644C << 0x17) >> 0x1B)) && !(D_800E699E & 1)) {
            D_80122EC0 = 9;
            D_800E7389 = 0;
            D_800E738A = 0;
        } else if (D_800E7389 == 0) {
            func_80044750(0xBF);
            func_800674B0();
            func_80042808();
        } else if (func_80044E8C() == 1) {
            func_80142BE8();
            func_801433C0();
            var_v0_2 = D_80122DD0;
            do {
                var_v0_2 += 0x10;
                *(s32 *)((unsigned char *)var_v0_2 + (-12)) = 0;
                *(s32 *)((unsigned char *)var_v0_2 + (-8)) = 0;
                *(s32 *)((unsigned char *)var_v0_2 + (-4)) = 0;
                *(s32 *)((unsigned char *)var_v0_2 + (-16)) = 0;
            } while (var_v0_2 != &D_80122ED0);
            func_8008585C();
            dec_bg_reset();
            set_dec_bri(0);
            func_80072B5C(1);
        }
        break;
    case 9:
        func_80147214();
        break;
    default:
        func_80046500();
        break;
    }
    if (D_80122E98 == 0) {
        Default_Disp();
    }
}
