void func_80134B94(void) {
    typedef struct { s32 (*f[41])(); } T15800Callbacks;
    T15800Callbacks callbacks;
        s32 temp_v0;

    callbacks = *(T15800Callbacks *)&D_8013C228;
    callbacks.f[D_800E738A](0x80);
    temp_v0 = *(s32 *)((unsigned char *)(sp + (D_800E738A * 4)) + (48));
    if (&D_80085368 == temp_v0) {
        D_801206DB |= 0x80;
        D_801206DF = D_80120657;
    }
    if (func_800853FC == temp_v0) {
        D_801206DF = D_80120657;
        if ((u8) D_80120657 < 9U) {
            D_801206DB &= 0xFF7F;
        }
    }
}
