/* T-23003 hand draft of main:func_8007132C (4,292 B), first typed form (GameState fields, no gs pointer: the original keeps
 * every access absolute). NOT close: 976 of 1073 instructions, 166 structurally differing, the u8 compare chain on
 * +0x3E/+0x3F/+0x40 is CSE'd into one register where the original reloads each byte. Not built (src keeps INCLUDE_ASM). */
void func_8007132C(void) {
    GsRec69C *p;
    s32 op;
    s32 t;
    s32 i;
    s32 n;

    func_80071280();
    func_80071038();
    if (D_800E6280.unk_03E >= 0x62U && D_800E6280.unk_03F >= 3U && D_800E6280.unk_040 != 0) {
        func_8004284C();
        return;
    }
    if (D_800E6280.unk_03E >= 0x62U && D_800E6280.unk_03F == 2 && D_800E6280.unk_040 == 0x1A) {
        n = 0;
        if (D_800E6280.unk_56C[82] == 0) {
            for (i = 1; i < 10; i++) {
                if (D_800E6280.unk_1BC[i].unk_0C.f.b1) {
                    n++;
                }
            }
            if (n >= 6) {
                D_800E6280.unk_71C -= 1;
                D_80122CD4 = 0xD;
                D_80122CD0 = 0;
                main_state_set(0x5D);
                return;
            }
        }
    }
    if (D_800E6280.unk_03E >= 0x62U && D_800E6280.unk_03F == 2 && D_800E6280.unk_040 == 0x1C && func_80071110() != 0 && D_800E6280.unk_56C[20] == 0) {
        D_80122CD4 = 2;
        D_80122CD0 = 7;
        D_800E6280.unk_56C[20] += 1;
        main_state_set(0x5D);
        return;
    }
    func_8006D138();
    func_8006D00C();
    if ((D_800E6280.unk_03F != 0xC || D_800E6280.unk_040 != 0x18) && D_800E6280.unk_71D == 0 && D_800E6280.unk_041 != 6
        && D_800E6280.unk_044[0].unk_00[D_800E6280.unk_040] == 0 && D_800E6280.unk_69C[D_800E6280.unk_71C].unk_00 == 0xFB
        && D_800E6280.unk_69C[0].unk_00 != 0x40 && D_800E6280.unk_69C[D_800E6280.unk_71C - 1].unk_00 == 0) {
        for (i = 0; i < 11; i++) {
            if (D_800E6280.unk_1BC[i].unk_0C.f.b1 && D_800E6280.unk_1BC[i].unk_0A >= 0x4C && !(D_800E6280.unk_1BC[i].unk_0C.h[1] & 1)) {
                D_800E6280.unk_1BC[i].unk_0C.b[2] |= 1;
                if (D_800E6280.unk_71C != 0) {
                    D_800E6280.unk_71C -= 1;
                }
                D_800E6280.unk_71D += 1;
                D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01 = i;
                main_state_set(0x59);
                return;
            }
        }
    }
    p = &D_800E6280.unk_69C[D_800E6280.unk_71C];
    op = p->unk_00;
    if (D_800E6280.unk_69C[0].unk_00 == 0x25 && p[-1].unk_00 == 0x25) {
        if (op == 0x32) {
            D_800E6280.unk_71C += 1;
            p = &D_800E6280.unk_69C[D_800E6280.unk_71C];
            op = p->unk_00;
        }
        if (op == 0x33) {
            D_800E6280.unk_71C += 1;
            p = &D_800E6280.unk_69C[D_800E6280.unk_71C];
            op = p->unk_00;
        }
    }
    if (op == 0x32) {
        t = p->unk_01;
        if (t == 0 && (D_800E6280.unk_71E & 1)) {
            D_800E6280.unk_71C += 1;
            p = &D_800E6280.unk_69C[D_800E6280.unk_71C];
            t = p->unk_01;
        }
        if (t != 0 && (D_800E6280.unk_71E & 2)) {
            D_800E6280.unk_71C += 1;
            p = &D_800E6280.unk_69C[D_800E6280.unk_71C];
        }
    }
    op = p->unk_00;
    if (op == 0x33 && (D_800E6280.unk_71E & 4)) {
        D_800E6280.unk_71C += 1;
        op = D_800E6280.unk_69C[D_800E6280.unk_71C].unk_00;
    }
    if (op == 0xFA) {
        D_800E6280.unk_71C += 1;
        op = D_800E6280.unk_69C[D_800E6280.unk_71C].unk_00;
    }
    if (op == 0xFB || op == 0xFF) {
        func_8006B0C8();
        if (appointment_find_today(D_800E6280.unk_03E, D_800E6280.unk_03F, D_800E6280.unk_040) > 0) {
            if (get_g_zyotai_s((u32)(D_800E6280.unk_54C[0].w << 3) >> 28) == 4) {
                D_800E6280.unk_54C[0].b[3] &= 0xFFDF;
                D_800E6280.unk_71F = 1;
                main_state_set(0x43);
                func_80042908(2);
                return;
            }
            if (D_800E6280.unk_724 != D_800E6280.unk_725) {
                D_800E6280.unk_54C[0].b[3] &= 0xFFDF;
                D_800E6280.unk_71F = 1;
                main_state_set(0x43);
                func_80042908(0);
                return;
            }
            if (D_800E6280.unk_54C[0].w & 0x20000000) {
                D_800E6280.unk_54C[0].b[3] &= 0xFFDF;
                D_800E6280.unk_71F = 1;
                main_state_set(0x43);
                func_80042908(1);
                return;
            }
        }
    }
    if (D_800E6280.unk_69C[D_800E6280.unk_71C].unk_00 == 0xFF) {
        t = D_800E6280.unk_0F4.h >> 12;
        if (t >= 6 && t < 0xB) {
            if ((D_800E6280.unk_040 + 6) / 7 == 3 && (D_800E6280.unk_F68.b[0] >> 4) != 4) {
                main_state_set(0x43);
                func_80042908(8);
                return;
            }
        }
        if (t < 6 && D_800E6280.unk_03F == 0xA
            && ((D_800E6280.unk_03E == 0x5F && D_800E6280.unk_040 == 8) || (D_800E6280.unk_03E == 0x60 && D_800E6280.unk_040 == 6)
                || (D_800E6280.unk_03E == 0x61 && D_800E6280.unk_040 == 5))) {
            main_state_set(0x43);
            func_80042908(9);
            return;
        }
        if (D_800E6280.unk_03E == 0x5F && D_800E6280.unk_03F == 0xA && D_800E6280.unk_040 == 0xF) {
            for (i = 0; i < 11; i++) {
                if (D_800E6280.unk_1BC[i].unk_0C.f.b1 && D_800E6280.unk_1BC[i].unk_0C.f.b12) {
                    if (get_g_zyotai_s(i) != 4 && func_8006B014(i) == 0) {
                        D_800E6280.unk_71F = 1;
                        main_state_set(0x43);
                        func_80042908(6);
                        return;
                    }
                }
            }
        }
        if (D_800E6280.unk_03E == 0x60 && D_800E6280.unk_03F == 0xA && D_800E6280.unk_040 == 0xD) {
            for (i = 0; i < 11; i++) {
                if (D_800E6280.unk_1BC[i].unk_0C.f.b1 && D_800E6280.unk_1BC[i].unk_0C.f.b13) {
                    if (get_g_zyotai_s(i) != 4 && func_8006B014(i) == 0) {
                        D_800E6280.unk_71F = 1;
                        main_state_set(0x43);
                        func_80042908(7);
                        return;
                    }
                }
            }
        }
    }
    p = &D_800E6280.unk_69C[D_800E6280.unk_71C];
    op = p->unk_00;
    n = 0;
    if (op == 0xFB) {
        for (i = 0; i < 11; i++) {
            if (D_800E6280.unk_1BC[i].unk_0C.f.b1 && D_800E6280.unk_1BC[i].unk_0A >= 0x3D && !D_800E6280.unk_1BC[i].unk_0C.f.b17) {
                n++;
                D_800E6280.unk_1BC[i].unk_0C.b[2] |= 2;
            }
        }
        if (n != 0) {
            main_state_set(0x44);
            return;
        }
        if (D_800E6280.unk_71F == 0) {
            if (D_800E6280.unk_03E == 0x61 && D_800E6280.unk_03F == 7 && D_800E6280.unk_040 == 0x1A) {
                D_800E6280.unk_71F = 1;
                main_state_set(0x43);
                func_80042908(5);
                return;
            }
            if (D_800E6280.unk_03D == 0 && D_800E6280.unk_03E != 0x62) {
                for (i = 0; i < 11; i++) {
                    if (D_800E6280.unk_1BC[i].unk_0C.f.b1) {
                        if (D_800E6280.unk_1BC[i].unk_0A >= 0x5B) {
                            func_80044750(0x50E);
                            D_800E6280.unk_71F = 1;
                            main_state_set(0x43);
                            func_80042908(4);
                            return;
                        }
                    }
                }
            }
            if (D_800E6280.unk_1BC[11].unk_0C.f.f9 != 0 && D_800E6280.unk_56C[77] == 0
                && (D_800E6280.unk_1BC[11].unk_0C.b[3] < 7U || (D_800E6280.unk_042 >= 0x2DBU && D_800E6280.unk_1BC[11].unk_0C.b[3] < 9U))
                && !(rand() & 0x7F)) {
                D_800E6280.unk_71F = 1;
                main_state_set(0x43);
                func_80042908(3);
                return;
            }
        }
        D_800E6280.unk_71F = 0;
        D_800E6280.unk_71C += 1;
        func_80072338();
        p = &D_800E6280.unk_69C[D_800E6280.unk_71C];
        op = p->unk_00;
    }
    switch (op) {
    case 0x0:
        for (i = 0; i != 0xE; i++) {
            if (i != D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01) {
                icon_can_use_set(i, 0);
            } else {
                icon_can_use_set(i, 1);
            }
        }
        main_state_set(0x40);
        return;
    case 0x10:
        main_state_set(0x46);
        return;
    case 0xFE:
        main_state_set(0x38);
        return;
    case 0xFD:
        func_8006CF80();
        main_state_set(0x39);
        return;
    case 0x20:
        func_8006CF80();
        bzero(D_80122DD0, 0x100);
        main_state_set(0x72);
        return;
    case 0x24:
        func_8006CF80();
        bzero(D_80122DD0, 0x100);
        if ((D_800E6280.unk_0F4.h >> 12) < 6U) {
            main_state_set(0x70);
            return;
        }
        main_state_set(0x71);
        return;
    case 0x28:
        func_8006CF80();
        main_state_set(0x42);
        return;
    case 0x2C:
        func_8006CF80();
        main_state_set(0x41);
        return;
    case 0x31:
        main_state_set(0x54);
        return;
    case 0x30:
        main_state_set(0x55);
        return;
    case 0x40:
        func_8006CF80();
        main_state_set(0x35);
        return;
    case 0x41:
        func_8006CF80();
        main_state_set(0x51);
        return;
    case 0x42:
        main_state_set(0x5F);
        return;
    case 0x43:
        func_8006CF80();
        main_state_set(0x36);
        return;
    case 0x44:
        D_800E6280.unk_71D += 1;
        if ((D_800E6280.unk_0F4.b[1] & 0xF) == 3) {
            func_80072B5C(0);
            return;
        }
        op = D_800E6280.unk_69C[D_800E6280.unk_71C - 1].unk_00;
        if (op == 0x28) {
            func_80072B5C(0);
            return;
        }
        if (op == 0x47) {
            func_80072B5C(0);
            return;
        }
        if (!D_800E6280.unk_1BC[p->unk_01].unk_0C.f.b1) {
            func_80072B5C(0);
            return;
        }
        main_state_set(0x59);
        return;
    case 0xFC:
        main_state_set(0x32);
        return;
    case 0x32:
        main_state_set(0x57);
        return;
    case 0x33:
        main_state_set(0x56);
        return;
    case 0x25:
        main_state_set(0x74);
        return;
    case 0x26:
        main_state_set(0x75);
        return;
    case 0x27:
        main_state_set(0x76);
        return;
    case 0x46:
        D_800E6280.unk_71D += 1;
        if (((ScheduleClubFlags *)&D_800E6280.unk_54C[0])->b29) {
            func_80072B5C(0);
            return;
        }
        main_state_set(0x5A);
        return;
    case 0x45:
        D_80122CD4 = p->unk_01;
        D_80122CD0 = D_800E6280.unk_69C[D_800E6280.unk_71C].unk_02;
        main_state_set(0x5D);
        return;
    case 0x47:
        D_800E6280.unk_71D += 1;
        main_state_set(0x61);
        return;
    case 0x48:
        D_800E6280.unk_F5F = p->unk_01;
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 = 0x32;
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0A = 0;
        D_80122CD0 = D_800E6280.unk_69C[D_800E6280.unk_71C].unk_02;
        main_state_set(0x5B);
        return;
    case 0x49:
        main_state_set(0x58);
        return;
    case 0x4A:
        main_state_set(0x37);
        return;
    default:
        main_state_set(0x34);
        return;
    }
}
