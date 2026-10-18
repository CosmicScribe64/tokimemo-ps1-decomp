u16 func_8011C750(void) {
    EventMovieState *g;
    s16 i;
    s16 v;
    u8 x;
    RECT rect;
    s32 saved[2];
    Rec24 *p;
    EventMovieWork *movie;
    u8 value;
    u32 color;
    u32 timer;

    g = (EventMovieState *)&D_800B1AF0;
    movie = (EventMovieWork *)D_800F1990;
    
    switch (g->unk_00) {
    case 0:
        D_800EED08 = 1;
        func_8011F530(D_800B1AE4);
        func_800433D0(0x207);
        movie->unk_06 = 0;
        movie->unk_02 = 0;
        movie->unk_04 = 0;
        movie->unk_00 = 0;
        movie->unk_19 = 0;
        movie->unk_18 = 0;
        D_80080710 = -1;
        movie->unk_0A = 0;
        movie->unk_1A = 0;
        func_800352B4(0);
        D_801258E0 = 2;
        D_800B1AF9 = 0;
        D_801258DC = 0; for (i = 0; i < 2; i++) { /* FAKE: one source line keeps the as1 store order (as in func_801348F0) T-24018 */
            ((Rec24 *)D_800EC190)[i + 4].unk_08 = (i << 6) + 0x80;
            ((Rec24 *)D_800EC190)[i + 4].unk_00 = 0x82000000;
            ((Rec24 *)D_800EC190)[i + 4].unk_0A = 0;
            ((Rec24 *)D_800EC190)[i + 4].unk_0E = 0;
            ((Rec24 *)D_800EC190)[i + 4].unk_0F = 0xA0;
            ((Rec24 *)D_800EC190)[i + 4].unk_10 = 0;
            ((Rec24 *)D_800EC190)[i + 4].unk_0C = (i * 2) + 0x15;
            ((Rec24 *)D_800EC190)[i + 4].unk_12 = 0x1E0;
            ((Rec24 *)D_800EC190)[i + 4].unk_14 = 0;
            ((Rec24 *)D_800EC190)[i + 4].unk_15 = 0;
            ((Rec24 *)D_800EC190)[i + 4].unk_16 = 0;
            ((Rec24 *)D_800EC190)[i + 4].unk_18 = 0;
            ((Rec24 *)D_800EC190)[i + 4].unk_1A = 0;
            ((Rec24 *)D_800EC190)[i + 4].unk_1C = 0x1000;
            ((Rec24 *)D_800EC190)[i + 4].unk_1E = 0x1000;
            ((Rec24 *)D_800EC190)[i + 4].unk_20 = 0;
            ((Rec24 *)D_800EC190)[i + 4].unk_04 = (i << 7) - 0xA0;
            ((Rec24 *)D_800EC190)[i + 4].unk_06 = -0x78;
            ((EventEffectTables *)D_800B0860)->unk_1031[i] = 0xB;
        }
        for (i = 0; i < 2; i++) {
            ((Rec24 *)D_800EC190)[i + 6].unk_08 = (i << 6) + 0x80;
            ((Rec24 *)D_800EC190)[i + 6].unk_00 = 0x82000000;
            ((Rec24 *)D_800EC190)[i + 6].unk_0A = 0xA0;
            ((Rec24 *)D_800EC190)[i + 6].unk_0E = 0;
            ((Rec24 *)D_800EC190)[i + 6].unk_0F = 0;
            ((Rec24 *)D_800EC190)[i + 6].unk_10 = 0;
            ((Rec24 *)D_800EC190)[i + 6].unk_0C = (i * 2) + 0xA;
            ((Rec24 *)D_800EC190)[i + 6].unk_12 = 0x1E0;
            ((Rec24 *)D_800EC190)[i + 6].unk_14 = 0;
            ((Rec24 *)D_800EC190)[i + 6].unk_15 = 0;
            ((Rec24 *)D_800EC190)[i + 6].unk_16 = 0;
            ((Rec24 *)D_800EC190)[i + 6].unk_18 = 0;
            ((Rec24 *)D_800EC190)[i + 6].unk_1A = 0;
            ((Rec24 *)D_800EC190)[i + 6].unk_1C = 0x1000;
            ((Rec24 *)D_800EC190)[i + 6].unk_1E = 0x1000;
            ((Rec24 *)D_800EC190)[i + 6].unk_20 = 0;
            ((Rec24 *)D_800EC190)[i + 6].unk_04 = (i << 7) - 0xA0;
            ((Rec24 *)D_800EC190)[i + 6].unk_06 = -0x78;
            ((EventEffectTables *)D_800B0860)->unk_1031[i + 2] = 0xB;
        }
        func_8002746C(0x433B, 1);
        g->unk_00 += 1;
        break;
    case 1:
        if (func_80015A44() & 1) {
            func_8002469C(0x140, 0x100, 0x180, 0xA0);
            rect.x = 0x140;
            rect.y = 0x100;
            rect.w = 0x180;
            rect.h = 0xA0;
            func_800651F8(&rect, (void *)0x80183000);
            g->unk_00 += 1;
        }
        break;
    case 2:
        func_8002746C(0x4344, 1);
        g->unk_00 += 1;
        break;
    case 3:
        if (func_80015A44() & 1) {
            func_8002469C(0x280, 0, 0x180, 0xA0);
            rect.x = 0x280;
            rect.y = 0;
            rect.w = 0x180;
            rect.h = 0xA0;
            func_800651F8(&rect, (void *)0x801A1000);
            g->unk_00 += 1;
        }
        break;
    case 4:
        func_8002746C(0x434E, 1);
        g->unk_00 += 1;
        break;
    case 5:
        if (func_80015A44() & 1) {
            func_8002469C(0x140, 0x100, 0x180, 0xA0);
            rect.x = 0x140;
            rect.y = 0x100;
            rect.w = 0x180;
            rect.h = 0xA0;
            func_800651F8(&rect, (void *)0x801BF000);
            g->unk_00 += 1;
        }
        break;
    case 6:
        func_8002746C(0x4357, 1);
        g->unk_00 += 1;
        break;
    case 7:
        if (func_80015A44() & 1) {
            func_8002469C(0x280, 0, 0x180, 0xA0);
            rect.x = 0x280;
            rect.y = 0;
            rect.w = 0x180;
            rect.h = 0xA0;
            func_800651F8(&rect, (void *)0x801DD000);
            g->unk_00 += 1;
        }
        break;
    case 8:
        for (i = 0; i < 4; i++) {
            ((Rec24 *)D_800EC190)[i + 4].unk_00 = 0x02000000;
        }
        g->unk_00 += 1;
        break;
    case 9:
        value = ((Rec24 *)D_800EC190)[4].unk_14;
        value += 2;
        color = (value & 0xFF);
        ((Rec24 *)D_800EC190)[7].unk_16 = color;
        ((Rec24 *)D_800EC190)[7].unk_15 = color;
        ((Rec24 *)D_800EC190)[7].unk_14 = color;
        ((Rec24 *)D_800EC190)[6].unk_16 = color;
        ((Rec24 *)D_800EC190)[6].unk_15 = color;
        ((Rec24 *)D_800EC190)[6].unk_14 = color;
        ((Rec24 *)D_800EC190)[5].unk_16 = color;
        ((Rec24 *)D_800EC190)[5].unk_15 = color;
        ((Rec24 *)D_800EC190)[5].unk_14 = color;
        ((Rec24 *)D_800EC190)[4].unk_16 = color;
        ((Rec24 *)D_800EC190)[4].unk_15 = color;
        ((Rec24 *)D_800EC190)[4].unk_14 = color;
        if (value == 0x80) {
            g->unk_00 += 1;
        }
        break;
    case 10:
        break;
    case 11:
        value = ((Rec24 *)D_800EC190)[4].unk_14;
        value -= 2;
        color = (value & 0xFF);
        ((Rec24 *)D_800EC190)[7].unk_16 = color;
        ((Rec24 *)D_800EC190)[7].unk_15 = color;
        ((Rec24 *)D_800EC190)[7].unk_14 = color;
        ((Rec24 *)D_800EC190)[6].unk_16 = color;
        ((Rec24 *)D_800EC190)[6].unk_15 = color;
        ((Rec24 *)D_800EC190)[6].unk_14 = color;
        ((Rec24 *)D_800EC190)[5].unk_16 = color;
        ((Rec24 *)D_800EC190)[5].unk_15 = color;
        ((Rec24 *)D_800EC190)[5].unk_14 = color;
        ((Rec24 *)D_800EC190)[4].unk_16 = color;
        ((Rec24 *)D_800EC190)[4].unk_15 = color;
        ((Rec24 *)D_800EC190)[4].unk_14 = color;
        if (value == 0) {
            for (i = 0; i < 4; i++) {
                ((Rec24 *)D_800EC190)[i + 4].unk_00 = 0x82000000;
            }
            g->unk_00 += 1;
        }
        break;
    case 12:
        saved[1] = D_800B1AF6;
        saved[0] = g->unk_00;
        func_8004AFB8();
        g->unk_00 = saved[0];
        if (saved[1] != D_800B1AF6) {
            D_800B1AF6--;
            g->unk_00 += 1;
        }
        break;
    case 13:
    case 16:
    case 19:
    case 21:
    case 23:
        if (func_80015A44() & 1) {
            g->unk_00 += 1;
        }
        break;
    case 14:
        saved[0] = g->unk_00;
        func_8004A9CC();
        D_800B1AF6 -= 1;
        g->unk_00 = saved[0] + 1;
        break;
    case 15:
        switch (*(u32 *)&D_80125320) {
        case 0:
            func_800469F4(0x42AE);
            break;
        case 1:
            func_800469F4(0x42B7);
            break;
        case 2:
            func_800469F4(0x42C0);
            break;
        case 3:
            func_800469F4(0x42C9);
            break;
        case 4:
            func_800469F4(0x42D2);
            break;
        }
        g->unk_00 += 1;
        break;
    case 17:
        func_8002755C(1);
        func_80027438(1);
        g->unk_00 += 1;
        break;
    case 18:
        saved[0] = g->unk_00;
        func_8011A62C();
        g->unk_06 -= 1;
        g->unk_00 = saved[0] + 1;
        break;
    case 20:
        saved[0] = g->unk_00;
        func_8011A7FC();
        g->unk_06 -= 1;
        g->unk_00 = saved[0] + 1;
        break;
    case 22:
        saved[0] = g->unk_00;
        func_8011A838();
        g->unk_06 -= 1;
        g->unk_00 = saved[0] + 1;
        break;
    default:
        func_800352B4(1);
        D_800EED08 = 0;
        func_800433D0(0x206);
        func_80011DFC();
        break;
    }
    if (D_800B1AF0 < 9U || D_800B1AF0 >= 12U) {
        return 0;
    }
    p = ((Rec24 *)D_800EC190);
    v = p[4].unk_0E;
    v += D_801258E0;
    if (v < 0) {
        v = 0;
        D_801258E0 = (func_800789A0() % 4) + 2;
    }
    if (v >= 0x40) {
        v = 0x3F;
        D_801258E0 = -2 - (func_800789A0() % 4);
    }
    x = p[4].unk_0F;
    p[7].unk_0E = v;
    p[6].unk_0E = v;
    p[5].unk_0E = v;
    p[4].unk_0E = v;
    if (x != 0) {
        p[4].unk_0A += 0x14;
        p[5].unk_0A += 0x14;
        x -= 0x14;
        p[5].unk_0F -= 0x14;
    } else {
        p[4].unk_0A -= 0x14;
        p[4].unk_06 += 0x14;
        p[5].unk_0A -= 0x14;
        p[5].unk_06 += 0x14;
    }
    if (p[6].unk_0F != 0) {
        p[6].unk_0A += 0x14;
        p[7].unk_0A += 0x14;
        p[6].unk_0F -= 0x14;
        p[7].unk_0F -= 0x14;
    } else {
        p[6].unk_0A -= 0x14;
        p[6].unk_06 += 0x14;
        p[7].unk_0A -= 0x14;
        p[7].unk_06 += 0x14;
    }
    p[4].unk_0F = x;
    if ((x == 0 && (g->unk_09 == 1 || g->unk_09 == 3)) || (p[6].unk_0F == 0 && (g->unk_09 == 0 || g->unk_09 == 2))) {
        switch (D_800B1AF9) { /* FAKE: g-> in the test, the symbol here; one access path keeps the byte in $v0 and drops the reload T-24018 */
        case 0:
            for (i = 0; i < 2; i++) {
                ((Rec24 *)D_800EC190)[i + 4].unk_0A = 0;
                ((Rec24 *)D_800EC190)[i + 4].unk_0F = 0xA0;
                ((Rec24 *)D_800EC190)[i + 4].unk_06 = -0x78;
                ((Rec24 *)D_800EC190)[i + 4].unk_0C = (i * 2) + 0x15;
            }
            func_800137AC(0x140, 0x100, 0x180, 0xA0, 0x80183000);
            break;
        case 1:
            for (i = 0; i < 2; i++) {
                ((Rec24 *)D_800EC190)[i + 6].unk_0A = 0;
                ((Rec24 *)D_800EC190)[i + 6].unk_0F = 0xA0;
                ((Rec24 *)D_800EC190)[i + 6].unk_06 = -0x78;
                ((Rec24 *)D_800EC190)[i + 6].unk_0C = (i * 2) + 0xA;
            }
            func_800137AC(0x280, 0, 0x180, 0xA0, 0x801A1000);
            break;
        case 2:
            for (i = 0; i < 2; i++) {
                ((Rec24 *)D_800EC190)[i + 4].unk_0A = 0;
                ((Rec24 *)D_800EC190)[i + 4].unk_0F = 0xA0;
                ((Rec24 *)D_800EC190)[i + 4].unk_06 = -0x78;
                ((Rec24 *)D_800EC190)[i + 4].unk_0C = (i * 2) + 0x15;
            }
            func_800137AC(0x140, 0x100, 0x180, 0xA0, 0x801BF000);
            break;
        case 3:
            for (i = 0; i < 2; i++) {
                ((Rec24 *)D_800EC190)[i + 6].unk_0A = 0;
                ((Rec24 *)D_800EC190)[i + 6].unk_0F = 0xA0;
                ((Rec24 *)D_800EC190)[i + 6].unk_06 = -0x78;
                ((Rec24 *)D_800EC190)[i + 6].unk_0C = (i * 2) + 0xA;
            }
            func_800137AC(0x280, 0, 0x180, 0xA0, 0x801DD000);
            break;
        }
        if (++D_800B1AF9 >= 4) {
            D_800B1AF9 = 0;
        }
    }
    timer = (D_801258DC + 1) & 0xFFFF;
    D_801258DC = timer;
    if (timer == 0x12C) {
        D_800B1AF0 += 1;
    }
}