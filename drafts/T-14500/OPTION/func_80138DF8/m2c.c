? func_8004E93C(s32, ?);                            /* extern */
? func_8004F984(?, s16, s16);                       /* extern */
extern s32 D_800E7208;
extern s8 D_800E7314;
extern u8 D_800E738D;
extern u8 D_8013D3DC;
extern ? D_8013D3F8;

void func_80138DF8(void) {
    s32 temp_s1;
    s32 temp_s1_2;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s0_3;
    s32 var_s0_4;
    s32 var_s1;
    s32 var_s1_2;

    if (D_800E738D == 0) {
        func_80138B4C(0);
        var_s0 = 0;
        do {
            temp_s1 = var_s0 + 1;
            func_8004E93C(temp_s1, 0);
            var_s0 = temp_s1;
        } while (temp_s1 != 7);
        D_800E738D += 1;
    } else {
        var_s0_2 = 0;
        if (D_800E7208 & 0x20) {
            if (D_800E7314 != -1) {
                D_8013D3DC += D_800E7314;
            }
            do {
                temp_s1_2 = var_s0_2 + 1;
                func_8004E93C(temp_s1_2, 1);
                var_s0_2 = temp_s1_2;
            } while (temp_s1_2 != 7);
            func_80042940(2);
        }
    }
    var_s0_3 = 0;
    var_s1 = -0x64;
    do {
        func_8004AE54((s32) ((u32) (D_8013D3DC + var_s0_3) % 58U), -0xA, var_s1, 6);
        var_s0_3 += 1;
        var_s1 += 0xA;
    } while (var_s0_3 < 0x10);
    var_s0_4 = 0;
    var_s1_2 = -0x64;
    do {
        func_8004B338((s32) (&D_8013D3F8 + (((u32) (D_8013D3DC + var_s0_4) % 58U) * 0x10)), 0xA, var_s1_2);
        var_s0_4 += 1;
        var_s1_2 += 0xA;
    } while (var_s0_4 != 0x10);
    func_8004F984(1, D_8011ECF6, D_8011ECFA);
    func_80050B54(1);
    func_8004FC10(1);
    func_80050B54(0);
    func_8013A4FC();
    func_8013A484();
}
