void func_8014354C(void) {
    s32 *var_s1;
    s32 *var_s0;

    func_8009B3C0(0x801F9004);
    var_s1 = &D_801227A0;
    var_s0 = &D_80122640;
    do {
        func_8009B430(0x801F900C, var_s0, 0);
        *(s32 **)((unsigned char *)var_s0 + (4)) = var_s1;
        var_s1 += 20;
        var_s0 += 4;
        *(s32 *)((unsigned char *)var_s0 + (-16)) = 0x80000000;
    } while (var_s1 != &D_80122CA0);
    func_8009B430(0x801F900C, &D_80122640, 0);
    D_80122644 = &D_801227A0;
    D_80122640 = 0;
    D_80150130 = -0x60;
    D_80150134 = 0x20;
    D_80150138 = -0x300;
    D_80150140 = 0;
    D_8015013C.unk0 = 0;
    *(s16 *)((unsigned char *)(&D_8015013C) + (2)) = 0;
}
