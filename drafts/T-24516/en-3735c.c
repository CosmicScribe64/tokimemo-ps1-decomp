void func_80137C58(void);
void func_80138CCC(void);
void func_8013735C(void) {
    s32 i;
    s32 n;
    DisplayActor44 *actors;

    actors = (DisplayActor44 *)D_8011ECD0;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8006BA40();
        actors[103].unk_26 = actors[0].unk_26;
        actors[103].unk_2A = actors[0].unk_2A;
        if ((D_800E6280.unk_F88 & 0x20) && D_80139C30 != 0) {
            D_80139C30--;
            D_80139C34[D_80139C30] = 1;
            D_80139C3C[D_80139C30] = (D_80139C58 >> 4) + 1;
            actors[D_80139C30 + 98].unk_26 = actors[103].unk_26;
            actors[D_80139C30 + 98].unk_01 = 4;
            actors[D_80139C30 + 98].unk_2A = actors[103].unk_2A;
            func_80044750(0x50C);
        }
        func_80137C58();
        func_80138CCC();
        if (D_80139C30 == 0 && D_80139C34[0] == 0 && D_80139C34[1] == 0 && D_80139C34[2] == 0 && D_80139C34[3] == 0 && D_80139C34[4] == 0) {
            func_8004284C();
        }
        break;
    case 1:
        actors[117].unk_03 |= 0x80;
        n = 0;
        for (i = 0; i < 5; i++) {
            if (D_80139C4C[i] != 0xFF) {
                actors[n + 98].unk_18 = D_80139C4C[i];
                n++;
            }
            actors[i + 98].unk_01 = 1;
        }
        for (i = n; i < 5; i++) {
            actors[i + 98].unk_18 = 0xFF;
        }
        Vblnk_Timer_Init();
        D_80139C30 = 0;
        func_8004284C();
        break;
    case 2:
        if (Vblnk_Timer() >= 0x2E) {
            if (actors[D_80139C30 + 98].unk_18 == 0xFF || D_80139C30 >= 5U) {
                if (D_80139C4C[0] == 0x1F && D_80139C4C[1] == 0x1F && D_80139C4C[2] == 0x1F && D_80139C4C[3] == 0x1F && D_80139C4C[4] == 0x1F) {
                    func_80042940(5);
                } else if (D_80139C4C[0] == 0x20 && D_80139C4C[1] == 0x20 && D_80139C4C[2] == 0x20 && D_80139C4C[3] == 0x20 && D_80139C4C[4] == 0x20) {
                    func_80042940(6);
                } else {
                    Vblnk_Timer_Init();
                    func_8004284C();
                }
                func_80044750(0x50B);
            } else {
                func_80044750(0x50A);
                actors[D_80139C30 + 98].unk_03 = 0x84;
                actors[D_80139C30 + 98].unk_26 = (D_80139C30 << 6) - 0x80;
                actors[D_80139C30 + 98].unk_2A = 0x38;
                D_80139C30++;
                Vblnk_Timer_Init();
            }
        }
        break;
    case 3:
        if (Vblnk_Timer() >= 0x79) {
            if (actors[96].unk_07 < 4U) {
                func_80048EB8(0);
                func_8004284C();
            } else {
                actors[96].unk_07 -= 4;
            }
            actors[97].unk_07 = actors[96].unk_07;
            for (i = 0; i < 20; i++) {
                actors[i + 98].unk_07 = actors[96].unk_07;
            }
        }
        break;
    case 4:
        if (D_80139C4C[0] == 0x10 && D_80139C4C[1] == 0x10 && D_80139C4C[2] == 0x10 && D_80139C4C[3] == 0x10 && D_80139C4C[4] == 0x10 && D_800E6280.unk_F5F == 2) {
            D_80122E9C = 3;
        } else if (D_800E6280.unk_F5F + 0x14 == D_80139C4C[0] && D_800E6280.unk_F5F + 0x14 == D_80139C4C[1] && D_800E6280.unk_F5F + 0x14 == D_80139C4C[2] && D_800E6280.unk_F5F + 0x14 == D_80139C4C[3] && D_800E6280.unk_F5F + 0x14 == D_80139C4C[4] && D_800E6280.unk_F5F != 2) {
            D_80122E9C = 3;
        } else if (D_80139C4C[0] < 0x21 && D_80139C4C[1] < 0x21 && D_80139C4C[2] < 0x21 && D_80139C4C[3] < 0x21 && D_80139C4C[4] < 0x21) {
            D_80122E9C = 1;
        } else {
            D_80122E9C = 4;
        }
        main_state_set(D_800E6280.unk_720);
        func_80042908(D_800E6280.unk_721);
        func_80042940(D_800E6280.unk_722);
        break;
    case 5:
        switch (D_800E6280.unk_110D) {
        case 0:
            func_80046290(0x1A0000B1, 0x170000B7, 0xE);
            func_80044750(0x300);
            D_800E6280.unk_110D += 1;
            break;
        case 1:
            if (func_800460EC() == 4) {
                D_800E6280.unk_110D += 1;
            }
            break;
        case 2:
            if (func_800460EC() == 2) {
                func_80044750(0x74);
                D_800E6280.unk_110D += 1;
            }
            break;
        case 3:
            Vblnk_Timer_Init();
            D_800E6280.unk_110D += 1;
            break;
        case 4:
            if (Vblnk_Timer() >= 0x79) {
                Vblnk_Timer_Init();
                func_80042940(3);
            }
            break;
        default:
            func_80046500();
            break;
        }
        break;
    case 6:
        switch (D_800E6280.unk_110D) {
        case 0:
            func_80046290(0x03000000, 0x01000007, 0xE);
            func_80044750(0x300);
            D_800E6280.unk_110D += 1;
            break;
        case 1:
            if (func_800460EC() == 4) {
                D_800E6280.unk_110D += 1;
            }
            break;
        case 2:
            if (func_800460EC() == 2) {
                func_80044750(0x74);
                D_800E6280.unk_110D += 1;
            }
            break;
        case 3:
            Vblnk_Timer_Init();
            D_800E6280.unk_110D += 1;
            break;
        case 4:
            if (Vblnk_Timer() >= 0x79) {
                Vblnk_Timer_Init();
                func_80042940(3);
            }
            break;
        default:
            func_80046500();
            break;
        }
        break;
    default:
        func_80046500();
        break;
    }
}
