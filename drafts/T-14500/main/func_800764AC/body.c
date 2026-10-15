s32 func_800764AC(u32 *arg0) {
    s32 padA[1];
    s32 cnt[32];
    s32 padB[1];
    s32 i;
    s32 n;
    s32 m;
    s32 idx;

    switch (D_800E6280.unk_54C[0].w & 0x1F) {
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
        return 1;
    default:
        for (i = 0; i < 32; i++) {
            cnt[i] = 0;
        }
        for (i = 0; i < 8; i++) {
            if (!((EntryFlags29 *)&D_800E6280.unk_54C[i])->b29) {
                break;
            }
            if (1) {
                cnt[((EntryFlags29 *)&D_800E6280.unk_54C[i])->idx]++;
            }
        }
        cnt[16] = 0;
        cnt[17] = 0;
        cnt[19] = 0;
        cnt[20] = 0;
        cnt[18] = 0;
        n = 0;
        m = 0;
        for (i = 0; i < 32; i++) {
            if (cnt[i] != 0) {
                n++;
            }
        }
        if (n < 2) {
            *arg0 = (u32)(D_800E6280.unk_54C[0].w << 2) >> 31;
            return 1;
        }
        for (i = 0; i < 8; i++) {
            if (!((EntryFlags29 *)&D_800E6280.unk_54C[i])->b29) {
                break;
            }
            if (cnt[((EntryFlags29 *)&D_800E6280.unk_54C[i])->idx] != 0) {
                idx = ((EntryFlags29 *)&D_800E6280.unk_54C[i])->idx;
                arg0[m] = idx;
                m++;
                cnt[((EntryFlags29 *)&D_800E6280.unk_54C[i])->idx] = 0;
                if (m >= 3) {
                    break;
                }
            }
        }
        return n;
    }
}
