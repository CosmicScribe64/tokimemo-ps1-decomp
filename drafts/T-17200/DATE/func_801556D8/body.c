void func_801556D8(void) {
    typedef struct { s32 w[17]; } M2cBlock44; /* m2c memcpy of 68 bytes */
    DateShorts8 sp58;
    DateShorts8 sp68;
    DisplayActor44 *temp_v0;
    s16 temp_s0;
    s16 var_s0;
    s16 var_s0_2;
    s32 temp_at;
    s8 *temp_t2;
    s8 *temp_t5;

    sp68 = D_8015F9B0;
    sp58 = D_8015F9C0;
    D_8015E208 = D_8015DF04;
    D_8015E20C = D_8015E040;
    D_8015E210 = D_8015E17C;
    D_800E6280.unk_1BC[5].unk_02 += 2;
    D_800E6280.unk_1BC[5].unk_06 += 1;
    D_800E6280.unk_1BC[5].unk_0A -= 0x14;
    check_para_limit();
    func_80155360();
    load_palette(D_8015F93C, 0x11, 1, 2, 0);
    func_80084E90(D_8015F940, D_8015F944, D_8015F948, D_8015F94C, D_8015F950, D_8015F954);
    func_800850D4(D_8015F930, D_8015F934, D_8015F92C, (s32) D_8015F938);
    ((DisplayActor44 *)D_8011ECD0)[96].unk_04 = 8;
    ((DisplayActor44 *)D_8011ECD0)[97].unk_04 = 8;
    var_s0 = 0;
    do {
        temp_t5 = &D_8015F9A0 + var_s0;
        var_s0 += 1;
        *temp_t5 = 0;
    } while (var_s0 < 8);
    var_s0_2 = 0;
    do {
        temp_v0 = &((DisplayActor44 *)D_8011ECD0)[var_s0_2];
        *(M2cBlock44 *)(temp_v0 + 0x1A08) = *(M2cBlock44 *)(&((DisplayActor44 *)D_8011ECD0)[96]);
        temp_v0[98].unk_16 = 5;
        temp_v0[98].unk_26 = (s16) (sp68.v[var_s0_2] - 0xA0);
        temp_v0[98].unk_2A = (s16) (sp58.v[var_s0_2] - 0x78);
        temp_t2 = &D_8015F9A8 + var_s0_2;
        temp_s0 = var_s0_2 + 1;
        temp_at = temp_s0 < 8;
        var_s0_2 = temp_s0;
        *temp_t2 = (s8) (func_800AE0D0() % 120);
    } while (temp_at != 0);
    ((DisplayActor44 *)D_8011ECD0)[98].unk_16 = 4;
    func_8004284C();
}
