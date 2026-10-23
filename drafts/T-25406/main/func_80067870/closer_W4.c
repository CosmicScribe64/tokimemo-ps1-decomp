/* T-25406 closer: first plain match (0 rows), before the FAKE clean-up; the committed body is in src/main/800674B0.c. */
void func_80067870(void) {
    s32 x1;
    s16 dx;
    s16 dy;
    s32 y1;
    s32 r;

    if (D_800B5C08 == 0 || D_800B5C08 >= 0xE) {
        func_800676AC(0);
        return;
    }
    func_800676AC(D_800B5C08);
    if (D_800B5C08 < 0xE) {
        if ((u32)D_800E6280.unk_F88 >> 16) {
            D_80125C55 = (D_80125C54 + 1) / 2;
        }
        if ((D_800E6280.unk_10F8 & 0x3F) == 0x3F && D_80125C55 != 0) {
            D_80125C55--;
        }
        dx = (rsin((D_800E6280.unk_10F8 & 0x7F) << 5) * D_80125C55) >> 12;
        D_8011F41C = ((-dx * 2) / 3) << 12;
        r = SquareRoot0(*(u8 *)&D_80125C54 * *(u8 *)&D_80125C54 - dx * dx);
        dy = 0xF - r;
        x1 = *(s16 *)&D_80125C50 + dx;
        D_8011ECD0[0x1B].unk_26 = x1;
        y1 = *(u8 *)&D_80125C54 + *(s16 *)&D_80125C52;
        D_8011ECD0[0x1B].unk_2A = y1 - dy;
        D_8011ECD0[0x1B].unk_18 = D_800B5C08 - 1;
        D_8011ECD0[0x1C].unk_26 = D_80125C50;
        D_8011ECD0[0x1C].unk_2A = D_80125C52;
        if (D_800E6280.unk_F80 & 2) {
            D_8011ECD0[0x1C].unk_18 = 0xE;
            switch (D_800E6280.unk_F80 & 0x5000) {
            case 0x1000:
                if (D_80125C54 >= 3) {
                    D_80125C54 -= 2;
                    D_80125C55 = (D_80125C54 + 1) / 2;
                    y1 = D_80125C52 + D_80125C54;
                }
                break;
            case 0x4000:
                if (D_80125C54 < 0xC4) {
                    D_80125C54 += 2;
                    D_80125C55 = (D_80125C54 + 1) / 2;
                    y1 = D_80125C52 + D_80125C54;
                }
                break;
            }
        } else if (D_800E6280.unk_F80 & 8) {
            D_8011ECD0[0x1C].unk_18 = 0xE;
            switch (D_800E6280.unk_F80 & 0x5000) {
            case 0x1000:
                D_80125C52 -= 4; y1 = D_80125C52 + D_80125C54;
                break;
            case 0x4000:
                D_80125C52 += 4; y1 = D_80125C52 + D_80125C54;
                break;
            }
            switch (D_800E6280.unk_F80 & 0xA000) {
            case 0x8000:
                D_80125C50 -= 4;
                x1 -= 4;
                break;
            case 0x2000:
                D_80125C50 += 4;
                x1 += 4;
                break;
            }
        } else {
            D_8011ECD0[0x1C].unk_18 = 0xD;
        }
        func_80049FF0(D_80125C50, D_80125C52, x1, y1 - dy, 0xF8B0C8, 0xF8B0C8, 1);
        if (D_80125C50 >= 0x201) {
            D_80125C50 = -0x200;
        }
        if (D_80125C50 < -0x200) {
            D_80125C50 = 0x200;
        }
        if (D_80125C52 >= 0x201) {
            D_80125C52 = -0x200;
        }
        if (D_80125C52 < -0x200) {
            D_80125C52 = 0x200;
        }
    }
}
