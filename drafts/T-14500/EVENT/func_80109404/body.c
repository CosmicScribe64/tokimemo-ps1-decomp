void func_80109404(void) {
    s32 sp44[16];
    s32 sp58[16];
    s32 sp6C[16];
    typedef struct { s32 w[17]; } M2cBlock44; /* m2c memcpy of 68 bytes */
    s16 temp_t3;
    s16 temp_t5;
    s16 temp_t9;
    s16 var_v1;
    s16 var_v1_2;
    s32 temp_a0;
    u8 *temp_t2;
    u8 *temp_v0;

    sp6C[0] = (s32) D_80123B7C;
    *(s32 *)((unsigned char *)(&sp6C[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80123B7C) + (4));
    *(s32 *)((unsigned char *)(&sp6C[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80123B7C) + (8));
    *(s32 *)((unsigned char *)(&sp6C[0]) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80123B7C) + (12));
    *(s32 *)((unsigned char *)(&sp6C[0]) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80123B7C) + (16));
    sp58[0] = (s32) D_80123B90;
    *(s32 *)((unsigned char *)(&sp58[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80123B90) + (4));
    *(s32 *)((unsigned char *)(&sp58[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80123B90) + (8));
    *(s32 *)((unsigned char *)(&sp58[0]) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80123B90) + (12));
    *(s32 *)((unsigned char *)(&sp58[0]) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80123B90) + (16));
    sp44[0] = (s32) D_80123BA4;
    *(s32 *)((unsigned char *)(&sp44[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80123BA4) + (4));
    *(s32 *)((unsigned char *)(&sp44[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80123BA4) + (8));
    *(s32 *)((unsigned char *)(&sp44[0]) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80123BA4) + (12));
    *(s32 *)((unsigned char *)(&sp44[0]) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80123BA4) + (16));
    func_80108330();
    func_80012D64(D_8012396C, 0x11, 1, 2, 0);
    func_8004C46C(D_80123970, D_80123974, D_80123978, D_8012397C, D_80123980, D_80123984);
    var_v1 = 0;
    do {
        temp_t2 = &D_800B0860[var_v1];
        var_v1 += 1;
        *(s8 *)((unsigned char *)temp_t2 + (4241)) = 0xC;
    } while (var_v1 < 6);
    func_8004C6B0(D_80123960, D_80123964, D_8012395C, (s32) D_80123968);
    D_800EAFE8 = 8;
    D_800EAFA4 = 8;
    D_800EAFE5 = 0xC;
    D_800EAFA1 = 0xC;
    *(M2cBlock44 *)(&D_800EB028) = *(M2cBlock44 *)(D_800EAFA0);
    D_800EB029 = 0xB;
    D_800EB02A = 0x40;
    D_800EB040 = 0x14;
    D_800EB04E = -6;
    D_800EB052 = -0x30;
    D_800EB02F = 0x80;
    var_v1_2 = 0;
    do {
        temp_a0 = var_v1_2 * 2;
        temp_v0 = D_800E9620 + (var_v1_2 * 0x44);
        *(M2cBlock44 *)(temp_v0 + 0x1A4C) = *(M2cBlock44 *)(&D_800EB028);
        temp_t3 = (&sp6C[0])[var_v1_2];
        temp_t5 = (&sp58[0])[var_v1_2];
        temp_t9 = (&sp44[0])[var_v1_2];
        var_v1_2 += 1;
        *(s16 *)((unsigned char *)temp_v0 + (6770)) = (s16) (temp_t5 - 0xA0);
        *(s16 *)((unsigned char *)temp_v0 + (6774)) = (s16) (temp_t9 - 0x78);
        *(s16 *)((unsigned char *)temp_v0 + (6756)) = temp_t3;
    } while (var_v1_2 < 0xA);
    func_80011DFC(temp_a0, D_800E9620, 0x44, &D_800EB028);
}
