s32 func_80148184(void) {
    s32 *var_s1;
    s32 *var_s2;
    Tc10 *var_s3;
    s32 var_s0;

    if (D_8015EDBC == 0) {
        D_8015EDBC = 0x20;
        func_80143730(0x80162000, 0xE);
        func_80143730(0x80172000, 0xC);
        func_8014866C();
        func_80146D60(0);
        func_801471D0(0x8018D000);
        load_csr_tp(0x80162000, 8, 1, 0, 0, 0x100, 0x100);
        func_80146F74(0, D_8015EDB4->unk74 - 0xBB8, D_8015EDB4->unk76 + 0x3E8, -0x109A0, 0, 0xBE, 0, 0, 0);
        var_s3 = &D_80127080[1];
        var_s2 = &D_8015F932;
        var_s1 = &D_8015F848;
        var_s0 = 1;
        do {
            func_801472D4(var_s0, 0, *(s32 *)((unsigned char *)var_s1 + (-8)), *(s32 *)((unsigned char *)var_s1 + (-4)), (s32) *(u8 *)((unsigned char *)var_s2 + (-2)), (s32) *(u8 *)((unsigned char *)var_s2 + (-1)));
            var_s0 += 1;
            var_s1 += 2;
            var_s2 += 2;
            var_s3 += 1;
            *(s32 *)((unsigned char *)var_s3 + (-16)) = 0;
        } while (var_s0 != 0x1F);
        D_80128880->x = 0;
        D_80128880[0].y = 0xBE;
        D_80128880[0].z = 0;
        func_80148B58(0);
        func_8004284C();
    }
}
