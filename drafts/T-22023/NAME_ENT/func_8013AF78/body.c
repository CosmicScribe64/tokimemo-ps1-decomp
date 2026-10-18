/* T-22023: about 50 diff lines. Structure from the asm: the zeroing loop is `for (i = 0; i < 3; i++) sel[i] = 0` through a pointer (sltu loop; a plain for gives bne, a do/while on a pointer gives the sltu form),
   the four byte globals are loaded raw into registers ($a2, $a1, $v1, $v0), masked with 0x7F into ugen temps ($t8, $t6, $t7, $t9) and copied back; frame is 8 bytes larger than the draft. */
s32 func_8013AF78(s32 arg0) {
    s32 pad[2];
    s32 sel[3];
    s32 *p;
    u8 a;
    u8 b;
    u8 c;
    u8 d;

    p = sel;
    do {
        *p++ = 0;
    } while (p < &sel[3]);
    a = D_80120BE7;
    b = D_80120B5F;
    c = D_80120BA3;
    d = D_80120C2B;
    b &= 0x7F;
    c &= 0x7F;
    a &= 0x7F;
    d &= 0x7F;
    if (arg0 == 0) {
        D_80120BE7 = a;
        D_80120B5F = b;
        D_80120BA3 = c;
        D_80120C2B = d;
        return 0;
    }
    sel[D_8014D0D4 % 3] = 1;
    D_80120BE7 = a;
    if (sel[0] != 0) {
        c |= 0x80;
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
