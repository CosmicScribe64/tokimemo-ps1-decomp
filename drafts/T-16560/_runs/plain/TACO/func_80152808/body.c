s32 func_80152808(u32 arg0) {
    s32 *var_s1;
    s32 *var_s2;
    s32 *var_s3;
    s32 var_s0;

    if (arg0 < (u8) *(u8 *)((unsigned char *)D_8015EDB4 + (2859))) {
        func_80152944(0x1D, (s32) *(u8 *)((unsigned char *)D_8015EDB4 + (2858)));
        *(u8 *)((unsigned char *)D_8015EDB4 + (2858)) = (u8) (*(u8 *)((unsigned char *)D_8015EDB4 + (2858)) + 1);
        *(u8 *)((unsigned char *)D_8015EDB4 + (2859)) = 0U;
    }
    var_s0 = 0x1D;
    if ((u8) *(u8 *)((unsigned char *)D_8015EDB4 + (2858)) >= 0x46U) {
        *(u8 *)((unsigned char *)D_8015EDB4 + (2858)) = 0U;
        var_s3 = &D_80160448;
        var_s2 = &D_801603E2;
        var_s1 = &D_80160378;
        *(u8 *)((unsigned char *)D_8015EDB4 + (2859)) = (u8) *(u8 *)((unsigned char *)D_8015EDB4 + (2858));
        do {
            func_801472D4(var_s0, 0, *(s32 *)((unsigned char *)var_s1 + (-8)), *(s32 *)((unsigned char *)var_s1 + (-4)), (s32) *(u8 *)((unsigned char *)var_s2 + (-2)), (s32) *(u8 *)((unsigned char *)var_s2 + (-1)));
            func_801478F8(var_s0, *var_s3);
            var_s0 += 1;
            var_s1 += 8;
            var_s2 += 2;
            var_s3 += 4;
        } while (var_s0 != 0x21);
        return;
    }
    *(u8 *)((unsigned char *)D_8015EDB4 + (2859)) = (u8) (*(u8 *)((unsigned char *)D_8015EDB4 + (2859)) + 1);
}
