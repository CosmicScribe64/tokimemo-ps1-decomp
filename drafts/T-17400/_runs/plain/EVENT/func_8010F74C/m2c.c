void func_8010F74C(void) {
    ? spD04;
    ? spC78;
    ? spBB8;
    ? spB38;
    ? spAAC;
    ? spA24;
    ? sp994;
    ? sp900;
    ? sp870;
    ? sp7D4;
    ? sp754;
    ? sp6D4;
    ? sp538;
    ? sp4C4;
    ? sp410;
    ? sp36C;
    ? sp2D8;
    ? sp21C;
    ? sp128;
    ? spAC;
    ? sp30;
    ? *sp2C;
    ? *var_v0;
    ? *var_v0_2;

    M2C_MEMCPY_ALIGNED(&spD04, &D_801244AC, 0x48);
    M2C_MEMCPY_ALIGNED(&spC78, &D_801244F4, 0x84);
    *(void (**)())((unsigned char *)(&spC78) + (132)) = (void (*)()) D_801244F4.f[0x21].unk0;
    *(s32 *)((unsigned char *)(&spC78 + 0x84) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_801244F4.f[0x21]) + (4));
    M2C_MEMCPY_ALIGNED(&spBB8, &D_80124580, 0xC0);
    M2C_MEMCPY_ALIGNED(&spB38, &D_80124640, 0x80);
    M2C_MEMCPY_ALIGNED(&spAAC, &D_801246C0, 0x8C);
    M2C_MEMCPY_ALIGNED(&spA24, &D_8012474C, 0x84);
    *(void (**)())((unsigned char *)(&spA24) + (132)) = (void (*)()) D_8012474C.f[0x21];
    M2C_MEMCPY_ALIGNED(&sp994, &D_801247D4, 0x90);
    M2C_MEMCPY_ALIGNED(&sp900, &D_80124864, 0x90);
    *(void (**)())((unsigned char *)(&sp900) + (144)) = (void (*)()) D_80124864.f[0x24];
    M2C_MEMCPY_ALIGNED(&sp870, &D_801248F8, 0x90);
    M2C_MEMCPY_ALIGNED(&sp7D4, &D_80124988, 0x9C);
    M2C_MEMCPY_ALIGNED(&sp754, &D_80124A24, 0x80);
    M2C_MEMCPY_ALIGNED(&sp6D4, &D_80124AA4, 0x80);
    M2C_MEMCPY_ALIGNED(&sp538, &D_80124B24, 0x198);
    *(void (**)())((unsigned char *)(&sp538) + (408)) = (void (*)()) D_80124B24.f[0x66];
    M2C_MEMCPY_ALIGNED(&sp4C4, &D_80124CC0, 0x6C);
    *(s32 *)((unsigned char *)(&sp4C4 + 0x6C) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80124CC0.f[0x1B]) + (4));
    *(void (**)())((unsigned char *)(&sp4C4) + (108)) = (void (*)()) D_80124CC0.f[0x1B].unk0;
    M2C_MEMCPY_ALIGNED(&sp410, &D_80124D34, 0xB4);
    M2C_MEMCPY_ALIGNED(&sp36C, &D_80124DE8, 0x9C);
    *(s32 *)((unsigned char *)(&sp36C + 0x9C) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80124DE8.f[0x27]) + (4));
    *(void (**)())((unsigned char *)(&sp36C) + (156)) = (void (*)()) D_80124DE8.f[0x27].unk0;
    M2C_MEMCPY_ALIGNED(&sp2D8, &D_80124E8C, 0x90);
    *(void (**)())((unsigned char *)(&sp2D8) + (144)) = (void (*)()) D_80124E8C.f[0x24];
    M2C_MEMCPY_ALIGNED(&sp21C, &D_80124F20, 0xB4);
    *(s32 *)((unsigned char *)(&sp21C + 0xB4) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80124F20.f[0x2D]) + (4));
    *(void (**)())((unsigned char *)(&sp21C) + (180)) = (void (*)()) D_80124F20.f[0x2D].unk0;
    M2C_MEMCPY_ALIGNED(&sp128, &D_80124FDC, 0xF0);
    *(void (**)())((unsigned char *)(&sp128) + (240)) = (void (*)()) D_80124FDC.f[0x3C];
    M2C_MEMCPY_ALIGNED(&spAC, &D_801250D0, 0x78);
    *(s32 *)((unsigned char *)(&spAC) + (120)) = (s32) *(s32 *)((unsigned char *)(&D_801250D0) + (120));
    M2C_MEMCPY_ALIGNED(&sp30, &D_8012514C, 0x7C);
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
