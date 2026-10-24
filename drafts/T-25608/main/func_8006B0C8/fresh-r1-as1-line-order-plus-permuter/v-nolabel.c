void func_8006B0C8(void) {
    s32 i;
    s32 j;
    s32 n;
    s32 k;
    GsWord tmp;
    s32 keys[8];
    s32 min;
    s32 pad[9];
    GameState *g;
    g = &D_800E6280;
    for (n = 0, j = 0; j < 8; j++) {
        if (((ScheduleEntryFlags *)&g->u54C.a[j])->active) {
            g->u54C.a[n++] = g->u54C.a[j];
        }
    }
    for (j = n; j < 8; j++) {
        bzero(&g->u54C.a[j], 4);
    }
    if (g->unk_03F == 12) {
        for (j = 0; j < n; j++) {
            if (((ScheduleEntryDate *)&g->u54C.a[j])->month == 1) {
                ((ScheduleEntryDate *)&g->u54C.a[j])->month = 13;
            }
        }
    }
    i = 0;
    while (i < n) {
        min = ((ScheduleEntryDate *)&g->u54C.a[i])->day + ((ScheduleEntryDate *)&g->u54C.a[i])->month * 100;
        k = i;
        for (j = i + 1; j < n; j++) {
            keys[j] = ((ScheduleEntryDate *)&g->u54C.a[j])->day + ((ScheduleEntryDate *)&g->u54C.a[j])->month * 100;
            if (keys[j] < min) {
                min = keys[j];
                k = j;
            }
        }
        tmp = g->u54C.a[i];
        g->u54C.a[i] = g->u54C.a[k];
        g->u54C.a[k] = tmp;
        i++;
    }
    for (j = 0; j < n; j++) {
        if (((ScheduleEntryDate *)&g->u54C.a[j])->month == 13) {
            ((ScheduleEntryDate *)&g->u54C.a[j])->month = 1;
        }
    }
}
