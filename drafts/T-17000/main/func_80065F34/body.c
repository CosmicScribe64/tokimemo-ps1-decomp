void func_80065F34(s32 arg0) {
    DisplayActor44 *var_v0_2;
    DisplayActor44 *var_v0_4;
    s16 temp_v0;
    s32 temp_a2;
    s32 temp_t4_2;
    s32 temp_t6;
    s32 var_a2;
    s32 var_v1;
    s32 var_v1_2;
    u8 temp_t0;
    u8 temp_t3;
    u8 temp_t4;
    u8 temp_t4_3;
    u8 temp_t6_2;
    u8 temp_t6_3;
    u8 temp_t8;
    u8 temp_t8_2;
    u8 *var_v0;
    u8 *var_v0_3;

    temp_v0 = D_800B5B24;
    if ((temp_v0 != 0) || (temp_v0 < 0xB)) {
        if (arg0 == 1) {
            temp_a2 = temp_v0 + 2;
            var_v1 = 0;
            if (temp_a2 > 0) {
                temp_t6 = (temp_v0 + 2) & 3;
                if (temp_t6 != 0) {
                    var_v0 = (0 << 6) + ((DisplayActor44 *)D_8011ECD0);
                    do {
                        var_v1 += 1;
                        *(u8 *)((unsigned char *)var_v0 + (207)) = (u8) (*(u8 *)((unsigned char *)var_v0 + (207)) | 0x80);
                        var_v0 += 0x44;
                    } while (temp_t6 != var_v1);
                    if (var_v1 != temp_a2) {
                        goto block_8;
                    }
                } else {
block_8:
                    var_v0_2 = &((DisplayActor44 *)D_8011ECD0)[var_v1];
                    do {
                        temp_t4 = *(u8 *)((unsigned char *)var_v0_2 + (207));
                        temp_t6_2 = *(u8 *)((unsigned char *)var_v0_2 + (275));
                        temp_t8 = *(u8 *)((unsigned char *)var_v0_2 + (343));
                        temp_t0 = *(u8 *)((unsigned char *)var_v0_2 + (411));
                        var_v0_2 += 4;
                        *(s8 *)((unsigned char *)var_v0_2 + (139)) = (s8) (temp_t0 | 0x80);
                        *(s8 *)((unsigned char *)var_v0_2 + (71)) = (s8) (temp_t8 | 0x80);
                        var_v0_2->unk_03 = temp_t6_2 | 0x80;
                        *(s8 *)((unsigned char *)var_v0_2 + (-65)) = (s8) (temp_t4 | 0x80);
                    } while (var_v0_2 != &((DisplayActor44 *)D_8011ECD0)[temp_a2]);
                }
            }
            k_disp_switch(0, 1);
            return;
        }
        var_a2 = temp_v0 + 2;
        if (((DisplayActor44 *)D_8011ECD0)->unk_00 & 0x80) {
            k_disp_switch(0, 0);
            var_a2 = D_800B5B24 + 2;
        }
        var_v1_2 = 0;
        if (var_a2 > 0) {
            temp_t4_2 = (temp_v0 + 2) & 3;
            if (temp_t4_2 != 0) {
                var_v0_3 = (0 << 6) + ((DisplayActor44 *)D_8011ECD0);
                do {
                    var_v1_2 += 1;
                    *(u8 *)((unsigned char *)var_v0_3 + (207)) = (u8) (*(u8 *)((unsigned char *)var_v0_3 + (207)) & 0x7F);
                    var_v0_3 += 0x44;
                } while (temp_t4_2 != var_v1_2);
                if (var_v1_2 != var_a2) {
                    goto block_18;
                }
            } else {
block_18:
                var_v0_4 = &((DisplayActor44 *)D_8011ECD0)[var_v1_2];
                do {
                    temp_t3 = *(u8 *)((unsigned char *)var_v0_4 + (207));
                    temp_t4_3 = *(u8 *)((unsigned char *)var_v0_4 + (275));
                    temp_t6_3 = *(u8 *)((unsigned char *)var_v0_4 + (343));
                    temp_t8_2 = *(u8 *)((unsigned char *)var_v0_4 + (411));
                    var_v0_4 += 4;
                    *(s8 *)((unsigned char *)var_v0_4 + (139)) = (s8) (temp_t8_2 & 0x7F);
                    *(s8 *)((unsigned char *)var_v0_4 + (71)) = (s8) (temp_t6_3 & 0x7F);
                    var_v0_4->unk_03 = temp_t4_3 & 0x7F;
                    *(s8 *)((unsigned char *)var_v0_4 + (-65)) = (s8) (temp_t3 & 0x7F);
                } while (var_v0_4 != &((DisplayActor44 *)D_8011ECD0)[var_a2]);
            }
        }
        D_80125CA0 = 0xB8;
    }
}
