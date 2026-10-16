extern u8 D_800E62BE;
extern u8 D_800E62BF;
extern u8 D_800E62C0;
extern u32 D_800E7378;
extern u8 D_800E738A;
extern ? D_8015EB88;

void func_80150ED4(void) {
    ? sp30;
    u32 var_v0;

    M2C_MEMCPY_ALIGNED(&sp30, &D_8015EB88, 0x9C);
    if ((((D_800E62BE == 0x5F) && (D_800E62BF == 7) && (D_800E62C0 == 0x17)) || ((D_800E62BE == 0x60) && (D_800E62BF == 7) && (D_800E62C0 == 0x1C)) || ((D_800E62BE == 0x61) && (D_800E62BF == 7) && (D_800E62C0 == 0x1B))) && (D_800B593C == 0x80) && (D_80122D04 == 0) && ((u8) D_800E738A >= 0x1AU)) {
        func_8006B900();
    }
    *(? (**)(?))((unsigned char *)(sp + (D_800E738A * 4)) + (48))(0x80);
    if (((u8) D_800E738A >= 0xDU) && ((u8) D_800E738A < 0x10U)) {
        var_v0 = (u32) D_800E7378 % 180U;
        if (var_v0 == 0) {
            func_8015114C();
            var_v0 = (u32) D_800E7378 % 180U;
        }
        if (var_v0 == 0x5A) {
            func_8015116C();
        }
    }
}
