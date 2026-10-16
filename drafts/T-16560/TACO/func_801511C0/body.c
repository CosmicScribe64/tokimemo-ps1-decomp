void func_801511C0(s32 arg0, s32 arg1) {
    s32 *var_s2;
    s32 var_s0;
    s32 var_s1;

    var_s2 = &D_8016024C[arg1];
    var_s0 = 0;
    var_s1 = arg0;
    do {
        func_801478F8(var_s1, *var_s2);
        var_s0 += 1;
        var_s1 += 1;
        var_s2 += 1;
    } while (var_s0 != 6);
    if (arg1 != 0) {
        func_801478F8(0x1B, (s32) D_80160280);
        return;
    }
    func_801478F8(0x1A, D_80160264);
}
