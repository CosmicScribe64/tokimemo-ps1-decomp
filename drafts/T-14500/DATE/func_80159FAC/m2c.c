extern u8 D_800E62BE;
extern u8 D_800E62BF;
extern u8 D_800E62C0;
extern u32 D_800E7378;
extern u8 D_800E738A;
extern ? D_80160940;

void func_80159FAC(void) {
    ? sp2C;

    M2C_MEMCPY_ALIGNED(&sp2C, &D_80160940, 0xB8);
    if ((((D_800E62BE == 0x5F) && (D_800E62BF == 7) && (D_800E62C0 == 0x17)) || ((D_800E62BE == 0x60) && (D_800E62BF == 7) && (D_800E62C0 == 0x1C)) || ((D_800E62BE == 0x61) && (D_800E62BF == 7) && (D_800E62C0 == 0x1B))) && (D_800B593C == 0x80) && (D_80122D04 == 0) && (((u8) D_800E738A < 0xDU) || ((u8) D_800E738A >= 0x21U))) {
        func_8006B900();
    }
    *(? (**)(?))((unsigned char *)(sp + (D_800E738A * 4)) + (44))(0x80);
    if ((D_80122D04 != 0) && (((u32) D_800E7378 % 150U) == 0)) {
        func_8015A170();
    }
}
