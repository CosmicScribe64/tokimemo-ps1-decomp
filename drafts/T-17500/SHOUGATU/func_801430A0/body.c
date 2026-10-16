s32 func_801430A0(void) {
    typedef struct { s32 (*f[48])(); } T15800Callbacks;
    T15800Callbacks callbacks;
        s32 var_v0;

    callbacks = *(T15800Callbacks *)&D_80146970;
    var_v0 = callbacks.f[D_800E6280.unk_110A](0x3C);
    if ((u8) D_800E6280.unk_110A < 0x11U) {
        D_80120657 = D_800B593C;
        if ((D_80120668 == 0) && (D_80120658 == 0)) {
            var_v0 = (s32) (s16) (D_80120666 + 1);
            D_80120666 = (s16) var_v0;
            if (var_v0 >= 3) {
                D_80120666 = 0;
                var_v0 = 0;
            }
        }
    }
    if (D_80122D04 != 0) {
        D_801206DB |= 0x80;
        D_801206DF = D_80120657;
        if (((u32) D_800E6280.unk_10F8 % 300U) == 0) {
            var_v0 = ((s32 (*)()) func_80143228)();
            D_801206DA = 3;
        }
    } else {
        D_801206DB &= 0xFF7F;
    }
    return var_v0;
}
