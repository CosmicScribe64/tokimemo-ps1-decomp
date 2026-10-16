s32 func_8013E424(s16 arg0, s16 arg1, s32 arg2) {
    s32 sp58;
    Tc14 *sp28;
    Tc14 *var_v1;
    s32 var_a0;
    s32 var_at;

    var_v1 = D_8015EC70;
    var_a0 = 0;
loop_1:
    var_at = var_a0 < 0x10;
    if (var_v1->unk13 & 0x80) {
        var_a0 += 1;
        var_v1 += 1;
        if (var_a0 == 0x10) {
            var_at = var_a0 < 0x10;
        } else {
            goto loop_1;
        }
    }
    if (var_at == 0) {
        return -1;
    }
    var_v1->unk12 = 0;
    var_v1->unk13 = 0x80;
    sp28 = var_v1;
    sp58 = var_a0;
    var_v1->unk0 = arg0;
    var_v1->unk2 = arg0;
    var_v1->unk4 = arg1;
    var_v1->unk8 = 0x3F - (func_800AE0D0() & 0x7F);
    var_v1->unkA = 0x3F - (func_800AE0D0() & 0x7F);
    var_v1->unkC = 0x50;
    var_v1->unk11 = (u8) arg2;
    return var_a0;
}
