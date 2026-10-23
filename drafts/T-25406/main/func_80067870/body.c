/* NON_MATCHING: T-25406, 14 funcdiff rows (240 words against 238). Every uopt colour but one is the original's:
 * the empty tests after dy and between the x1 and y1 statements (FAKE) put the x1 sum and the y1 sum in different
 * blocks, so both take $v0 and the forwarded `15 - ret` range takes $v1; cvt(D_80125C52) then gets $a0, unk_F80 $a1
 * and D_800B5C08 one $a3 range, as in the original. Left: (1) the D_8011ECD0[0x1C].unk_26 store sits after the block
 * split, so D_80125C50 is loaded again (`lui t4; lh t4`, 2 words); the original reuses the x1 load. (2) The T-0018
 * promo gap: the D_80125C54 global range takes $a3 beside cvt(D_80125C54) in $a2 (10 rows). See
 * drafts/T-25406/main/func_80067870/NOTES.md. */
void func_80067870(void) {
    s32 x1;
    s16 dx;
    s16 dy;
    s32 y1;

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
        dy = 0xF - SquareRoot0(D_80125C54 * D_80125C54 - dx * dx);
        if ((s32)D_80125C54) { /* FAKE T-25406: ends the uopt block after dy (see the guard comment) */
        }
        x1 = D_80125C50 + dx;
        D_8011ECD0[0x1B].unk_26 = x1;
        if ((s32)D_80125C54) { /* FAKE T-25406: puts the x1 sum and the y1 sum in different uopt blocks */
        }
        y1 = D_80125C52 + D_80125C54;
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
        if ((s32)D_80125C54) { /* FAKE T-17002, kept by T-25406: an extra cvt(D_80125C54) use that lowers its priority so it takes $a2 */
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
