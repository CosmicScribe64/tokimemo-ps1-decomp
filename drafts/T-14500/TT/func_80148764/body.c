s32 func_80148764(void) {
    volatile u8 *p; /* FAKE: volatile forces the p[5] reload. T-11560 */

    p = D_801569E8;
    if (p[5] == 0 && p[6] == 0) {
        return 1;
    }
    if (p[5] == 3 && p[6] == 3) {
        return 2;
    }
    return 0;
}
