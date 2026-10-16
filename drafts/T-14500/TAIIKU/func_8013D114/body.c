void func_8013D114(void) {
    s32 sp2C;
    Taiiku2s32 *var_v1;

    if (D_8014996C <= 0) {
        D_80149980 = (s32) D_80149980 + 0x18000;
    } else {
        D_80149980 = (s32) D_80149980 + 0xFFFF0000;
    }
    var_v1 = &D_80149968;
    D_8014997C += D_80149974.unk_00;
    if (*(s16 *)((unsigned char *)(&D_80149968) + (22)) >= 0x61) {
        D_80149990.unk_00 = 1;
        *(s16 *)((unsigned char *)(&D_80149968) + (22)) = 0x60;
        D_801499B0->unk0 = 0;
        func_8013D254();
        goto block_7;
    }
    if (*(s16 *)((unsigned char *)(&D_80149968) + (22)) < -0x60) {
        D_80149990.unk_00 = 2;
        *(s16 *)((unsigned char *)(&D_80149968) + (22)) = -0x60;
        D_801499B0->unk0 = 0;
        func_8013D254();
block_7:
        var_v1 = &D_80149968;
    }
    if (*(s16 *)((unsigned char *)var_v1 + (26)) < 0) {
        *(s16 *)((unsigned char *)var_v1 + (26)) = 0;
    }
    if (*(s16 *)((unsigned char *)var_v1 + (26)) >= 0x80) {
        *(s16 *)((unsigned char *)var_v1 + (26)) = 0x80;
    }
    sp2C = D_8014997C * 4;
    (*(s32 *)D_80149984) = *(s16 *)((unsigned char *)(&sp2C) + (2));
    sp2C = (s32) D_80149980 * 2;
    D_80149984[1] = *(s16 *)((unsigned char *)(&sp2C) + (2));
}
