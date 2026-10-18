s32 func_8011DDB8(void) {
    s32 padA;
    register u8 c;
    register u8 x;
    s32 sp54;
    s32 stage;
    RECT sp48;
    s16 i;

    switch (D_800B1AF0) {
    case 0:
        D_800EED08 = 1;
        ((EventMovieWork *)D_800F1990)->unk_06 = 0;
        ((EventMovieWork *)D_800F1990)->unk_02 = 0;
        ((EventMovieWork *)D_800F1990)->unk_04 = 0;
        ((EventMovieWork *)D_800F1990)->unk_00 = 0;
        ((EventMovieWork *)D_800F1990)->unk_19 = 0;
        ((EventMovieWork *)D_800F1990)->unk_18 = 0;
        D_80080710 = -1;
        ((EventMovieWork *)D_800F1990)->unk_0A = 0;
        ((EventMovieWork *)D_800F1990)->unk_1A = 0;
        func_800352B4(0);
        D_801258DC = 0;
        for (i = 0; i < 2; i++) {
            ((Rec24 *)D_800EC190)[i + 4].unk_08 = (i << 6) + 0x80;
            ((Rec24 *)D_800EC190)[i + 4].unk_00 = 0x82000000;
            ((Rec24 *)D_800EC190)[i + 4].unk_0A = 0xA0;
            ((Rec24 *)D_800EC190)[i + 4].unk_0E = 0;
            ((Rec24 *)D_800EC190)[i + 4].unk_0F = 0;
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
            ((Rec24 *)D_800EC190)[i + 6].unk_0A = 0;
            ((Rec24 *)D_800EC190)[i + 6].unk_0E = 0;
            ((Rec24 *)D_800EC190)[i + 6].unk_0F = 0xA0;
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
        func_8002746C(0x4321, 1);
        D_800B1AF0 += 1;
        break;
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
            D_800B1AF0 += 1;
            break;
        }
        break;
    case 2:
        func_8002469C(0x140, 0x100, 0x140, 0xA0);
        sp48.x = 0x140;
        sp48.y = 0x100;
        sp48.w = 0x140;
        sp48.h = 0xA0;
        func_800651F8(&sp48, 0x80183000);
        D_800B1AF0 += 1;
        break;
    case 3:
        func_8002746C(0x431A, 1);
        D_800B1AF0 += 1;
        break;
    case 5:
        func_8002469C(0x140, 0x100, 0x140, 0xA0);
        sp48.x = 0x140;
        sp48.y = 0x100;
        sp48.w = 0x140;
        sp48.h = 0xA0;
        func_800651F8(&sp48, 0x8019C000);
        D_800B1AF0 += 1;
        break;
    case 6:
        func_8002746C(0x4313, 1);
        D_800B1AF0 += 1;
        break;
    case 8:
        func_8002469C(0x140, 0x100, 0x140, 0xA0);
        sp48.x = 0x140;
        sp48.y = 0x100;
        sp48.w = 0x140;
        sp48.h = 0xA0;
        func_800651F8(&sp48, 0x801B5000);
        D_800B1AF0 += 1;
        break;
    case 9:
        func_8002746C(0x430C, 1);
        D_800B1AF0 += 1;
        break;
    case 11:
        func_8002469C(0x140, 0x100, 0x140, 0xA0);
        sp48.x = 0x140;
        sp48.y = 0x100;
        sp48.w = 0x140;
        sp48.h = 0xA0;
        func_800651F8(&sp48, 0x801CE000);
        D_800B1AF0 += 1;
        break;
    case 12:
        func_8002746C(0x4305, 1);
        D_800B1AF0 += 1;
        break;
    case 14:
        func_8002469C(0x140, 0, 0x140, 0xA0);
        D_800B1AF0 += 1;
        break;
    case 15:
        func_8002746C(0x42ED, 1);
        D_800B1AF0 += 1;
        break;
    case 17:
        func_8002469C(0x140, 0x100, 0x140, 0xA0);
        D_800B1AF0 += 1;
        break;
    case 18:
        func_8002746C(0x42F5, 1);
        D_800B1AF0 += 1;
        break;
    case 20:
        func_8002469C(0x280, 0, 0x140, 0xA0);
        D_800B1AF0 += 1;
        break;
    case 21:
        for (i = 0; i < 4; i++) {
            ((Rec24 *)D_800EC190)[i + 4].unk_00 &= 0x7FFFFFFF;
        }
        D_800B1AF0 += 1;
        break;
    case 22:
        stage = D_800B1AF0;
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
        D_800B1AF0 = stage;
        if (c == 0x80) {
            D_800B1AF0 = stage + 1;
            return func_800433D0(0x500);
        }
        break;
    case 23:
        stage = D_800B1AF0;
        D_800B1AF0 = stage;
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
        D_800B1AF0 += 1;
        break;
    case 25:
        func_80012D40(0);
        for (i = 0; i < 4; i++) {
            ((Rec24 *)D_800EC190)[i + 4].unk_00 |= 0x80000000;
        }
        D_800B1AF0 += 1;
        break;
    case 26:
        func_8002746C(0x42FD, 1);
        D_800B1AF0 += 1;
        break;
    case 28:
        func_8002469C(0x140, 0x100, 0x140, 0xA0);
        sp48.x = 0x140;
        sp48.y = 0;
        sp48.w = 0x140;
        sp48.h = 0xA0;
        func_80065234(&sp48, 0x280, 0);
        D_800B1AF0 += 1;
        return func_80012D40(1);
    case 29:
        for (i = 0; i < 2; i++) {
            ((Rec24 *)D_800EC190)[i + 4].unk_00 = 0x02000000;
            ((Rec24 *)D_800EC190)[i + 4].unk_0A = 0xA0;
            ((Rec24 *)D_800EC190)[i + 4].unk_0F = 0;
            ((Rec24 *)D_800EC190)[i + 4].unk_06 = -0x78;
        }
        for (i = 0; i < 2; i++) {
            ((Rec24 *)D_800EC190)[i + 6].unk_00 = 0x02000000;
            ((Rec24 *)D_800EC190)[i + 6].unk_0A = 0;
            ((Rec24 *)D_800EC190)[i + 6].unk_0F = 0xA0;
            ((Rec24 *)D_800EC190)[i + 6].unk_06 = -0x78;
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
            D_800B1AF0 += 1;
            break;
        }
        break;
    case 31:
        sp48.x = 0x280;
        sp48.y = 0;
        sp48.w = 0x140;
        sp48.h = 0xA0;
        func_80065234(&sp48, 0x140, 0x100);
        func_800137AC(0x280, 0, 0x140, 0xA0, 0x801CE000);
        for (i = 0; i < 2; i++) {
            ((Rec24 *)D_800EC190)[i + 4].unk_0A = 0xA0;
            ((Rec24 *)D_800EC190)[i + 4].unk_0F = 0;
            ((Rec24 *)D_800EC190)[i + 4].unk_06 = -0x78;
        }
        for (i = 0; i < 2; i++) {
            ((Rec24 *)D_800EC190)[i + 6].unk_0A = 0;
            ((Rec24 *)D_800EC190)[i + 6].unk_0F = 0xA0;
            ((Rec24 *)D_800EC190)[i + 6].unk_06 = -0x78;
        }
        D_801258DC = 0;
        D_800B1AF0 += 1;
        break;
    case 33:
        sp48.x = 0x280;
        sp48.y = 0;
        sp48.w = 0x140;
        sp48.h = 0xA0;
        func_80065234(&sp48, 0x140, 0x100);
        func_800137AC(0x280, 0, 0x140, 0xA0, 0x801B5000);
        for (i = 0; i < 2; i++) {
            ((Rec24 *)D_800EC190)[i + 4].unk_0A = 0xA0;
            ((Rec24 *)D_800EC190)[i + 4].unk_0F = 0;
            ((Rec24 *)D_800EC190)[i + 4].unk_06 = -0x78;
        }
        for (i = 0; i < 2; i++) {
            ((Rec24 *)D_800EC190)[i + 6].unk_0A = 0;
            ((Rec24 *)D_800EC190)[i + 6].unk_0F = 0xA0;
            ((Rec24 *)D_800EC190)[i + 6].unk_06 = -0x78;
        }
        D_801258DC = 0;
        D_800B1AF0 += 1;
        break;
    case 35:
        sp48.x = 0x280;
        sp48.y = 0;
        sp48.w = 0x140;
        sp48.h = 0xA0;
        func_80065234(&sp48, 0x140, 0x100);
        func_800137AC(0x280, 0, 0x140, 0xA0, 0x8019C000);
        for (i = 0; i < 2; i++) {
            ((Rec24 *)D_800EC190)[i + 4].unk_0A = 0xA0;
            ((Rec24 *)D_800EC190)[i + 4].unk_0F = 0;
            ((Rec24 *)D_800EC190)[i + 4].unk_06 = -0x78;
        }
        for (i = 0; i < 2; i++) {
            ((Rec24 *)D_800EC190)[i + 6].unk_0A = 0;
            ((Rec24 *)D_800EC190)[i + 6].unk_0F = 0xA0;
            ((Rec24 *)D_800EC190)[i + 6].unk_06 = -0x78;
        }
        D_801258DC = 0;
        D_800B1AF0 += 1;
        break;
    case 36:
        func_8011ED50(8);
        if (D_801258DC >= 8U) {
            x = ((Rec24 *)D_800EC190)[4].unk_14 - 4;
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
        }
        if (++D_801258DC == 0x14) {
            D_800B1AF0 += 1;
            break;
        }
        break;
    case 37:
        sp48.x = 0x280;
        sp48.y = 0;
        sp48.w = 0x140;
        sp48.h = 0xA0;
        func_80065234(&sp48, 0x140, 0x100);
        func_800137AC(0x280, 0, 0x140, 0xA0, 0x80183000);
        for (i = 0; i < 2; i++) {
            ((Rec24 *)D_800EC190)[i + 4].unk_0A = 0xA0;
            ((Rec24 *)D_800EC190)[i + 4].unk_0F = 0;
            ((Rec24 *)D_800EC190)[i + 4].unk_06 = -0x78;
        }
        for (i = 0; i < 2; i++) {
            ((Rec24 *)D_800EC190)[i + 6].unk_0A = 0;
            ((Rec24 *)D_800EC190)[i + 6].unk_0F = 0xA0;
            ((Rec24 *)D_800EC190)[i + 6].unk_06 = -0x78;
        }
        D_801258DC = 0;
        D_800B1AF0 += 1;
        break;
    case 38:
        func_8011ED50(8);
        x = ((Rec24 *)D_800EC190)[4].unk_14 - 4;
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
        if (c < 4U) {
            D_800B1AF0 += 1;
            break;
        }
        break;
    case 39:
        for (i = 0; i < 4; i++) {
            ((Rec24 *)D_800EC190)[i + 4].unk_00 |= 0x80000000;
        }
        D_800B1AF0 += 1;
        break;
    case 40:
        stage = D_800B1AF0;
        D_800B1AF0 = stage;
        sp54 = D_800B1AF6;
        func_8004AFB8();
        D_800B1AF0 = stage;
        if (sp54 != D_800B1AF6) {
            D_800B1AF6 -= 1;
            D_800B1AF0 = stage + 1;
            break;
        }
        break;
    case 42:
        stage = D_800B1AF0;
        D_800B1AF0 = stage;
        func_8004A9CC();
        D_800B1AF6 -= 1;
        D_800B1AF0 = stage + 1;
        break;
    case 43:
        stage = D_800B1AF0;
        D_800B1AF0 = stage;
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
        stage = D_800B1AF0;
        D_800B1AF0 = stage;
        func_8011A62C();
        D_800B1AF6 -= 1;
        D_800B1AF0 = stage + 1;
        break;
    case 48:
        stage = D_800B1AF0;
        D_800B1AF0 = stage;
        func_8011A7FC();
        D_800B1AF6 -= 1;
        D_800B1AF0 = stage + 1;
        break;
    case 50:
        stage = D_800B1AF0;
        D_800B1AF0 = stage;
        func_8011A838();
        D_800B1AF6 -= 1;
        D_800B1AF0 = stage + 1;
        break;
    default:
        func_80011DFC();
        D_800EED08 = 0;
        func_800433D0(0x206);
        func_800352B4(1);
        break;
    }
}
