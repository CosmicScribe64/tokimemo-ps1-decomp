s32 func_80140364(s32 arg0, s32 arg1) {
    s32 sp38;
    TcActor *sp34;
    Tc14 *sp2C;
    s32 sp28;
    Tc14 *var_a2;
    Tc14 *var_v0;
    TcActor *var_a3;
    s32 temp_a1;
    s32 var_at;
    s32 var_v1;

    if (arg0 == 0) {
        var_a3 = D_8015EDB4;
        var_v0 = D_8015E4F0;
    } else {
        var_v0 = D_8015E630;
        var_a3 = &D_8015EDB4[8];
    }
    var_v1 = 0;
    if (arg1 & 0x20) {
        var_a2 = var_v0;
loop_5:
        var_at = var_v1 < 0x10;
        if (var_a2->unk13 & 0x80) {
            var_v1 += 1;
            var_a2 += 1;
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
            sp38 = var_v1;
            sp2C = var_a2;
            sp34 = var_a3;
            func_801335A0(1, 0x503);
        }
        var_a2->unk13 = 0x92;
        temp_a1 = var_v1 & 1;
        var_a2->unk0 = var_a3->unk74;
        var_a2->unk2 = (var_a3->unk76 - (temp_a1 * 0x60)) + 0x40;
        var_a2->unk4 = var_a3->unk78;
        sp34 = var_a3;
        sp2C = var_a2;
        sp28 = temp_a1;
        var_a2->unk8 = (s16) (func_800A0070((s32) var_a3->unk68) / 256);
        if (temp_a1 != 0) {
            sp2C = var_a2;
            var_a2->unkA = (s16) ((s32) -func_800A0140((s32) var_a3->unk68) / 256);
        } else {
            sp2C = var_a2;
            var_a2->unkA = (s16) (func_800A0140((s32) var_a3->unk68) / 256);
        }
        var_a2->unkC = -0x40;
        var_a2->unk12 = 0;
        var_a2->unk11 = 0;
        return 1;
    }
    return 0;
}
