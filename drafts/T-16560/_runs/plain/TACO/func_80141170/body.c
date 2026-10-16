s32 func_80141170(s32 arg0, s32 arg1) {
    s32 sp38;
    TcActor *sp34;
    Tc14 *sp2C;
    Tc14 *var_a0;
    Tc14 *var_v0_2;
    TcActor *var_a2;
    s32 var_at;
    s32 var_v0;
    s32 var_v1;

    if (arg0 == 0) {
        var_a2 = D_8015EDB4;
        var_a0 = D_8015E270;
    } else {
        var_a0 = D_8015E3B0;
        var_a2 = D_8015EDB4 + 0x440;
    }
    var_v0 = 0;
    if (arg1 & 0x20) {
        var_v0_2 = var_a0;
        var_v1 = 0;
loop_5:
        var_at = var_v1 < 0x10;
        if (var_v0_2->unk13 & 0x80) {
            var_v1 += 1;
            var_v0_2 += 0x14;
            if (var_v1 == 0x10) {
                var_at = var_v1 < 0x10;
            } else {
                goto loop_5;
            }
        }
        if (var_at == 0) {
            return -1;
        }
        if (arg0 == 0) {
            sp2C = var_v0_2;
            sp38 = var_v1;
            sp34 = var_a2;
            func_801335A0(1, 0x502);
        }
        var_v0_2->unk13 = 0x98;
        var_v0_2->unk0 = (var_a2->unk74 + ((var_v1 & 1) << 7)) - 0x40;
        var_v0_2->unk2 = (var_a2->unk76 - (arg0 << 6)) + 0x40;
        var_v0_2->unk8 = 0;
        var_v0_2->unkA = 0;
        var_v0_2->unkC = -0x200;
        var_v0_2->unk12 = 0;
        var_v0_2->unk4 = var_a2->unk78;
        var_v0 = 1;
        /* Duplicate return node #13. Try simplifying control flow for better match */
        return var_v0;
    }
    return var_v0;
}
