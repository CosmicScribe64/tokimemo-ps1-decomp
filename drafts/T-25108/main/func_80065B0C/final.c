/* NON_MATCHING: T-25108, 9 funcdiff lines (2 instructions): the original sets the shared constant 3 ($s5) in
 * the else arm before the menu_check call, IDO after the reload of the selection index; three branch offsets
 * follow from that. drafts/T-25108/main/func_80065B0C/NOTES.md has the uopt analysis.
 *
 * Cursor step of the 15-icon menu: every third frame of the pad repeat (`unk_F6C % 3 == 2`) moves the
 * selected icon's position in D_80125C10 and the cursor record by the pad direction (`unk_F6F` pixels), unless
 * func_80065964 rejects the move, then clamps the icon to x -0x8A..0x80, y -0x5C..0x5C. Without pad input
 * it runs the menu cursor check instead. A selection change restarts the 0x20-frame blink of
 * D_800B5AA0, which scales the selected icons with rsin while `a` is 0; the loop sets the display modes of
 * the icon records 64 + i and 80 + i, and the tail sets the page counter limits as func_80065900 does.
 * The selection index is read through `g`: the stores to D_80125C10 then reload the index expression (the
 * original recomputes the address for the y store), and uopt keeps the index in $s3, re-read after each call. */
void func_80065B0C(s32 a) {
    s32 dx;
    s32 dy;
    s32 n;
    u8 old;
    s32 i;
    GameState *g = &D_800E6280;

    old = g->unk_1093;
    dx = 0;
    dy = 0;
    if (g->unk_F80 & 1) {
        if (g->unk_F6C % 3U == 2) {
            switch (g->unk_F80 & 0xA000) {
            case 0x8000:
                dx = -g->unk_F6F;
                break;
            case 0x2000:
                dx = g->unk_F6F;
                break;
            }
            /* T-25108: an unsigned selector; with a signed one the case constant 0x1000 shares the register
             * of the loop's halfword 0x1000 stores. */
            switch ((u32)g->unk_F80 & 0x5000) {
            case 0x1000:
                dy = -g->unk_F6F;
                break;
            case 0x4000:
                dy = g->unk_F6F;
                break;
            }
            if (func_80065964(g->unk_1093, dx, dy) == 0) {
                dx = 0;
                dy = 0;
            }
            ((s16 (*)[2])D_80125C10)[g->unk_1093][0] += dx;
            ((s16 (*)[2])D_80125C10)[g->unk_1093][1] += dy;
            /* record 0 fields, not D_8011ECF6/D_8011ECFA: as1 keeps the y load behind the x store */
            ((DisplayActor44 *)D_8011ECD0)[0].unk_26 += dx;
            ((DisplayActor44 *)D_8011ECD0)[0].unk_2A += dy;
            func_800654D0(g->unk_F6D);
            if (((s16 (*)[2])D_80125C10)[g->unk_1093][0] > 0x80) {
                ((s16 (*)[2])D_80125C10)[g->unk_1093][0] = 0x80;
            }
            if (((s16 (*)[2])D_80125C10)[g->unk_1093][0] < -0x8A) {
                ((s16 (*)[2])D_80125C10)[g->unk_1093][0] = -0x8A;
            }
            if (((s16 (*)[2])D_80125C10)[g->unk_1093][1] < -0x5C) {
                ((s16 (*)[2])D_80125C10)[g->unk_1093][1] = -0x5C;
            }
            if (((s16 (*)[2])D_80125C10)[g->unk_1093][1] > 0x5C) {
                ((s16 (*)[2])D_80125C10)[g->unk_1093][1] = 0x5C;
            }
        }
    } else {
        menu_check(0, ((DisplayActor44 *)D_8011ECD0)[0].unk_26, ((DisplayActor44 *)D_8011ECD0)[0].unk_2A);
        func_8004FC10(0);
    }
    if (old != D_800B5A9C) {
        D_800B5A9C = g->unk_1093;
        D_800B5AA0 = 0x20;
    } else if (D_800B5AA0 != 0) {
        D_800B5AA0--;
    }
    n = 14;
    if (g->unk_F6D == 15) {
        n = 15;
    }
    if (g->unk_F6C == 2) {
        n = 15;
    }
    for (i = 0; i < n; i++) {
        if (D_80125C90[i] == 0) {
            ((DisplayActor44 *)D_8011ECD0)[i + 80].unk_02 = 0;
            ((DisplayActor44 *)D_8011ECD0)[i + 64].unk_43 = 0x51;
            ((DisplayActor44 *)D_8011ECD0)[i + 64].unk_01 = 3;
        } else {
            ((DisplayActor44 *)D_8011ECD0)[i + 80].unk_02 = 1;
            ((DisplayActor44 *)D_8011ECD0)[i + 64].unk_01 = 4;
            ((DisplayActor44 *)D_8011ECD0)[i + 80].unk_01 = 3;
            if (((GsRecF90 (*)[2])g->unk_F90)[i][0].unk_06 != 0) {
                ((DisplayActor44 *)D_8011ECD0)[i + 64].unk_43 = 0x52;
                ((DisplayActor44 *)D_8011ECD0)[i + 80].unk_01 = 2;
                ((DisplayActor44 *)D_8011ECD0)[i + 64].unk_01 = 3;
                if (a == 0) {
                    ((DisplayActor44 *)D_8011ECD0)[i + 80].unk_1A = rsin((0x20 - D_800B5AA0) << 6) + 0x1000;
                    ((DisplayActor44 *)D_8011ECD0)[i + 80].unk_1C = rsin((0x20 - D_800B5AA0) << 6) + 0x1000;
                } else {
                    ((DisplayActor44 *)D_8011ECD0)[i + 80].unk_1A = 0x1000;
                    ((DisplayActor44 *)D_8011ECD0)[i + 80].unk_1C = 0x1000;
                }
            } else {
                ((DisplayActor44 *)D_8011ECD0)[i + 64].unk_43 = 0x53;
                if (a == 0) {
                    ((DisplayActor44 *)D_8011ECD0)[i + 80].unk_1A = 0x1000;
                    ((DisplayActor44 *)D_8011ECD0)[i + 80].unk_1C = 0x1000;
                    ((DisplayActor44 *)D_8011ECD0)[i + 64].unk_1A = 0x1000;
                    ((DisplayActor44 *)D_8011ECD0)[i + 64].unk_1C = 0x1000;
                } else {
                    ((DisplayActor44 *)D_8011ECD0)[i + 80].unk_1A = 0x1000;
                    ((DisplayActor44 *)D_8011ECD0)[i + 80].unk_1C = 0x1000;
                }
            }
        }
    }
    if (g->unk_035) {
        D_8012059A[0] = 0xE;
        if (D_8012059A[1] >= 4) {
            D_8012059A[1] = 0;
        }
    } else {
        D_8012059A[0] = 0xD;
        if (D_8012059A[1] >= 6) {
            D_8012059A[1] = 0;
        }
    }
}
