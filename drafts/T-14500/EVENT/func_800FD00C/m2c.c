extern ? D_801214A0;

void func_800FD00C(void) {
    ? sp30;

    M2C_MEMCPY_ALIGNED(&sp30, &D_801214A0, 0x9C);
    func_80078950("%d\n", D_800B1AF6);
    *(? (**)(?))((unsigned char *)(sp + (D_800B1AF6 * 4)) + (48))(0x80);
    if (D_800EECE0 != 0) {
        D_800EB02B |= 0x80;
        if ((u32) ((u32) D_800B1AE4 % 40U) < 0x14U) {
            D_800EC2F8 &= 0x7FFFFFFF;
        } else {
            D_800EC2F8 |= 0x80000000;
        }
        D_800EC30E = D_800F19AB;
        D_800EC30D = D_800F19AB;
        D_800EC30C = D_800F19AB;
        D_800EB02F = D_800F19AB;
        return;
    }
    D_800EB02B &= 0xFF7F;
    D_800EC2F8 |= 0x80000000;
}
