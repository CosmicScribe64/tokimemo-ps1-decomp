void func_80047560(void) {
    s32 sum;
    u32 i;
    s32 q;
    s32 off;
    s16 x;
    s16 y;

    if (D_801255DC != 0) {
        i = 0;
        sum = 0;
        if (D_801255DC != 0) {
            do {
                sum += i;
                i += 1;
            } while (i < D_801255DC);
        }
        q = (s16)rsin(sum * 4) / 4 / (s32)(*(s8 *)&D_801255DC + 0x100); /* FAKE: T-24303, s8 view keeps the divisor's cvt out of the global's register (a0) */
        off = (q << 16) >> 16;
        x = off + 0xA0;
        D_800E6280.unk_014[0] = (s16)q + 0xA0 + ((u32)off < 0);
        D_800E6280.unk_014[1] = x;
        D_800E6280.unk_018[0] = off + 0x168;
        y = off + 0x78;
        D_800E6280.unk_018[1] = y;
        D_801255E0 = x;
        D_801255E2 = y;
        D_801255DC += 4;
        if (D_801255DC > 0x80) {
            D_801255DC = 0;
        }
    } else {
        D_801255E0 = 0xA0;
        D_801255E2 = 0x78;
        D_800E6280.unk_014[0] = 0xA0;
        D_800E6280.unk_014[1] = 0xA0;
        D_800E6280.unk_018[0] = 0x168;
        D_800E6280.unk_018[1] = 0x78;
    }
}
