s32 func_801328C4(s32 arg0, s32 arg1) {
    RpgDirTable sp4C;
    s32 *var_s1;
    s32 temp_s3;
    s32 var_a0;
    s32 var_s0;
    s32 var_s0_2;

    sp4C = D_8015E4F0;
    var_s0 = 0;
    var_s1 = sp4C.v;
    do {
        func_8013E97C(var_s0 + 0x28, ((s32) ((*(s32 *)var_s1) * D_8015EDB0.unk_04) / 1000) + arg0, (arg1 + ((s32) (*(s32 *)((unsigned char *)var_s1 + (4)) * D_8015EDB0.unk_04) / 1000)) - (func_800A0070((s32) (D_8015EDB0.unk_04 << 0xB) / 1000) >> 7));
        var_s0 += 1;
        var_s1 += 2;
    } while (var_s0 < 8);
    temp_s3 = D_8015EDB0.unk_04 + 0x19;
    D_8015EDB0.unk_04 = temp_s3;
    var_s0_2 = 0;
    if (temp_s3 >= 0x3E9) {
        var_a0 = 0x28;
        do {
            func_8013E810(var_a0, 9, 1, 0);
            var_s0_2 += 1;
            var_a0 = var_s0_2 + 0x28;
        } while (var_s0_2 != 8);
    }
}
