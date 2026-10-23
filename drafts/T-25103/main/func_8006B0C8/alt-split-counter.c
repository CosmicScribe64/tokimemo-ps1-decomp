/* T-25103 structural lead, not the best row count (254 funcdiff rows, 130 for body.c): a separate counter m for the
 * compaction, clear and month loops, and the compaction test written as `if (!active) continue;`. No swap label is
 * needed and the base constant stays unallocated. The inner sort loop now has the original's SR order and ugen
 * temporaries; left: uopt's colouring (first-loop walk/end in v0/v1, m in a1, min and the entry walk swapped between
 * a0/a1, remainder counts in a3), the swap/undo ring one off from word 228, the month preheader order (words 71-74,
 * 265-268) and the outer t5 increment (word 136). The do-while(0) and pad[9] are the FAKEs of body.c. See NOTES.md. */
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
        if (!((ScheduleEntryFlags *)&D_800E6280.unk_54C[m])->active) {
            continue;
        }
        D_800E6280.unk_54C[n++] = D_800E6280.unk_54C[m];
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
