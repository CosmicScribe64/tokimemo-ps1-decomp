void func_8006B0C8(void) {
    s32 i;
    s32 j;
    s32 n;
    s32 k;
    GsWord tmp;
    s32 keys[8];
    s32 min;
    s32 pad[9]; /* FAKE: T-25608, unused; the original frame (0xB0, a0 spill at 0x40) has nine more words below g. */
    GameState *g;

    g = &D_800E6280;
    for (n = 0, j = 0; j < 8; j++) {
        if (((ScheduleEntryFlags *)&g->unk_54C[j])->active) {
            g->unk_54C[n++] = g->unk_54C[j];
        }
    }
    for (j = n; j < 8; j++) {
        bzero(&g->unk_54C[j], 4);
    }
    if (g->unk_03F == 12) {
        for (j = 0; j < n; j++) {
            if (SCHED_DATE(j)->month == 1) {
                SCHED_DATE(j)->month = 13;
            }
        }
    }
    i = 0;
    while (i - n < 0) {
        min = SCHED_DATE(i)->day + SCHED_DATE(i)->month * 100;
        k = i;
        for (j = i + 1; j < n; j++) {
            keys[j] = SCHED_DATE(j)->day + SCHED_DATE(j)->month * 100;
            if (keys[j] < min) {
                min = keys[j];
                k = j;
            }
        }
        /* FAKE: T-25608 (from T-25103), the label splits the swap off the inner-loop exit; without it uopt keeps
         * &D_800E6280 in a register from the first loop to the swap, the original never does. */
        goto swap;
    swap:
        tmp = g->unk_54C[i];
        g->unk_54C[i] = g->unk_54C[k];
        g->unk_54C[k] = tmp;
        i++;
    }
    for (j = 0; j < n; j++) {
        if (SCHED_DATE(j)->month == 13) {
            SCHED_DATE(j)->month = 1;
        }
    }
}
