extern s16 D_800E6556;
extern s16 D_800E655A;
extern s16 D_800E655E;
extern ? D_8015F9A0;
extern ? D_8015F9A8;

void func_801556D8(void) {
    s16 temp_s0;
    s16 var_s0;
    s16 var_s0_2;
    s32 temp_at;
    s8 *temp_t2;
    s8 *temp_t5;
    void *temp_v0;

    sp68[0].unk0 = (s32) D_8015F9B0.unk0;
    *(s32 *)((unsigned char *)(&sp68[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8015F9B0) + (4));
    *(s32 *)((unsigned char *)(&sp68[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8015F9B0) + (8));
    *(s32 *)((unsigned char *)(&sp68[0]) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8015F9B0) + (12));
    sp58[0].unk0 = (s32) D_8015F9C0.unk0;
    *(s32 *)((unsigned char *)(&sp58[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8015F9C0) + (4));
    *(s32 *)((unsigned char *)(&sp58[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8015F9C0) + (8));
    *(s32 *)((unsigned char *)(&sp58[0]) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8015F9C0) + (12));
    D_8015E208 = D_8015DF04;
    D_8015E20C = D_8015E040;
    D_8015E210 = D_8015E17C;
    D_800E6556 += 2;
    D_800E655A += 1;
    D_800E655E -= 0x14;
    func_80084D3C();
    func_80155360();
    func_80043914(D_8015F93C, 0x11, 1, 2, 0);
    func_80084E90(D_8015F940, D_8015F944, D_8015F948, D_8015F94C, D_8015F950, D_8015F954);
    func_800850D4(D_8015F930, D_8015F934, D_8015F92C, (s32) D_8015F938);
    D_80120654 = 8;
    D_80120698 = 8;
    var_s0 = 0;
    do {
        temp_t5 = &D_8015F9A0 + var_s0;
        var_s0 += 1;
        *temp_t5 = 0;
    } while (var_s0 < 8);
    var_s0_2 = 0;
    do {
        temp_v0 = D_8011ECD0 + (var_s0_2 * 0x44);
        M2C_MEMCPY_ALIGNED(temp_v0 + 0x1A08, D_80120650, 0x44);
        *(s16 *)((unsigned char *)temp_v0 + (6686)) = 5;
        *(s16 *)((unsigned char *)temp_v0 + (6702)) = (s16) ((&sp68[0])[var_s0_2] - 0xA0);
        *(s16 *)((unsigned char *)temp_v0 + (6706)) = (s16) ((&sp58[0])[var_s0_2] - 0x78);
        temp_t2 = &D_8015F9A8 + var_s0_2;
        temp_s0 = var_s0_2 + 1;
        temp_at = temp_s0 < 8;
        var_s0_2 = temp_s0;
        *temp_t2 = (s8) (func_800AE0D0() % 120);
    } while (temp_at != 0);
    D_801206EE = 4;
    func_8004284C();
}
