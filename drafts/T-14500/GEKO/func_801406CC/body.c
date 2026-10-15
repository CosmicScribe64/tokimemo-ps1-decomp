void func_801406CC(void) {
    s32 sp44[16];
    s32 sp6C[16];
    s32 sp58[16];
    typedef struct { s32 w[17]; } M2cBlock44; /* m2c memcpy of 68 bytes */
    s16 temp_t2_2;
    s16 temp_t4;
    s16 temp_t8;
    s16 var_v1;
    s16 var_v1_2;
    u8 *temp_t2;
    u8 *temp_v0;

    sp6C[0] = (s32) D_801472C4;
    *(s32 *)((unsigned char *)(&sp6C[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_801472C4) + (4));
    *(s32 *)((unsigned char *)(&sp6C[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_801472C4) + (8));
    *(s32 *)((unsigned char *)(&sp6C[0]) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_801472C4) + (12));
    *(s32 *)((unsigned char *)(&sp6C[0]) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_801472C4) + (16));
    sp58[0] = (s32) D_801472D8;
    *(s32 *)((unsigned char *)(&sp58[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_801472D8) + (4));
    *(s32 *)((unsigned char *)(&sp58[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_801472D8) + (8));
    *(s32 *)((unsigned char *)(&sp58[0]) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_801472D8) + (12));
    *(s32 *)((unsigned char *)(&sp58[0]) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_801472D8) + (16));
    sp44[0] = (s32) D_801472EC;
    *(s32 *)((unsigned char *)(&sp44[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_801472EC) + (4));
    *(s32 *)((unsigned char *)(&sp44[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_801472EC) + (8));
    *(s32 *)((unsigned char *)(&sp44[0]) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_801472EC) + (12));
    *(s32 *)((unsigned char *)(&sp44[0]) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_801472EC) + (16));
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
    D_80120698 = 8;
    D_80120654 = 8;
    D_80120695 = 0xC;
    D_80120651 = 0xC;
    *(M2cBlock44 *)(D_801206D8) = *(M2cBlock44 *)(D_80120650);
    D_801206D9 = 0xB;
    D_801206DA = 0x40;
    D_801206F0 = 0x14;
    D_801206FE = -6;
    D_80120702 = -0x30;
    D_801206DF = 0x80;
    var_v1_2 = 0;
    do {
        temp_v0 = D_8011ECD0 + (var_v1_2 * 0x44);
        *(M2cBlock44 *)(temp_v0 + 0x1A4C) = *(M2cBlock44 *)(D_801206D8);
        temp_t8 = (&sp6C[0])[var_v1_2];
        temp_t4 = (&sp58[0])[var_v1_2];
        temp_t2_2 = (&sp44[0])[var_v1_2];
        var_v1_2 += 1;
        *(s16 *)((unsigned char *)temp_v0 + (6770)) = (s16) (temp_t4 - 0xA0);
        *(s16 *)((unsigned char *)temp_v0 + (6774)) = (s16) (temp_t2_2 - 0x78);
        *(s16 *)((unsigned char *)temp_v0 + (6756)) = temp_t8;
    } while (var_v1_2 < 0xA);
    func_8004284C();
}
