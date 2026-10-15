void func_8010F74C(void) {
    typedef struct { s32 w[18]; } M2cBlock48; /* m2c memcpy of 72 bytes */
    typedef struct { s32 w[33]; } M2cBlock84; /* m2c memcpy of 132 bytes */
    typedef struct { s32 w[48]; } M2cBlockC0; /* m2c memcpy of 192 bytes */
    typedef struct { s32 w[32]; } M2cBlock80; /* m2c memcpy of 128 bytes */
    typedef struct { s32 w[35]; } M2cBlock8C; /* m2c memcpy of 140 bytes */
    typedef struct { s32 w[36]; } M2cBlock90; /* m2c memcpy of 144 bytes */
    typedef struct { s32 w[39]; } M2cBlock9C; /* m2c memcpy of 156 bytes */
    typedef struct { s32 w[102]; } M2cBlock198; /* m2c memcpy of 408 bytes */
    typedef struct { s32 w[27]; } M2cBlock6C; /* m2c memcpy of 108 bytes */
    typedef struct { s32 w[45]; } M2cBlockB4; /* m2c memcpy of 180 bytes */
    typedef struct { s32 w[60]; } M2cBlockF0; /* m2c memcpy of 240 bytes */
    typedef struct { s32 w[30]; } M2cBlock78; /* m2c memcpy of 120 bytes */
    typedef struct { s32 w[31]; } M2cBlock7C; /* m2c memcpy of 124 bytes */
    s32 spD04[18];
    s32 spC78[34];
    s32 spBB8[48];
    s32 spB38[32];
    s32 spAAC[35];
    s32 spA24[34];
    s32 sp994[36];
    s32 sp900[37];
    s32 sp870[36];
    s32 sp7D4[39];
    s32 sp754[32];
    s32 sp6D4[32];
    s32 sp538[103];
    s32 sp4C4[28];
    s32 sp410[45];
    s32 sp36C[40];
    s32 sp2D8[37];
    s32 sp21C[46];
    s32 sp128[61];
    s32 spAC[31];
    s32 sp30[31];
    s32 *sp2C;
    s32 *var_v0;
    s32 *var_v0_2;

    *(M2cBlock48 *)(&spD04) = *(M2cBlock48 *)(&D_801244AC);
    *(M2cBlock84 *)(&spC78) = *(M2cBlock84 *)(&D_801244F4);
    *(s32 *)((unsigned char *)(&spC78) + (132)) = (s32) *(s32 *)((unsigned char *)(&D_801244F4) + (132));
    *(s32 *)((unsigned char *)(&spC78 + 0x84) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_801244F4 + 0x84) + (4));
    *(M2cBlockC0 *)(&spBB8) = *(M2cBlockC0 *)(&D_80124580);
    *(M2cBlock80 *)(&spB38) = *(M2cBlock80 *)(&D_80124640);
    *(M2cBlock8C *)(&spAAC) = *(M2cBlock8C *)(&D_801246C0);
    *(M2cBlock84 *)(&spA24) = *(M2cBlock84 *)(&D_8012474C);
    *(s32 *)((unsigned char *)(&spA24) + (132)) = (s32) *(s32 *)((unsigned char *)(&D_8012474C) + (132));
    *(M2cBlock90 *)(&sp994) = *(M2cBlock90 *)(&D_801247D4);
    *(M2cBlock90 *)(&sp900) = *(M2cBlock90 *)(&D_80124864);
    *(s32 *)((unsigned char *)(&sp900) + (144)) = (s32) *(s32 *)((unsigned char *)(&D_80124864) + (144));
    *(M2cBlock90 *)(&sp870) = *(M2cBlock90 *)(&D_801248F8);
    *(M2cBlock9C *)(&sp7D4) = *(M2cBlock9C *)(&D_80124988);
    *(M2cBlock80 *)(&sp754) = *(M2cBlock80 *)(&D_80124A24);
    *(M2cBlock80 *)(&sp6D4) = *(M2cBlock80 *)(&D_80124AA4);
    *(M2cBlock198 *)(&sp538) = *(M2cBlock198 *)(&D_80124B24);
    *(s32 *)((unsigned char *)(&sp538) + (408)) = (s32) *(s32 *)((unsigned char *)(&D_80124B24) + (408));
    *(M2cBlock6C *)(&sp4C4) = *(M2cBlock6C *)(&D_80124CC0);
    *(s32 *)((unsigned char *)(&sp4C4 + 0x6C) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80124CC0 + 0x6C) + (4));
    *(s32 *)((unsigned char *)(&sp4C4) + (108)) = (s32) *(s32 *)((unsigned char *)(&D_80124CC0) + (108));
    *(M2cBlockB4 *)(&sp410) = *(M2cBlockB4 *)(&D_80124D34);
    *(M2cBlock9C *)(&sp36C) = *(M2cBlock9C *)(&D_80124DE8);
    *(s32 *)((unsigned char *)(&sp36C + 0x9C) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80124DE8 + 0x9C) + (4));
    *(s32 *)((unsigned char *)(&sp36C) + (156)) = (s32) *(s32 *)((unsigned char *)(&D_80124DE8) + (156));
    *(M2cBlock90 *)(&sp2D8) = *(M2cBlock90 *)(&D_80124E8C);
    *(s32 *)((unsigned char *)(&sp2D8) + (144)) = (s32) *(s32 *)((unsigned char *)(&D_80124E8C) + (144));
    *(M2cBlockB4 *)(&sp21C) = *(M2cBlockB4 *)(&D_80124F20);
    *(s32 *)((unsigned char *)(&sp21C + 0xB4) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80124F20 + 0xB4) + (4));
    *(s32 *)((unsigned char *)(&sp21C) + (180)) = (s32) *(s32 *)((unsigned char *)(&D_80124F20) + (180));
    *(M2cBlockF0 *)(&sp128) = *(M2cBlockF0 *)(&D_80124FDC);
    *(s32 *)((unsigned char *)(&sp128) + (240)) = (s32) *(s32 *)((unsigned char *)(&D_80124FDC) + (240));
    *(M2cBlock78 *)(&spAC) = *(M2cBlock78 *)(&D_801250D0);
    *(s32 *)((unsigned char *)(&spAC) + (120)) = (s32) *(s32 *)((unsigned char *)(&D_801250D0) + (120));
    *(M2cBlock7C *)(&sp30) = *(M2cBlock7C *)(&D_8012514C);
    var_v0 = &spD04;
    switch (D_801243B0) {
    case 0:
        var_v0 = &spD04;
        break;
    case 1:
        var_v0 = &spC78;
        break;
    case 2:
        var_v0 = &spBB8;
        break;
    case 3:
        var_v0 = &spB38;
        break;
    case 4:
        var_v0 = &spAAC;
        break;
    case 5:
        var_v0 = &spA24;
        break;
    case 6:
        var_v0 = &sp994;
        break;
    case 7:
        var_v0 = &sp900;
        break;
    case 8:
        var_v0 = &sp870;
        break;
    case 9:
        var_v0 = &sp7D4;
        break;
    case 10:
        var_v0 = &sp754;
        break;
    case 11:
        var_v0 = &sp6D4;
        break;
    case 12:
        var_v0 = &sp538;
        break;
    case 13:
        var_v0 = &sp4C4;
        break;
    case 14:
        var_v0 = &sp410;
        break;
    case 15:
        var_v0 = &sp36C;
        break;
    case 16:
        var_v0 = &sp2D8;
        break;
    case 17:
        var_v0 = &sp21C;
        break;
    case 18:
        var_v0 = &sp128;
        break;
    case 19:
        var_v0 = &spAC;
        break;
    case 20:
        var_v0 = &sp30;
        break;
    }
    sp2C = var_v0;
    *(var_v0 + (D_800B1AF6 * 4))(0x80, &spB38, &spAAC, &spA24);
    var_v0_2 = sp2C;
    if (D_800EECF0 != 0) {
        func_8011B848();
    }
    if ((var_v0_2 == &spC78) && ((u8) D_800B1AF6 >= 0xCU) && ((u8) D_800B1AF6 < 0x15U)) {
        sp2C = var_v0_2;
        func_8011C164();
    }
    if (var_v0_2 == &spC78) {
        if (D_800B1AF6 == 0x15) {
            func_8011C11C();
        }
    }
}
