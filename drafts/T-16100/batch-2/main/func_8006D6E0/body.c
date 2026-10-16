void func_8006D6E0(void) {
    GsRec69C *var_a0;
    u8 temp_t9;
    u8 var_v0;
    u8 var_v0_2;
    u8 var_v0_3;
    u8 var_v1;
    GsRec69C *var_a0_2;

    var_v0 = D_800E62C1;
    var_a0 = D_800E6280.unk_69C;
    var_v1 = 0;
loop_1:
    if (var_a0->unk_00 == 0xFB) {
        if (var_v0 == 0) {
            D_800E699C = var_v1;
            if (var_a0[-1].unk_00 != 0x25) {
                temp_t9 = D_800E699C - 1;
                D_800E699C = temp_t9;
                if (var_a0[-2].unk_00 != 0x25) {
                    D_800E699C = temp_t9 - 1;
                }
            }
            var_v0_2 = D_800E699C;
        } else {
            var_v0 -= 1;
            goto block_8;
        }
    } else {
block_8:
        var_v1 += 1;
        var_a0 += 1;
        if ((s32) var_v1 >= 0x20) {
            var_v0_2 = D_800E699C;
        } else {
            goto loop_1;
        }
    }
    if (var_v0_2 != 0) {
        var_v0_2 = (var_v0_2 - 1) & 0xFF;
    }
    D_800E699C = var_v0_2;
    if ((s32) var_v0_2 < 0x20) {
        var_a0_2 = &D_800E6280.unk_69C[var_v0_2];
loop_14:
        var_v0_3 = var_a0_2->unk_00;
        if (var_v0_3 == 0x25) {
            var_a0_2->unk_00 = 0U;
            var_v0_3 = 0 & 0xFF;
        }
        var_a0_2 += 1;
        if ((var_v0_3 != 0xFF) && ((u32) var_a0_2 < (u32) &D_800E6280.unk_69C[32])) {
            goto loop_14;
        }
    }
}
