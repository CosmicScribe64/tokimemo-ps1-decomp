void func_801391E4(void) {
    s32 sp3C;
    GameState *var_s1;
    s32 temp_a0;
    s32 temp_hi;
    s32 var_s0;
    u32 temp_t1;
    u32 temp_t9;
    u8 temp_t6;
    u8 var_t2;

    func_80083808();
    temp_t6 = D_800E6280.unk_110A;
    switch (temp_t6) {
    case 0:
        func_80044750(0xBF);
        func_80044890(1, 0xC043, 0xC01D, 0xC982, 0xC935, 0xC91B);
        func_80085E30(1, 0);
        dec_bg_reset();
        set_dec_bri(0);
        message_window_init();
        k_reset(0);
        func_8006612C((s32) "　");
        func_80065F34(0);
        hizuke_init();
        parameter_disp_switch(0);
        icon_disp_switch(0);
        func_8008585C();
        func_800AE120(D_800E6280.unk_10F4);
        func_8004284C();
        break;
    case 1:
        if (func_80044E8C() == 1) {
            k_reset(0);
            temp_t9 = (u16) D_800E6280.unk_0F4.h >> 0xC;
            switch (temp_t9) {
            case 7:
                func_8006612C((s32) "国立競技場");
                func_80044750(0x207);
                dec_bg_cd_read(0x41B1, 1);
                break;
            case 8:
                func_8006612C((s32) "インターハイ");
                func_80044750(0x209);
                dec_bg_cd_read(0x41BA, 1);
                break;
            case 9:
                func_8006612C((s32) "インターハイ");
                func_80044750(0x208);
                dec_bg_cd_read(0x41BA, 1);
                break;
            case 10:
                func_8006612C((s32) "インターハイ");
                func_80044750(0x201);
                dec_bg_cd_read(0x41BA, 1);
                break;
            default:
                func_80046500();
                break;
            }
            func_80065F34(0);
            func_8004284C();
        }
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            temp_a0 = func_8008647C();
            var_t2 = 0xC;
            if ((u8) D_800E6280.unk_1BC[0xC].unk_0C.b[3] >= 0x41U) {
                goto block_42;
            }
            sp3C = temp_a0;
            if (func_8008667C(temp_a0) < 2) {
                D_8013C790 = (u8) temp_a0;
                var_s1 = &D_800E6280;
                var_s0 = 0;
                do {
                    if ((((u16) D_800E6280.unk_0F4.h >> 0xC) == ((u8) var_s1->unk_1BC[0].unk_0C.b[2] >> 4)) && (((u32) (var_s1->unk_1BC[0].unk_0C.w << 0x1E) >> 0x1F) == 1)) {
                        if (func_8008667C(var_s0) < 2) {
                            D_8013C790 = (u8) var_s0;
                        }
                        if ((((u8) D_800E6280.unk_1BC[0].unk_0C.h[1] >> 4) == 0xA) && (((u16) D_800E6280.unk_0F4.h >> 0xC) == 0xA)) {
                            if ((func_8008667C(0) < 2) && (func_8008667C(0xA) >= 2)) {
                                D_8013C790 = 0;
                            } else if ((func_8008667C(0) >= 2) && (func_8008667C(0xA) < 2)) {
                                D_8013C790 = 0xA;
                            } else if ((func_8008667C(0) < 2) && (func_8008667C(0xA) < 2)) {
                                if ((D_800E6280.unk_1BC[0].unk_02 + D_800E6280.unk_1BC[0].unk_06) >= (D_800E6280.unk_1BC[0xA].unk_02 + D_800E6280.unk_1BC[0xA].unk_06)) {
                                    D_8013C790 = 0;
                                } else {
                                    D_8013C790 = 0xA;
                                }
                            }
                        }
                    }
                    var_s0 += 1;
                    var_s1 += 0x38;
                } while (var_s0 != 0xB);
            } else {
                var_t2 = 0xFF;
                if ((u8) D_800E6280.unk_1BC[0xB].unk_0C.b[3] >= 2U) {
                    D_8013C790 = 0xB;
                } else {
block_42:
                    D_8013C790 = var_t2;
                }
            }
            if (D_8013C790 != 0xFF) {
                func_80044890(0, 0xBF98, 0xBF79, D_800B3688[D_8013C790], D_800B36C8[D_8013C790], D_800B3708[D_8013C790]);
            }
            func_8004284C();
        }
        break;
    case 3:
        if ((func_80044E8C() == 1) || (D_8013C790 == 0xFF)) {
            dec_bg_show_set(1, func_8005751C(1));
            cd_read_sectors(0xBU, 0x801E8000, 0xA9C5);
            func_80065F34(1);
            func_8004284C();
        }
        break;
    case 4:
        normal_date_bg_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            func_800847B8(0U);
            func_80084E4C();
            func_80138FC0();
            D_800CA160 = (s32) D_8013C750;
            D_800CA164 = (s32) D_8013C754;
            D_800CA168 = (s32) D_8013C758;
            temp_t1 = (u16) D_800E6280.unk_0F4.h >> 0xC;
            switch (temp_t1) {
            case 7:
                D_800CA148 = 1;
                break;
            case 8:
                D_800CA148 = 2;
                break;
            case 9:
                D_800CA148 = 3;
                break;
            case 10:
                D_800CA148 = 4;
                break;
            default:
                func_80046500();
                break;
            }
            D_800CA14C = 0;
            func_8004284C();
        }
        break;
    case 6:
    case 8:
        normal_date_speak();
        break;
    case 7:
        temp_hi = func_800AE0D0() % 6;
        D_800CA148 = temp_hi + 6;
        D_8013C794 = (s16) temp_hi;
        func_8004284C();
        break;
    case 9:
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
    Default_Disp();
}
