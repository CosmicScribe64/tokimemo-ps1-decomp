s32 func_8006C988(u8 *arg0, u8 *arg1, s32 arg2) {
    GameState *var_t1;
    s32 temp_t7;
    s32 var_a2;
    s32 var_a3;
    s32 var_a3_2;
    u8 temp_v1;
    u8 var_v0;
    u8 var_v1;
    u8 *var_t1_2;
    u8 *var_t1_3;

    var_v0 = 0xFF;
    var_v1 = 0;
    var_a2 = 0;
    var_t1 = ((u8 *)&D_800E6280);
    var_a3 = 0;
loop_1:
    var_a3 += 1;
    if (var_t1->unk_75E[0].unk_04 != 0) {
        temp_v1 = var_t1->unk_75E[0].unk_02;
        var_a2 += 1;
        if ((temp_v1 != 0) && (temp_v1 != 1) && (temp_v1 != 2) && (temp_v1 != 0xF)) {

        } else {
            var_v0 = var_t1->unk_75E[0].unk_00;
        }
        var_t1 += 8;
        var_v1 = var_t1->unk_75E[0].unk_00;
        if (var_a3 < 0x100) {
            goto loop_1;
        }
    }
    var_a3_2 = var_a2;
    if (var_a2 < 0x100) {
        temp_t7 = (0x100 - var_a2) & 3;
        if (temp_t7 != 0) {
            var_t1_2 = (var_a3_2 * 8) + ((u8 *)&D_800E6280);
            do {
                var_a3_2 += 1;
                *(s8 *)((unsigned char *)var_t1_2 + (1890)) = 0;
                var_t1_2 += 8;
            } while ((temp_t7 + var_a2) != var_a3_2);
            if (var_a3_2 != 0x100) {
                goto block_14;
            }
        } else {
block_14:
            var_t1_3 = (var_a3_2 * 8) + ((u8 *)&D_800E6280);
            do {
                var_t1_3 += 0x20;
                *(s8 *)((unsigned char *)var_t1_3 + (1866)) = 0;
                *(s8 *)((unsigned char *)var_t1_3 + (1874)) = 0;
                *(s8 *)((unsigned char *)var_t1_3 + (1882)) = 0;
                *(s8 *)((unsigned char *)var_t1_3 + (1858)) = 0;
            } while (var_t1_3 != &D_800E6A80);
        }
    }
    *arg0 = var_v0;
    *arg1 = var_v1;
    if (var_a2 >= 0xFF) {
        var_a2 = 0xFF;
    }
    return var_a2;
}
