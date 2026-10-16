void func_80144E20(void) {
    GameState *g = &D_800E6280;
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
    void (**base)();

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
    base = spDEC.f;
    switch (D_800CA2F8) {
    case 0:
        base = spDEC.f;
        break;
    case 1:
        base = spD60.f;
        break;
    case 2:
        base = spCA0.f;
        break;
    case 3:
        base = spC20.f;
        break;
    case 4:
        base = spB94.f;
        break;
    case 5:
        base = spB0C.f;
        break;
    case 6:
        base = spA7C.f;
        break;
    case 7:
        base = sp9E8.f;
        break;
    case 8:
        base = sp920.f;
        break;
    case 9:
        base = sp884.f;
        break;
    case 10:
        base = sp804.f;
        break;
    case 11:
        base = sp77C.f;
        break;
    case 12:
        base = sp5DC.f;
        break;
    case 13:
        base = sp568.f;
        break;
    case 14:
        base = sp4B4.f;
        break;
    case 15:
        base = sp410.f;
        break;
    case 16:
        base = sp384.f;
        break;
    case 17:
        base = sp254.f;
        break;
    case 18:
        base = sp12C.f;
        break;
    case 19:
        base = spB0.f;
        break;
    case 20:
        base = sp34.f;
        break;
    }
    if (&normal_date_move_place == base[g->unk_110A]) {
        if (D_800E6280.unk_110D == 0) {
            D_8015BF48 = (u8) D_8015BF44;
        }
        if ((D_800CA2F8 != 1) && (D_800CA2F8 != 4) && (D_800CA2F8 != 0xC)) {
            if (((D_800E6280.unk_110D == 0) && (D_8015BF48 == 1)) || ((D_800E6280.unk_110D == 0xE) && (D_8015BF48 == 0))) {
                D_8015BF44 = 1 - D_8015BF48;
            }
        }
        D_800E6280.unk_110D += 1;
    }
    if ((((D_800E6280.unk_03E == 0x5F) && (D_800E6280.unk_03F == 7) && (D_800E6280.unk_040 == 0x17)) || ((D_800E6280.unk_03E == 0x60) && (D_800E6280.unk_03F == 7) && (D_800E6280.unk_040 == 0x1C)) || ((D_800E6280.unk_03E == 0x61) && (D_800E6280.unk_03F == 7) && (D_800E6280.unk_040 == 0x1B))) && (D_800B593C == 0x80) && ((u8) D_8015BF44 != 0) && (func_80146224 != base[g->unk_110A])) {
    func_8006B900();
    D_80122D44 = 1;
    } else if ((D_800CA2F8 == 1) && (D_800CA2FC == 0) && (g->unk_110A >= 0xC) && (g->unk_110A < 0x15) && (D_800E6280.unk_03F >= 3) && (D_800E6280.unk_03F < 9) && (D_800B593C == 0x80)) {
    func_8006B900();
    D_80122D44 = 1;
    } else if ((D_800CA2F8 == 0x14) && (g->unk_110A >= 7) && (g->unk_110A < 0x11) && (D_800B593C == 0x80)) {
    func_8006B900();
    D_80122D44 = 1;
    } else if ((D_800CA2F8 == 0x11) && (g->unk_110A < 8) && (D_800B593C == 0x80)) {
    func_8006B900();
    D_80122D44 = 1;
    } else if ((D_800CA2F8 == 0x11) && (g->unk_110A >= 9) && (g->unk_110A < 0x1E) && (D_800B593C == 0x80)) {
    func_8006B900();
    D_80122D44 = 1;
    } else if ((D_800CA2F8 == 0x11) && (g->unk_110A >= 0x1F) && (g->unk_110A < 0x2C) && (D_800B593C == 0x80)) {
    func_8006B900();
    D_80122D44 = 1;
    } else if ((D_800CA2F8 == 0x11) && (g->unk_110A >= 0x2D) && (g->unk_110A < 0x31) && (D_800B593C == 0x80)) {
    func_8006B900();
    D_80122D44 = 1;
    } else if ((D_800CA2F8 == 0x11) && (g->unk_110A >= 0x3E) && (g->unk_110A < 0x49) && (D_800B593C == 0x80)) {
    func_8006B900();
    D_80122D44 = 1;
    } else if ((D_800CA2F8 == 0x10) && (g->unk_110A < 8) && (D_800B593C == 0x80)) {
    func_8006B900();
    D_80122D44 = 1;
    } else if ((D_800CA2F8 == 0x10) && (g->unk_110A >= 9) && (g->unk_110A < 0x13) && (D_800B593C == 0x80)) {
    func_8006B900();
    D_80122D44 = 1;
    } else if ((D_800CA2F8 == 0x10) && (g->unk_110A >= 0x14) && (g->unk_110A < 0x22) && (D_800B593C == 0x80)) {
    func_8006B900();
    D_80122D44 = 1;
    }
    base[g->unk_110A](0x80);
    if ((D_800CA2F8 == 0xC) && (g->unk_110A >= 0x45) && (g->unk_110A < 0x5B) && (D_80122D34 != 0)) {
        D_80120653 |= 0x80;
        func_80140D38();
    } else if ((D_800CA2F8 == 0xC) && (g->unk_110A >= 0x5B)) {
        D_80120653 &= 0x7F;
    }
    if (D_80122D14 != 0) {
        func_8013FD6C();
    }
    if ((D_800CA2F8 == 1) && (g->unk_110A >= 0xC)) {
        if (g->unk_110A < 0x15) {
            func_801407D0();
        }
    }
    if (D_800CA2F8 == 1) {
        if (g->unk_110A == 0x15) {
            func_80140788();
        }
    }
    if ((D_800CA2F8 == 0xB) && (g->unk_110A < 9) && (D_800B3CFC == 1) && (D_800CA2FC != 2)) {
        D_800CA2FC = 2;
        func_80044750(0x601);
    }
}
