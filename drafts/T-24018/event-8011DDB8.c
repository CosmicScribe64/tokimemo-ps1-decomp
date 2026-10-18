s32 func_8011DDB8(void) {
    EventMovieState *g;
    s32 current;
    s32 sp54;
    u32 sp50;
    RECT rect;
    EventMovieWork *movie;
    /* FAKE: unused view reserves the observed frame slot. T-24018. */
    EventMovieWork *header;
    register u8 c;
    register u8 x;
    u8 value;
    u32 color;
    s16 var_v1;

    g = (EventMovieState *)&D_800B1AF0;
    movie = (EventMovieWork *)D_800F1990;

    switch (g->unk_00) {
    case 0:
        D_800EED08 = 1;
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
        D_801258DC = 0;
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_08 = (var_v1 << 6) + 0x80;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_00 = 0x82000000;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_0A = 0xa0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_0E = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_0F = 0x0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_10 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_0C = (var_v1 * 2) + 0x15;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_12 = 0x1E0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_14 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_15 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_16 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_18 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_1A = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_1C = 0x1000;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_1E = 0x1000;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_20 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_04 = (var_v1 << 7) - 0xA0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_06 = -0x78;
            ((EventEffectTables *)D_800B0860)->unk_1031[var_v1] = 0xb;
        }
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_08 = (var_v1 << 6) + 0x80;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_00 = 0x82000000;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_0A = 0x0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_0E = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_0F = 0xa0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_10 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_0C = (var_v1 * 2) + 0xa;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_12 = 0x1E0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_14 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_15 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_16 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_18 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_1A = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_1C = 0x1000;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_1E = 0x1000;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_20 = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_04 = (var_v1 << 7) - 0xA0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_06 = -0x78;
            ((EventEffectTables *)D_800B0860)->unk_1031[var_v1 + 2] = 0xb;
        }
        func_8002746C(0x4321, 1);
        g->unk_00 += 1;
        return;
    case 1:
    case 4:
    case 7:
    case 10:
    case 13:
    case 16:
    case 19:
    case 27:
    case 41:
    case 44:
    case 47:
    case 49:
        if (func_80015A44() & 1) {
            g->unk_00 += 1;
            return;
        }
        return;
    case 2:
        func_8002469C(0x140, 0x100, 0x140, 0xA0);
        rect.x = 0x140;
        rect.y = 0x100;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_800651F8(&rect, (void *)0x80183000);
        g->unk_00 += 1;
        return;
    case 3:
        func_8002746C(0x431A, 1);
        g->unk_00 += 1;
        return;
    case 5:
        func_8002469C(0x140, 0x100, 0x140, 0xA0);
        rect.x = 0x140;
        rect.y = 0x100;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_800651F8(&rect, (void *)0x8019C000);
        g->unk_00 += 1;
        return;
    case 6:
        func_8002746C(0x4313, 1);
        g->unk_00 += 1;
        return;
    case 8:
        func_8002469C(0x140, 0x100, 0x140, 0xA0);
        rect.x = 0x140;
        rect.y = 0x100;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_800651F8(&rect, (void *)0x801B5000);
        g->unk_00 += 1;
        return;
    case 9:
        func_8002746C(0x430C, 1);
        g->unk_00 += 1;
        return;
    case 11:
        func_8002469C(0x140, 0x100, 0x140, 0xA0);
        rect.x = 0x140;
        rect.y = 0x100;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_800651F8(&rect, (void *)0x801CE000);
        g->unk_00 += 1;
        return;
    case 12:
        func_8002746C(0x4305, 1);
        g->unk_00 += 1;
        return;
    case 14:
        func_8002469C(0x140, 0, 0x140, 0xA0);
        g->unk_00 += 1;
        return;
    case 15:
        func_8002746C(0x42ED, 1);
        g->unk_00 += 1;
        return;
    case 17:
        func_8002469C(0x140, 0x100, 0x140, 0xA0);
        g->unk_00 += 1;
        return;
    case 18:
        func_8002746C(0x42F5, 1);
        g->unk_00 += 1;
        return;
    case 20:
        func_8002469C(0x280, 0, 0x140, 0xA0);
        g->unk_00 += 1;
        return;
    case 21:
        for (var_v1 = 0; var_v1 < 4; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_00 = (s32) (((Rec24 *)D_800EC190)[var_v1 + 4].unk_00 & 0x7FFFFFFF);
        }
        g->unk_00++;
        return;
    case 22:
        sp50 = D_800B1AF0;
        x = ((Rec24 *)D_800EC190)[4].unk_14 + 2;
        c = x;
        ((Rec24 *)D_800EC190)[4].unk_15 =
            ((Rec24 *)D_800EC190)[4].unk_16 =
            ((Rec24 *)D_800EC190)[5].unk_14 =
            ((Rec24 *)D_800EC190)[5].unk_15 =
            ((Rec24 *)D_800EC190)[5].unk_16 =
            ((Rec24 *)D_800EC190)[6].unk_14 =
            ((Rec24 *)D_800EC190)[6].unk_15 =
            ((Rec24 *)D_800EC190)[6].unk_16 =
            ((Rec24 *)D_800EC190)[7].unk_14 =
            ((Rec24 *)D_800EC190)[7].unk_15 =
            ((Rec24 *)D_800EC190)[7].unk_16 =
            ((Rec24 *)D_800EC190)[4].unk_14 =
            c;
        D_800B1AF0 = sp50;
        if (c == 0x80) {
            D_800B1AF0 = sp50 + 1;
            return func_800433D0(0x500);
        }
        break;
    case 23:
        sp50 = D_800B1AF0;
        D_800B1AF0 = sp50;
        if (!(D_801258DC & 7)) {
            func_8011ED50(4);
        }
        if (++D_801258DC == 0x140) {
            D_801258DC = 0;
            D_800B1AF0 += 1;
            break;
        }
        break;
    case 24:
        g->unk_00++;
        return;
    case 25:
        func_80012D40(0);
        for (var_v1 = 0; var_v1 < 4; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_00 = (s32) (((Rec24 *)D_800EC190)[var_v1 + 4].unk_00 | 0x80000000);
        }
        g->unk_00 += 1;
        return;
    case 26:
        func_8002746C(0x42FD, 1);
        g->unk_00 += 1;
        return;
    case 28:
        func_8002469C(0x140, 0x100, 0x140, 0xA0);
        rect.x = 0x140;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_80065234(&rect, 0x280, 0);
        g->unk_00 += 1;
        return func_80012D40(1);
    case 29:
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_00 = 0x02000000;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_0A = 0xA0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_0F = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_06 = -0x78;
        }
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_00 = 0x02000000;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_0A = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_0F = 0xA0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_06 = -0x78;
        }
        D_801258DC = 0;
        func_800433D0(0x501);
        D_800B1AF0 += 1;
        break;
    case 30:
    case 32:
    case 34:
        func_8011ED50(8);
        if (++D_801258DC == 0x14) {
            g->unk_00 += 1;
            return;
        }
        break;
    case 31:
        rect.x = 0x280;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_80065234(&rect, 0x140, 0x100);
        func_800137AC(0x280U, 0U, 0x140U, 0xA0U, 0x801CE000);
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_0A = 0xA0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_0F = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_06 = -0x78;
        }
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_0A = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_0F = 0xA0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_06 = -0x78;
        }
        D_801258DC = 0;
        g->unk_00 += 1;
        return;
    case 33:
        rect.x = 0x280;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_80065234(&rect, 0x140, 0x100);
        func_800137AC(0x280U, 0U, 0x140U, 0xA0U, 0x801B5000);
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_0A = 0xA0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_0F = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_06 = -0x78;
        }
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_0A = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_0F = 0xA0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_06 = -0x78;
        }
        D_801258DC = 0;
        g->unk_00 += 1;
        return;
    case 35:
        rect.x = 0x280;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_80065234(&rect, 0x140, 0x100);
        func_800137AC(0x280U, 0U, 0x140U, 0xA0U, 0x8019C000);
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_0A = 0xA0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_0F = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_06 = -0x78;
        }
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_0A = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_0F = 0xA0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_06 = -0x78;
        }
        D_801258DC = 0;
        g->unk_00 += 1;
        return;
    case 36:
        func_8011ED50(8);
        if ((u16) D_801258DC >= 8U) {
            value = ((Rec24 *)D_800EC190)[4].unk_14;
        value -= 4;
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
        }
        if (++D_801258DC == 0x14) {
            g->unk_00 += 1;
            return;
        }
        break;
    case 37:
        rect.x = 0x280;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_80065234(&rect, 0x140, 0x100);
        func_800137AC(0x280U, 0U, 0x140U, 0xA0U, 0x80183000);
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_0A = 0xA0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_0F = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_06 = -0x78;
        }
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_0A = 0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_0F = 0xA0;
            ((Rec24 *)D_800EC190)[var_v1 + 6].unk_06 = -0x78;
        }
        D_801258DC = 0;
        g->unk_00 += 1;
        return;
    case 38:
        func_8011ED50(8);
        value = ((Rec24 *)D_800EC190)[4].unk_14;
        value -= 4;
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

        if (value < 4U) {
            g->unk_00 += 1;
            return;
        }
        break;
    case 39:
        for (var_v1 = 0; var_v1 < 4; var_v1++) {
            ((Rec24 *)D_800EC190)[var_v1 + 4].unk_00 = (s32) (((Rec24 *)D_800EC190)[var_v1 + 4].unk_00 | 0x80000000);
        }
        g->unk_00++;
        return;
    case 40:
        sp50 = D_800B1AF0;
        D_800B1AF0 = sp50;
        sp54 = D_800B1AF6;
        func_8004AFB8();
        D_800B1AF0 = sp50;
        if (sp54 != D_800B1AF6) {
            D_800B1AF6 -= 1;
            D_800B1AF0 = sp50 + 1;
            break;
        }
        break;
    case 42:
        sp50 = D_800B1AF0;
        D_800B1AF0 = sp50;
        func_8004A9CC();
        D_800B1AF6 -= 1;
        D_800B1AF0 = sp50 + 1;
        break;
    case 43:
        sp50 = D_800B1AF0;
        D_800B1AF0 = sp50;
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
        D_800B1AF0 += 1;
        break;
    case 45:
        func_8002755C(1);
        func_80027438(1);
        D_800B1AF0 += 1;
        break;
    case 46:
        sp50 = D_800B1AF0;
        D_800B1AF0 = sp50;
        func_8011A62C();
        D_800B1AF6 -= 1;
        D_800B1AF0 = sp50 + 1;
        break;
    case 48:
        sp50 = D_800B1AF0;
        D_800B1AF0 = sp50;
        func_8011A7FC();
        D_800B1AF6 -= 1;
        D_800B1AF0 = sp50 + 1;
        break;
    case 50:
        sp50 = D_800B1AF0;
        D_800B1AF0 = sp50;
        func_8011A838();
        D_800B1AF6 -= 1;
        D_800B1AF0 = sp50 + 1;
        break;
    default:
        func_80011DFC();
        D_800EED08 = 0;
        func_800433D0(0x206);
        func_800352B4(1);
        break;
    }
    return;
}