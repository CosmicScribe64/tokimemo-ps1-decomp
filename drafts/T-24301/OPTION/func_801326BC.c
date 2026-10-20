/* T-24301 draft, 99 rows: IDO unrolls the first copy loop by 4 and hoists the % 4; the original
 * recomputes the modulo in every iteration of a plain pointer loop. */
/* Palette cycle: four 128-word palettes at 0x801CBEF0 (RAM above the overlay, filled at load). */
#define OPTION_PALETTES ((s32 (*)[128])0x801CBEF0)

void func_801326BC(void) {
    s32 i;
    s32 j;
    s32 t;

    if (D_8013D300 != 0) {
        ((SpriteSlot24 *)&D_801220D0)->unk_10 = D_800E6280.unk_03A;
        ((SpriteSlot24 *)&D_801220D0)->unk_11 = D_800E6280.unk_03A;
        ((SpriteSlot24 *)&D_801220D0)->unk_12 = D_800E6280.unk_03A;
        if (D_800E6280.unk_10F8 & 1) {
            ((SpriteSlot24 *)&D_801220D0)->unk_0C += 1;
            ((SpriteSlot24 *)&D_801220D0)->unk_0E -= 1;
        }
        t = D_8013D308 + 1;
        if (t > 0x100) {
            for (i = 0; i < 0x80; i++) {
                D_800E6280.unk_163C[i] = OPTION_PALETTES[D_8013D304 % 4][i];
            }
            for (j = 0; j <= t - 0x100; j++) {
                D_800E6280.unk_163C[j + 0x10] = ((s32 *)OPTION_PALETTES)[(D_8013D304 + 1) % 4 * 128 + j + 0x10];
                D_800E6280.unk_163C[j + 0x38] = OPTION_PALETTES[(D_8013D304 + 1) % 4][j + 0x38];
                if (j + 0x60 < 0x80) {
                    D_800E6280.unk_163C[j + 0x60] = OPTION_PALETTES[(D_8013D304 + 1) % 4][j + 0x60];
                }
            }
            D_8013D308 = t;
            if (t > 0x128) {
                D_8013D304 += 1;
                D_8013D308 = 0;
            }
            func_80043914(D_800E6280.unk_163C, 0x1A, 1, 1, 0);
            return;
        }
        D_8013D308 = t;
    }
}
