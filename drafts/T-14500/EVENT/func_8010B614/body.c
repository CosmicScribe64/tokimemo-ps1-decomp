void func_8010B614(void) {
    u8 *p;
    s32 i;
    D_800B1746 = i = 4;
    func_8004BC20(i);
    p = D_800B0860 + D_800B1746 * 0x34;
    p[0x1B0] &= 0xFFFD;
    func_80011DFC();
}
