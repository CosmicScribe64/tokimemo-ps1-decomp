s32 func_801430A0(void) {
    s32 sp2C[16];
    typedef struct { s32 w[48]; } M2cBlockC0; /* m2c memcpy of 192 bytes */
    s32 var_v0;

    *(M2cBlockC0 *)(&sp2C[0]) = *(M2cBlockC0 *)(&D_80146970);
    var_v0 = (&sp2C[0])[D_800E738A](0x3C);
    if ((u8) D_800E738A < 0x11U) {
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
        if (((u32) D_800E7378 % 300U) == 0) {
            var_v0 = func_80143228();
            D_801206DA = 3;
        }
    } else {
        D_801206DB &= 0xFF7F;
    }
    return var_v0;
}
