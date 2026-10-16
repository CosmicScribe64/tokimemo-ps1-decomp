void func_8014494C(void) {
    typedef struct { s32 w[16]; } M2cBlock40; /* m2c memcpy of 64 bytes */
    typedef struct { s32 w[17]; } M2cBlock44; /* m2c memcpy of 68 bytes */
    s32 temp_a0;

    func_80140050();
    temp_a0 = D_800F6474 + 1;
    D_800F6474 = temp_a0;
    if (D_800F6474 == 0) {
        *(M2cBlock40 *)(&D_8012B948) = *(M2cBlock40 *)(D_8012B8C0);
        *(s32 *)((unsigned char *)(&D_8012B948 + 0x3C) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8012B8C0->pad1A[0x22]) + (4));
        *(M2cBlock44 *)(&D_8012B948) = *(M2cBlock44 *)(D_8012B8C0);
        D_8012B95E = 4;
    }
    if (((u16) D_800D9250 == 7) && ((u16) D_800D9254 == 1) && (func_8005B32C(temp_a0) == 0)) {
        D_8012B94A = 1;
        return;
    }
    D_8012B94A = 0;
    D_8012B960 = 0;
}
