void func_80132390(void) {
    s32 sp74;
    s32 spA8;
    s32 spA4;
    s32 sp70;
    typedef struct { s32 w[12]; } M2cBlock30; /* m2c memcpy of 48 bytes */
    typedef struct { s32 w[13]; } M2cBlock34; /* m2c memcpy of 52 bytes */
    s32 spB4;
    s32 sp78[13];
    s32 sp44[13];
    GameState *var_s1;
    s32 temp_a0;
    s32 var_s0;
    u32 temp_t6;
    u32 temp_t8;
    u32 var_v0;

    *(M2cBlock30 *)(&sp78) = *(M2cBlock30 *)(&D_801342F4);
    *(s32 *)((unsigned char *)(&sp78) + (48)) = (s32) *(s32 *)((unsigned char *)(&D_801342F4) + (48));
    *(M2cBlock34 *)(&sp44) = *(M2cBlock34 *)(&D_80134328);
    switch (D_800E738A) {
    case 0:
        temp_a0 = func_8008647C();
        if ((((u32) (D_800E67CC * 4) >> 0x1F) == 1) && (D_800E62BE == ((u32) (D_800E67CC << 0x14) >> 0x19)) && (D_800E62BF == ((u16) D_800E67CC >> 0xC)) && (D_800E62C0 == (D_800E67CE & 0x1F))) {
            D_801342D0 = 0xFF;
            func_80042940(6);
        } else if (((u32) ((u32) (D_800E66B0 << 0x14) >> 0x1D) >= 3U) && (D_800E66C3 == 0)) {
            D_800E66C3 = 1;
            D_801342D0 = 0xB;
            if ((u8) D_800E62BF >= 6U) {
                if ((u8) D_800E62BF < 0xAU) {
                    func_80062CD0(spA4);
                } else {
                    goto block_13;
                }
            } else {
block_13:
                func_80062CD0(sp70);
            }
            func_8004284C();
        } else if (((u8) D_800E66EB >= 0x41U) && (D_800E66FC == 0)) {
            D_800E66FC = 1;
            D_801342D0 = 0xC;
            if ((u8) D_800E62BF >= 6U) {
                if ((u8) D_800E62BF < 0xAU) {
                    func_80062CD0(spA8);
                } else {
                    goto block_21;
                }
            } else {
block_21:
                func_80062CD0(sp74);
            }
            func_8004284C();
        } else {
            spB4 = temp_a0;
            if (func_8008667C(temp_a0) < 2) {
                D_801342D0 = (u8) temp_a0;
                var_v0 = (u16) D_800E6374 >> 0xC;
                var_s1 = &D_800E6280;
                var_s0 = 0;
                do {
                    if ((var_v0 == ((u8) var_s1->unk_1BC[0].unk_0C.b[2] >> 4)) && (((u32) (var_s1->unk_1BC[0].unk_0C.w << 0x1E) >> 0x1F) == 1)) {
                        if (func_8008667C(var_s0) < 2) {
                            D_801342D0 = (u8) var_s0;
                        }
                        if ((((u8) D_800E644A >> 4) == 0xA) && (((u16) D_800E6374 >> 0xC) == 0xA) && (((u32) (D_800E6678 << 0x1E) >> 0x1F) == 1)) {
                            if ((func_8008667C(0) < 2) && (func_8008667C(0xA) >= 2)) {
                                D_801342D0 = 0;
                            } else if ((func_8008667C(0) >= 2) && (func_8008667C(0xA) < 2)) {
                                D_801342D0 = 0xA;
                            } else if ((func_8008667C(0) < 2) && (func_8008667C(0xA) < 2)) {
                                if ((D_800E643E + D_800E6442) >= (D_800E666E + D_800E6672)) {
                                    D_801342D0 = 0;
                                } else {
                                    D_801342D0 = 0xA;
                                }
                            }
                        }
                    }
                    var_s0 += 1;
                    temp_t8 = (u16) D_800E6374 >> 0xC;
                    var_v0 = temp_t8;
                    var_s1 += 0x38;
                } while (var_s0 != 0xB);
                if (temp_t8 == ((u8) *(&D_800E644A + (D_801342D0 * 0x38)) >> 4)) {
                    func_80133E5C();
                } else if (((u8) D_800E62BF >= 6U) && ((u8) D_800E62BF < 0xAU)) {
                    func_80062CD0(*(s32 *)((unsigned char *)(sp + (D_801342D0 * 4)) + (120)));
                } else {
                    func_80062CD0(*(s32 *)((unsigned char *)(sp + (D_801342D0 * 4)) + (68)));
                }
                func_8004284C();
            } else {
                D_801342D0 = 0xFF;
                func_80042940(6);
            }
        }
        break;
    case 1:
        func_8004284C();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            temp_t6 = (u16) D_800E6374 >> 0xC;
            switch (temp_t6) {
            case 6:
            case 7:
            case 8:
                dec_bg_cd_read(0x4097, 1);
                break;
            case 9:
                dec_bg_cd_read(0x40A0, 1);
                break;
            case 10:
                dec_bg_cd_read(0x40A9, 1);
                break;
            default:
                func_80046500();
                break;
            }
            func_8004284C();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_80065F34(1);
            D_80125CA0 = 0xC0;
            dec_bg_show_set(1, func_8005751C(1));
            func_80044750(0xC1);
            func_80044890(1, 0xC043, 0xC01D, D_800B3688[D_801342D0], D_800B36C8[D_801342D0], D_800B3708[D_801342D0]);
            func_8004284C();
        }
        break;
    case 4:
        normal_date_bg_fadein();
        break;
    case 5:
        func_80044750(0x71);
        func_80042808();
        break;
    case 6:
        func_8004284C();
        break;
    case 7:
        D_800CA148 = 0x10;
        D_800CA14C = 0;
        func_8004284C();
        /* fallthrough */
    case 8:
        normal_date_speak();
        break;
    case 9:
        func_80042908(3);
        break;
    default:
        func_80046500();
        break;
    }
    Default_Disp();
}
