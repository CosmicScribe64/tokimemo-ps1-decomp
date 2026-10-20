void func_80139290(void) {
    u32 frame;

    frame = D_800E6280.unk_1104.u;
    switch (frame) {
    case 0x1D:
        D_801206AC = 1;
        break;
    case 0x58:
        D_801206AC = 0;
        break;
    case 0x66:
        D_801206AC = 2;
        break;
    case 0x83:
        D_801206AC = 3;
        break;
    }
    if (frame >= 0xBF && frame < 0x70000000) {
        switch (((frame - 0xBE) % 0xF2) / 0xB) {
        case 0: D_801206AC = 4; break;
        case 1: D_801206AC = 3; break;
        case 2: D_801206AC = 5; break;
        case 3: D_801206AC = 6; break;
        case 4: D_801206AC = 3; break;
        case 5: D_801206AC = 0xC; break;
        case 6: D_801206AC = 0xB; break;
        case 7: D_801206AC = 0xE; break;
        case 8: D_801206AC = 3; break;
        case 9: D_801206AC = 8; break;
        case 10: D_801206AC = 7; break;
        case 11: D_801206AC = 9; break;
        case 12: D_801206AC = 3; break;
        case 13: D_801206AC = 5; break;
        case 14: D_801206AC = 6; break;
        case 15: D_801206AC = 3; break;
        case 16: D_801206AC = 0xD; break;
        case 17: D_801206AC = 0xB; break;
        case 18: D_801206AC = 4; break;
        case 19: D_801206AC = 3; break;
        case 20: D_801206AC = 0xA; break;
        case 21: D_801206AC = 7; break;
        }
    }
    if (frame == 0x70000000) {
        D_801206AC = 3;
    }
    switch (frame - 0x70000000) {
    case 0x3B:
        D_801206AC = 2;
        break;
    case 0x49:
        D_801206AC = 0;
        break;
    case 0x66:
        D_801206AC = 1;
        break;
    case 0xA1:
        if (D_800E6280.unk_03E == 0x60) {
            D_801206AC = 0;
        }
        break;
    }
    if (frame == 0x95) {
        D_801206F0 = 0x10;
        D_80120734 = 0x14;
        D_80120778 = 0x18;
        D_801207BC = 0x1D;
        if (D_80122EAC & 1) {
            D_80120734 = 0x20;
        }
        if (D_80122EAC & 2) {
            D_80120778 = 0x24;
        }
        if (D_80122EAC & 4) {
            D_801207BC = 0x28;
        }
    }
    if (frame >= 0xD1 && frame < 0x70000000) {
        switch (((frame - 0xC9) % 0x4E) / 0x27) {
        case 0:
            D_801206F0 = 0x11;
            D_80120734 = 0x15;
            D_80120778 = 0x19;
            D_801207BC = 0x1D;
            if (D_80122EAC & 1) {
                D_80120734 = 0x21;
            }
            if (D_80122EAC & 2) {
                D_80120778 = 0x25;
            }
            if (D_80122EAC & 4) {
                D_801207BC = 0x29;
            }
            break;
        case 1:
            D_801206F0 = 0x10;
            D_80120734 = 0x14;
            D_80120778 = 0x18;
            D_801207BC = 0x1C;
            if (D_80122EAC & 1) {
                D_80120734 = 0x20;
            }
            if (D_80122EAC & 2) {
                D_80120778 = 0x24;
            }
            if (D_80122EAC & 4) {
                D_801207BC = 0x28;
            }
            break;
        }
    }
    if (frame == 0x70000000) {
        D_801206F0 = 0x10;
        D_80120734 = 0x14;
        D_80120778 = 0x18;
        D_801207BC = 0x1C;
        if (D_80122EAC & 1) {
            D_80120734 = 0x20;
        }
        if (D_80122EAC & 2) {
            D_80120778 = 0x24;
        }
        if (D_80122EAC & 4) {
            D_801207BC = 0x28;
        }
    }
    if (frame - 0x70000000 == 0x3B && D_800E6280.unk_03E != 0x60) {
        D_801206F0 = 0xF;
        D_80120734 = 0x13;
        D_80120778 = 0x17;
        D_801207BC = 0x1B;
        if (D_80122EAC & 1) {
            D_80120734 = 0x1F;
        }
        if (D_80122EAC & 2) {
            D_80120778 = 0x23;
        }
        if (D_80122EAC & 4) {
            D_801207BC = 0x27;
        }
    }
    if (frame - 0x70000000 == 0x68 && D_800E6280.unk_03E == 0x60) {
        D_801206F0 = 0x12;
        D_80120734 = 0x16;
        D_80120778 = 0x1A;
        D_801207BC = 0x1E;
        if (D_80122EAC & 1) {
            D_80120734 = 0x22;
        }
        if (D_80122EAC & 2) {
            D_80120778 = 0x26;
        }
        if (D_80122EAC & 4) {
            D_801207BC = 0x2A;
        }
    }
}
