extern u8 D_800E738A;
extern ? D_80144FB4;

void func_80138240(void) {
    ? sp30;
    u8 temp_v0;

    M2C_MEMCPY_ALIGNED(&sp30, &D_80144FB4, 0x94);
    if ((D_80144F10 == 0) && ((u8) D_800E738A >= 0x13U) && ((u8) D_800E738A < 0x1CU)) {
        func_8004A414(0x60, -0x10, 0x30, 8, 8, 0xD);
    }
    temp_v0 = D_800E738A;
    if ((D_80144F10 == 1) && (temp_v0 >= 0x13U) && (temp_v0 < 0x1CU)) {
        func_8004A414(-0x3C, -0x20, 0x20, 8, 8, 0xD);
    }
    *(? (**)())((unsigned char *)(sp + (temp_v0 * 4)) + (48))();
}
