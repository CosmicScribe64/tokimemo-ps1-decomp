s16 *func_8004E970(s32);                            /* extern */

void func_80132384(void) {
    Arrs4x8 sp50;
    s16 *temp_v0;
    s16 *temp_v0_2;
    s16 temp_s0;
    s16 temp_v1;
    s16 temp_v1_2;
    s32 temp_s1;
    s32 var_a0;

    if (*(s16 *)((unsigned char *)D_8011ECD0 + (6566)) < -0xA0) {
        *(s16 *)((unsigned char *)D_8011ECD0 + (6566)) = (s16) (*(s16 *)((unsigned char *)D_8011ECD0 + (6566)) + 0xC);
    } else {
        *(s16 *)((unsigned char *)D_8011ECD0 + (6566)) = -0xA0;
    }
    func_801325A0(&sp50);
    var_a0 = 0;
    do {
        temp_s1 = var_a0 + 1;
        if (D_800E6280.unk_1109 != temp_s1) {
            temp_v0 = func_8004E970(var_a0);
            temp_v1 = *temp_v0;
            if (temp_v1 < -0x78) {
                *temp_v0 = temp_v1 + 0xC;
            } else {
                *temp_v0 = -0x78;
            }
        } else {
            temp_s0 = (var_a0 * 0x14) - 0x58;
            temp_v0_2 = func_8004E970(var_a0);
            temp_v1_2 = *(s16 *)((unsigned char *)temp_v0_2 + (2));
            if (temp_v1_2 < temp_s0) {
                *(s16 *)((unsigned char *)temp_v0_2 + (2)) = (s16) (temp_v1_2 + 0xC);
                *(s16 *)((unsigned char *)D_8011ECD0 + (6638)) = (s16) (*(s16 *)((unsigned char *)D_8011ECD0 + (6638)) + 0xC);
                if (*(s16 *)((unsigned char *)temp_v0_2 + (2)) >= temp_s0) {
                    *(s16 *)((unsigned char *)temp_v0_2 + (2)) = temp_s0;
                    *(s16 *)((unsigned char *)D_8011ECD0 + (6638)) = (s16) (*(s16 *)((unsigned char *)(&sp50.a[D_800E6280.unk_1093]) + (16)) + 6);
                }
            } else {
                *(s16 *)((unsigned char *)temp_v0_2 + (2)) = temp_s0;
                *(s16 *)((unsigned char *)D_8011ECD0 + (6638)) = (s16) (*(s16 *)((unsigned char *)(&sp50.a[D_800E6280.unk0]) + (16)) + 6);
            }
        }
        var_a0 = temp_s1;
    } while (temp_s1 != 8);
}
