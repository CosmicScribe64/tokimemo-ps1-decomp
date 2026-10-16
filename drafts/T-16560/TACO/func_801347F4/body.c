void func_801347F4(s32 arg0) {
    Tc10 *var_s0;
    TcObj50 *var_s1;
    s32 temp_s3;

    temp_s3 = arg0 + 4;
    func_8009B3C0(temp_s3);
    var_s1 = D_80127480;
    var_s0 = D_80127080;
    do {
        func_8009B430(temp_s3 + 8, (u8 *) var_s0, 0);
        var_s0->unk4[0] = (s32) var_s1;
        var_s1 += 1;
        var_s0 += 1;
        *(s32 *)((unsigned char *)var_s0 + (-16)) = 0x80000000;
    } while (var_s1 != D_80128880);
}
