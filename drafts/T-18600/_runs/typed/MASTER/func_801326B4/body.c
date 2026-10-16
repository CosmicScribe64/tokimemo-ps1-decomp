void func_801326B4(void) {
    s32 sp1E0;
    s32 sp1DC;
    MasterWords13 sp1A8;
    MasterWords13 sp174;
    MasterWords13 sp140;
    MasterWords13 sp10C;
    MasterWords13 spD8;
    MasterWords13 spA4;
    MasterWords13 sp70;
    MasterWords13 sp3C;
    GameState *sp34;
    GameState *var_v1;
    GsHalf *var_a0_3;
    s32 temp_a0;
    s32 temp_a1;
    s32 temp_v0;
    s32 var_a0;
    u16 var_a0_2;
    u32 temp_t4;
    u32 temp_t8;
    u32 temp_v1;
    u32 temp_v1_2;
    u8 temp_t0;

    sp1A8 = D_8013C2F4;
    sp174 = D_8013C328;
    sp140 = D_8013C35C;
    sp10C = D_8013C390;
    spD8 = D_8013C3C4;
    spA4 = D_8013C3F8;
    sp70 = D_8013C42C;
    sp3C = D_8013C460;
    func_80083808();
    temp_t0 = D_800E6280.unk_110A;
    switch (temp_t0) {
    case 0:
        if (D_800E6280.unk_110D == 0) {
            set_dec_bri(0);
            func_8006BC28(0);
            func_80084E4C();
            func_8006BD6C(0);
            hizuke_init();
            message_window_init();
            temp_a1 = func_8008647C();
            temp_a0 = temp_a1;
            if ((u8) D_800E6280.unk_1BC[0xC].unk_0C.b[3] >= 0x41U) {
                D_8013C2E0 = 0xC;
            } else {
                sp1DC = temp_a1;
                if (func_8008667C(temp_a0) < 2) {
                    D_8013C2E0 = temp_a1;
                    var_v1 = &D_800E6280;
                    var_a0 = 0;
                    do {
                        if ((((u16) D_800E6280.unk_0F4.h >> 0xC) == ((u8) var_v1->unk_1BC[0].unk_0C.b[2] >> 4)) && (((u32) (var_v1->unk_1BC[0].unk_0C.w << 0x1E) >> 0x1F) == 1)) {
                            sp34 = var_v1;
                            sp1E0 = var_a0;
                            temp_v0 = func_8008667C(var_a0);
                            if (temp_v0 < 2) {
                                D_8013C2E0 = var_a0;
                            }
                        }
                        var_a0 += 1;
                        var_v1 += 0x38;
                    } while (var_a0 != 0xB);
                } else if ((u8) D_800E6280.unk_1BC[0xB].unk_0C.b[3] >= 2U) {
                    D_8013C2E0 = 0xB;
                } else {
                    D_8013C2E0 = 0xFF;
                    func_80042940(0xE);
                }
            }
            if (D_8013C2E0 != 0xFF) {
                func_80044750(0xBF);
                func_80044890(1, 0xC043, 0xC01D, D_800B3688[D_8013C2E0], D_800B36C8[D_8013C2E0], D_800B3708[D_8013C2E0]);
            }
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        } else if (func_80044E8C() == 1) {
            k_reset(0);
            temp_v1 = (u16) D_800E6280.unk_0F4.h >> 0xC;
            if ((temp_v1 == 1) && (D_8013C2E0 == 0) && (((u8) D_800E6280.unk_1BC[0].unk_0C.h[1] >> 4) == 1)) {
                func_8006612C((s32) "近所の公園");
                D_80125CA0 = 0xC0;
                dec_bg_cd_read(0x42D8, 1);
            } else if (((temp_v1 == 1) && (D_8013C2E0 == 1) && (((u8) D_800E6280.unk_1BC[1].unk_0C.h[1] >> 4) == 1)) || (temp_v1 == 5) || (temp_v1 == 0)) {
                func_8006612C((s32) "舞台裏");
                D_80125CA0 = 0xC0;
                dec_bg_cd_read(0x40F4, 1);
            } else {
                func_8006612C((s32) "自宅前");
                D_80125CA0 = 0xC0;
                dec_bg_cd_read(0x414C, 1);
            }
            func_8004284C();
        }
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_800847B8((u8) D_8013C2E0);
            temp_v1_2 = (u16) D_800E6280.unk_0F4.h >> 0xC;
            if ((temp_v1_2 == 1) && (D_8013C2E0 == 0) && (((u8) D_800E6280.unk_1BC[0].unk_0C.h[1] >> 4) == 1)) {
                func_80062CD0(0x544A);
            } else if ((temp_v1_2 == 1) && (D_8013C2E0 == 1)) {
                if (((u8) D_800E6280.unk_1BC[1].unk_0C.h[1] >> 4) == 1) {
                    func_80062CD0(0x5D6E);
                } else {
                    goto block_42;
                }
            } else {
block_42:
                if ((temp_v1_2 == 5) || (temp_v1_2 == 0)) {
                    func_80062CD0(sp174.v[D_8013C2E0]);
                } else {
                    func_80062CD0(sp1A8.v[D_8013C2E0]);
                }
            }
            func_8004284C();
        }
        break;
    case 2:
        normal_date_bg_fadein();
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            D_800E6280.unk_F5F = (u8) D_8013C2E0;
            func_80083474();
            if (((u16) D_800E6280.unk_0F4.h >> 0xC) == 3) {
                cd_read_sectors(3U, 0x801E8000, 0xA9B7);
            }
            if (D_8013C2E0 == 0xC) {
                func_80083440(0U);
            }
        }
        break;
    case 4:
        var_a0_2 = D_800E6280.unk_0F4.h;
        temp_t8 = var_a0_2 >> 0xC;
        switch (temp_t8) {
        case 0:
            D_800CA148 = (s16) sp140.v[D_8013C2E0];
            break;
        case 1:
            if ((D_8013C2E0 == 0) && (((u8) D_800E6280.unk_1BC[0].unk_0C.h[1] >> 4) == 1)) {
                D_800CA148 = 7;
                func_8004284C();
                var_a0_2 = D_800E6280.unk_0F4.h;
            } else if ((D_8013C2E0 == 1) && (((u8) D_800E6280.unk_1BC[1].unk_0C.h[1] >> 4) == 1)) {
                D_800CA148 = 8;
                func_8004284C();
                var_a0_2 = D_800E6280.unk_0F4.h;
            } else {
                D_800CA148 = (s16) sp10C.v[D_8013C2E0];
            }
            break;
        case 2:
            D_800CA148 = (s16) spD8.v[D_8013C2E0];
            break;
        case 3:
            if (cd_read_sync_status() & 1) {
                func_80132034();
                D_800CA160 = (s32) D_8013C2D0;
                D_800CA164 = (s32) D_8013C2D4;
                D_800CA168 = (s32) D_8013C2D8;
                D_800CA148 = (s16) spA4.v[D_8013C2E0];
                if (D_8013C2E0 == 0xB) {
                    func_80047550();
                    func_80044750(0x500);
                }
                func_80044750(0x200);
                func_8004284C();
            }
            var_a0_2 = D_800E6280.unk_0F4.h;
            break;
        case 4:
            D_800CA148 = (s16) sp70.v[D_8013C2E0];
            break;
        case 5:
            if ((D_8013C2E0 == 0) && (((u8) D_800E6280.unk_1BC[0].unk_0C.h[1] >> 4) == 5)) {
                D_800CA148 = 5;
                func_8004284C();
                var_a0_2 = D_800E6280.unk_0F4.h;
            } else if ((D_8013C2E0 == 3) && (((u8) D_800E6280.unk_1BC[3].unk_0C.h[1] >> 4) == 5)) {
                D_800CA148 = 0xC;
                func_8004284C();
                var_a0_2 = D_800E6280.unk_0F4.h;
            } else {
                D_800CA148 = (s16) sp3C.v[D_8013C2E0];
            }
            break;
        default:
            func_80046500();
            var_a0_2 = D_800E6280.unk_0F4.h;
            break;
        }
        D_800CA14C = 0;
        if ((var_a0_2 >> 0xC) != 3) {
            if (D_8013C2E0 == 0xB) {
                func_80047550();
                func_80044750(0x500);
            }
            func_80044750(0x200);
            func_8004284C();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_girl_in();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        temp_t4 = (u16) D_800E6280.unk_0F4.h >> 0xC;
        switch (temp_t4) {
        case 0:
        case 2:
            if (D_8013C2E0 == 0xA) {
                D_800CA148 = 2;
                func_8004284C();
            } else if (D_8013C2E0 == 0xB) {
                if (((u32) (D_800E6280.unk_1BC[0xB].unk_0C.w << 0x14) >> 0x1D) == 0) {
                    D_800CA148 = 3;
                } else {
                    D_800CA148 = 4;
                }
                func_80042940(0xC);
            } else if (D_8013C2E0 == 0xC) {
                D_800CA148 = 5;
                func_80042940(0xC);
            } else {
                D_800CA148 = 1;
                func_8004284C();
            }
            break;
        case 1:
        case 3:
        case 5:
            if (D_8013C2E0 == 0xB) {
                if (((u32) (D_800E6280.unk_1BC[0xB].unk_0C.w << 0x14) >> 0x1D) == 0) {
                    D_800CA148 = 2;
                } else {
                    D_800CA148 = 3;
                }
                func_80042940(0xC);
            } else if (D_8013C2E0 == 0xC) {
                D_800CA148 = 4;
                func_80042940(0xC);
            } else {
                D_800CA148 = 1;
                func_8004284C();
            }
            break;
        case 4:
            if (D_8013C2E0 == 0) {
                func_80042940(0xA);
            } else if (D_8013C2E0 == 0xB) {
                if (((u32) (D_800E6280.unk_1BC[0xB].unk_0C.w << 0x14) >> 0x1D) == 0) {
                    D_800CA148 = 2;
                } else {
                    D_800CA148 = 3;
                }
                func_80042940(0xC);
            } else if (D_8013C2E0 == 0xC) {
                D_800CA148 = 4;
                func_80042940(0xC);
            } else {
                D_800CA148 = 1;
                func_8004284C();
            }
            break;
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_bggirl_fadeout();
        break;
    case 11:
        func_80042808();
        break;
    case 12:
        normal_date_girl_out();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        var_a0_3 = &D_800E6280.unk_0F4;
        if ((D_8013C2E0 == 0xFF) && (var_a0_3 = (GsHalf *) var_a0_3->h, (((u32) var_a0_3 >> 0xC) == 3))) {
            func_8004284C();
        } else {
            normal_date_bg_fadeout((u16) var_a0_3);
        }
        break;
    case 15:
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
    Default_Disp();
}
