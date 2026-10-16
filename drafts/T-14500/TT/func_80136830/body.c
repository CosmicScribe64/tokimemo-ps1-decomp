/* NON_MATCHING: T-11820, 17 lines: the head (old D_80152AF4 value, *D_80158A6C, page) differs; the original loads the old value straight into v0 and keeps the page in the ugen temp t7, here uopt copies the old value through t7 into a0 and gives the page v0. The unrolled six-record loop (i + 1 compare) matches. Tried: old/t/v locals in all orders, compare against the globals, assignment in the condition. */
void func_80136830(void) {
    u8 *c;
    u8 *p;
    s32 i;
    s32 old;
    s32 v;

    c = D_80158A6C;
    v = *(s32 *)c;
    old = D_80152AF4;
    D_80152AF0 = old;
    D_80152AF4 = v / 0x8000;
    if (old != v / 0x8000) {
        func_80133AD0(0x611);
    }
    if (*(s32 *)c == 0) {
        for (i = 0, p = D_80158AA4 + 0xC8; i < 6; i++, p += 0x64) {
            *(s16 *)(p + 0x42) = 0x3C0A;
        }
    } else {
        for (i = 0, p = D_80158AA4 + 0xC8; i < 6; i++, p += 0x64) {
            if (*(s32 *)c / 0x8000 == i + 1) {
                *(s16 *)(p + 0x42) = 0x3C09;
            } else {
                *(s16 *)(p + 0x42) = 0x3C0A;
            }
        }
    }
}
