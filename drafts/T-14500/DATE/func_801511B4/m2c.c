extern u32 D_800E7378;
extern u8 D_800E738A;
extern ? D_8015EC24;

void func_801511B4(void) {
    M2C_MEMCPY_ALIGNED(&sp2C[0], &D_8015EC24, 0xA0);
    (&sp2C[0])[D_800E738A](0x80);
    if (D_80122D04 != 0) {
        D_801206DB |= 0x80;
        if ((u32) ((u32) D_800E7378 % 40U) < 0x14U) {
            D_80121FD4 &= 0x7FFFFFFF;
        } else {
            D_80121FD4 |= 0x80000000;
        }
        D_80121FEA = D_800B593C;
        D_80121FE9 = D_800B593C;
        D_80121FE8 = D_800B593C;
        D_801206DF = D_800B593C;
        return;
    }
    D_801206DB &= 0xFF7F;
    D_80121FD4 |= 0x80000000;
}
