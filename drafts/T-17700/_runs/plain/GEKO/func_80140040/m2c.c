void func_80140040(void) {
    s16 var_v1;
    s16 var_v1_2;
    s32 temp_lo;
    s32 var_v0;
    void *temp_v0;
    void *temp_v0_2;

    var_v1 = 0;
    do {
        if ((u32) D_801470AC >= (u32) (var_v1 * 0x1E)) {
            temp_v0 = D_8011ECD0 + (var_v1 * 0x44);
            *(s16 *)((unsigned char *)temp_v0 + (6778)) = -1;
            *(s16 *)((unsigned char *)temp_v0 + (6782)) = 1;
        }
        temp_v0_2 = D_8011ECD0 + (var_v1 * 0x44);
        *(s32 *)((unsigned char *)temp_v0_2 + (6772)) = (s32) (*(s32 *)((unsigned char *)temp_v0_2 + (6772)) + *(s32 *)((unsigned char *)temp_v0_2 + (6780)));
        *(s32 *)((unsigned char *)temp_v0_2 + (6768)) = (s32) (*(s32 *)((unsigned char *)temp_v0_2 + (6768)) + *(s32 *)((unsigned char *)temp_v0_2 + (6776)));
        if (*(s16 *)((unsigned char *)temp_v0_2 + (6774)) >= 0xA1) {
            *(u8 *)((unsigned char *)temp_v0_2 + (6735)) = (u8) (*(u8 *)((unsigned char *)temp_v0_2 + (6735)) & ~0x80);
        }
        var_v1 += 1;
    } while (var_v1 < 0xA);
    var_v0 = 0;
    var_v1_2 = 0;
    do {
        temp_lo = var_v1_2 * 0x44;
        var_v1_2 += 1;
        if (*(u8 *)((unsigned char *)(&D_8011ECD0[temp_lo]) + (6735)) & 0x80) {
            var_v0 = 1;
        }
    } while (var_v1_2 < 0xA);
    if (var_v0 == 0) {
        func_800AE0D0();
    }
    D_801470AC += 1;
}
