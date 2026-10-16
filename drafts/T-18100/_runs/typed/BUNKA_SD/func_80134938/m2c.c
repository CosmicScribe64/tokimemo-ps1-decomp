? func_800485BC(s32, ?);                            /* extern */

void func_80134938(void) {
    DisplayActor44 *var_s0;
    s16 temp_t9;
    s32 temp_t0;
    s32 var_s1;
    s32 var_s1_2;

    var_s1 = 0;
    do {
        func_80043914(D_8013C71C, (var_s1 + 0x10) & 0xFF, 1, 1, var_s1);
        var_s1 += 1;
    } while (var_s1 < 0x10);
    func_80048F64(0x60);
    *(s8 *)((unsigned char *)D_8011ECD0 + (6529)) = 5;
    *(s32 *)((unsigned char *)D_8011ECD0 + (6584)) = 0x01000000;
    *(s8 *)((unsigned char *)D_8011ECD0 + (6531)) = 0x80;
    *(u8 **)((unsigned char *)D_8011ECD0 + (6540)) = (u8 *) D_8013C714;
    *(u8 **)((unsigned char *)D_8011ECD0 + (6544)) = (u8 *) D_8013C718;
    *(s32 **)((unsigned char *)D_8011ECD0 + (6580)) = (s32 *) D_8013C710;
    *(s16 *)((unsigned char *)D_8011ECD0 + (6550)) = 0xA;
    *(s8 *)((unsigned char *)D_8011ECD0 + (6535)) = 0;
    *(s16 *)((unsigned char *)D_8011ECD0 + (6566)) = -0xA0;
    *(s16 *)((unsigned char *)D_8011ECD0 + (6570)) = -0x78;
    *(s8 *)((unsigned char *)D_8011ECD0 + (6533)) = 0x80;
    *(s8 *)((unsigned char *)D_8011ECD0 + (6595)) = 0;
    *(s8 *)((unsigned char *)D_8011ECD0 + (6532)) = 0;
    var_s0 = D_8011ECD0 + 0x44;
    var_s1_2 = 1;
    do {
        func_80048F64(var_s1_2 + 0x60);
        *(s8 *)((unsigned char *)var_s0 + (6529)) = 4;
        *(s32 *)((unsigned char *)var_s0 + (6584)) = 0x01000000;
        *(s8 *)((unsigned char *)var_s0 + (6531)) = 0;
        *(u8 **)((unsigned char *)var_s0 + (6540)) = (u8 *) D_8013C714;
        *(u8 **)((unsigned char *)var_s0 + (6544)) = (u8 *) D_8013C718;
        temp_t9 = var_s1_2 - 1;
        var_s1_2 += 1;
        var_s0 += 0x44;
        *(s16 *)((unsigned char *)var_s0 + (6482)) = temp_t9;
        *(s8 *)((unsigned char *)var_s0 + (6466)) = 1;
        *(s8 *)((unsigned char *)var_s0 + (6467)) = 0;
        *(s16 *)((unsigned char *)var_s0 + (6498)) = -0xA0;
        *(s16 *)((unsigned char *)var_s0 + (6502)) = -0x78;
        *(s8 *)((unsigned char *)var_s0 + (6465)) = 0x80;
        *(s8 *)((unsigned char *)var_s0 + (6527)) = 0;
        *(s8 *)((unsigned char *)var_s0 + (6464)) = 0;
        *(s32 **)((unsigned char *)var_s0 + (6512)) = (s32 *) D_8013C710;
    } while (var_s1_2 != 5);
    temp_t0 = D_80122EAC * 2;
    *(s16 *)((unsigned char *)D_8011ECD0 + (6754)) = (s16) (temp_t0 + 2);
    *(s8 *)((unsigned char *)D_8011ECD0 + (6735)) = 0x80;
    *(s16 *)((unsigned char *)D_8011ECD0 + (6822)) = (s16) (temp_t0 + 3);
    func_800485BC(*(s32 *)((unsigned char *)D_8013C710 + (44)), 1);
    func_800485BC(*(s32 *)((unsigned char *)D_8013C710 + (48)), 1);
    func_800485BC(*(s32 *)((unsigned char *)D_8013C710 + (52)), 1);
    func_800485BC(*(s32 *)((unsigned char *)D_8013C710 + (56)), 1);
}
