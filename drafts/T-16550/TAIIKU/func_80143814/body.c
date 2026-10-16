void func_80143814(void) {
    TaiShorts12 sp70;
    TaiShorts12 sp5C;
    DisplayActor44 *var_s0;
    s16 *var_s2;
    s16 *var_s3;
    s16 temp_t0;
    s16 temp_t7;
    s32 var_s1;


    sp70 = D_8014A2F0;
    *(u16 *)((unsigned char *)(sp70.value) + (16)) = (u16) D_8014A2F0.value[8];
    sp5C = D_8014A304;
    var_s0 = ((DisplayActor44 *)D_8011ECD0);
    var_s1 = 0;
    var_s2 = sp70.value;
    var_s3 = sp5C.value;
    *(u16 *)((unsigned char *)(sp5C.value) + (16)) = (u16) D_8014A304.value[8];
    do {
        func_80048F64(var_s1 + 0x60);
        *(s32 *)((unsigned char *)var_s0 + (6584)) = 0x01000000;
        *(s8 *)((unsigned char *)var_s0 + (6535)) = 0x80;
        *(s32 *)((unsigned char *)var_s0 + (6540)) = (s32) D_80149FC4;
        *(s32 **)((unsigned char *)var_s0 + (6580)) = &D_8014A208;
        *(s32 *)((unsigned char *)var_s0 + (6544)) = (s32) D_80149FFC;
        temp_t7 = *var_s2;
        temp_t0 = *var_s3;
        var_s1 += 1;
        var_s0 += 1;
        var_s2 += 2;
        var_s3 += 2;
        *(s8 *)((unsigned char *)var_s0 + (6464)) = 0;
        *(s8 *)((unsigned char *)var_s0 + (6465)) = 0x80;
        *(s16 *)((unsigned char *)var_s0 + (6486)) = 0x1000;
        *(s16 *)((unsigned char *)var_s0 + (6488)) = 0x1000;
        *(s8 *)((unsigned char *)var_s0 + (6466)) = 2;
        *(s16 *)((unsigned char *)var_s0 + (6480)) = (s16) D_8014A034;
        *(s16 *)((unsigned char *)var_s0 + (6502)) = temp_t7;
        *(s16 *)((unsigned char *)var_s0 + (6484)) = temp_t0;
    } while (var_s1 < 9);
    if ((u8) D_8014A110 != 0) {
        *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6552)) = 0xE;
    }
    *(s8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6529)) = 2;
    *(s8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6530)) = 0x40;
    *(s8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6531)) = 0xA4;
    *(s8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6597)) = 3;
    *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6634)) = -0x98;
    *(s8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6598)) = 0x40;
    *(s8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6599)) = 0xA4;
    *(s8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6665)) = 4;
    *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6702)) = -0x90;
    *(s8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6666)) = 0x40;
    *(s8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6667)) = 0xA4;
    *(s8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6733)) = 5;
    *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6770)) = -0x88;
    *(s8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6734)) = 0x40;
    *(s8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6735)) = 0xA4;
    *(s8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6801)) = 1;
    *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6838)) = -0xA0;
    *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6842)) = (s16) *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6570));
    *(s8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6869)) = 7;
    *(s8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6937)) = 5;
    *(s8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7005)) = 6;
    *(s8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7073)) = 7;
    *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7110)) = -0x30;
    *(s8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7074)) = 0x40;
    *(s8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7075)) = 0xA4;
    *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6566)) = -0xA0;
}
