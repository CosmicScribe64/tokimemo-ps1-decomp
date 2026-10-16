void func_80138C9C(void) {
    s32 sp4C[16];
    s16 *sp44;
    s16 *temp_t8;
    s16 *temp_v0;
    s16 *temp_v1;
    s16 temp_t7;
    s16 temp_v0_2;
    s16 var_s0;
    s16 var_s0_2;
    s16 var_s2;
    s32 temp_hi;
    s32 var_t1;
    s32 var_t1_2;
    s32 var_t2;
    s32 var_t9;
    s32 var_t9_2;
    u8 *temp_s1;

    D_80147220 = 0;
    D_80147244 = 0;
    D_8014723C = 0;
    D_80147240 = 0;
    var_s2 = 0;
    if (!(D_800F53A0.girl[9].unk_10[0] & 2) && (D_800F53A0.girl[9].unk_06 >= 0x14)) {
        D_80147240 = 1;
    }
    var_s0 = 0;
    var_t1 = 0 * 8;
    do {
        temp_s1 = ((var_t1 - var_s0) * 8) + &D_800F53A0;
        if (((*(s32 *)((unsigned char *)temp_s1 + (440)) & 2) || ((var_s0 == 9) && (D_80147240 == 1))) && ((func_8005F220(var_s0) >= 0x32U) || ((D_80147240 == 1) && (var_s0 == 9))) && ((var_t9 = var_s2 * 2, (D_80147240 == 0)) || (var_t9 = var_s2 * 2, (var_s0 != 0)) || (var_t9 = var_s2 * 2, ((func_8005EBE0(0) < 0x46U) == 0)))) {
            *(&sp4C[0] + var_t9) = var_s0;
            D_8014723C += 1;
            var_s2 += 1;
            *(u8 *)((unsigned char *)temp_s1 + (441)) = (u8) (*(u8 *)((unsigned char *)temp_s1 + (441)) | 1);
        }
        var_s0 += 1;
        var_t1 = var_s0 * 8;
    } while (var_s0 < 0xB);
    var_s0_2 = 0;
    if (D_8014723C > 0) {
        var_t1_2 = 0 * 2;
        do {
            func_800BCDF0("%d ", *(&sp4C[0] + var_t1_2));
            var_s0_2 += 1;
            var_t1_2 = var_s0_2 * 2;
        } while (var_s0_2 < D_8014723C);
        var_s0_2 = 0;
    }
    func_800BCDF0("\n");
    if (D_8014723C > 0) {
        do {
            temp_hi = (s32) (var_s0_2 + (s16) ((u32) D_800F6468 % (u32) D_8014723C)) % (s16) D_8014723C;
            temp_t8 = &(&D_80147224)[var_s0_2];
            temp_t7 = var_s0_2 + 1;
            var_s0_2 = temp_t7;
            *temp_t8 = (&sp4C[0])[temp_hi];
        } while (temp_t7 < D_8014723C);
        var_s0_2 = 0;
    }
    if (D_8014723C > 0) {
        var_t2 = var_s0_2 * 2;
        do {
            func_800BCDF0("%d ", *(&D_80147224 + var_t2));
            var_s0_2 += 1;
            var_t2 = var_s0_2 * 2;
        } while (var_s0_2 < D_8014723C);
        var_s0_2 = 0;
    }
    func_800BCDF0("\n");
    var_t9_2 = var_s0_2 * 2;
    if (D_8014723C > 0) {
loop_27:
        temp_v1 = &D_80147224 + var_t9_2;
        if ((*temp_v1 == 0) && (sp44 = temp_v1, ((func_8005EBE0(0) < 0x46U) == 0))) {
            temp_v0 = &(&D_80147224)[D_8014723C];
            *temp_v1 = *(s16 *)((unsigned char *)temp_v0 + (-2));
            *(s16 *)((unsigned char *)temp_v0 + (-2)) = 0;
        } else {
            temp_v0_2 = *temp_v1;
            if (temp_v0_2 == 0) {
                *temp_v1 = D_80147224;
                D_80147224 = 0;
            } else if ((temp_v0_2 == 9) && (D_80147240 != 0)) {
                *temp_v1 = D_80147224;
                D_80147224 = 9;
            } else {
                var_s0_2 += 1;
                var_t9_2 = var_s0_2 * 2;
                if (var_s0_2 < D_8014723C) {
                    goto loop_27;
                }
            }
        }
    }
    func_8004DE1C();
}
