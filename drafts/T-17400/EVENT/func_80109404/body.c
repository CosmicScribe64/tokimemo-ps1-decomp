void func_80109404(void) {
    typedef struct { s32 w[17]; } M2cBlock44; /* m2c memcpy of 68 bytes */
    EvShorts10 sp44;
    EvShorts10 sp58;
    EvShorts10 sp6C;
    DisplayActor44 *temp_a3;
    DisplayActor44 *temp_v0;
    s16 temp_t3;
    s16 temp_t5;
    s16 temp_t9;
    s16 var_v1;
    s16 var_v1_2;
    s32 temp_a0;
    u8 *temp_t2;

    sp6C = D_80123B7C;
    sp58 = D_80123B90;
    sp44 = D_80123BA4;
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
    *(s8 *)((unsigned char *)D_800E9620 + (6600)) = 8;
    *(s8 *)((unsigned char *)D_800E9620 + (6532)) = 8;
    *(s8 *)((unsigned char *)D_800E9620 + (6597)) = 0xC;
    temp_a3 = &((DisplayActor44 *)D_800E9620)[98];
    *(s8 *)((unsigned char *)D_800E9620 + (6529)) = 0xC;
    *(M2cBlock44 *)(temp_a3) = *(M2cBlock44 *)(&((DisplayActor44 *)D_800E9620)[96]);
    *(s8 *)((unsigned char *)D_800E9620 + (6665)) = 0xB;
    *(s8 *)((unsigned char *)D_800E9620 + (6666)) = 0x40;
    *(s16 *)((unsigned char *)D_800E9620 + (6688)) = 0x14;
    *(s16 *)((unsigned char *)D_800E9620 + (6702)) = -6;
    *(s16 *)((unsigned char *)D_800E9620 + (6706)) = -0x30;
    *(s8 *)((unsigned char *)D_800E9620 + (6671)) = 0x80;
    var_v1_2 = 0;
    do {
        temp_a0 = var_v1_2 * 2;
        temp_v0 = &((DisplayActor44 *)D_800E9620)[var_v1_2];
        *(M2cBlock44 *)(temp_v0 + 0x1A4C) = *(M2cBlock44 *)(temp_a3);
        temp_t3 = sp6C.v[var_v1_2];
        temp_t5 = sp58.v[var_v1_2];
        temp_t9 = sp44.v[var_v1_2];
        var_v1_2 += 1;
        temp_v0[99].unk_26 = (s16) (temp_t5 - 0xA0);
        temp_v0[99].unk_2A = (s16) (temp_t9 - 0x78);
        temp_v0[99].unk_18 = temp_t3;
    } while (var_v1_2 < 0xA);
    func_80011DFC(temp_a0, ((DisplayActor44 *)D_800E9620), 0x44, temp_a3);
}
