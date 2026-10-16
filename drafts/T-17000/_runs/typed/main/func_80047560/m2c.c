void func_80047560(void) {
    s16 temp_a2;
    s16 temp_a3;
    s32 temp_lo;
    s32 var_v1;
    u32 var_v0;
    u8 temp_t5;

    if (D_801255DC != 0) {
        var_v0 = 0;
        var_v1 = 0;
        if (D_801255DC != 0) {
            do {
                var_v1 += var_v0;
                var_v0 += 1;
            } while (var_v0 < (u8) D_801255DC);
        }
        temp_lo = (s32) (rsin(var_v1 * 4) / 4) / (s32) (D_801255DC + 0x100);
        temp_t5 = (D_801255DC + 4) & 0xFF;
        D_800E6280.unk_014[0] = (s16) temp_lo + 0xA0;
        temp_a2 = (s16) temp_lo + 0xA0;
        D_800E6280.unk_014[1] = temp_a2;
        D_800E6280.unk_018[0] = (s16) temp_lo + 0x168;
        temp_a3 = (s16) temp_lo + 0x78;
        D_800E6280.unk_018[1] = temp_a3;
        D_801255E0 = temp_a2;
        D_801255E2 = temp_a3;
        D_801255DC = temp_t5;
        if (temp_t5 >= 0x81U) {
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
