void func_80104C3C(void) {
    typedef struct { s32 w[17]; } M2cBlock44; /* m2c memcpy of 68 bytes */
    s16 var_v1;
    s32 temp_lo;
    u8 *temp_v0;

    *(M2cBlock44 *)(&D_800EB028) = *(M2cBlock44 *)(D_800EAFA0);
    D_800EB03E = 4;
    D_800EB02A = 9;
    var_v1 = 0;
    do {
        temp_lo = var_v1 * 0x24;
        var_v1 += 1;
        temp_v0 = &D_800EC190[temp_lo];
        *(s32 *)((unsigned char *)temp_v0 + (144)) = (s32) (*(s32 *)((unsigned char *)temp_v0 + (144)) | 0x80000000);
    } while (var_v1 < 6);
    D_800EAFA3 &= 0xFF7F;
    D_800EAFE7 &= 0xFF7F;
    D_800EECD4 = 0;
    D_800B0867 = 0x7F;
    D_800B0866 = 0x7F;
    D_800B0868 = 0x7F;
    func_80011DFC(D_800EC190, 0x24, 0x80000000);
}
