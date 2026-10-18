u16 func_8011C750(void) {
    s32 padA;
    s16 i;
    s16 y;
    register u8 c;
    register u8 x;
    RECT sp54;
    s32 sv[2];
    u8 d;
    s32 padC;

    switch (((EventMovieState *)&D_800B1AF0)->unk_00) {
    case 0:
        D_800EED08 = 1;
        func_8011F530(D_800B1AE4);
        func_800433D0(0x207);
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
        D_801258E0 = 2;
        ((EventMovieState *)&D_800B1AF0)->unk_09 = 0;
        D_801258DC = 0;
        for (i = 0; i < 2; i++) {
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
            ((EventEffectTables *)D_800B0860)->unk_1031[i + 0] = 0xB;
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
        ((EventMovieState *)&D_800B1AF0)->unk_00 += 1;
        break;
    case 1:
        if (func_80015A44() & 1) {
            func_8002469C(0x140, 0x100, 0x180, 0xA0);
            sp54.x = 0x140;
            sp54.y = 0x100;
            sp54.w = 0x180;
            sp54.h = 0xA0;
            func_800651F8(&sp54, 0x80183000);
            ((EventMovieState *)&D_800B1AF0)->unk_00 += 1;
        }
        break;
    case 2:
        func_8002746C(0x4344, 1);
        ((EventMovieState *)&D_800B1AF0)->unk_00 += 1;
        break;
    case 3:
        if (func_80015A44() & 1) {
            func_8002469C(0x280, 0, 0x180, 0xA0);
            sp54.x = 0x280;
            sp54.y = 0;
            sp54.w = 0x180;
            sp54.h = 0xA0;
            func_800651F8(&sp54, 0x801A1000);
            ((EventMovieState *)&D_800B1AF0)->unk_00 += 1;
        }
        break;
    case 4:
        func_8002746C(0x434E, 1);
        ((EventMovieState *)&D_800B1AF0)->unk_00 += 1;
        break;
    case 5:
        if (func_80015A44() & 1) {
            func_8002469C(0x140, 0x100, 0x180, 0xA0);
            sp54.x = 0x140;
            sp54.y = 0x100;
            sp54.w = 0x180;
            sp54.h = 0xA0;
            func_800651F8(&sp54, 0x801BF000);
            ((EventMovieState *)&D_800B1AF0)->unk_00 += 1;
        }
        break;
    case 6:
        func_8002746C(0x4357, 1);
        ((EventMovieState *)&D_800B1AF0)->unk_00 += 1;
        break;
    case 7:
        if (func_80015A44() & 1) {
            func_8002469C(0x280, 0, 0x180, 0xA0);
            sp54.x = 0x280;
            sp54.y = 0;
            sp54.w = 0x180;
            sp54.h = 0xA0;
            func_800651F8(&sp54, 0x801DD000);
            ((EventMovieState *)&D_800B1AF0)->unk_00 += 1;
        }
        break;
    case 8:
        for (i = 0; i < 4; i++) {
            ((Rec24 *)D_800EC190)[i + 4].unk_00 = 0x02000000;
        }
        ((EventMovieState *)&D_800B1AF0)->unk_00 += 1;
        break;
    case 9:
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
        if (c == 0x80) {
            ((EventMovieState *)&D_800B1AF0)->unk_00 += 1;
        }
        break;
    case 10:
        break;
    case 11:
        x = ((Rec24 *)D_800EC190)[4].unk_14 - 2;
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
        if (c == 0) {
            for (i = 0; i < 4; i++) {
                ((Rec24 *)D_800EC190)[i + 4].unk_00 = 0x82000000;
            }
            ((EventMovieState *)&D_800B1AF0)->unk_00 += 1;
        }
        break;
    case 12:
        sv[1] = ((EventMovieState *)&D_800B1AF0)->unk_06;
        sv[0] = ((EventMovieState *)&D_800B1AF0)->unk_00;
        func_8004AFB8();
        ((EventMovieState *)&D_800B1AF0)->unk_00 = sv[0];
        if (sv[1] != ((EventMovieState *)&D_800B1AF0)->unk_06) {
            ((EventMovieState *)&D_800B1AF0)->unk_00 = sv[0] + 1;
            ((EventMovieState *)&D_800B1AF0)->unk_06 -= 1;
        }
        break;
    case 13:
    case 16:
    case 19:
    case 21:
    case 23:
        if (func_80015A44() & 1) {
            ((EventMovieState *)&D_800B1AF0)->unk_00 += 1;
        }
        break;
    case 14:
        sv[0] = ((EventMovieState *)&D_800B1AF0)->unk_00;
        func_8004A9CC();
        ((EventMovieState *)&D_800B1AF0)->unk_06 -= 1;
        ((EventMovieState *)&D_800B1AF0)->unk_00 = sv[0] + 1;
        break;
    case 18:
        sv[0] = ((EventMovieState *)&D_800B1AF0)->unk_00;
        func_8011A62C();
        ((EventMovieState *)&D_800B1AF0)->unk_06 -= 1;
        ((EventMovieState *)&D_800B1AF0)->unk_00 = sv[0] + 1;
        break;
    case 20:
        sv[0] = ((EventMovieState *)&D_800B1AF0)->unk_00;
        func_8011A7FC();
        ((EventMovieState *)&D_800B1AF0)->unk_06 -= 1;
        ((EventMovieState *)&D_800B1AF0)->unk_00 = sv[0] + 1;
        break;
    case 22:
        sv[0] = ((EventMovieState *)&D_800B1AF0)->unk_00;
        func_8011A838();
        ((EventMovieState *)&D_800B1AF0)->unk_06 -= 1;
        ((EventMovieState *)&D_800B1AF0)->unk_00 = sv[0] + 1;
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
        ((EventMovieState *)&D_800B1AF0)->unk_00 += 1;
        break;
    case 17:
        func_8002755C(1);
        func_80027438(1);
        ((EventMovieState *)&D_800B1AF0)->unk_00 += 1;
        break;
    default:
        func_800352B4(1);
        D_800EED08 = 0;
        func_800433D0(0x206);
        func_80011DFC();
        break;
    }
    if (((EventMovieState *)&D_800B1AF0)->unk_00 < 9U || ((EventMovieState *)&D_800B1AF0)->unk_00 >= 0xCU) {
        return 0;
    }
    y = ((Rec24 *)D_800EC190)[4].unk_0E + D_801258E0;
    if (y < 0) {
        y = 0;
        D_801258E0 = (func_800789A0() % 4) + 2;
    }
    if (y >= 0x40) {
        y = 0x3F;
        D_801258E0 = -2 - (func_800789A0() % 4);
    }
    ((Rec24 *)D_800EC190)[4].unk_0E = ((Rec24 *)D_800EC190)[5].unk_0E = ((Rec24 *)D_800EC190)[6].unk_0E = ((Rec24 *)D_800EC190)[7].unk_0E = y;
    d = ((Rec24 *)D_800EC190)[4].unk_0F;
    if (d != 0) {
        ((Rec24 *)D_800EC190)[4].unk_0A += 0x14;
        ((Rec24 *)D_800EC190)[5].unk_0A += 0x14;
        d -= 0x14;
        ((Rec24 *)D_800EC190)[5].unk_0F -= 0x14;
    } else {
        ((Rec24 *)D_800EC190)[4].unk_0A -= 0x14;
        ((Rec24 *)D_800EC190)[4].unk_06 += 0x14;
        ((Rec24 *)D_800EC190)[5].unk_0A -= 0x14;
        ((Rec24 *)D_800EC190)[5].unk_06 += 0x14;
    }
    if (((Rec24 *)D_800EC190)[6].unk_0F != 0) {
        ((Rec24 *)D_800EC190)[6].unk_0A += 0x14;
        ((Rec24 *)D_800EC190)[7].unk_0A += 0x14;
        ((Rec24 *)D_800EC190)[6].unk_0F -= 0x14;
        ((Rec24 *)D_800EC190)[7].unk_0F -= 0x14;
    } else {
        ((Rec24 *)D_800EC190)[6].unk_0A -= 0x14;
        ((Rec24 *)D_800EC190)[6].unk_06 += 0x14;
        ((Rec24 *)D_800EC190)[7].unk_0A -= 0x14;
        ((Rec24 *)D_800EC190)[7].unk_06 += 0x14;
    }
    ((Rec24 *)D_800EC190)[4].unk_0F = d;
    if ((d == 0 && (((EventMovieState *)&D_800B1AF0)->unk_09 == 1 || ((EventMovieState *)&D_800B1AF0)->unk_09 == 3)) || (((Rec24 *)D_800EC190)[6].unk_0F == 0 && (((EventMovieState *)&D_800B1AF0)->unk_09 == 0 || ((EventMovieState *)&D_800B1AF0)->unk_09 == 2))) {
        switch (((EventMovieState *)&D_800B1AF0)->unk_09) {
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
        ((EventMovieState *)&D_800B1AF0)->unk_09 += 1;
        if (((EventMovieState *)&D_800B1AF0)->unk_09 >= 4U) {
            ((EventMovieState *)&D_800B1AF0)->unk_09 = 0;
        }
    }
    if (++D_801258DC == 0x12C) {
        ((EventMovieState *)&D_800B1AF0)->unk_00 += 1;
    }
    return D_801258DC;
}
