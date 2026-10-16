void func_80063668(void) {
    s32 sp28;
    s16 *var_v1;
    s16 temp_v0;
    s16 var_a0;
    s16 var_a3;
    s32 var_a2;
    s32 var_at;
    s32 var_v0;
    u8 *var_v1_2;

    D_8011F513 = 0x80;
    D_8011F544 &= 0x7FFFFFFF;
    D_801217D0->unk_00 &= 0x7FFFFFFF;
    D_801217F4 &= 0x7FFFFFFF;
    sp28 = (s32) D_8011F4E0;
    if (D_800E62BC != 0) {
        var_a3 = 0;
        if (func_800460EC() & 4) {
            var_a3 = getCDlevel() + 1;
        }
        D_80125C80[D_800E7378 & 3] = var_a3;
        var_a0 = 0;
        var_v1 = D_80125C80;
        do {
            temp_v0 = *var_v1;
            var_v1 += 2;
            var_at = (u32) var_v1 < (u32) &D_80125C88;
            if (var_a0 < temp_v0) {
                var_a0 = temp_v0;
                var_at = (u32) var_v1 < (u32) &D_80125C88;
            }
        } while (var_at != 0);
        var_a2 = 0;
        var_v1_2 = D_800B5A68[D_800E71DF];
        var_v0 = 0;
        do {
            if ((u8) *var_v1_2 < (u32) var_a3) {
                var_a2 = var_v0 + 1;
            }
            var_v0 += 1;
            var_v1_2 += 1;
        } while (var_v0 != 3);
        if (var_a2 >= 4) {
            var_a2 = 3;
        }
        if ((D_801217DF == 0) || !(D_800E7378 & 1)) {
            if (var_a3 == 0) {
                D_801217DF = 0;
            } else if (var_a2 == 0) {
                D_801217DF = 0;
            } else if (var_a2 == 1) {
                D_801217DF = 0x10;
            } else if (var_a2 == 2) {
                D_801217DF = 0x20;
            } else if (var_a2 >= 3) {
                D_801217DF = 0x30;
            }
        }
        if (D_800E7200 & 0x80) {
            D_800B5A98 = 0;
        }
        if (D_800B5A98 < var_a3) {
            D_800B5A98 = (s32) var_a3;
        }
    } else {
        D_801217DF = D_8011F524 * 0x10;
    }
    D_80121803 = sp28 << 5;
}
