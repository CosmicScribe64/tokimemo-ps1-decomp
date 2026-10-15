s16 func_80141944(void) {
    s32 sp2C[16];
    typedef struct { s32 w[26]; } M2cBlock68; /* m2c memcpy of 104 bytes */
    s16 var_v0;
    s32 var_at;
    s32 var_at_2;
    u8 temp_v1;

    *(M2cBlock68 *)(&sp2C[0]) = *(M2cBlock68 *)(&D_80148880);
    var_v0 = (&sp2C[0])[D_800F647A](0x80);
    temp_v1 = D_800F647A;
    if (temp_v1 < 8U) {
        D_8012B8C7 = (s8) (u8) D_801317EB;
        var_at = temp_v1 < 0xAU;
        if (D_8012B8D8 == 0) {
            var_at = temp_v1 < 0xAU;
            if (D_8012B8C8 == 0) {
                var_v0 = D_8012B8D6 + 1;
                D_8012B8D6 = var_v0;
                if (var_v0 >= 3) {
                    D_8012B8D6 = 0;
                    var_v0 = 0;
                }
                goto block_5;
            }
        }
    } else {
block_5:
        var_at = temp_v1 < 0xAU;
    }
    if (var_at == 0) {
        var_at_2 = temp_v1 < 0xAU;
        if (D_80148820 != 0) {
            var_v0 = func_801428EC();
            goto block_9;
        }
    } else {
block_9:
        var_at_2 = temp_v1 < 0xAU;
    }
    if (var_at_2 == 0) {
        if (D_80148824 != 0) {
            var_v0 = func_80142C14();
        }
    }
    return var_v0;
}
