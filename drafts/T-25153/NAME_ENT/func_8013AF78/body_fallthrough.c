/* T-25153 draft, 61 funcdiff rows (not a match): best register structure with the original exit
 * shape (b, c, a in $a1, $v1, $a2). FAKE: empty `if (arg0) {}` (arg0 bit order) and the (u32)
 * cast selector store (alias kill for c). Left: d and the loop end bound need $v0. See NOTES.md. */
s32 func_8013AF78(s32 arg0) {
    s32 a;
    s32 b;
    s32 sel[3];
    s32 c;
    s32 d;

    if (arg0) {
    }
    for (c = 0; c < 3; c++) {
        sel[c] = 0;
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
        return 0;
    }
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
        d = (u8)(d | 0x80);
        D_80120C2B = d;
        D_80120BA3 = c | 0x80;
    }
}
