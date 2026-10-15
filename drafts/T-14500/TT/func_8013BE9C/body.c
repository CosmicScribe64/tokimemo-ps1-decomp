/* NON_MATCHING: T-10320, saved pointers swap $a2/$a3 (base vs arg0) and the constant stores schedule one slot apart, 23 lines */
void func_8013BE9C(u8 *arg0) {
    u8 *base;
    u8 *p;

    base = D_80158A90;
    func_80133AD0(0x606);
    p = func_8013BD10(base, base + 0xD80);
    *(s32 *)(p + 0x20) = *(s32 *)(arg0 + 0xC);
    *(s32 *)(p + 0x24) = *(s32 *)(arg0 + 0x10);
    *(s16 *)(p + 0x14) = *(s16 *)arg0;
    *(s32 *)(p + 0x30) = 0;
    *(s32 *)(p + 0x34) = 0;
    *(s32 *)(p + 0x2C) = 0;
    *(s32 *)(p + 0x28) = 0;
    p[0] = 1;
    *(s16 *)(p + 0x16) = *(s16 *)(arg0 + 2);
    p[0x40] = 0x80;
    *(s16 *)(p + 0x10) = 6;
    *(s16 *)(p + 0x62) = 0;
    *(s16 *)(p + 0x50) = 0;
    *(s16 *)(p + 4) = 0x90;
    p[0xD] = 0;
    p[0xE] = 0;
    p[0xF] = 0;
    *(s16 *)(p + 6) = 0;
    *(s16 *)(p + 8) = 0;
    *(s16 *)(p + 0xA) = 0;
    *(s32 *)(p + 0x38) = D_8014E34C;
    func_8013BE08(p, 0x2C);
}
