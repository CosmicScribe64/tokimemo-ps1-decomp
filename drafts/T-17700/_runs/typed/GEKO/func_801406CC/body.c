void func_801406CC(void) {
    typedef struct { s32 w[17]; } M2cBlock44; /* m2c memcpy of 68 bytes */
    GekoShorts10 sp44;
    GekoShorts10 sp58;
    GekoShorts10 sp6C;
    DisplayActor44 *temp_a3;
    DisplayActor44 *temp_v0;
    s16 temp_t2_2;
    s16 temp_t4;
    s16 temp_t8;
    s16 var_v1;
    s16 var_v1_2;
    u8 *temp_t2;

    sp6C = D_801472C4;
    sp58 = D_801472D8;
    sp44 = D_801472EC;
    func_8013F410();
    load_palette(D_8014708C, 0x11, 1, 2, 0);
    func_80084E90(D_80147090, D_80147094, D_80147098, D_8014709C, D_801470A0, D_801470A4);
    var_v1 = 0;
    do {
        temp_t2 = &D_800E6280 + var_v1;
        var_v1 += 1;
        *(s8 *)((unsigned char *)temp_t2 + (4319)) = 0xC;
    } while (var_v1 < 6);
    func_800850D4(D_80147080, D_80147084, D_8014707C, (s32) D_80147088);
    D_800CA360 = 1;
    *(s8 *)((unsigned char *)D_8011ECD0 + (6600)) = 8;
    *(s8 *)((unsigned char *)D_8011ECD0 + (6532)) = 8;
    temp_a3 = &((DisplayActor44 *)D_8011ECD0)[98];
    *(s8 *)((unsigned char *)D_8011ECD0 + (6597)) = 0xC;
    *(s8 *)((unsigned char *)D_8011ECD0 + (6529)) = 0xC;
    *(M2cBlock44 *)(temp_a3) = *(M2cBlock44 *)(&((DisplayActor44 *)D_8011ECD0)[96]);
    *(s8 *)((unsigned char *)D_8011ECD0 + (6665)) = 0xB;
    *(s8 *)((unsigned char *)D_8011ECD0 + (6666)) = 0x40;
    *(s16 *)((unsigned char *)D_8011ECD0 + (6688)) = 0x14;
    *(s16 *)((unsigned char *)D_8011ECD0 + (6702)) = -6;
    *(s16 *)((unsigned char *)D_8011ECD0 + (6706)) = -0x30;
    *(s8 *)((unsigned char *)D_8011ECD0 + (6671)) = 0x80;
    var_v1_2 = 0;
    do {
        temp_v0 = &((DisplayActor44 *)D_8011ECD0)[var_v1_2];
        *(M2cBlock44 *)(temp_v0 + 0x1A4C) = *(M2cBlock44 *)(temp_a3);
        temp_t8 = sp6C.v[var_v1_2];
        temp_t4 = sp58.v[var_v1_2];
        temp_t2_2 = sp44.v[var_v1_2];
        var_v1_2 += 1;
        temp_v0[99].unk_26 = (s16) (temp_t4 - 0xA0);
        temp_v0[99].unk_2A = (s16) (temp_t2_2 - 0x78);
        temp_v0[99].unk_18 = temp_t8;
    } while (var_v1_2 < 0xA);
    func_8004284C();
}
