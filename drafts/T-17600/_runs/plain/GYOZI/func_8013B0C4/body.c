void func_8013B0C4(void) {
    typedef struct { s32 w[36]; } M2cBlock90; /* m2c memcpy of 144 bytes */
    s32 sp34[36];
    u8 temp_v0;

    *(M2cBlock90 *)(&sp34) = *(M2cBlock90 *)(&D_801476F4);
    if ((D_80147650 == 0) && ((u8) D_800F647A >= 0x13U) && ((u8) D_800F647A < 0x1CU)) {
        func_80056574(0x60, -0x10, 0x30, 8, 8, 0xD);
    }
    temp_v0 = D_800F647A;
    if ((D_80147650 == 1) && (temp_v0 >= 0x13U) && (temp_v0 < 0x1CU)) {
        func_80056574(-0x3C, -0x20, 0x20, 8, 8, 0xD);
    }
    (*(s32 (**)())((unsigned char *)(sp + (temp_v0 * 4)) + (52)))();
}
