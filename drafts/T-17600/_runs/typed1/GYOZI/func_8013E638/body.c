s32 func_8013E638(void) {
    s16 var_v1;

    var_v1 = 0;
    do {
        if ((D_800F53A0.girl[var_v1].unk_10[1] & 1) && (var_v1 != D_800F62CF)) {
            D_800F53A0.girl[var_v1].unk_0A += 1;
        }
        var_v1 += 1;
    } while (var_v1 < 0xB);
    D_800F53A0.girl[D_800F62CF].unk_06 += 8;
    D_800F53A0.girl[D_800F62CF].unk_0A += 8;
    func_8008FB00(&D_800F53A0, 0x38);
    func_80050D60(0, 0);
    func_80081D30(1);
}
