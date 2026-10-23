/* T-25301 alternative draft, 18 funcdiff rows (not a match, not built): the mask statements in
 * d, c, b, a order on one line give the original's as1 order, but the ugen temporaries follow that
 * order (d t6 .. a t9; original b t6, c t7, a t8, d t9). FAKEs: both empty tests, the goto label,
 * the one-line masks and the (u32) cast selector store (alias kill for c). See NOTES.md. */
s32 func_8013AF78(s32 arg0) {
    s32 a;
    s32 b;
    s32 sel[3];
    s32 c;
    s32 d;

    if (arg0) {
    }
    for (d = 0; d < 3; d++) {
        sel[d] = 0;
    }
    d = D_80120C2B & 0x7F; c = D_80120BA3 & 0x7F; b = D_80120B5F & 0x7F; a = D_80120BE7 & 0x7F;
    if (arg0 == 0) {
        D_80120BE7 = a;
        D_80120B5F = b;
        D_80120BA3 = c;
        D_80120C2B = d;
        goto ret0;
    ret0:
        return 0;
    }
    ((s32 *)(u32)sel)[D_8014D0D4 % 3] = 1;
    D_80120BE7 = a;
    if (sel[0] != 0) {
        a = (u8)(a | 0x80);
        c = (u8)(c | 0x80);
        D_80120BE7 = a;
    }
    D_80120B5F = b;
    if (sel[1] != 0) {
        b = (u8)(b | 0x80);
        D_80120B5F = b;
    }
    D_80120C2B = d;
    D_80120BA3 = c;
    if (sel[2] != 0) {
        d = (u8)(d | 0x80);
        D_80120C2B = d;
        D_80120BA3 = c = (u8)(c | 0x80);
    }
}
