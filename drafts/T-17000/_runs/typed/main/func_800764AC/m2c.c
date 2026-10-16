s32 func_800764AC(u32 *arg0) {
    ? spAC;
    s32 sp7C;
    s32 sp78;
    s32 sp74;
    s32 sp70;
    s32 sp6C;
    GameState *var_v1_2;
    GameState *var_v1_4;
    s32 *temp_a0;
    s32 *var_v1;
    s32 *var_v1_3;
    s32 temp_a3;
    s32 temp_t7;
    s32 temp_t7_2;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 var_a0;
    s32 var_a1;

    temp_t7 = D_800E6280.unk_54C[0].w & 0x1F;
    switch (temp_t7) {
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
        return 1;
    default:
        var_v1 = &sp2C[0];
        do {
            var_v1 += 4;
            *(s32 *)((unsigned char *)var_v1 + (-4)) = 0;
        } while ((u32) var_v1 < (u32) &spAC);
        var_v1_2 = &D_800E6280;
loop_6:
        temp_v0 = var_v1_2->unk_54C[0].w;
        var_v1_2 += 4;
        if (!(temp_v0 & 0x20000000)) {

        } else {
            temp_a0 = &(&sp2C[0])[temp_v0 & 0x1F];
            *temp_a0 += 1;
            if ((u32) var_v1_2 < (u32) &D_800E6280.unk_01C[0].unk_04) {
                goto loop_6;
            }
        }
        sp6C = 0;
        sp70 = 0;
        sp78 = 0;
        sp7C = 0;
        sp74 = 0;
        var_a0 = 0;
        var_a1 = 0;
        var_v1_3 = &sp2C[0];
        do {
            temp_t7_2 = *var_v1_3;
            var_v1_3 += 4;
            if (temp_t7_2 != 0) {
                var_a0 += 1;
            }
        } while ((u32) var_v1_3 < (u32) &spAC);
        if (var_a0 < 2) {
            *arg0 = (u32) (D_800E6280.unk_54C[0].w * 4) >> 0x1F;
            return 1;
        }
        var_v1_4 = &D_800E6280;
loop_16:
        temp_v0_2 = var_v1_4->unk_54C[0].w;
        temp_a3 = temp_v0_2 & 0x1F;
        if ((temp_v0_2 & 0x20000000) && (((&sp2C[0])[temp_a3] == 0) || (*(arg0 + (var_a1 * 4)) = temp_a3, var_a1 += 1, (&sp2C[0])[var_v1_4->unk_54C[0].w & 0x1F] = 0, ((var_a1 < 3) != 0)))) {
            var_v1_4 += 4;
            if (var_v1_4 != &D_800E6280.unk_01C[0].unk_04) {
                goto loop_16;
            }
        }
        return var_a0;
    }
}
