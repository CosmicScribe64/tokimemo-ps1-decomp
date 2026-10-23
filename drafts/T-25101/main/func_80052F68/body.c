/* T-25101 best draft of main:func_80052F68: funcdiff --resolve 69 rows (compared NON_MATCHING C),
 * frame 0x80 and val at sp+0x48 exact, loop 1 IV registers exact.
 * Needs in include/main_api.h, in place of the D_800B4349 / D_800B4355 / D_800B4361 lines:
 *     extern s8 D_800B4348[12];
 *     extern u8 D_800B4354[12];
 *     extern u8 D_800B4360[12];
 * and, before a default-C build can link, the three bases in config/symbol_addrs_main.txt
 * (no instruction names them; the loops start at entry 1). See NOTES.md. */
s32 func_80052F68(u8 *arg0, s16 *arg1) {
    GameState *g = &D_800E6280;
    s32 i;
    s32 j;
    s32 k;
    s32 m;
    s32 v;
    s32 sum;
    s32 x;
    s16 val[11];
    s16 *pv;

    /* FAKE: T-25101, store through a copy of the array address so uopt creates the val
     * induction pointer after arg0's (target v1 = arg0 + 3, a2 = sp + 0x4E) */
    pv = val;
    for (k = 1; k < 11; k++) {
        pv[k] = arg0[k] * 4;
        val[k] += g->unk_1BC[k].unk_0A * D_800B4348[k];
    }
    sum = g->unk_11C.unk_02;
    if (sum >= 141) {
        sum = 140;
    }
    val[2] += D_800B437C[sum / 10];
    for (i = 1; i < 11; i++) {
        for (m = 0; m < 7; m++) {
            if ((D_800B4354[i] >> (7 - m)) & 1) {
                v = (&g->unk_0FC)[7 - m].unk_02;
                if (m == 7) {
                    v /= 20;
                } else {
                    v /= 10;
                }
                if (v >= 15) {
                    v = 14;
                }
                val[i] += D_800B436C[v];
                val[i] += D_800B436C[v] / 2;
            }
            if ((D_800B4360[i] >> (7 - m)) & 1) {
                x = (&g->unk_0FC)[7 - m].unk_02;
                if (m == 7) {
                    x /= 20;
                } else {
                    x /= 10;
                }
                if (x >= 15) {
                    x = 14;
                }
                val[i] += D_800B436C[x];
            }
        }
    }
    for (m = 1; m < 11; m++) {
        sum = 0;
        for (j = 1; j < 36; j++) {
            sum += g->unk_1BC[m].unk_14[j];
        }
        if (sum >= 4) {
            val[m] += sum * 10;
        } else {
            val[m] = 0;
        }
    }
    for (m = 1; m < 11; m++) {
        arg1[m] = val[m];
    }
}
