void func_8013BAC4(Tc14 *arg0, s32 arg1, s32 arg2) {
    s32 i;
    u32 damage;
    s32 init;
    s32 xdist;
    s32 ytest;
    s32 radius;
    s32 width;
    s32 x;
    s32 y;
    s32 z;
    T24502Byte9 damageTable;
    s32 scores[256];
    s32 j;
    s32 offset;
    Tc14 *p;

    damageTable = D_8015F548;
    switch (D_800E6280.unk_1109) {
    case 0xC:
        for (init = 0; init < 256; init++) scores[init] = 0x190;
        break;
    case 0x16:
        for (init = 0; init < 256; init++) scores[init] = 0xC8;
        break;
    case 0x20:
        for (init = 0; init < 256; init++) scores[init] = 0x320;
        break;
    default:
        for (init = 0; init < 256; init++) scores[init] = D_8015F504[init];
        break;
    }
    D_8015F500 = 0;
    if (arg0->unk13 & 0x20) {
        for (i = 0; i != 2; i++) {
            if (D_800E6280.unk_10F8 & 1) {
                x = (i + arg0)->unk0;
                y = (i + arg0)->unk2;
                if (arg2 == 0) z = D_8015EDB4[0].unk78;
                else z = D_8015EDB4[8].unk78;
                for (j = 16; j != 63; j++) {
                    if (((j + D_8015EDB4)->unk0[1] & 0x80) && ((j + D_8015EDB4)->unk84[3] & 1) && (j + D_8015EDB4)->unk84[1] != 0 && (j + D_8015EDB4)->unk84[0] != 0) {
                        radius = (j + D_8015EDB4)->unk84[0] * 16;
                        xdist = func_800AE0C0(x - (j + D_8015EDB4)->unk74);
                        ytest = func_800AE0C0(y - (j + D_8015EDB4)->unk76) < radius + 0x18;
                        if (xdist < radius + 0x18 && ytest) {
                            if ((j + D_8015EDB4)->unk78 < z) {
                                if (D_8015F500 == 0 && arg2 == 0) {
                                    D_8015F500 = 1;
                                    func_801335A0(1, 0x507);
                                }
                                if ((j + D_8015EDB4)->unk84[1] != 0) (j + D_8015EDB4)->unk84[1]--;
                                func_8013D2A0(x, y, (s16)((j + D_8015EDB4)->unk78 - 0x200));
                                D_8015EDF0 += (u32)scores[(j + D_8015EDB4)->unk84[2] % 17U] / 20U;
                                if ((j + D_8015EDB4)->unk84[1] == 0) D_8015EDF0 += scores[(j + D_8015EDB4)->unk84[2] % 17U];
                            }
                        }
                    }
                }
            }
        }
    }
    i = 0;
    offset = 0;
    p = arg0;
    do {
        if ((p->unk13 & 0x80) && (p->unk13 & 15) != 0) {
            x = p->unk0;
            y = p->unk2;
            z = p->unk4;
            damage = damageTable.bytes[p->unk13 & 15];
            if ((p->unk13 & 15) == 8) width = 0x60;
            else width = 0x30;
            for (j = 16; j != 63; j++) {
                if (((j + D_8015EDB4)->unk0[1] & 0x80) && ((j + D_8015EDB4)->unk84[3] & 1) && (j + D_8015EDB4)->unk84[1] != 0 && (j + D_8015EDB4)->unk84[0] != 0) {
                    radius = (j + D_8015EDB4)->unk84[0] * 16;
                    if (func_800AE0C0(z - (j + D_8015EDB4)->unk78) < radius + 0x100) {
                        xdist = func_800AE0C0(x - (j + D_8015EDB4)->unk74);
                        ytest = func_800AE0C0(y - (j + D_8015EDB4)->unk76) < radius + width;
                        if (xdist < radius + width && ytest) {
                            if (D_8015F500 == 0 && arg2 == 0) {
                                D_8015F500 = 1;
                                func_801335A0(1, 0x507);
                            }
                            if (damage < (j + D_8015EDB4)->unk84[1]) (j + D_8015EDB4)->unk84[1] -= damage;
                            else (j + D_8015EDB4)->unk84[1] = 0;
                            if ((j + D_8015EDB4)->unk84[1] == 0) (j + D_8015EDB4)->unk84[3] &= 0xF0;
                            p->unk12 = 0;
                            p->unk13 = 0;
                            func_8013D2A0((s16)(((j + D_8015EDB4)->unk74 + x) / 2), (s16)(((j + D_8015EDB4)->unk76 + y) / 2), (s16)(z - 0x200));
                            D_8015EDF0 += ((u32)scores[(j + D_8015EDB4)->unk84[2] % 17U] / 10U) * damage;
                            if ((j + D_8015EDB4)->unk84[1] == 0) {
                                func_801335A0(1, 0x508);
                                D_8015EDF0 += scores[(j + D_8015EDB4)->unk84[2] % 17U];
                            }
                            break;
                        }
                    }
                }
            }
        }
        offset += 20;
        p++;
    } while (offset != 320);
}
