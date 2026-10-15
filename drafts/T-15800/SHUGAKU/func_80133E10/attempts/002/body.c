void func_80133E10(void) {
    typedef struct { s32 (*f[44])(); } T15800Callbacks;
    T15800Callbacks callbacks;
        s32 sp2C[43];
    s32 temp_v1;
    s32 var_t1;
    u8 temp_a0;
    u8 temp_v0;

    temp_v0 = D_800E738A;
    callbacks = *(T15800Callbacks *)&D_8013C008;
    if (temp_v0 < 8U) {
        func_8006B900();
        goto block_5;
    }
    if (temp_v0 >= 0x1DU) {
        var_t1 = temp_v0 * 4;
        if (temp_v0 < 0x23U) {
            func_8006B900();
            goto block_5;
        }
    } else {
block_5:
        var_t1 = temp_v0 * 4;
    }
    callbacks.f[var_t1 / 4](0x80);
    temp_v1 = (s32) callbacks.f[D_800E738A];
    if (&D_80085368 == temp_v1) {
        D_801206DB |= 0x80;
        D_801206DF = D_80120657;
    }
    if (((u8) D_800E738A >= 0xDU) && ((u8) D_800E738A < 0x16U)) {
        D_801206DF = D_80120657;
    }
    temp_a0 = D_80120657 & 0xFF;
    if (func_800853FC == temp_v1) {
        D_801206DF = temp_a0;
        if (temp_a0 < 8U) {
            D_801206DB &= 0xFF7F;
        }
    }
}
