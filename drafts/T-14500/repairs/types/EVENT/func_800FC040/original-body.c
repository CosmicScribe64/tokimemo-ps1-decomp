void func_800FC040(void) {
    typedef struct { s32 w[42]; } M2cBlockA8; /* m2c memcpy of 168 bytes */
    s32 var_v1;

    *(M2cBlockA8 *)(&sp28[0]) = *(M2cBlockA8 *)(&D_801212B8);
    (&sp28[0])[D_800B1AF6](0x80);
    if (D_800EECE0 != 0) {
        D_800EB06F |= 0x80;
        D_800EB073 = D_800F19AB;
        if ((u8) D_801212B4 == 0) {
            D_800EB06E = 3;
        }
        if ((D_800EB084 == 6) && ((u8) D_801212B4 == 0)) {
            D_800EB06E = 0;
        }
        var_v1 = 0;
        if (func_80015A64() & 4) {
            var_v1 = func_800458E0();
        }
        if (var_v1 != 0) {
            if ((u8) D_801212B4 == 0) {
                D_800EB06E = 3;
                D_801212B4 = 1;
            }
        }
    } else {
        D_800EB06F &= 0xFF7F;
    }
}
