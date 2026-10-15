extern u8 D_800E738A;
extern ? D_80146804;

void func_801419D0(void) {
    s16 temp_v0;

    M2C_MEMCPY_ALIGNED(&sp2C[0], &D_80146804, 0xA0);
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
