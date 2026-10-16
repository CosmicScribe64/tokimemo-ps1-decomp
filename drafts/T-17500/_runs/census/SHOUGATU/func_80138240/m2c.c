extern u8 D_800E738A;
extern s32 sp;
void func_80138240(void) {
    typedef struct { s32 w[37]; } M2cBlock94; /* m2c memcpy of 148 bytes */
    s32 sp30[37];
    u8 temp_v0;

    *(M2cBlock94 *)(&sp30) = *(M2cBlock94 *)(&D_80144FB4);
    if ((D_80144F10 == 0) && ((u8) D_800E738A >= 0x13U) && ((u8) D_800E738A < 0x1CU)) {
        func_8004A414(0x60, -0x10, 0x30, 8, 8, 0xD);
    }
    temp_v0 = D_800E738A;
    if ((D_80144F10 == 1) && (temp_v0 >= 0x13U) && (temp_v0 < 0x1CU)) {
        func_8004A414(-0x3C, -0x20, 0x20, 8, 8, 0xD);
    }
    (*(s32 (**)())((unsigned char *)(sp + (temp_v0 * 4)) + (48)))();
}
