s32 srn_set(s32 arg0) {
    SrnRec28 *rec;
    s32 w0;
    s32 h0;
    s32 tw;
    s32 th;
    s32 ox;
    s32 oy;
    s32 cw;
    s32 ch;
    s32 xoff;
    s32 rows;
    s32 yoff;
    u32 *src;
    s32 row;
    s32 left;
    s32 col;
    s32 wid;
    s32 hgt;
    s32 x;
    s32 y;
    s32 cols;
    s32 ybase;

    rec = (SrnRec28 *)(D_80121750 + arg0 * 0x28);
    h0 = rec->unk_0F;
    th = rec->unk_11;
    w0 = rec->unk_0E;
    tw = rec->unk_10;
    ox = rec->unk_12;
    oy = rec->unk_13;
    cw = rec->unk_14;
    ch = rec->unk_15;
    xoff = rec->unk_02;
    yoff = rec->unk_06;
    src = rec->unk_08;
    if (h0 == 0 || w0 == 0) {
        return -1;
    }
    wid = rec->unk_16;
    hgt = rec->unk_18;
    left = rec->unk_1C;
    row = 0;
    cols = cw * tw / w0;
    rows = ch * th / h0;
    if (rows > 0) {
        y = yoff;
        ybase = y + h0 * oy;
        do {
            if (ybase >= -0x77 && cols > 0) {
                col = 0;
                do {
                    if (y >= 0x79) {
                        return 0;
                    }
                    x = tw * col + xoff;
                    if (x + ox * w0 >= -0x9F) {
                        if ((*src & 0xFFFF) != 0) {
                            mod_set_gpu(0, *src & 0xFFFF, x, y, wid, hgt, D_80121750[0xC]);
                        }
                    }
                    left--;
                    src++;
                    if (left == 0) {
                        return 0;
                    }
                    col++;
                } while (col != cols);
            }
            row++;
            ybase += th;
            y += th;
        } while (row < rows);
    }
}
