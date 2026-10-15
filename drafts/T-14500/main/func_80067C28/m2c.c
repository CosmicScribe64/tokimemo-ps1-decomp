extern ? D_800B5A68;
extern u8 D_800E62BC;
extern u8 D_800E71DF;
extern s32 D_800E7378;
extern s16 D_80125C80;
extern ? D_80125C88;

void func_80067C28(void) {
    s16 *var_v1;
    s16 *var_v1_2;
    s16 temp_v0;
    s16 temp_v0_2;
    s16 var_a1;
    s16 var_a1_2;
    s16 var_a3;
    s32 var_a0;
    s32 var_a2;
    s32 var_at;
    s32 var_at_2;
    u8 *var_v0;

    if (D_800E62BC != 0) {
        var_a3 = 0;
        if (func_800460EC() & 4) {
            var_a3 = getCDlevel() + 1;
        }
        (&D_80125C80)[D_800E7378 & 3] = var_a3;
        var_a1 = 0;
        var_v1 = &D_80125C80;
        do {
            temp_v0 = *var_v1;
            var_v1 += 2;
            var_at = (u32) var_v1 < (u32) &D_80125C88;
            if (var_a1 < temp_v0) {
                var_a1 = temp_v0;
                var_at = (u32) var_v1 < (u32) &D_80125C88;
            }
        } while (var_at != 0);
        var_a1_2 = 0;
        var_v1_2 = &D_80125C80;
        do {
            temp_v0_2 = *var_v1_2;
            var_v1_2 += 2;
            var_at_2 = (u32) var_v1_2 < (u32) &D_80125C88;
            if (var_a1_2 < temp_v0_2) {
                var_a1_2 = temp_v0_2;
                var_at_2 = (u32) var_v1_2 < (u32) &D_80125C88;
            }
        } while (var_at_2 != 0);
        var_a2 = 0;
        var_v0 = (D_800E71DF * 3) + &D_800B5A68;
        var_a0 = 0;
        do {
            if ((u8) *var_v0 < (u32) var_a3) {
                var_a2 = var_a0 + 1;
            }
            var_a0 += 1;
            var_v0 += 1;
        } while (var_a0 != 3);
        if (var_a2 >= 4) {
            var_a2 = 3;
        }
        if ((D_80120668 == 0) || !(D_800E7378 & 1)) {
            if (var_a3 == 0) {
                D_80120668 = 0;
            } else if (var_a2 == 0) {
                D_80120668 = 0;
            } else if (var_a2 == 1) {
                D_80120668 = 1;
            } else if (var_a2 == 2) {
                D_80120668 = 2;
            } else if (var_a2 >= 3) {
                D_80120668 = 3;
            }
        }
    }
}
