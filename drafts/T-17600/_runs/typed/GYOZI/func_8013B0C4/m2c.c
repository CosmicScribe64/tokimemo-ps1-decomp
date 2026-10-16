? func_80056574(?, ?, ?, ?, s32, s32);              /* extern */

void func_8013B0C4(void) {
    ? sp34;
    u8 temp_v0;

    M2C_MEMCPY_ALIGNED(&sp34, &D_801476F4, 0x90);
    if ((D_80147650 == 0) && ((u8) D_800F647A >= 0x13U) && ((u8) D_800F647A < 0x1CU)) {
        func_80056574(0x60, -0x10, 0x30, 8, 8, 0xD);
    }
    temp_v0 = D_800F647A;
    if ((D_80147650 == 1) && (temp_v0 >= 0x13U) && (temp_v0 < 0x1CU)) {
        func_80056574(-0x3C, -0x20, 0x20, 8, 8, 0xD);
    }
    *(? (**)())((unsigned char *)(sp + (temp_v0 * 4)) + (52))();
}
