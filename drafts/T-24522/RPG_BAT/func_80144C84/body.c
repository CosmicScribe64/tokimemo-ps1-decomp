/* Source-only NON_MATCHING draft, T-24522: 28 literal rows / 28 aligned.
 * Third-arm counter store/reload and scheduling gap; not production coverage. */
s32 func_80144C84(void) {
    s32 home; /* FAKE: T-24522, original extra frame home; source not recovered. */
    RpgStat *stat;
    s32 lim;
    s32 roll;

    stat = &D_8015EBFC;
    if (D_800E6280.unk_118.unk_02 >= 0x97) {
        lim = 0x96;
    } else {
        lim = D_800E6280.unk_118.unk_02;
    }
    switch (func_8013E9A8(3)) {
    case 0:
        D_8015EBAC = 0;
        break;
    case 1:
        D_8015EBAC = 1;
        break;
    case 2:
        D_8015EBAC = 2;
        break;
    }
    /* FAKE: T-24522, multiply-by-one wrappers retain original sum operand order. */
    if (func_801447B8()) {
        if ((D_8015EBFC.unk_0C * 1) + stat->unk_00 + D_8015EBFC.unk_04 + D_8015EBFC.unk_08 < 0x15) {
            stat->unk_00 = 0xF;
            D_8015EBFC.unk_04 = 0xF;
            D_8015EBFC.unk_08 = 5;
            D_8015EBFC.unk_0C = 0x1E;
        }
    } else {
        if ((D_8015EBFC.unk_0C * 1) + stat->unk_00 + D_8015EBFC.unk_04 + D_8015EBFC.unk_08 < 0x15) {
            stat->unk_00 = 0xA;
            D_8015EBFC.unk_04 = 0xA;
            D_8015EBFC.unk_08 = 0;
            D_8015EBFC.unk_0C = 0x28;
        }
    }
    roll = func_8013E9A8((D_8015EBE0 * 1) + stat->unk_00 + D_8015EBFC.unk_04 + D_8015EBFC.unk_08 + D_8015EBFC.unk_0C);
    if (roll < stat->unk_00) {
        stat->unk_00 -= 5;
        return 1;
    }
    if (roll < (D_8015EBFC.unk_04 * 1) + stat->unk_00) {
        /* FAKE: T-24522, comma return preserves the original decrement schedule. */
        return (D_8015EBFC.unk_04 -= 5, 0);
    }
    if (roll < (D_8015EBE0 * 1) + stat->unk_00 + D_8015EBFC.unk_04 + D_8015EBFC.unk_08) {
        if (D_8015EBCC[D_8015EBAC] == 1) {
            D_8015EBB0 = func_8013EA00(0x64, 0x64);
            return 4;
        }
        D_8015EBFC.unk_08 -= 5;
        if (D_8015EBFC.unk_08 < 0) {
            D_8015EBFC.unk_08 = 0;
        }
        D_8015EBE0 -= 0x14;
        if (D_8015EBE0 < 0) {
            D_8015EBE0 = 0;
        }
        return 3;
    }
    D_8015EBFC.unk_0C -= 5;
    if (func_801447B8() == 0) {
        D_8015EBD8 += 0x19;
    }
    if (D_8015EBCC[D_8015EBAC] == 1) {
        D_8015EBB0 = func_8013EA00(0x64, 0x64);
        return 4;
    }
    if (D_8015EDF0 != 0x40000000 && func_8013E9A8(0x64) < 5) {
        D_8015EBB0 = D_8015EDEC / 10;
        if (D_8015ED98 & 0x100) {
            D_8015EBB0 = D_8015EBB0 / 10;
        }
        D_8015EBFC.unk_38 += 2;
        return 6;
    }
    if (D_8015ED98 != 0x100) {
        if (func_8013E9A8(0x64) < 5) {
            D_8015EBB0 = func_8013EA00(0xA, 0xA);
            D_8015EBB0 = D_8015EBD8 * D_8015EBB0;
            D_8015EBB0 /= 1000;
            D_8015EBFC.unk_38 += 1;
            return 5;
        }
    }
    D_8015EBB0 = func_8013EA00(0x7D, 0x4B);
    D_8015EBB0 *= 0xE1 - lim;
    D_8015EBB0 /= 150;
    D_8015EBB0 = D_8015EBD8 * D_8015EBB0;
    D_8015EBB0 /= 1000;
    if (D_8015ED98 & 0x100) {
        D_8015EBB0 = D_8015EBB0 / 10;
    }
    D_8015EBFC.unk_38 += 1;
    return 2;
}
