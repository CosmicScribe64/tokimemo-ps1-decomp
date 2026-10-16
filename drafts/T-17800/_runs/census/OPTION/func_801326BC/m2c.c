extern s16 D_801220DC;
extern s16 D_801220DE;
extern s32 D_800E6480;
extern s32 D_800E7378;
extern s32 D_800E78BC;
extern u8 D_800E62BA;
void func_801326BC(void) {
    GameState *var_v0;
    GameState *var_v0_2;
    s32 temp_a3;
    s32 temp_t8;
    s32 var_a0;
    s32 var_v1;
    s32 var_v1_2;

    if (D_8013D300 != 0) {
        D_801220E0 = D_800E62BA;
        D_801220E1 = D_800E62BA;
        D_801220E2 = D_800E62BA;
        if (D_800E7378 & 1) {
            D_801220DC += 1;
            D_801220DE -= 1;
        }
        var_v1 = 0;
        temp_a3 = D_8013D308 + 1;
        if (temp_a3 >= 0x101) {
            var_v0 = &D_800E6280;
            do {
                temp_t8 = *(s32 *)((((s32) D_8013D304 % 4) << 9) + 0x801CBEF0 + var_v1);
                var_v0 += 4;
                var_v1 += 4;
                *(s32 *)((unsigned char *)var_v0 + (5688)) = temp_t8;
            } while ((u32) var_v0 < (u32) &D_800E6480);
            var_a0 = 0;
            if ((temp_a3 - 0x100) >= 0) {
                var_v0_2 = &D_800E6280;
                var_v1_2 = 0;
                do {
                    var_v0_2->unk_163C[0x10] = *(s32 *)((unsigned char *)(((((s32) (D_8013D304 + 1) % 4) << 7) * 4) + 0x801CBEF0 + var_v1_2) + (64));
                    var_v0_2->unk_163C[0x38] = *(s32 *)((unsigned char *)((((s32) (D_8013D304 + 1) % 4) << 9) + 0x801CBEF0 + var_v1_2) + (224));
                    if ((var_a0 + 0x60) < 0x80) {
                        var_v0_2->unk_163C[0x60] = *(s32 *)((unsigned char *)((((s32) (D_8013D304 + 1) % 4) << 9) + 0x801CBEF0 + var_v1_2) + (384));
                    }
                    var_a0 += 1;
                    var_v1_2 += 4;
                    var_v0_2 += 4;
                } while ((temp_a3 - 0x100) >= var_a0);
            }
            D_8013D308 = temp_a3;
            if (temp_a3 >= 0x129) {
                D_8013D304 += 1;
                D_8013D308 = 0;
            }
            load_palette(&D_800E78BC, 0x1A, 1, 1, 0);
            return;
        }
        D_8013D308 = temp_a3;
    }
}
