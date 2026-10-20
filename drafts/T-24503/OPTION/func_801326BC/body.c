void func_801326BC(void) {
    GameState *var_v0;
    s32 palette;
    s32 temp_a3;
    s32 temp_t8;
    s32 var_a0;
    s32 var_v1;

    if (D_8013D300 != 0) {
        ((SpriteSlot24 *)&D_801220D0)->unk_10 = D_800E6280.unk_03A;
        ((SpriteSlot24 *)&D_801220D0)->unk_11 = D_800E6280.unk_03A;
        ((SpriteSlot24 *)&D_801220D0)->unk_12 = D_800E6280.unk_03A;
        if (D_800E6280.unk_10F8 & 1) {
            ((SpriteSlot24 *)&D_801220D0)->unk_0C += 1;
            ((SpriteSlot24 *)&D_801220D0)->unk_0E -= 1;
        }
        var_v1 = 0;
        temp_a3 = D_8013D308 + 1;
        if (temp_a3 >= 0x101) {
            palette = D_8013D304;
            var_v0 = &D_800E6280;
            do {
                temp_t8 = *(s32 *)(((palette % 4) << 9) + 0x801CBEF0 + var_v1);
                var_v0 = (GameState *)((u8 *)var_v0 + 4);
                var_v1 += 4;
                *(s32 *)((unsigned char *)var_v0 + (5688)) = temp_t8;
            } while ((u32) var_v0 < (u32) ((u8 *)&D_800E6280 + 0x200));
            var_a0 = 0;
            if ((temp_a3 - 0x100) >= 0) {
                var_v0 = &D_800E6280;
                var_v1 = 0;
                do {
                    var_v0->unk_163C[0x10] = *(s32 *)((unsigned char *)(((((palette + 1) % 4) << 7) * 4) + 0x801CBEF0 + var_v1) + (64));
                    var_v0->unk_163C[0x38] = *(s32 *)((unsigned char *)((((palette + 1) % 4) << 9) + 0x801CBEF0 + var_v1) + (224));
                    if ((var_a0 + 0x60) < 0x80) {
                        var_v0->unk_163C[0x60] = *(s32 *)((unsigned char *)((((palette + 1) % 4) << 9) + 0x801CBEF0 + var_v1) + (384));
                    }
                    var_a0 += 1;
                    var_v1 += 4;
                    var_v0 = (GameState *)((u8 *)var_v0 + 4);
                } while ((temp_a3 - 0x100) >= var_a0);
            }
            D_8013D308 = temp_a3;
            if (temp_a3 >= 0x129) {
                D_8013D304 += 1;
                D_8013D308 = 0;
            }
            func_80043914(D_800E6280.unk_163C, 0x1A, 1, 1, 0);
            return;
        }
        D_8013D308 = temp_a3;
    }
}
