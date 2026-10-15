/* FAKE: dead initial assignments preserve induction-register numbering. T-10850 */
s32 func_80055A38(s32 arg0) {
    s32 b;
    s32 n;
    s32 sum;
    s32 j;
    u8 *p;
    u8 *q;
    u8 *r;

    n = 0;
    b = 0;
    p = D_80123324;
    r = D_80123120;
    q = D_80123324;
    for (; b < 2; b++) {
        n = 0;
        p = q;
        sum = 0;
        for (j = 4; j < 0xF00; j++) {
            if (*p != 0) {
                n++;
            }
            sum += *p++;
        }
        if (sum == *(s32 *)(r + 0x200) && n != 0) {
            D_800E6280.unk_1117 = b;
            return 1;
        }
        q += 0xF00;
        r += 0xF00;
    }
    return -1;
}
