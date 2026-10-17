/* T-22009 hand draft (409 diff rows at best: the loop index and the hoisted constants take v1/a0 the other way round,
 * the frame is 48 bytes against 64; the original keeps the selector D_800B1AF0 in t0). Needs s32 return type; not built. */
s32 func_8011D5C4(void) {
    s16 i;
    u16 t;

    switch (D_800B1AF0) {
    case 0:
        D_800EED08 = 1;
        func_800433D0(0x203);
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
        i = 0;
        do {
            ((Rec24 *)D_800EC190)[i + 10].unk_08 = (i << 6) + 0x80;
            ((Rec24 *)D_800EC190)[i + 10].unk_00 = 0x82000000;
            ((Rec24 *)D_800EC190)[i + 10].unk_0A = 0xA0;
            ((Rec24 *)D_800EC190)[i + 10].unk_0E = 0;
            ((Rec24 *)D_800EC190)[i + 10].unk_0F = 0;
            ((Rec24 *)D_800EC190)[i + 10].unk_10 = 0;
            ((Rec24 *)D_800EC190)[i + 10].unk_0C = (i * 2) + 0x15;
            ((Rec24 *)D_800EC190)[i + 10].unk_12 = 0x1E0;
            ((Rec24 *)D_800EC190)[i + 10].unk_14 = 0;
            ((Rec24 *)D_800EC190)[i + 10].unk_15 = 0;
            ((Rec24 *)D_800EC190)[i + 10].unk_16 = 0;
            ((Rec24 *)D_800EC190)[i + 10].unk_18 = 0;
            ((Rec24 *)D_800EC190)[i + 10].unk_1A = 0;
            ((Rec24 *)D_800EC190)[i + 10].unk_1C = 0x1000;
            ((Rec24 *)D_800EC190)[i + 10].unk_1E = 0x1000;
            ((Rec24 *)D_800EC190)[i + 10].unk_20 = 0;
            ((Rec24 *)D_800EC190)[i + 10].unk_04 = (i << 7) - 0xA0;
            ((Rec24 *)D_800EC190)[i + 10].unk_06 = -0x96;
            D_800B0860[i + 0x1037] = 0xC;
            i++;
        } while (i < 2);
        func_8002746C(0x4332, 1);
        D_800B1AF0 += 1;
        return 0;
    case 1:
        if (func_80015A44() & 1) {
            func_8002469C(0x140, 0x100, 0x140, 0xA0);
            D_800B1AF0 += 1;
        }
        return 0;
    case 2:
        func_800433D0(0x502);
        func_80015D28(0x31, 0x801AE000, 0xACA8);
        func_8011C650();
        D_800B1AF0 += 1;
        return 0;
    case 3:
    case 11:
    case 14:
    case 16:
    case 18:
        if (func_80015A44() & 1) {
            D_800B1AF0 += 1;
        }
        return 0;
    case 4:
        func_80012D64(D_801258C0, 0x11, 1, 0, 0);
        func_8004C46C(D_801258C4, D_801258C8, D_801258CC, D_801258D0, D_801258D4, D_801258D8);
        D_800B1AF0 += 1;
        return 0;
    case 5:
        i = 0;
        do {
            ((Rec24 *)D_800EC190)[i + 4].unk_00 &= 0x7FFFFFFF;
            i++;
        } while (i < 8);
        func_800433D0(0x503);
        D_800B1AF0 += 1;
        return 0;
    case 6:
        ((Rec24 *)D_800EC190)[4].unk_15 = ((Rec24 *)D_800EC190)[4].unk_16 = ((Rec24 *)D_800EC190)[4].unk_14 = ((Rec24 *)D_800EC190)[4].unk_14 + 1;
        i = 1;
        do {
            ((Rec24 *)D_800EC190)[i + 4].unk_14 = ((Rec24 *)D_800EC190)[i + 4].unk_15 = ((Rec24 *)D_800EC190)[i + 4].unk_16 = ((Rec24 *)D_800EC190)[4].unk_14;
            i++;
        } while (i < 8);
        if (((Rec24 *)D_800EC190)[4].unk_14 == 0x40) {
            D_800B1AF0 += 1;
        }
        return 0;
    case 7:
        ((Rec24 *)D_800EC190)[4].unk_15 = ((Rec24 *)D_800EC190)[4].unk_16 = ((Rec24 *)D_800EC190)[4].unk_14 = ((Rec24 *)D_800EC190)[4].unk_14 + 1;
        i = 1;
        do {
            ((Rec24 *)D_800EC190)[i + 4].unk_14 = ((Rec24 *)D_800EC190)[i + 4].unk_15 = ((Rec24 *)D_800EC190)[i + 4].unk_16 = ((Rec24 *)D_800EC190)[4].unk_14;
            i++;
        } while (i < 8);
        if (((Rec24 *)D_800EC190)[4].unk_14 == 0x80) {
            D_800B1AF0 += 1;
        }
        break;
    case 8:
        break;
    case 9:
        ((Rec24 *)D_800EC190)[4].unk_15 = ((Rec24 *)D_800EC190)[4].unk_16 = ((Rec24 *)D_800EC190)[4].unk_14 = ((Rec24 *)D_800EC190)[4].unk_14 - 2;
        i = 1;
        do {
            ((Rec24 *)D_800EC190)[i + 4].unk_14 = ((Rec24 *)D_800EC190)[i + 4].unk_15 = ((Rec24 *)D_800EC190)[i + 4].unk_16 = ((Rec24 *)D_800EC190)[4].unk_14;
            i++;
        } while (i < 8);
        if (((Rec24 *)D_800EC190)[4].unk_14 == 0) {
            i = 0;
            do {
                ((Rec24 *)D_800EC190)[i + 4].unk_00 |= 0x80000000;
                i++;
            } while (i < 8);
            D_800B1AF0 += 1;
        }
        break;
    case 10:
        switch (D_80125320) {
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
        return 0;
    case 12:
        func_8002755C(1);
        func_80027438(1);
        D_800B1AF0 += 1;
        return 0;
    case 13:
        func_8011A62C();
        D_800B1AF6 -= 1;
        D_800B1AF0 += 1;
        return 0;
    case 15:
        func_8011A7FC();
        D_800B1AF6 -= 1;
        D_800B1AF0 += 1;
        return 0;
    case 17:
        func_8011A838();
        D_800B1AF6 -= 1;
        D_800B1AF0 += 1;
        return 0;
    default:
        func_80011DFC();
        D_800EED08 = 0;
        func_800433D0(0x206);
        func_800352B4(1);
        return 0;
    }
    t = D_801258DC + 1;
    i = 0;
    if ((t % 19) == 0x12) {
        do {
            ((Rec24 *)D_800EC190)[i + 10].unk_06 += 1;
            i++;
        } while (i < 2);
        ((Rec24 *)D_800EC190)[i + 10].unk_0A -= 1;
    }
    i = 0;
    if ((t % 5) == 4) {
        do {
            ((Rec24 *)D_800EC190)[i + 4].unk_06 += 1;
            i++;
        } while (i < 6);
        i = 3;
        do {
            if (((Rec24 *)D_800EC190)[i + 4].unk_0A != 0) {
                ((Rec24 *)D_800EC190)[i + 4].unk_0A -= 1;
            } else {
                ((Rec24 *)D_800EC190)[i + 1].unk_0A -= 1;
            }
            i++;
        } while (i < 6);
    }
    D_801258DC = t;
    if (t == 0x12C) {
        D_800B1AF0 += 1;
    }
}
