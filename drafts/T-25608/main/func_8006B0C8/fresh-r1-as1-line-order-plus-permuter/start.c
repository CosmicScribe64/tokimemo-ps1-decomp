/* NON_MATCHING: T-25608, 90 funcdiff --resolve rows (317 words, same length); from the T-25103 body (130 rows):
 * every access goes through a GameState pointer, so the month and undo preheaders compute the end first and the
 * inner loop sets up the keys walk first, as in the original. Left: the outer key walk's increment three slots early
 * (word 136) and the ugen scratch ring from the swap on (words 228-311). BRIEF-r1.md here. */
#ifdef NON_MATCHING
#define SCHED_DATE(i) ((ScheduleEntryDate *)&g->unk_54C[i])
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
    for (i = 0; i < n; i++) {
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
    }
    for (j = 0; j < n; j++) {
        if (SCHED_DATE(j)->month == 13) {
            SCHED_DATE(j)->month = 1;
        }
    }
}
#undef SCHED_DATE
#else
INCLUDE_ASM("asm/nonmatchings/main/800674B0", func_8006B0C8);
#endif
