void func_8013506C(void) {
    s32 i;
    s32 t;
    u32 v;
    u32 x;
    u32 y;

    for (i = 0; i < 0x80; i++) {
        if (((u32 *)D_800E6280.unk_183C)[i] == 0) {
            /* Own statement: written inline, IDO calls both functions first. */
            t = func_800AE0D0() % 320;
            ((u32 *)D_800E6280.unk_183C)[i] = (t << 16) + func_800AE0D0() % 240;
            break;
        }
    }
    for (i = 0; i < 0x80; i++) {
        v = ((u32 *)D_800E6280.unk_183C)[i];
        /* y before x: the mask range gets $v0 as in the original. */
        y = v & 0xFFFF;
        x = v >> 16;
        if (v != 0) {
            func_80143574(x - 0xA0, y - 0x78, (x * y + D_800E6280.unk_10F8) & 0xFFFFFF);
        }
    }
}
