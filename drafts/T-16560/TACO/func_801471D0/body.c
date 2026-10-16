void func_801471D0(s32 arg0) {
    s32 *var_s3;
    Tc10 *var_s0;
    Tc10 *var_s0_2;
    Tc10 *var_s2_2;
    TcObj50 *var_s2;
    s32 temp_s3;
    s32 var_s1;
    u32 temp_s6;

    temp_s3 = arg0 + 4;
    func_8009B3C0(temp_s3);
    var_s2 = D_80127480;
    var_s0 = D_80127080;
    do {
        func_8009B430(temp_s3 + 8, (u8 *) var_s0, 0);
        var_s0->unk4[0] = (s32) var_s2;
        var_s2 += 1;
        var_s0 += 1;
        *(s32 *)((unsigned char *)var_s0 + (-16)) = 0x80000000;
    } while ((u32) var_s2 < (u32) D_80128880);
    func_8009B3C0(temp_s3);
    temp_s6 = *(u32 *)((unsigned char *)temp_s3 + (4));
    var_s1 = 0;
    if (temp_s6 != 0) {
        var_s3 = &D_80127480[1].unk0;
        var_s2_2 = &D_80127080[1];
        var_s0_2 = D_80127080;
        do {
            func_8009B430(temp_s3 + 8, (u8 *) var_s2_2, var_s1);
            var_s1 += 1;
            *(s32 **)((unsigned char *)var_s0_2 + (20)) = var_s3;
            var_s3 += 20;
            var_s0_2 += 1;
            var_s2_2 += 1;
            var_s0_2->unk0 = 0x80000000;
        } while ((u32) var_s1 < temp_s6);
    }
}
