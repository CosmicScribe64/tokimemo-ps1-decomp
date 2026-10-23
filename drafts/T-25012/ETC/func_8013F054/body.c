s32 func_8013F054(void) {
    s32 idx;
    u8 *p;
    s32 i;
    s32 type;
    s32 a;
    u8 b;
    u8 v;

    idx = D_80150064;
    if (idx) {
    }
    b = 0;
    a = 0;
    type = 0;
    if (D_800E6280.unk_1124 == 0) {
        if (D_801500D8 == 1 || D_801500D8 == 3) {
            type = D_800E6280.unk_75E[idx - 1].unk_00;
            a = D_800E6280.unk_75E[idx - 1].unk_02;
            b = D_800E6280.unk_75E[idx - 1].unk_01;
        } else if ((D_801500D8 == 0 || D_801500D8 == 2) && idx >= 0) {
            p = (u8 *)&D_800E6280 + idx * 8;
            do {
                v = p[0x756];
                p -= 8;
                if ((v == 0xC && D_800E71DF == 0xC) || (v == 0xB && D_800E71DF == 0xB)) {
                    a = p[0x760];
                    b = p[0x75F];
                    type = v;
                    break;
                }
            } while ((u32)p >= (u32)&D_800E6280);
        }
    } else {
        type = ((EtcConfig8 *)((u8 *)&D_800E7D10 + D_80150080 * 8))[11].unk_00;
        a = ((EtcConfig8 *)((u8 *)&D_800E7D10 + D_80150080 * 8))[11].unk_02;
        b = ((EtcConfig8 *)((u8 *)&D_800E7D10 + D_80150080 * 8))[11].unk_01;
    }
    if (D_800E71DF == 0xC && type == 0xC) {
        if (a == 4) {
            func_8013F320(1);
        } else if (a == 6) {
            func_8013F320(2);
        } else if (a == 7) {
            func_8013F320(3);
        } else if (a == 2 && b == 0x4F) {
            func_8013F320(5);
        } else if (a == 2 && b == 0x50) {
            func_8013F320(4);
        } else if (a == 2 && b == 0x51) {
            func_8013F320(6);
        }
    } else if (D_800E71DF == 0xB && type == 0xB) {
        if (a == 2 && b == 0x4A) {
            func_8013F320(0);
        } else if ((a == 2 && b == 0x4C) || (a == 0xB && b != 0x1E)) {
            func_8013F320(1);
        } else if ((a == 2 && b == 0x4E) || (a == 0xB && b == 0x1E)) {
            func_8013F320(2);
        } else if (a == 0x12) {
            func_8013F320(3);
        }
    }
}
