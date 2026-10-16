? func_8013D254();                                  /* extern */
extern ? D_80149968;
extern s32 D_8014996C;
extern s32 D_80149974;
extern s32 D_8014997C;
extern s32 D_80149980;
extern ? D_80149984;
extern s32 D_80149990;
extern s16 D_801499B0;

void func_8013D114(void) {
    s32 sp2C;
    ? *var_v1;

    if (D_8014996C <= 0) {
        D_80149980 += 0x18000;
    } else {
        D_80149980 += 0xFFFF0000;
    }
    var_v1 = &D_80149968;
    D_8014997C += D_80149974;
    if (*(s16 *)((unsigned char *)(&D_80149968) + (22)) >= 0x61) {
        D_80149990 = 1;
        *(s16 *)((unsigned char *)(&D_80149968) + (22)) = 0x60;
        D_801499B0 = 0;
        func_8013D254();
        goto block_7;
    }
    if (*(s16 *)((unsigned char *)(&D_80149968) + (22)) < -0x60) {
        D_80149990 = 2;
        *(s16 *)((unsigned char *)(&D_80149968) + (22)) = -0x60;
        D_801499B0 = 0;
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
    D_80149984.unk0 = (s16) *(s16 *)((unsigned char *)(&sp2C) + (2));
    sp2C = D_80149980 * 2;
    *(s16 *)((unsigned char *)(&D_80149984) + (2)) = (s16) *(s16 *)((unsigned char *)(&sp2C) + (2));
}
