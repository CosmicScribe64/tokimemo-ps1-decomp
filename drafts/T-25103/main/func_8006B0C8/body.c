/* NON_MATCHING: T-25103, 130 funcdiff --resolve rows (318 words, same length); left: SR init and
 * increment order in the inner sort loop and in the two month loops, ugen ring after it. NOTES.md here. */
#ifdef NON_MATCHING
#define SCHED_DATE(i) ((ScheduleEntryDate *)&D_800E6280.unk_54C[i])
void func_8006B0C8(void) {
    s32 i;
    s32 j;
    s32 n;
    s32 k;
    GsWord tmp;
    s32 keys[8];
    s32 min;
    s32 pad[10]; /* FAKE: T-25103, unused; the original frame (0xB0, a0 spill at 0x40) has ten more words below keys. */

    for (n = 0, j = 0; j < 8; j++) {
        if (((ScheduleEntryFlags *)&D_800E6280.unk_54C[j])->active) {
            D_800E6280.unk_54C[n++] = D_800E6280.unk_54C[j];
        }
    }
    for (j = n; j < 8; j++) {
        bzero(&D_800E6280.unk_54C[j], 4);
    }
    if (D_800E6280.unk_03F == 12) {
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
            /* FAKE: T-25103, the do-while(0) starts a new ucode block, so uopt does not forward the keys[j]
             * store into a CSE register ($v0); ugen forwards it in a ring temporary, as the original. */
            do {
                if (keys[j] < min) {
                    min = keys[j];
                    k = j;
                }
            } while (0);
        }
        /* FAKE: T-25103, the label splits the swap off the inner-loop exit; without it uopt keeps
         * &D_800E6280 in a register (rlda) from the first loop to the swap, the original never does. */
        goto swap;
    swap:
        tmp = D_800E6280.unk_54C[i];
        D_800E6280.unk_54C[i] = D_800E6280.unk_54C[k];
        D_800E6280.unk_54C[k] = tmp;
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
