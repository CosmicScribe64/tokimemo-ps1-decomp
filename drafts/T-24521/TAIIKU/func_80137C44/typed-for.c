s32 func_80137C44(void) {
    s32 i;
    s32 x;
    s16 t;
    TaiRunnerA *r;
    DisplayActor44 *p;

    r = &D_80149208.player;
    for (i = 0; i < 4; i++, r++) {
        if (r->unk_00 != 0) {
            ((DisplayActor44 *)D_8011ECD0)[107 + i].unk_26 = ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26 - 8;
            if (i == 0) {
                t = D_801492B8[D_80149208.player.unk_04];
                if (D_80122E94 == 1) {
                    ((DisplayActor44 *)D_8011ECD0)[107].unk_18 = (((s16)((DisplayActor44 *)D_8011ECD0)[46 + D_801491D8].unk_16 >> 3) * 6) + t;
                } else {
                    ((DisplayActor44 *)D_8011ECD0)[107].unk_18 = (((s16)((DisplayActor44 *)D_8011ECD0)[46 + r->unk_02].unk_16 >> 3) * 6) + t;
                }
            } else {
                ((DisplayActor44 *)D_8011ECD0)[107 + i].unk_18 = (((s16)((DisplayActor44 *)D_8011ECD0)[46 + r->unk_02].unk_16 >> 3) * 6) + D_801492B8[r->unk_03];
            }
            if ((i + 0x90) == ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_18) {
                if (D_80122E94 == 1) {
                    ((DisplayActor44 *)D_8011ECD0)[107 + i].unk_18 = D_80149320[D_801491DC] * 6 + 5;
                } else {
                    ((DisplayActor44 *)D_8011ECD0)[107 + i].unk_18 = (((s16)((DisplayActor44 *)D_8011ECD0)[46 + r->unk_02].unk_16 >> 3) * 6) + 5;
                }
            }
        }
    }
    r = &D_80149208.player;
    p = (DisplayActor44 *)D_8011ECD0;
    do {
        if (((r[1].unk_05 == 2 && p[108].unk_02 != 0) || r[1].unk_00 == 1) && ((x = r[1].unk_20.halves.high - D_80149298.halves.high) < 0x140) && x >= -0x13F) {
            p[108].unk_03 = 0xA4;
        } else {
            p[108].unk_03 = 0;
        }
        p++;
        r++;
    } while (p != (DisplayActor44 *)D_8011ECD0 + 3);
    return D_80149298.halves.high;
}


/* Display-record view shared with the matched game-C sibling. T-24008. */
#define DISP(n) (((DisplayActor44 *)D_8011ECD0)[n])
/* Keep the explicit final else and assign result slot zero first: uopt spills
 * the result-loop pointer and writes the promoted first byte after its neighbors. T-24008. */
