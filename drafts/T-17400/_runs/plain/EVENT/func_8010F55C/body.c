void func_8010F55C(void) {
    s32 sp30[16];
    typedef struct { s32 w[60]; } M2cBlockF0; /* m2c memcpy of 240 bytes */
    void (**temp_v0)(s32, u8);
    void (*var_v1)(s32, u8);

    *(M2cBlockF0 *)(&sp30[0]) = *(M2cBlockF0 *)(&D_801243B8);
    temp_v0 = &(&sp30[0])[D_800B1AF6];
    *(void (**)(s32, u8))((unsigned char *)(&sp30[0]) + (240)) = (void (*)(s32, u8)) D_801243B8.f[0x3C];
    var_v1 = *temp_v0;
    if (&func_80111E60 == var_v1) {
        switch (D_801243B0) {
        case 1:
            var_v1 = func_80111F04;
block_23:
            *temp_v0 = var_v1;
            break;
        case 2:
            var_v1 = func_801139CC;
            goto block_23;
        case 3:
            var_v1 = func_8011220C;
            goto block_23;
        case 4:
            var_v1 = func_80112460;
            goto block_23;
        case 5:
            var_v1 = func_80112750;
            goto block_23;
        case 6:
            var_v1 = func_80112930;
            goto block_23;
        case 7:
            var_v1 = func_80112BEC;
            goto block_23;
        case 8:
            var_v1 = func_801130F0;
            goto block_23;
        case 9:
            var_v1 = func_80113300;
            goto block_23;
        case 10:
            var_v1 = func_80113520;
            goto block_23;
        case 11:
            var_v1 = func_801136FC;
            goto block_23;
        case 12:
            var_v1 = func_801138D8;
            goto block_23;
        case 13:
            var_v1 = func_80113A40;
            goto block_23;
        case 14:
            var_v1 = func_80113CA0;
            goto block_23;
        case 15:
            var_v1 = func_80113FD4;
            goto block_23;
        case 16:
            var_v1 = func_801142E8;
            goto block_23;
        case 17:
            var_v1 = func_801144AC;
            goto block_23;
        case 18:
            var_v1 = func_801146B4;
            goto block_23;
        case 19:
            var_v1 = func_80114784;
            goto block_23;
        case 20:
            var_v1 = func_80114948;
            goto block_23;
        }
    }
    var_v1(0x80, D_800B1AF6);
}
