void func_8006190C(void) {
    TaiikuBig *var_s1;
    TaiikuBig *var_s1_2;
    s32 var_s2;
    u32 *var_s0;
    u32 *var_s0_2;
    void *temp_s3;

    func_8009B3C0(0x801F0004);
    temp_s3 = (s32 *)0x801F0008 + 4;
    if (*(s32 *)0x801F0008 != 0xD) {
        return;
    }
    var_s1 = D_801227A0;
    var_s0 = D_80122640;
    do {
        func_8009B430((s32) temp_s3, (u8 *) var_s0, 0);
        *(TaiikuBig **)((unsigned char *)var_s0 + (4)) = var_s1;
        var_s1 += 0x50;
        var_s0 += 0x10;
        *(s32 *)((unsigned char *)var_s0 + (-16)) = 0;
    } while ((u32) var_s1 < (u32) D_80122CA0);
    var_s1_2 = D_801227A0;
    var_s0_2 = D_80122640;
    var_s2 = rand();
    do {
        func_8009B430((s32) temp_s3, (u8 *) var_s0_2, var_s2 % 13);
        *(TaiikuBig **)((unsigned char *)var_s0_2 + (4)) = var_s1_2;
        var_s1_2 += 0x50;
        var_s0_2 += 0x10;
        var_s2 += 1;
        *(s32 *)((unsigned char *)var_s0_2 + (-16)) = 0;
    } while (var_s1_2 != &D_80122BB0);
}
