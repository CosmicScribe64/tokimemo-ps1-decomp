/* T-25153 draft, 57 funcdiff rows (not a match). FAKE: the empty `if (arg0) {}` gives arg0 the first
 * uopt bit so it keeps $a0; the (u32) cast on sel makes the selector store alias the globals, so
 * c's mask is not copy-propagated into the sel[0] arm (one range in $v1). `d = 0; ... return d;`
 * reproduces the original's nonzero exit value but costs a final move. See NOTES.md. */
s32 func_8013AF78(s32 arg0) {
    s32 a;
    s32 b;
    s32 sel[3];
    s32 c;
    s32 d;

    if (arg0) {
    }
    for (b = 0; b < 3; b++) {
        sel[b] = 0;
    }
    b = D_80120B5F & 0x7F;
    c = D_80120BA3 & 0x7F;
    a = D_80120BE7 & 0x7F;
    d = D_80120C2B & 0x7F;
    if (arg0 == 0) {
        D_80120BE7 = a;
        D_80120B5F = b;
        D_80120BA3 = c;
        D_80120C2B = d;
        d = 0;
    } else {
        ((s32 *)(u32)sel)[D_8014D0D4 % 3] = 1;
        D_80120BE7 = a;
        if (sel[0] != 0) {
            c = (u8)(c | 0x80);
            D_80120BE7 = a | 0x80;
        }
        D_80120B5F = b;
        if (sel[1] != 0) {
            D_80120B5F = b | 0x80;
        }
        D_80120C2B = d;
        D_80120BA3 = c;
        if (sel[2] != 0) {
            d |= 0x80;
            D_80120C2B = d;
            D_80120BA3 = c | 0x80;
        }
    }
    return d;
}
