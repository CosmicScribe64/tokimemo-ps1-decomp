void func_8014AF88(void) {
    TaiikuBig *var_s2;
    TaiikuBig *var_s2_2;
    s32 var_s1;
    u32 *var_s0;
    u32 *var_s0_2;
    u8 *var_s3;

    func_8009B3C0(0x801F8004);
    var_s2 = D_801227A0;
    var_s0 = D_80122640;
    var_s1 = 0;
    do {
        func_8009B430(0x801F800C, (u8 *) var_s0, var_s1);
        var_s1 += 1;
        *(TaiikuBig **)((unsigned char *)var_s0 + (4)) = var_s2;
        var_s2 += 0x50;
        var_s0 += 4;
        *(s32 *)((unsigned char *)var_s0 + (-16)) = 0x80000000;
    } while (var_s1 < 2);
    func_8009B3C0(0x801F9004);
    var_s2_2 = D_80122840;
    var_s3 = D_80122660;
    var_s0_2 = D_80122640;
    do {
        func_8009B430(0x801F900C, var_s3, 0);
        *(TaiikuBig **)((unsigned char *)var_s0_2 + (36)) = var_s2_2;
        var_s2_2 += 0x50;
        var_s0_2 += 4;
        var_s3 += 0x10;
        *(s32 *)((unsigned char *)var_s0_2 + (16)) = 0x80000000;
    } while (var_s2_2 != D_80122B10);
}
