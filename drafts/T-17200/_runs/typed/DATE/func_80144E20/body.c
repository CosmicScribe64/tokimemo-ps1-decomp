void func_80144E20(void) {
    FnTbl18 spDEC;
    FnTbl35 spD60;
    FnTbl48 spCA0;
    FnTbl32 spC20;
    FnTbl35 spB94;
    FnTbl34 spB0C;
    FnTbl36 spA7C;
    FnTbl37 sp9E8;
    FnTbl50 sp920;
    FnTbl39 sp884;
    FnTbl32 sp804;
    FnTbl34 sp77C;
    FnTbl104 sp5DC;
    FnTbl29 sp568;
    FnTbl45 sp4B4;
    FnTbl41 sp410;
    FnTbl35 sp384;
    FnTbl76 sp254;
    FnTbl74 sp12C;
    FnTbl31 spB0;
    FnTbl31 sp34;
    s32 *sp30;
    s32 (**var_a3)(s32);
    s32 *var_v0;
    s32 var_t8;
    u8 var_t7;

    spDEC = D_8015C080;
    spD60 = D_8015C0C8;
    spCA0 = D_8015C154;
    spC20 = D_8015C214;
    spB94 = D_8015C294;
    spB0C = D_8015C320;
    spA7C = D_8015C3A8;
    sp9E8 = D_8015C438;
    sp920 = D_8015C4CC;
    sp884 = D_8015C594;
    sp804 = D_8015C630;
    sp77C = D_8015C6B0;
    sp5DC = D_8015C738;
    sp568 = D_8015C8D8;
    sp4B4 = D_8015C94C;
    sp410 = D_8015CA00;
    sp384 = D_8015CAA4;
    sp254 = D_8015CB30;
    sp12C = D_8015CC60;
    spB0 = D_8015CD88;
    sp34 = D_8015CE04;
    var_v0 = spDEC.f;
    switch (D_800CA2F8) {
    case 0:
        var_v0 = spDEC.f;
        break;
    case 1:
        var_v0 = spD60.f;
        break;
    case 2:
        var_v0 = spCA0.f;
        break;
    case 3:
        var_v0 = spC20.f;
        break;
    case 4:
        var_v0 = spB94.f;
        break;
    case 5:
        var_v0 = spB0C.f;
        break;
    case 6:
        var_v0 = spA7C.f;
        break;
    case 7:
        var_v0 = sp9E8.f;
        break;
    case 8:
        var_v0 = sp920.f;
        break;
    case 9:
        var_v0 = sp884.f;
        break;
    case 10:
        var_v0 = sp804.f;
        break;
    case 11:
        var_v0 = sp77C.f;
        break;
    case 12:
        var_v0 = sp5DC.f;
        break;
    case 13:
        var_v0 = sp568.f;
        break;
    case 14:
        var_v0 = sp4B4.f;
        break;
    case 15:
        var_v0 = sp410.f;
        break;
    case 16:
        var_v0 = sp384.f;
        break;
    case 17:
        var_v0 = sp254.f;
        break;
    case 18:
        var_v0 = sp12C.f;
        break;
    case 19:
        var_v0 = spB0.f;
        break;
    case 20:
        var_v0 = sp34.f;
        break;
    }
    var_a3 = var_v0 + (D_800E6280.unk_110A * 4);
    if (&normal_date_move_place == *var_a3) {
        if (D_800E6280.unk_110D == 0) {
            D_8015BF48 = (u8) D_8015BF44;
        }
        if ((D_800CA2F8 != 1) && (D_800CA2F8 != 4)) {
            var_t7 = D_800E6280.unk_110D + 1;
            if (D_800CA2F8 != 0xC) {
                if (((D_800E6280.unk_110D == 0) && (D_8015BF48 == 1)) || ((D_800E6280.unk_110D == 0xE) && (D_8015BF48 == 0))) {
                    D_8015BF44 = 1 - D_8015BF48;
                }
                goto block_34;
            }
        } else {
block_34:
            var_t7 = D_800E6280.unk_110D + 1;
        }
        D_800E6280.unk_110D = var_t7;
    }
    if ((((D_800E6280.unk_03E == 0x5F) && (D_800E6280.unk_03F == 7) && (D_800E6280.unk_040 == 0x17)) || ((D_800E6280.unk_03E == 0x60) && (D_800E6280.unk_03F == 7) && (D_800E6280.unk_040 == 0x1C)) || ((D_800E6280.unk_03E == 0x61) && (D_800E6280.unk_03F == 7) && (D_800E6280.unk_040 == 0x1B))) && (D_800B593C == 0x80) && ((u8) D_8015BF44 != 0) && (func_80146224 != *var_a3)) {
        sp30 = var_v0;
        func_8006B900();
        D_80122D44 = 1;
        var_a3 = var_v0 + (D_800E6280.unk_110A * 4);
    } else if ((D_800CA2F8 == 1) && (D_800CA2FC == 0) && ((u8) D_800E6280.unk_110A >= 0xCU) && ((u8) D_800E6280.unk_110A < 0x15U) && ((u8) D_800E6280.unk_03F >= 3U) && ((u8) D_800E6280.unk_03F < 9U) && (D_800B593C == 0x80)) {
        sp30 = var_v0;
        func_8006B900();
        D_80122D44 = 1;
        var_a3 = var_v0 + (D_800E6280.unk_110A * 4);
    } else {
        if ((D_800CA2F8 == 0x14) && ((u8) D_800E6280.unk_110A >= 7U) && ((u8) D_800E6280.unk_110A < 0x11U) && (D_800B593C == 0x80)) {
            sp30 = var_v0;
            func_8006B900();
            var_t8 = D_800E6280.unk_110A * 4;
            D_80122D44 = 1;
            goto block_101;
        }
        if ((D_800CA2F8 == 0x11) && ((u8) D_800E6280.unk_110A < 8U) && (D_800B593C == 0x80)) {
            sp30 = var_v0;
            func_8006B900();
            D_80122D44 = 1;
            var_a3 = var_v0 + (D_800E6280.unk_110A * 4);
        } else {
            if ((D_800CA2F8 == 0x11) && ((u8) D_800E6280.unk_110A >= 9U) && ((u8) D_800E6280.unk_110A < 0x1EU) && (D_800B593C == 0x80)) {
                sp30 = var_v0;
                func_8006B900();
                var_t8 = D_800E6280.unk_110A * 4;
                D_80122D44 = 1;
                goto block_101;
            }
            if ((D_800CA2F8 == 0x11) && ((u8) D_800E6280.unk_110A >= 0x1FU) && ((u8) D_800E6280.unk_110A < 0x2CU) && (D_800B593C == 0x80)) {
                sp30 = var_v0;
                func_8006B900();
                D_80122D44 = 1;
                var_a3 = var_v0 + (D_800E6280.unk_110A * 4);
            } else {
                if ((D_800CA2F8 == 0x11) && ((u8) D_800E6280.unk_110A >= 0x2DU) && ((u8) D_800E6280.unk_110A < 0x31U) && (D_800B593C == 0x80)) {
                    sp30 = var_v0;
                    func_8006B900();
                    var_t8 = D_800E6280.unk_110A * 4;
                    D_80122D44 = 1;
                    goto block_101;
                }
                if ((D_800CA2F8 == 0x11) && ((u8) D_800E6280.unk_110A >= 0x3EU)) {
                    if (((u8) D_800E6280.unk_110A < 0x49U) && (D_800B593C == 0x80)) {
                        sp30 = var_v0;
                        func_8006B900();
                        D_80122D44 = 1;
                        var_a3 = var_v0 + (D_800E6280.unk_110A * 4);
                    } else {
                        goto block_87;
                    }
                } else {
block_87:
                    if ((D_800CA2F8 == 0x10) && ((u8) D_800E6280.unk_110A < 8U) && (D_800B593C == 0x80)) {
                        sp30 = var_v0;
                        func_8006B900();
                        var_t8 = D_800E6280.unk_110A * 4;
                        D_80122D44 = 1;
                        goto block_101;
                    }
                    if ((D_800CA2F8 == 0x10) && ((u8) D_800E6280.unk_110A >= 9U) && ((u8) D_800E6280.unk_110A < 0x13U) && (D_800B593C == 0x80)) {
                        sp30 = var_v0;
                        func_8006B900();
                        D_80122D44 = 1;
                        var_a3 = var_v0 + (D_800E6280.unk_110A * 4);
                    } else if ((D_800CA2F8 == 0x10) && ((u8) D_800E6280.unk_110A >= 0x14U) && ((u8) D_800E6280.unk_110A < 0x22U) && (D_800B593C == 0x80)) {
                        sp30 = var_v0;
                        func_8006B900();
                        var_t8 = D_800E6280.unk_110A * 4;
                        D_80122D44 = 1;
block_101:
                        var_a3 = var_v0 + var_t8;
                    }
                }
            }
        }
    }
    *var_a3(0x80);
    if ((D_800CA2F8 == 0xC) && ((u8) D_800E6280.unk_110A >= 0x45U) && ((u8) D_800E6280.unk_110A < 0x5BU) && (D_80122D34 != 0)) {
        *(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6531)) = (u8) (*(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6531)) | 0x80);
        func_80140D38();
    } else if ((D_800CA2F8 == 0xC) && ((u8) D_800E6280.unk_110A >= 0x5BU)) {
        *(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6531)) = (u8) (*(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6531)) & 0x7F);
    }
    if (D_80122D14 != 0) {
        func_8013FD6C();
    }
    if ((D_800CA2F8 == 1) && (D_800E6280.unk_110A >= 0xCU)) {
        if (D_800E6280.unk_110A < 0x15U) {
            func_801407D0();
        }
    }
    if (D_800CA2F8 == 1) {
        if (D_800E6280.unk_110A == 0x15) {
            func_80140788();
        }
    }
    if ((D_800CA2F8 == 0xB) && (D_800E6280.unk_110A < 9U) && (D_800B3CFC == 1) && (D_800CA2FC != 2)) {
        D_800CA2FC = 2;
        func_80044750(0x601);
    }
}
