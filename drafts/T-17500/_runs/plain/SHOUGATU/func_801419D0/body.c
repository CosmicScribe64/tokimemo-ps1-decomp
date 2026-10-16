void func_801419D0(void) {
    typedef struct { s32 w[40]; } M2cBlockA0; /* m2c memcpy of 160 bytes */
    s16 temp_v0;

    *(M2cBlockA0 *)(&sp2C[0]) = *(M2cBlockA0 *)(&D_80146804);
    (&sp2C[0])[D_800E738A](0x80);
    if ((u8) D_800E738A < 0x10U) {
        D_80120657 = D_800B593C;
        if ((D_80120668 == 0) && (D_80120658 == 0)) {
            temp_v0 = D_80120666 + 1;
            D_80120666 = temp_v0;
            if (temp_v0 >= 3) {
                D_80120666 = 0;
            }
        }
    }
    func_80141D20();
}
