void func_80132384(void) {
    s16 *temp_v0;
    s16 *temp_v0_2;
    s16 temp_s0;
    s16 temp_v1;
    s16 temp_v1_2;
    s32 temp_s1;
    s32 var_a0;

    if (D_80120676 < -0xA0) {
        D_80120676 += 0xC;
    } else {
        D_80120676 = -0xA0;
    }
    func_801325A0((s32) &sp50[0]);
    var_a0 = 0;
    do {
        temp_s1 = var_a0 + 1;
        if (D_800E7389 != temp_s1) {
            temp_v0 = get_k_work(var_a0);
            temp_v1 = *temp_v0;
            if (temp_v1 < -0x78) {
                *temp_v0 = temp_v1 + 0xC;
            } else {
                *temp_v0 = -0x78;
            }
        } else {
            temp_s0 = (var_a0 * 0x14) - 0x58;
            temp_v0_2 = get_k_work(var_a0);
            temp_v1_2 = *(s16 *)((unsigned char *)temp_v0_2 + (2));
            if (temp_v1_2 < temp_s0) {
                *(s16 *)((unsigned char *)temp_v0_2 + (2)) = (s16) (temp_v1_2 + 0xC);
                D_801206BE += 0xC;
                if (*(s16 *)((unsigned char *)temp_v0_2 + (2)) >= temp_s0) {
                    *(s16 *)((unsigned char *)temp_v0_2 + (2)) = temp_s0;
                    D_801206BE = *(s16 *)((unsigned char *)(&(&sp50[0])[D_800E7313]) + (16)) + 6;
                }
            } else {
                *(s16 *)((unsigned char *)temp_v0_2 + (2)) = temp_s0;
                D_801206BE = *(s16 *)((unsigned char *)(&(&sp50[0])[D_800E7313]) + (16)) + 6;
            }
        }
        var_a0 = temp_s1;
    } while (temp_s1 != 8);
}
