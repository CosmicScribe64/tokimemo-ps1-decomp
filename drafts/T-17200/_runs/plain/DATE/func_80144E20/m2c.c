extern u8 D_800E62BE;
extern u8 D_800E62BF;
extern u8 D_800E62C0;
extern u8 D_800E738A;
extern u8 D_800E738D;
extern u8 D_8015BF48;
extern ? func_8007EAAC;

void func_80144E20(void) {
    ? spDEC;
    ? spD60;
    ? spCA0;
    ? spC20;
    ? spB94;
    ? spB0C;
    ? spA7C;
    ? sp9E8;
    ? sp920;
    ? sp884;
    ? sp804;
    ? sp77C;
    ? sp5DC;
    ? sp568;
    ? sp4B4;
    ? sp410;
    ? sp384;
    ? sp254;
    ? sp12C;
    ? spB0;
    ? sp34;
    ? *sp30;
    ? (**var_a3)(?);
    ? *var_v0;
    s32 var_t8;
    u8 var_t7;

    M2C_MEMCPY_ALIGNED(&spDEC, &D_8015C080, 0x48);
    M2C_MEMCPY_ALIGNED(&spD60, &D_8015C0C8, 0x8C);
    M2C_MEMCPY_ALIGNED(&spCA0, &D_8015C154, 0xC0);
    M2C_MEMCPY_ALIGNED(&spC20, &D_8015C214, 0x80);
    M2C_MEMCPY_ALIGNED(&spB94, &D_8015C294, 0x8C);
    M2C_MEMCPY_ALIGNED(&spB0C, &D_8015C320, 0x84);
    *(void (**)())((unsigned char *)(&spB0C) + (132)) = (void (*)()) D_8015C320.f[0x21];
    M2C_MEMCPY_ALIGNED(&spA7C, &D_8015C3A8, 0x90);
    M2C_MEMCPY_ALIGNED(&sp9E8, &D_8015C438, 0x90);
    *(void (**)())((unsigned char *)(&sp9E8) + (144)) = (void (*)()) D_8015C438.f[0x24];
    M2C_MEMCPY_ALIGNED(&sp920, &D_8015C4CC, 0xC8);
    M2C_MEMCPY_ALIGNED(&sp884, &D_8015C594, 0x9C);
    M2C_MEMCPY_ALIGNED(&sp804, &D_8015C630, 0x80);
    M2C_MEMCPY_ALIGNED(&sp77C, &D_8015C6B0, 0x84);
    *(void (**)())((unsigned char *)(&sp77C) + (132)) = (void (*)()) D_8015C6B0.f[0x21];
    M2C_MEMCPY_ALIGNED(&sp5DC, &D_8015C738, 0x198);
    *(s32 *)((unsigned char *)(&sp5DC + 0x198) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8015C738.f[0x66]) + (4));
    *(void (**)())((unsigned char *)(&sp5DC) + (408)) = (void (*)()) D_8015C738.f[0x66].unk0;
    M2C_MEMCPY_ALIGNED(&sp568, &D_8015C8D8, 0x6C);
    *(s32 *)((unsigned char *)(&sp568 + 0x6C) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8015C8D8.f[0x1B]) + (4));
    *(void (**)())((unsigned char *)(&sp568) + (108)) = (void (*)()) D_8015C8D8.f[0x1B].unk0;
    M2C_MEMCPY_ALIGNED(&sp4B4, &D_8015C94C, 0xB4);
    M2C_MEMCPY_ALIGNED(&sp410, &D_8015CA00, 0x9C);
    *(s32 *)((unsigned char *)(&sp410 + 0x9C) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8015CA00.f[0x27]) + (4));
    *(void (**)())((unsigned char *)(&sp410) + (156)) = (void (*)()) D_8015CA00.f[0x27].unk0;
    M2C_MEMCPY_ALIGNED(&sp384, &D_8015CAA4, 0x84);
    *(s32 *)((unsigned char *)(&sp384 + 0x84) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8015CAA4.f[0x21]) + (4));
    *(void (**)())((unsigned char *)(&sp384) + (132)) = (void (*)()) D_8015CAA4.f[0x21].unk0;
    M2C_MEMCPY_ALIGNED(&sp254, &D_8015CB30, 0x12C);
    *(void (**)())((unsigned char *)(&sp254) + (300)) = (void (*)()) D_8015CB30.f[0x4B];
    M2C_MEMCPY_ALIGNED(&sp12C, &D_8015CC60, 0x120);
    *(s32 *)((unsigned char *)(&sp12C + 0x120) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8015CC60.f[0x48]) + (4));
    *(void (**)())((unsigned char *)(&sp12C) + (288)) = (void (*)()) D_8015CC60.f[0x48].unk0;
    M2C_MEMCPY_ALIGNED(&spB0, &D_8015CD88, 0x78);
    *(void (**)())((unsigned char *)(&spB0) + (120)) = (void (*)()) D_8015CD88.f[0x1E];
    M2C_MEMCPY_ALIGNED(&sp34, &D_8015CE04, 0x7C);
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
    if (&func_8007EAAC == *var_a3) {
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
