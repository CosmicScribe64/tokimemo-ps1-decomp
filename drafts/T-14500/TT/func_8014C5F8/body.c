/* FAKE: shared zero local preserves constant allocation. T-10850 */
void func_8014C5F8(u8 *arg0) {
    u8 *p;
    s32 zero;

    p = (u8 *)func_8014AF00(D_80158A98, D_80158A98 + 0x1E00);
    if (p != 0) {
        zero = 0;
        p[0] = 1;
        *(s16 *)(p + 4) = 0x71;
        p[0xD] = p[0xE] = zero;
        *(s16 *)(p + 0xA) = 0;
        *(s16 *)(p + 8) = 0;
        *(s16 *)(p + 6) = 0;
        p[0xF] = 0;
        *(s32 *)(p + 0x20) = *(s32 *)(arg0 + 0xC);
        *(s32 *)(p + 0x24) = *(s32 *)(arg0 + 0x10);
        *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
        *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
        func_8014C580(p);
    }
}
