void func_80135ACC(s32 arg0) {
    s32 i;
    s32 j;
    s32 d;
    s32 r0;
    s32 r1;
    s32 r2;
    s32 r3;
    s32 r4;
    s32 r5;
    s32 r6;
    s32 r7;
    Fix32 x;
    Fix32 y;
    s32 px;
    s32 py;

    for (j = 0, py = -25; j < 12; j++, py += 25) {
        for (i = 0, px = -25; i < 15; i++, px += 25) {
            x.h[1] = px;
            y.h[1] = py;
            d = func_80132A74(D_80139228[arg0].unk_00.h[1], D_80139228[arg0].unk_04.h[1], x.h[1], y.h[1]);
            r0 = D_80139278[arg0] * 5;
            r1 = r0 + 5;
            if (r0 * r0 <= d) {
                if (d <= r1 * r1) {
                    D_801392AC[i][j].unk_00.w += (x.w - D_80139228[arg0].unk_00.w) * 2 / r0;
                    D_801392AC[i][j].unk_04.w += (y.w - D_80139228[arg0].unk_04.w) * 2 / r0;
                } else {
                    r2 = r0 + 10;
                    r3 = r0 + 15;
                    if (r2 * r2 < d) {
                        if (d <= r3 * r3) {
                            D_801392AC[i][j].unk_00.w -= (x.w - D_80139228[arg0].unk_00.w) * 3 / r2;
                            D_801392AC[i][j].unk_04.w -= (y.w - D_80139228[arg0].unk_04.w) * 3 / r2;
                        } else {
                            r4 = r0 + 20;
                            r5 = r0 + 25;
                            if (r4 * r4 < d) {
                                if (d <= r5 * r5) {
                                    D_801392AC[i][j].unk_00.w += (x.w - D_80139228[arg0].unk_00.w) * 3 / r4;
                                    D_801392AC[i][j].unk_04.w += (y.w - D_80139228[arg0].unk_04.w) * 3 / r4;
                                } else {
                                    r6 = r0 + 30;
                                    r7 = r0 + 35;
                                    if (r6 * r6 < d && d <= r7 * r7) {
                                        D_801392AC[i][j].unk_00.w -= (x.w - D_80139228[arg0].unk_00.w) * 2 / r6;
                                        D_801392AC[i][j].unk_04.w -= (y.w - D_80139228[arg0].unk_04.w) * 2 / r6;
                                    }
                                }
                            }; /* FAKE: empty statement (found with the permuter) gives the original's register order, j in s2 before the grid base and the constant 3; T-20920 */
                        }
                    }
                }
            }
        }
    }
}
