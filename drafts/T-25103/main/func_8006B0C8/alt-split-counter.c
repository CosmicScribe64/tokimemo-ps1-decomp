/* T-25103 alternative, not the best body: the compaction, clear and month loops use a separate counter m.
 * The inner SR order matches the original; the base constant is allocated again. See NOTES.md. */
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
    s32 m;
    s32 pad[9];

    for (n = 0, m = 0; m < 8; m++) {
        if (((ScheduleEntryFlags *)&D_800E6280.unk_54C[m])->active) {
            D_800E6280.unk_54C[n++] = D_800E6280.unk_54C[m];
        }
    }
    for (m = n; m < 8; m++) {
        bzero(&D_800E6280.unk_54C[m], 4);
    }
    if (D_800E6280.unk_03F == 12) {
        for (m = 0; m < n; m++) {
            if (SCHED_DATE(m)->month == 1) {
                SCHED_DATE(m)->month = 13;
            }
        }
    }
    for (i = 0; i < n; i++) {
        min = SCHED_DATE(i)->day + SCHED_DATE(i)->month * 100;
        k = i;
        for (j = i + 1; j < n; j++) {
            keys[j] = SCHED_DATE(j)->day + SCHED_DATE(j)->month * 100;
            do {
                if (keys[j] < min) {
                    min = keys[j];
                    k = j;
                }
            } while (0);
        }
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
#else
INCLUDE_ASM("asm/nonmatchings/main/800674B0", func_8006B0C8);
#endif
