s32 func_80015A64();                                /* extern */
s32 func_800458E0();                                /* extern */

void func_800FC040(void) {
    s32 var_v1;

    M2C_MEMCPY_ALIGNED(&sp28[0], &D_801212B8, 0xA8);
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
