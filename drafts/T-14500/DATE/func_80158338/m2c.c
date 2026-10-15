extern u8 D_800E62BE;
extern u8 D_800E62BF;
extern u8 D_800E62C0;
extern u8 D_800E738A;
extern ? D_8016020C;

void func_80158338(void) {
    ? sp28;

    M2C_MEMCPY_ALIGNED(&sp28, &D_8016020C, 0xFC);
    if ((((D_800E62BE == 0x5F) && (D_800E62BF == 7) && (D_800E62C0 == 0x17)) || ((D_800E62BE == 0x60) && (D_800E62BF == 7) && (D_800E62C0 == 0x1C)) || ((D_800E62BE == 0x61) && (D_800E62BF == 7) && (D_800E62C0 == 0x1B))) && (D_800B593C == 0x80) && (D_80122D04 == 0)) {
        func_8006B900();
    }
    *(? (**)(?))((unsigned char *)(sp + (D_800E738A * 4)) + (40))(0x80);
}
