void func_8014F820(void) {
    s32 *var_v0;
    TcActor *var_v1;
    s32 temp_t2;
    s32 var_s0;
    u8 *temp_t0;
    u8 temp_t9;
    u8 temp_v0;

    if (D_800E6280.unk_1104.u == 1) {
        var_s0 = 0;
        do {
            temp_t0 = &D_8015EDB4->unk0[var_s0];
            var_s0 += 0x88;
            *(s16 *)((unsigned char *)temp_t0 + (3792)) = (s16) ((func_800AE0D0() % 8) + 5);
        } while (var_s0 != 0x1188);
    }
    D_80127480[1].unk1C += D_8015EDB4[27].unk78;
    var_v0 = &D_80127480[1].unk0;
    var_v1 = &D_8015EDB4[1];
    do {
        *(s32 *)((unsigned char *)var_v0 + (108)) = (s32) (*(s32 *)((unsigned char *)var_v0 + (108)) + *(s16 *)((unsigned char *)var_v1 + (3792)));
        *(s32 *)((unsigned char *)var_v0 + (188)) = (s32) (*(s32 *)((unsigned char *)var_v0 + (188)) + *(s16 *)((unsigned char *)var_v1 + (3928)));
        temp_t2 = *(s32 *)((unsigned char *)var_v0 + (348));
        *(s32 *)((unsigned char *)var_v0 + (268)) = (s32) (*(s32 *)((unsigned char *)var_v0 + (268)) + *(s16 *)((unsigned char *)var_v1 + (4064)));
        var_v0 += 80;
        *(s32 *)((unsigned char *)var_v0 + (28)) = (s32) (temp_t2 + *(s16 *)((unsigned char *)var_v1 + (4200)));
        var_v1 += 4;
    } while (var_v0 != &D_80127480[33].unk0);
    if (D_800E6280.unk_036 != 0) {
        temp_t9 = D_800E6280.unk_038 - 1;
        temp_v0 = temp_t9 & 0xFF;
        D_800E6280.unk_037 = temp_v0;
        D_800E6280.unk_036 = temp_v0;
        D_800E6280.unk_038 = temp_t9;
    }
    if ((u32) D_800E6280.unk_1104.u < 0x191U) {
        func_80147E80(0x11);
    }
    if ((u32) D_800E6280.unk_1104.u >= 0x258U) {
        D_8015EDCC += 1;
        func_80042908(0x2D);
        return;
    }
    D_800E6280.unk_1104.u += 1;
}
