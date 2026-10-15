void func_80144E20(void) {
    typedef struct { s32 w[18]; } M2cBlock48; /* m2c memcpy of 72 bytes */
    typedef struct { s32 w[35]; } M2cBlock8C; /* m2c memcpy of 140 bytes */
    typedef struct { s32 w[48]; } M2cBlockC0; /* m2c memcpy of 192 bytes */
    typedef struct { s32 w[32]; } M2cBlock80; /* m2c memcpy of 128 bytes */
    typedef struct { s32 w[33]; } M2cBlock84; /* m2c memcpy of 132 bytes */
    typedef struct { s32 w[36]; } M2cBlock90; /* m2c memcpy of 144 bytes */
    typedef struct { s32 w[50]; } M2cBlockC8; /* m2c memcpy of 200 bytes */
    typedef struct { s32 w[39]; } M2cBlock9C; /* m2c memcpy of 156 bytes */
    typedef struct { s32 w[102]; } M2cBlock198; /* m2c memcpy of 408 bytes */
    typedef struct { s32 w[27]; } M2cBlock6C; /* m2c memcpy of 108 bytes */
    typedef struct { s32 w[45]; } M2cBlockB4; /* m2c memcpy of 180 bytes */
    typedef struct { s32 w[75]; } M2cBlock12C; /* m2c memcpy of 300 bytes */
    typedef struct { s32 w[72]; } M2cBlock120; /* m2c memcpy of 288 bytes */
    typedef struct { s32 w[30]; } M2cBlock78; /* m2c memcpy of 120 bytes */
    typedef struct { s32 w[31]; } M2cBlock7C; /* m2c memcpy of 124 bytes */
    s32 spDEC[18];
    s32 spD60[35];
    s32 spCA0[48];
    s32 spC20[32];
    s32 spB94[35];
    s32 spB0C[34];
    s32 spA7C[36];
    s32 sp9E8[37];
    s32 sp920[50];
    s32 sp884[39];
    s32 sp804[32];
    s32 sp77C[34];
    s32 sp5DC[103];
    s32 sp568[28];
    s32 sp4B4[45];
    s32 sp410[40];
    s32 sp384[34];
    s32 sp254[76];
    s32 sp12C[73];
    s32 spB0[31];
    s32 sp34[31];
    s32 *sp30;
    s32 (**var_a3)(s32);
    s32 *var_v0;
    s32 var_t8;
    u8 var_t7;

    *(M2cBlock48 *)(&spDEC) = *(M2cBlock48 *)(&D_8015C080);
    *(M2cBlock8C *)(&spD60) = *(M2cBlock8C *)(&D_8015C0C8);
    *(M2cBlockC0 *)(&spCA0) = *(M2cBlockC0 *)(&D_8015C154);
    *(M2cBlock80 *)(&spC20) = *(M2cBlock80 *)(&D_8015C214);
    *(M2cBlock8C *)(&spB94) = *(M2cBlock8C *)(&D_8015C294);
    *(M2cBlock84 *)(&spB0C) = *(M2cBlock84 *)(&D_8015C320);
    *(s32 *)((unsigned char *)(&spB0C) + (132)) = (s32) *(s32 *)((unsigned char *)(&D_8015C320) + (132));
    *(M2cBlock90 *)(&spA7C) = *(M2cBlock90 *)(&D_8015C3A8);
    *(M2cBlock90 *)(&sp9E8) = *(M2cBlock90 *)(&D_8015C438);
    *(s32 *)((unsigned char *)(&sp9E8) + (144)) = (s32) *(s32 *)((unsigned char *)(&D_8015C438) + (144));
    *(M2cBlockC8 *)(&sp920) = *(M2cBlockC8 *)(&D_8015C4CC);
    *(M2cBlock9C *)(&sp884) = *(M2cBlock9C *)(&D_8015C594);
    *(M2cBlock80 *)(&sp804) = *(M2cBlock80 *)(&D_8015C630);
    *(M2cBlock84 *)(&sp77C) = *(M2cBlock84 *)(&D_8015C6B0);
    *(s32 *)((unsigned char *)(&sp77C) + (132)) = (s32) *(s32 *)((unsigned char *)(&D_8015C6B0) + (132));
    *(M2cBlock198 *)(&sp5DC) = *(M2cBlock198 *)(&D_8015C738);
    *(s32 *)((unsigned char *)(&sp5DC + 0x198) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8015C738 + 0x198) + (4));
    *(s32 *)((unsigned char *)(&sp5DC) + (408)) = (s32) *(s32 *)((unsigned char *)(&D_8015C738) + (408));
    *(M2cBlock6C *)(&sp568) = *(M2cBlock6C *)(&D_8015C8D8);
    *(s32 *)((unsigned char *)(&sp568 + 0x6C) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8015C8D8 + 0x6C) + (4));
    *(s32 *)((unsigned char *)(&sp568) + (108)) = (s32) *(s32 *)((unsigned char *)(&D_8015C8D8) + (108));
    *(M2cBlockB4 *)(&sp4B4) = *(M2cBlockB4 *)(&D_8015C94C);
    *(M2cBlock9C *)(&sp410) = *(M2cBlock9C *)(&D_8015CA00);
    *(s32 *)((unsigned char *)(&sp410 + 0x9C) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8015CA00 + 0x9C) + (4));
    *(s32 *)((unsigned char *)(&sp410) + (156)) = (s32) *(s32 *)((unsigned char *)(&D_8015CA00) + (156));
    *(M2cBlock84 *)(&sp384) = *(M2cBlock84 *)(&D_8015CAA4);
    *(s32 *)((unsigned char *)(&sp384 + 0x84) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8015CAA4 + 0x84) + (4));
    *(s32 *)((unsigned char *)(&sp384) + (132)) = (s32) *(s32 *)((unsigned char *)(&D_8015CAA4) + (132));
    *(M2cBlock12C *)(&sp254) = *(M2cBlock12C *)(&D_8015CB30);
    *(s32 *)((unsigned char *)(&sp254) + (300)) = (s32) *(s32 *)((unsigned char *)(&D_8015CB30) + (300));
    *(M2cBlock120 *)(&sp12C) = *(M2cBlock120 *)(&D_8015CC60);
    *(s32 *)((unsigned char *)(&sp12C + 0x120) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8015CC60 + 0x120) + (4));
    *(s32 *)((unsigned char *)(&sp12C) + (288)) = (s32) *(s32 *)((unsigned char *)(&D_8015CC60) + (288));
    *(M2cBlock78 *)(&spB0) = *(M2cBlock78 *)(&D_8015CD88);
    *(s32 *)((unsigned char *)(&spB0) + (120)) = (s32) *(s32 *)((unsigned char *)(&D_8015CD88) + (120));
    *(M2cBlock7C *)(&sp34) = *(M2cBlock7C *)(&D_8015CE04);
    var_v0 = &spDEC;
    switch (D_800CA2F8) {
    case 0:
        var_v0 = &spDEC;
        break;
    case 1:
        var_v0 = &spD60;
        break;
    case 2:
        var_v0 = &spCA0;
        break;
    case 3:
        var_v0 = &spC20;
        break;
    case 4:
        var_v0 = &spB94;
        break;
    case 5:
        var_v0 = &spB0C;
        break;
    case 6:
        var_v0 = &spA7C;
        break;
    case 7:
        var_v0 = &sp9E8;
        break;
    case 8:
        var_v0 = &sp920;
        break;
    case 9:
        var_v0 = &sp884;
        break;
    case 10:
        var_v0 = &sp804;
        break;
    case 11:
        var_v0 = &sp77C;
        break;
    case 12:
        var_v0 = &sp5DC;
        break;
    case 13:
        var_v0 = &sp568;
        break;
    case 14:
        var_v0 = &sp4B4;
        break;
    case 15:
        var_v0 = &sp410;
        break;
    case 16:
        var_v0 = &sp384;
        break;
    case 17:
        var_v0 = &sp254;
        break;
    case 18:
        var_v0 = &sp12C;
        break;
    case 19:
        var_v0 = &spB0;
        break;
    case 20:
        var_v0 = &sp34;
        break;
    }
    var_a3 = var_v0 + (D_800E738A * 4);
    if (&normal_date_move_place == *var_a3) {
        if (D_800E738D == 0) {
            D_8015BF48 = (u8) D_8015BF44;
        }
        if ((D_800CA2F8 != 1) && (D_800CA2F8 != 4)) {
            var_t7 = D_800E738D + 1;
            if (D_800CA2F8 != 0xC) {
                if (((D_800E738D == 0) && (D_8015BF48 == 1)) || ((D_800E738D == 0xE) && (D_8015BF48 == 0))) {
                    D_8015BF44 = 1 - D_8015BF48;
                }
                goto block_34;
            }
        } else {
block_34:
            var_t7 = D_800E738D + 1;
        }
        D_800E738D = var_t7;
    }
    if ((((D_800E62BE == 0x5F) && (D_800E62BF == 7) && (D_800E62C0 == 0x17)) || ((D_800E62BE == 0x60) && (D_800E62BF == 7) && (D_800E62C0 == 0x1C)) || ((D_800E62BE == 0x61) && (D_800E62BF == 7) && (D_800E62C0 == 0x1B))) && (D_800B593C == 0x80) && ((u8) D_8015BF44 != 0) && (func_80146224 != *var_a3)) {
        sp30 = var_v0;
        func_8006B900();
        D_80122D44 = 1;
        var_a3 = var_v0 + (D_800E738A * 4);
    } else if ((D_800CA2F8 == 1) && (D_800CA2FC == 0) && ((u8) D_800E738A >= 0xCU) && ((u8) D_800E738A < 0x15U) && ((u8) D_800E62BF >= 3U) && ((u8) D_800E62BF < 9U) && (D_800B593C == 0x80)) {
        sp30 = var_v0;
        func_8006B900();
        D_80122D44 = 1;
        var_a3 = var_v0 + (D_800E738A * 4);
    } else {
        if ((D_800CA2F8 == 0x14) && ((u8) D_800E738A >= 7U) && ((u8) D_800E738A < 0x11U) && (D_800B593C == 0x80)) {
            sp30 = var_v0;
            func_8006B900();
            var_t8 = D_800E738A * 4;
            D_80122D44 = 1;
            goto block_101;
        }
        if ((D_800CA2F8 == 0x11) && ((u8) D_800E738A < 8U) && (D_800B593C == 0x80)) {
            sp30 = var_v0;
            func_8006B900();
            D_80122D44 = 1;
            var_a3 = var_v0 + (D_800E738A * 4);
        } else {
            if ((D_800CA2F8 == 0x11) && ((u8) D_800E738A >= 9U) && ((u8) D_800E738A < 0x1EU) && (D_800B593C == 0x80)) {
                sp30 = var_v0;
                func_8006B900();
                var_t8 = D_800E738A * 4;
                D_80122D44 = 1;
                goto block_101;
            }
            if ((D_800CA2F8 == 0x11) && ((u8) D_800E738A >= 0x1FU) && ((u8) D_800E738A < 0x2CU) && (D_800B593C == 0x80)) {
                sp30 = var_v0;
                func_8006B900();
                D_80122D44 = 1;
                var_a3 = var_v0 + (D_800E738A * 4);
            } else {
                if ((D_800CA2F8 == 0x11) && ((u8) D_800E738A >= 0x2DU) && ((u8) D_800E738A < 0x31U) && (D_800B593C == 0x80)) {
                    sp30 = var_v0;
                    func_8006B900();
                    var_t8 = D_800E738A * 4;
                    D_80122D44 = 1;
                    goto block_101;
                }
                if ((D_800CA2F8 == 0x11) && ((u8) D_800E738A >= 0x3EU)) {
                    if (((u8) D_800E738A < 0x49U) && (D_800B593C == 0x80)) {
                        sp30 = var_v0;
                        func_8006B900();
                        D_80122D44 = 1;
                        var_a3 = var_v0 + (D_800E738A * 4);
                    } else {
                        goto block_87;
                    }
                } else {
block_87:
                    if ((D_800CA2F8 == 0x10) && ((u8) D_800E738A < 8U) && (D_800B593C == 0x80)) {
                        sp30 = var_v0;
                        func_8006B900();
                        var_t8 = D_800E738A * 4;
                        D_80122D44 = 1;
                        goto block_101;
                    }
                    if ((D_800CA2F8 == 0x10) && ((u8) D_800E738A >= 9U) && ((u8) D_800E738A < 0x13U) && (D_800B593C == 0x80)) {
                        sp30 = var_v0;
                        func_8006B900();
                        D_80122D44 = 1;
                        var_a3 = var_v0 + (D_800E738A * 4);
                    } else if ((D_800CA2F8 == 0x10) && ((u8) D_800E738A >= 0x14U) && ((u8) D_800E738A < 0x22U) && (D_800B593C == 0x80)) {
                        sp30 = var_v0;
                        func_8006B900();
                        var_t8 = D_800E738A * 4;
                        D_80122D44 = 1;
block_101:
                        var_a3 = var_v0 + var_t8;
                    }
                }
            }
        }
    }
    *var_a3(0x80);
    if ((D_800CA2F8 == 0xC) && ((u8) D_800E738A >= 0x45U) && ((u8) D_800E738A < 0x5BU) && (D_80122D34 != 0)) {
        D_80120653 |= 0x80;
        func_80140D38();
    } else if ((D_800CA2F8 == 0xC) && ((u8) D_800E738A >= 0x5BU)) {
        D_80120653 &= 0x7F;
    }
    if (D_80122D14 != 0) {
        func_8013FD6C();
    }
    if ((D_800CA2F8 == 1) && (D_800E738A >= 0xCU)) {
        if (D_800E738A < 0x15U) {
            func_801407D0();
        }
    }
    if (D_800CA2F8 == 1) {
        if (D_800E738A == 0x15) {
            func_80140788();
        }
    }
    if ((D_800CA2F8 == 0xB) && (D_800E738A < 9U) && (D_800B3CFC == 1) && (D_800CA2FC != 2)) {
        D_800CA2FC = 2;
        func_80044750(0x601);
    }
}
