/* T-25010 draft of main func_80065964 (424 B), about 68 rows (funcdiff --resolve). Start from this, not from the
 * T-25001 drafts: a `for`/`do` loop with a constant 15-trip count is unrolled by IDO (216 instructions); the
 * `goto` loop below stays rolled like the original (`li at,15; bne`), so only the frame and the registers differ.
 * Left: the original has frame 0x68, saves s0/s1 (q and ty), and stores p, y0, y1 to sp+0x28, x0 to 0x24 and x1
 * to 0x20 without reloading them; each of x0, y0, x1, y1 then lives in two registers (copies v0, a3, t0, t3; the
 * four corner tests use (v0,a3), (t0,t1), (t2,t3), (t4,t5)), and the loop rematerialises the D_80125C10 base and
 * the 15. That is uopt live-range splitting under register pressure; this draft has no frame and keeps one
 * register per value (y1 is rematerialised as y0 + 15 per use).
 * Tried without the frame appearing: eight corner variables (copies propagated), a local `s32 c[4]` (scalarised,
 * no stores), `s32 *pa = &x0; ...` (y1 then kept in a register, still no stores), `(void)&x0`, all coordinates
 * written as inline expressions (CSE'd, no frame). */
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
    y0 = p[1] + dy;
    x1 = x0 + 19;
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
    i = 0;
loop:
    {
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
        i++;
    }
    if (i != 15) goto loop;
    return r;
}
