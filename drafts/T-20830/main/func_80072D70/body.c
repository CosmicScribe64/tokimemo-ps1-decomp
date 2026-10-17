s32 func_80072D70(void) {
    s32 state;
    s32 lim;
    s32 i;
    s32 k;
    s32 flag;
    s32 start;
    GameState *g = &D_800E6280;

    flag = 0;
    lim = g->unk_118.unk_02;
    lim = lim / 8 + 0xCC;
    if (lim >= 0xFF) {
        lim = 0xFE;
    }
    switch (g->unk_0F4.b[1] & 0xF) {
    case 0:
        state = 0;
        if ((g->unk_0F6.b[0] >> 5) == 0) {
            if ((g->unk_0FC.unk_02 < 0x14) && (lim < (rand() & 0xFF))) {
                state = 1;
                flag = 1;
            } else if (g->unk_0FC.unk_02 == 0) {
                state = 1;
                flag = 1;
            }
            if ((g->unk_11C.unk_02 >= 0x51) && ((u32) lim < (u32) (D_800E7D10 & 0xFF))) {
                state += 2;
                flag = 1;
            }
        }
        if (state == 1) {
            func_80042940(1);
        }
        if (state == 2) {
            func_80042940(2);
        }
        if (state == 3) {
            func_80042940(3);
        }
        break;
    case 1:
        state = 1;
        if ((g->unk_11C.unk_02 >= 0x51) && ((u32) lim < (u32) (D_800E7D10 & 0xFF))) {
            state = 3;
            flag = 1;
        }
        if (g->unk_0FC.unk_02 >= 0x1F) {
            state = 0;
            flag = 1;
        }
        if (state == 3) {
            func_80042940(3);
        }
        if (state == 0) {
            func_80042940(5);
        }
        break;
    case 2:
        state = 2;
        if ((g->unk_0FC.unk_02 < 0x14) && (lim < (rand() & 0xFF))) {
            state = 3;
            flag = 1;
        } else if (g->unk_0FC.unk_02 == 0) {
            state = 3;
            flag = 1;
        }
        if (g->unk_11C.unk_02 < 0x46) {
            state = 0;
            flag = 1;
        }
        if (state == 3) {
            func_80042940(3);
        }
        if (state == 0) {
            func_80042940(6);
        }
        break;
    case 3:
        state = 3;
        if ((g->unk_11C.unk_02 < 0x46) && (g->unk_0FC.unk_02 >= 0x1F)) {
            state = 0;
            flag = 1;
        }
        if (state == 0) {
            func_80042940(7);
            break;
        }
        if (((u8 *) g->unk_044)[g->unk_040] == 0) {
            start = rand() % 11;
            for (i = 0; i != 11; i++) {
                k = (start + i) % 11;
                if (g->unk_1BC[k].unk_0C.f.b1 && (get_h_tokimeki(k) >= 0x33U) && (get_g_zyotai_s(start) != 4)) {
                    if ((rand() & 0xFF) < 0x3F) {
                        g->unk_75D = k;
                        main_state_set(0x62);
                        return 1;
                    }
                }
            }
        }
        break;
    default:
        g->unk_0F4.b[1] &= 0xFFF0;
        break;
    }
    if (flag == 0) {
        func_80042908(0xFF);
    }
}
