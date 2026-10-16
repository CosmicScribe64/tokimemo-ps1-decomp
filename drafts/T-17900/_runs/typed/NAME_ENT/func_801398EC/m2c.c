void func_801398EC(void) {
    Rec24 *var_a0;
    s32 temp_hi;
    s32 var_v0;
    u32 var_a2;

    temp_hi = (s32) D_8014D0D4 % 3;
    if (temp_hi == 0) {
        if (D_80123120[D_800E8BEF].flags[0] == 0) {
            var_a2 = D_800E6280.unk_10F8;
            D_801217D0->unk_00 = 0;
            D_801217D0->unk_0E = (((u32) (var_a2 >> 2) % 3U) * 0x10) + 0x80;
        } else {
            var_a2 = D_800E6280.unk_10F8;
            D_801217D0->unk_00 = 0x80000000;
        }
    } else {
        var_a2 = D_800E6280.unk_10F8;
        D_801217D0->unk_00 = 0x80000000;
    }
    var_a0 = D_801217D0 + 0x24;
    var_v0 = 1;
    do {
        if (temp_hi == 0) {
            if (*(u8 *)((unsigned char *)(&D_80123120[D_800E8BEF].unk_000[var_v0]) + (1672)) == 0) {
                var_a0->unk_00 = 0;
                var_a0->unk_0E = ((var_v0 / 8) * 0x30) + (((u32) (var_a2 >> 2) % 3U) * 0x10) + 0x80;
            } else {
                goto block_10;
            }
        } else {
block_10:
            var_a0->unk_00 = 0x80000000;
        }
        if (temp_hi == 0) {
            if (*(u8 *)((unsigned char *)(&D_80123120[D_800E8BEF].unk_000[var_v0]) + (1673)) == 0) {
                *(s32 *)((unsigned char *)var_a0 + (36)) = 0;
                *(s8 *)((unsigned char *)var_a0 + (50)) = (s8) ((((s32) (var_v0 + 1) / 8) * 0x30) + (((u32) (var_a2 >> 2) % 3U) * 0x10) + 0x80);
            } else {
                goto block_15;
            }
        } else {
block_15:
            *(s32 *)((unsigned char *)var_a0 + (36)) = 0x80000000;
        }
        var_v0 += 2;
        var_a0 += 0x48;
    } while (var_v0 != 0xD);
}
