s32 func_801404FC(s32 arg0, s32 arg1) {
    s32 sp3C;
    TcActor *sp38;
    Tc14 *sp2C;
    Tc14 *var_a0;
    Tc14 *var_v1;
    TcActor *var_a3;
    s32 var_at;
    s32 var_v0;
    s32 var_v0_2;

    if (arg0 == 0) {
        var_a3 = D_8015EDB4;
        var_a0 = D_8015E4F0;
    } else {
        var_a0 = D_8015E630;
        var_a3 = D_8015EDB4 + 0x440;
    }
    var_v0 = 0;
    if (arg1 & 0x20) {
        var_v1 = var_a0;
        var_v0_2 = 0;
loop_5:
        var_at = var_v0_2 < 0x140;
        if (var_v1->unk13 & 0x80) {
            var_v0_2 += 0x14;
            var_v1 += 0x14;
            if (var_v0_2 == 0x140) {
                var_at = var_v0_2 < 0x140;
            } else {
                goto loop_5;
            }
        }
        if (var_at == 0) {
            return -1;
        }
        if (arg0 == 0) {
            sp2C = var_v1;
            sp38 = var_a3;
            func_801335A0(1, 0x503);
        }
        var_v1->unk13 = 0x91;
        var_v1->unk0 = var_a3->unk74;
        var_v1->unk2 = var_a3->unk76 + 0x40;
        var_v1->unk4 = var_a3->unk78 + 0x40;
        var_v1->unk8 = 0;
        var_v1->unkA = 0;
        var_v1->unkC = -0x100;
        var_v1->unk12 = 0;
        sp2C = var_v1;
        func_801407D4((s32) &sp3C, subroutine_arg1, subroutine_arg2);
        var_v0 = 1;
        sp2C->unk11 = (u8) sp3C;
        /* Duplicate return node #13. Try simplifying control flow for better match */
        return var_v0;
    }
    return var_v0;
}
