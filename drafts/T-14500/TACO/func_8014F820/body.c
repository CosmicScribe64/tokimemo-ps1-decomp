void func_8014F820(void) {
    s32 *var_v0;
    TcActor *var_v1;
    s32 temp_t2;
    s32 var_s0;
    u8 *temp_t0;
    u8 temp_t9;
    u8 temp_v0;

    if (D_800E7384 == 1) {
        var_s0 = 0;
        do {
            temp_t0 = &D_8015EDB4->pad0[var_s0];
            var_s0 += 0x88;
            *(s16 *)((unsigned char *)temp_t0 + (3792)) = (s16) ((func_800AE0D0() % 8) + 5);
        } while (var_s0 != 0x1188);
    }
    D_801274EC += *(s16 *)((unsigned char *)D_8015EDB4 + (3792));
    var_v0 = &D_801274D0;
    var_v1 = D_8015EDB4 + 0x88;
    do {
        *(s32 *)((unsigned char *)var_v0 + (108)) = (s32) (*(s32 *)((unsigned char *)var_v0 + (108)) + *(s16 *)((unsigned char *)var_v1 + (3792)));
        *(s32 *)((unsigned char *)var_v0 + (188)) = (s32) (*(s32 *)((unsigned char *)var_v0 + (188)) + *(s16 *)((unsigned char *)var_v1 + (3928)));
        temp_t2 = *(s32 *)((unsigned char *)var_v0 + (348));
        *(s32 *)((unsigned char *)var_v0 + (268)) = (s32) (*(s32 *)((unsigned char *)var_v0 + (268)) + *(s16 *)((unsigned char *)var_v1 + (4064)));
        var_v0 += 0x140;
        *(s32 *)((unsigned char *)var_v0 + (28)) = (s32) (temp_t2 + *(s16 *)((unsigned char *)var_v1 + (4200)));
        var_v1 += 0x220;
    } while (var_v0 != &D_80127ED0);
    if (D_800E62B6 != 0) {
        temp_t9 = D_800E62B8 - 1;
        temp_v0 = temp_t9 & 0xFF;
        D_800E62B7 = temp_v0;
        D_800E62B6 = temp_v0;
        D_800E62B8 = temp_t9;
    }
    if ((u32) D_800E7384 < 0x191U) {
        func_80147E80(0x11);
    }
    if ((u32) D_800E7384 >= 0x258U) {
        D_8015EDCC += 1;
        func_80042908(0x2D);
        return;
    }
    D_800E7384 += 1;
}
