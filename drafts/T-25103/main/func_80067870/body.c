/* T-25103 draft of main:func_80067870: 75 funcdiff --resolve rows (from 122 for the T-17002 draft today).
 * Real changes against T-17002: `(-dx * 2) / 3` (the original negates then shifts, no `at` multiply expansion),
 * `D_80125C55--` (no CVT on the decrement, so the byte stays a ugen temporary as in the original),
 * `x1 = dx + D_80125C50` and the two 0x8-branch case bodies on one line (autotweak), no redundant K&R casts.
 * FAKE (T-25103): `D_8011F46A = (u32)D_80125C52;` adds a fourth use of cvt(D_80125C52) so that range is coloured
 * before the sub(15, ret) phantom and takes $a0 as in the original.
 * FAKE (T-17002): the two empty `if ((s32)D_80125C54) {}` tests.
 * Left: D_800B5C08 is not one $a3 range (the phantom takes $a1, unk_F80 then $a3), which shifts the ugen ring by
 * one from word 14 to word 97; D_80125C54's global range is $a3 beside cvt(D_80125C54) in $a2 (T-0018 promo gap).
 * See NOTES.md. */
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
        if (D_800E6280.unk_F88 >> 16) {
            D_80125C55 = (D_80125C54 + 1) / 2;
        }
        if ((D_800E6280.unk_10F8 & 0x3F) == 0x3F && D_80125C55 != 0) {
            D_80125C55--;
        }
        dx = (rsin((D_800E6280.unk_10F8 & 0x7F) << 5) * D_80125C55) >> 12;
        D_8011F41C = ((-dx * 2) / 3) << 12;
        dy = 0xF - SquareRoot0(D_80125C54 * D_80125C54 - dx * dx);
        if ((s32)D_80125C54) {
        }
        x1 = dx + D_80125C50;
        D_8011F422 = x1;
        y1 = D_80125C52 + D_80125C54;
        D_8011F426 = y1 - dy;
        D_8011F414 = D_800B5C08 - 1;
        D_8011F466 = D_80125C50;
        D_8011F46A = (u32)D_80125C52;
        if (D_800E6280.unk_F80 & 2) {
            D_8011F458 = 0xE;
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
            D_8011F458 = 0xE;
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
            D_8011F458 = 0xD;
        }
        if ((s32)D_80125C54) {
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
