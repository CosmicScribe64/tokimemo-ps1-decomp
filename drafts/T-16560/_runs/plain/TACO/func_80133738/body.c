void func_80133738(void) {
    s32 sp28;
    s32 var_v1;
    u8 *temp_v0;

    if ((D_800E7389 & 1) && (D_800E738A == 1)) {
        if ((D_8015EDBC == 0) && (D_800E7208 & 0x10) && (D_800E7378 & 0xFF)) {
            sp28 = 3;
            if (func_80045288() == 1) {
                sp28 = 3;
                var_v1 = 3;
                if (func_800460EC() & 2) {
                    if (*(s16 *)((unsigned char *)D_8015EDB4 + (1208)) < D_8015EDB4->unk78) {
                        var_v1 = (func_800AE0D0() & 1) + 2;
                    } else if (D_800E7374 & 1) {
                        var_v1 = func_800AE0D0() % 6;
                    }
                    temp_v0 = (var_v1 * 8) + &D_8015E238;
                    func_80046290((*(s32 *)temp_v0), *(s32 *)((unsigned char *)temp_v0 + (4)), 8);
                    func_80044750(0x300);
                }
            }
        }
    }
}
