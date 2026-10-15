s32 func_8013DB80(s16 arg0, s16 arg1, s16 arg2, s32 arg3) {
    s32 input_xyz[3];
    s32 transformed_xyz[3];
    s32 sp60;
    s16 sp3A;
    Tc14 *sp2C;
    Tc14 *var_v1;
    s32 temp_a0;
    s32 var_a2;
    s32 var_at;

    sp3A = arg2;
    if (arg2 == 0) {
        sp3A = 0x100;
    }
    var_v1 = D_8015EB30;
    var_a2 = 0;
loop_3:
    var_at = var_a2 < 0x10;
    if (var_v1->unk13 & 0x80) {
        var_a2 += 1;
        var_v1 += 0x14;
        if (var_a2 == 0x10) {
            var_at = var_a2 < 0x10;
        } else {
            goto loop_3;
        }
    }
    if (var_at == 0) {
        return -1;
    }
    var_v1->unk12 = 0;
    var_v1->unk13 = 0x80;
    var_v1->unk0 = arg0;
    var_v1->unk2 = arg0;
    var_v1->unk4 = arg1;
    if (arg3 & 0x80) {
        input_xyz[0] = D_8015EDB4->unk74 - arg0;
        input_xyz[1] = D_8015EDB4->unk76 - arg0;
        input_xyz[2] = D_8015EDB4->unk78 - arg1;
        if (arg3 & 0x40) {
            sp2C = var_v1;
            sp60 = var_a2;
            input_xyz[0] = (input_xyz[0] - (func_800AE0D0() & 0xFF)) + 0x7F;
            input_xyz[1] = (input_xyz[1] - (func_800AE0D0() & 0xFF)) + 0x7F;
            input_xyz[2] = (input_xyz[2] - (func_800AE0D0() & 0xFF)) + 0x7F;
        }
        sp2C = var_v1;
        sp60 = var_a2;
        func_800AA460((s32) &input_xyz[0], (s32) &transformed_xyz[0]);
        var_v1 = sp2C;
        var_a2 = sp60;
        temp_a0 = (0x1000 / sp3A) + 1;
        var_v1->unk8 = (s16) (transformed_xyz[0] / temp_a0);
        var_v1->unkA = (s16) (transformed_xyz[1] / temp_a0);
        var_v1->unkC = (s16) (transformed_xyz[2] / temp_a0);
    } else {
        var_v1->unk8 = 0;
        var_v1->unkA = 0;
        var_v1->unkC = sp3A;
    }
    var_v1->padE[3] = (u8) arg3;
    return var_a2;
}
