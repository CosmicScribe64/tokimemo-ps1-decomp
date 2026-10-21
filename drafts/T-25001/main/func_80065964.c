/* T-25001: func_80065964 drafts. Natural four-corner test (f_nat) and a block-local macro per corner (f_mac) and do-while loop (f_do) all compile to a frameless leaf whose loop uopt splits around idx (beqz a0 peel). The original has frame 0x68, saves s0/s1, keeps dead stores of p/y0/y1 to sp+0x28, x0 to 0x24, x1 to 0x20, and eight coordinate registers (four copies: v0=x0, t0=x1, a3=y0, t3=y1), one pair per corner test. Unknown source shape; candidate ideas: live-range splitting from many block-scoped locals, an address-taken local. */
s32 func_80065964(s32 idx, s32 dx, s32 dy) {
    s16 *p;
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    s32 i;
    s32 r;
    s16 *q;
    s32 tx;
    s32 ty;

    p = (s16 *)&D_80125C10[idx * 4];
    r = 1;
    x0 = p[0] + dx;
    x1 = x0 + 19;
    y0 = p[1] + dy;
    y1 = y0 + 15;
    if (x0 < -160) {
        return 0;
    }
    if (x1 > 160) {
        return 0;
    }
    if (y0 < -120) {
        return 0;
    }
    if (y0 > 120) {
        return 0;
    }
    for (i = 0; i < 15; i++) {
        if (i != idx) {
            q = (s16 *)&D_80125C10[i * 4];
            tx = q[0];
            ty = q[1];
            if (x0 >= tx && x0 < tx + 20 && y0 >= ty && y0 < ty + 16) {
                r = 0;
            }
            if (x1 >= tx && x1 < tx + 20 && y0 >= ty && y0 < ty + 16) {
                r = 0;
            }
            if (x0 >= tx && x0 < tx + 20 && y1 >= ty && y1 < ty + 16) {
                r = 0;
            }
            if (x1 >= tx && x1 < tx + 20 && y1 >= ty && y1 < ty + 16) {
                r = 0;
            }
        }
    }
    return r;
}

/* macro variant */
#define HIT(px, py) { s32 hx = (px); s32 hy = (py); if (hx >= tx && hx < tx + 20 && hy >= ty && hy < ty + 16) { r = 0; } }
s32 func_80065964(s32 idx, s32 dx, s32 dy) {
    s16 *p;
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    s32 i;
    s32 r;
    s16 *q;
    s32 tx;
    s32 ty;

    p = (s16 *)&D_80125C10[idx * 4];
    r = 1;
    x0 = p[0] + dx;
    x1 = x0 + 19;
    y0 = p[1] + dy;
    y1 = y0 + 15;
    if (x0 < -160) {
        return 0;
    }
    if (x1 > 160) {
        return 0;
    }
    if (y0 < -120) {
        return 0;
    }
    if (y0 > 120) {
        return 0;
    }
    for (i = 0; i < 15; i++) {
        if (i != idx) {
            q = (s16 *)&D_80125C10[i * 4];
            tx = q[0];
            ty = q[1];
            HIT(x0, y0);
            HIT(x1, y0);
            HIT(x0, y1);
            HIT(x1, y1);
        }
    }
    return r;
}
#undef HIT
