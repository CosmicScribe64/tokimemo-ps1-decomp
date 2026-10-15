extern ? D_800B3778;
extern u8 D_800E71DF;
extern s32 D_800E7208;
extern u32 D_800E7384;
extern s32 D_80145078;
extern s16 D_80145080;

s32 func_80137120(void) {
    s16 temp_v0;
    s32 temp_a0;
    u32 temp_v0_2;
    u32 var_v0;

    if (D_80145078 == 0) {
        temp_v0 = D_80145080 + 1;
        if (D_80145080 < 0x5A) {
            D_80145080 = temp_v0;
            if (temp_v0 == 1) {
                func_80044750(0x60A);
            }
            if (D_80145080 == 0x1E) {
                func_80044750(0x604);
            }
            return 0;
        }
        temp_v0_2 = D_800E7384;
        if (!(temp_v0_2 & 7) && (temp_v0_2 < 0x40U)) {
            temp_a0 = (((*(&D_800B3778 + ((D_800E71DF * 7) + (temp_v0_2 >> 3))) + 0x505) & 0xFF) | 0x600) & 0xFFFF;
            D_800E7384 = temp_v0_2;
            func_80044750(temp_a0);
        }
        var_v0 = temp_v0_2 + 1;
        if (!(var_v0 & 0x3F) && (var_v0 < 0x140U)) {
            D_800E7384 = var_v0;
            func_8004E9F4(1);
            func_8004E788(-0x80, 0x30, 0, (s32) "プルルルルル", 0);
            func_80044750(0x608);
            var_v0 = D_800E7384;
        }
        if ((D_800E7208 & 0x400040) && (var_v0 >= 0x40U)) {
            D_800E7384 = var_v0;
            func_80044750(0x1508);
            goto block_18;
        }
        D_800E7384 = var_v0;
        if (var_v0 < 0x140U) {
            return 0;
        }
block_18:
        func_8004E9F4(1);
        goto block_19;
    }
block_19:
    D_80145078 += 1;
    return func_8004284C();
}
