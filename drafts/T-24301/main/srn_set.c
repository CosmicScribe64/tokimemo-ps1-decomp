/* T-24301 draft (autotweak best), ~70 rows: written from the asm; signed s32 field copies (K&R promotes u8 to
 * unsigned); the original keeps y in $t1 and its running copy in $s3, spills the two u16 fields at 0x7C/0x78
 * and cols in $fp + 0x64. D_8012175C is record 0's +0xC byte. */
s32 srn_set(s32 idx) {
    u8 *p;
    u32 *data;
    s32 count;
    s32 row;
    s32 col;
    s32 rows;
    s32 cols;
    s32 x;
    s32 y;
    s32 a;
    s32 yend;
    s32 b;
    s32 nw;
    s32 nh;
    s32 cw;
    s32 ch;
    s32 ox;
    s32 oy;
    s32 vw;
    s32 vh;
    u32 v;

    p = D_80121750 + idx * 0x28;
    nh = p[0xF];
    ch = p[0x11];
    nw = p[0xE];
    cw = p[0x10];
    ox = p[0x12];
    oy = p[0x13];
    vw = p[0x14];
    vh = p[0x15];
    x = *(s16 *)(p + 2);
    y = *(s16 *)(p + 6);
    data = *(u32 **)(p + 8);
    if (nh == 0 || nw == 0) {
        return -1;
    }
    a = *(u16 *)(p + 0x16);
    b = *(u16 *)(p + 0x18);
    count = *(s32 *)(p + 0x1C);
    cols = vw * cw / nw;
    rows = vh * ch / nh;
    yend = y + nh * oy;
    for (row = 0; row < rows; row++) {
        if (yend >= -0x77) {
            for (col = 0; col < cols; col++) {
                if (y >= 0x79) {
                    return 0;
                }
                if ((cw * col + x) + ox * nw >= -0x9F) {
                    v = *data & 0xFFFF;
                    if ((*data & 0xFFFF) != 0) {
                        mod_set_gpu(0, v, (cw * col + x), y, a, b, D_80121750[0xC]);
                    }
                }
                data++;
                if (--count == 0) {
                    return 0;
                }
            }
        }
        y += ch;
        yend += ch;
    }
}
