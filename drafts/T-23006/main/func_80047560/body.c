void func_80047560(void) {
    s32 sum;
    u32 i;
    s32 q;
    s16 off;
    s16 x;
    s16 y;
    u8 next;

    if (D_801255DC != 0) {
        i = 0;
        sum = 0;
        if (D_801255DC != 0) {
            do {
                sum += i;
                i += 1;
            } while (i < (u8)D_801255DC);
        }
        q = (s16)rsin(sum * 4) / 4 / (s32)(D_801255DC + 0x100);
        next = D_801255DC + 4;
        D_800E6280.unk_014[0] = (s16)q + 0xA0;
        off = q;
        x = off + 0xA0;
        D_800E6280.unk_014[1] = x;
        D_800E6280.unk_018[0] = off + 0x168;
        y = off + 0x78;
        D_800E6280.unk_018[1] = y;
        D_801255E0 = x;
        D_801255E2 = y;
        D_801255DC = next;
        if (next >= 0x81U) {
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
