/* T-25406 second draft of main:func_80067870: 15 funcdiff rows, 240 words. Same block structure as body.c, plus
 * every read of D_80125C54 through `*(u8 *)&D_80125C54` (FAKE) and no empty test before the call. tools/cvt_pass.py
 * then writes no CVT for those reads, cvt(D_80125C54) disappears, and the global takes $a2 everywhere as in the
 * original (the T-0018 promo rows of body.c are gone). New differences: uopt puts the promoted variable first in the
 * y1 sums (`addu t0,a2,a0` against `addu t1,a0,a2`, 5 rows) and y1 outranks x1 (0.857 against 0.75), so x1/y1 take
 * t1/t0. The D_80125C50 reload of body.c is still there. Not in the tree. */
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
            D_80125C55 = (*(u8 *)&D_80125C54 + 1) / 2;
        }
        if ((D_800E6280.unk_10F8 & 0x3F) == 0x3F && D_80125C55 != 0) {
            D_80125C55--;
        }
        dx = (rsin((D_800E6280.unk_10F8 & 0x7F) << 5) * D_80125C55) >> 12;
        D_8011F41C = ((-dx * 2) / 3) << 12;
        dy = 0xF - SquareRoot0(*(u8 *)&D_80125C54 * *(u8 *)&D_80125C54 - dx * dx);
        if (*(u8 *)&D_80125C54) {
        }
        x1 = D_80125C50 + dx;
        D_8011ECD0[0x1B].unk_26 = x1;
        if (*(u8 *)&D_80125C54) {
        }
        y1 = D_80125C52 + *(u8 *)&D_80125C54;
        D_8011ECD0[0x1B].unk_2A = D_80125C52 + *(u8 *)&D_80125C54 - dy;
        D_8011ECD0[0x1B].unk_18 = D_800B5C08 - 1;
        D_8011ECD0[0x1C].unk_26 = D_80125C50;
        D_8011ECD0[0x1C].unk_2A = D_80125C52;
        if (D_800E6280.unk_F80 & 2) {
            D_8011ECD0[0x1C].unk_18 = 0xE;
            switch (D_800E6280.unk_F80 & 0x5000) {
            case 0x1000:
                if (*(u8 *)&D_80125C54 >= 3) {
                    D_80125C54 -= 2;
                    D_80125C55 = (*(u8 *)&D_80125C54 + 1) / 2;
                    y1 = D_80125C52 + *(u8 *)&D_80125C54;
                }
                break;
            case 0x4000:
                if (*(u8 *)&D_80125C54 < 0xC4) {
                    D_80125C54 += 2;
                    D_80125C55 = (*(u8 *)&D_80125C54 + 1) / 2;
                    y1 = D_80125C52 + *(u8 *)&D_80125C54;
                }
                break;
            }
        } else if (D_800E6280.unk_F80 & 8) {
            D_8011ECD0[0x1C].unk_18 = 0xE;
            switch (D_800E6280.unk_F80 & 0x5000) {
            case 0x1000:
                D_80125C52 -= 4; y1 = D_80125C52 + *(u8 *)&D_80125C54;
                break;
            case 0x4000:
                D_80125C52 += 4; y1 = D_80125C52 + *(u8 *)&D_80125C54;
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
