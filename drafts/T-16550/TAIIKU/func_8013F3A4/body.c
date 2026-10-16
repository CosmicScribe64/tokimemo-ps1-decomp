void func_8013F3A4(s32 arg0) {
    s32 sp28;
    DisplayActor44 *temp_v0;
    DisplayActor44 *temp_v0_2;
    DisplayActor44 *temp_v0_3;
    DisplayActor44 *temp_v0_4;
    DisplayActor44 *temp_v0_5;
    DisplayActor44 *temp_v0_6;
    DisplayActor44 *temp_v0_7;
    DisplayActor44 *temp_v0_8;
    s16 temp_v1_2;
    s16 temp_v1_4;
    s16 temp_v1_6;
    s16 temp_v1_9;
    s16 var_a1;
    s32 temp_t3;
    s32 temp_t6;
    s32 temp_t8;
    s32 temp_v1;
    s32 temp_v1_3;
    s32 temp_v1_5;
    s32 temp_v1_7;
    s32 temp_v1_8;
    s32 var_at;
    s32 var_v1;

    if (arg0 == 0) {
        var_v1 = D_80122EC4;
        var_a1 = D_801499C2;
        if (D_80149990.unk_00 != 0) {
            *(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7075)) = (u8) (*(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7075)) | 0x40);
            *(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7347)) = (u8) (*(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7347)) | 0x40);
        } else {
            *(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7075)) = (u8) (((DisplayActor44 *)D_8011ECD0)->unk_00 & 0xFFBF);
            *(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7347)) = (u8) (*(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7347)) & 0xFFBF);
        }
    } else {
        if (D_801499CB[arg0] == 1) {
            temp_v0 = &((DisplayActor44 *)D_8011ECD0)[arg0];
            *(u8 *)((unsigned char *)temp_v0 + (7075)) = (u8) (*(u8 *)((unsigned char *)temp_v0 + (7075)) | 0x40);
            *(u8 *)((unsigned char *)temp_v0 + (7347)) = (u8) (*(u8 *)((unsigned char *)temp_v0 + (7347)) | 0x40);
        } else {
            temp_v0_2 = &((DisplayActor44 *)D_8011ECD0)[arg0];
            *(u8 *)((unsigned char *)temp_v0_2 + (7075)) = (u8) (*(u8 *)((unsigned char *)temp_v0_2 + (7075)) & 0xFFBF);
            *(u8 *)((unsigned char *)temp_v0_2 + (7347)) = (u8) (*(u8 *)((unsigned char *)temp_v0_2 + (7347)) & 0xFFBF);
        }
        sp28 = func_8013FB14(arg0);
        var_v1 = sp28;
        var_a1 = func_8013FBA8(arg0);
    }
    switch (var_v1) {
    case 0:
    case 6:
        temp_v0_3 = &((DisplayActor44 *)D_8011ECD0)[arg0];
        *(s16 *)((unsigned char *)temp_v0_3 + (7110)) = (s16) (((var_a1 >> 3) + (var_a1 >> 6)) - 0x48);
        *(s16 *)((unsigned char *)temp_v0_3 + (7382)) = (s16) (*(s16 *)((unsigned char *)temp_v0_3 + (7110)) + 0x10);
        return;
    case 1:
        temp_v1 = var_a1 - 0x200;
        temp_v0_4 = &((DisplayActor44 *)D_8011ECD0)[arg0];
        if (temp_v1 < 0x80) {
            *(s16 *)((unsigned char *)temp_v0_4 + (7114)) = (s16) (-0x48 - (temp_v1 >> 4));
            *(s16 *)((unsigned char *)temp_v0_4 + (7110)) = (s16) ((temp_v1 >> 2) + 0x48);
            *(s16 *)((unsigned char *)temp_v0_4 + (7114)) = (s16) ((*(s16 *)((unsigned char *)temp_v0_4 + (7114)) - arg0) + 3);
        } else {
            temp_t8 = (s32) (var_a1 - 0x280) >> 3;
            *(s16 *)((unsigned char *)temp_v0_4 + (7114)) = (s16) (-0x50 - temp_t8);
            *(s16 *)((unsigned char *)temp_v0_4 + (7110)) = (s16) (temp_t8 + 0x68);
            *(s16 *)((unsigned char *)temp_v0_4 + (7114)) = (s16) ((*(s16 *)((unsigned char *)temp_v0_4 + (7114)) - arg0) + 3);
        }
        temp_v1_2 = *(s16 *)((unsigned char *)temp_v0_4 + (7110));
        if ((temp_v1_2 >= 0x68) && (*(s16 *)((unsigned char *)temp_v0_4 + (7096)) < 0x8E)) {
            if (temp_v0_4 == ((DisplayActor44 *)D_8011ECD0)) {
                *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7096)) = 0x90;
            } else {
                *(s16 *)((unsigned char *)temp_v0_4 + (7096)) = 0x8E;
            }
            *(u8 *)((unsigned char *)temp_v0_4 + (7075)) = 0x84U;
        }
        if (temp_v1_2 < 0x68) {
            *(s16 *)((unsigned char *)temp_v0_4 + (7382)) = (s16) (temp_v1_2 + 0x10);
            *(s16 *)((unsigned char *)temp_v0_4 + (7386)) = (s16) *(s16 *)((unsigned char *)temp_v0_4 + (7114));
            return;
        }
        *(s16 *)((unsigned char *)temp_v0_4 + (7382)) = temp_v1_2;
        *(s16 *)((unsigned char *)temp_v0_4 + (7386)) = (s16) (*(s16 *)((unsigned char *)temp_v0_4 + (7114)) - 8);
        return;
    case 2:
        temp_v0_5 = &((DisplayActor44 *)D_8011ECD0)[arg0];
        if (var_a1 < 0x380) {
            temp_t6 = (s32) (var_a1 - 0x300) >> 4;
            *(s16 *)((unsigned char *)temp_v0_5 + (7114)) = (s16) (-0x60 - temp_t6);
            *(s16 *)((unsigned char *)temp_v0_5 + (7110)) = (s16) (0x78 - temp_t6);
            *(s16 *)((unsigned char *)temp_v0_5 + (7114)) = (s16) ((*(s16 *)((unsigned char *)temp_v0_5 + (7114)) - arg0) + 3);
        } else {
            temp_v1_3 = var_a1 - 0x380;
            *(s16 *)((unsigned char *)temp_v0_5 + (7114)) = (s16) (-0x68 - (temp_v1_3 >> 4));
            *(s16 *)((unsigned char *)temp_v0_5 + (7110)) = (s16) (0x68 - (temp_v1_3 >> 2));
            *(s16 *)((unsigned char *)temp_v0_5 + (7114)) = (s16) ((*(s16 *)((unsigned char *)temp_v0_5 + (7114)) - arg0) + 3);
        }
        temp_v1_4 = *(s16 *)((unsigned char *)temp_v0_5 + (7110));
        if ((temp_v1_4 < 0x69) && (*(s16 *)((unsigned char *)temp_v0_5 + (7096)) >= 0x8E)) {
            if (temp_v0_5 == ((DisplayActor44 *)D_8011ECD0)) {
                *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7096)) = 0x8A;
            } else {
                *(s16 *)((unsigned char *)temp_v0_5 + (7096)) = 0x86;
            }
            *(u8 *)((unsigned char *)temp_v0_5 + (7075)) = 0x84U;
        }
        if (temp_v1_4 < 0x68) {
            *(s16 *)((unsigned char *)temp_v0_5 + (7382)) = (s16) (temp_v1_4 - 0x10);
            *(s16 *)((unsigned char *)temp_v0_5 + (7386)) = (s16) *(s16 *)((unsigned char *)temp_v0_5 + (7114));
            return;
        }
        *(s16 *)((unsigned char *)temp_v0_5 + (7382)) = temp_v1_4;
        *(s16 *)((unsigned char *)temp_v0_5 + (7386)) = (s16) (*(s16 *)((unsigned char *)temp_v0_5 + (7114)) - 8);
        return;
    case 3:
        temp_v0_6 = &((DisplayActor44 *)D_8011ECD0)[arg0];
        *(s16 *)((unsigned char *)temp_v0_6 + (7110)) = (s16) ((-(var_a1 >> 3) - (var_a1 >> 6)) + 0x48);
        *(s16 *)((unsigned char *)temp_v0_6 + (7382)) = (s16) (*(s16 *)((unsigned char *)temp_v0_6 + (7110)) - 0x10);
        *(s16 *)((unsigned char *)temp_v0_6 + (7386)) = (s16) *(s16 *)((unsigned char *)temp_v0_6 + (7114));
        return;
    case 4:
        temp_v1_5 = var_a1 - 0x200;
        temp_v0_7 = &((DisplayActor44 *)D_8011ECD0)[arg0];
        if (temp_v1_5 < 0x80) {
            *(s16 *)((unsigned char *)temp_v0_7 + (7114)) = (s16) ((temp_v1_5 >> 4) - 0x70);
            *(s16 *)((unsigned char *)temp_v0_7 + (7110)) = (s16) (-0x48 - (temp_v1_5 >> 2));
            *(s16 *)((unsigned char *)temp_v0_7 + (7114)) = (s16) ((*(s16 *)((unsigned char *)temp_v0_7 + (7114)) - arg0) + 3);
        } else {
            temp_t3 = (s32) (var_a1 - 0x280) >> 3;
            *(s16 *)((unsigned char *)temp_v0_7 + (7114)) = (s16) (temp_t3 - 0x68);
            *(s16 *)((unsigned char *)temp_v0_7 + (7110)) = (s16) (-0x68 - temp_t3);
            *(s16 *)((unsigned char *)temp_v0_7 + (7114)) = (s16) ((*(s16 *)((unsigned char *)temp_v0_7 + (7114)) - arg0) + 3);
        }
        temp_v1_6 = *(s16 *)((unsigned char *)temp_v0_7 + (7110));
        if (temp_v1_6 < -0x68) {
            var_at = temp_v1_6 < -0x68;
            if (*(u8 *)((unsigned char *)temp_v0_7 + (7075)) != 0) {
                *(u8 *)((unsigned char *)temp_v0_7 + (7075)) = 0U;
                goto block_42;
            }
        } else {
block_42:
            var_at = temp_v1_6 < -0x68;
        }
        if (var_at != 0) {
            *(s16 *)((unsigned char *)temp_v0_7 + (7382)) = temp_v1_6;
            *(s16 *)((unsigned char *)temp_v0_7 + (7386)) = (s16) (*(s16 *)((unsigned char *)temp_v0_7 + (7114)) + 8);
            return;
        }
        *(s16 *)((unsigned char *)temp_v0_7 + (7382)) = (s16) (temp_v1_6 - 0x10);
        *(s16 *)((unsigned char *)temp_v0_7 + (7386)) = (s16) *(s16 *)((unsigned char *)temp_v0_7 + (7114));
        return;
    case 5:
        temp_v1_7 = var_a1 - 0x300;
        temp_v0_8 = &((DisplayActor44 *)D_8011ECD0)[arg0];
        if (temp_v1_7 < 0x80) {
            *(s16 *)((unsigned char *)temp_v0_8 + (7114)) = (s16) ((temp_v1_7 >> 4) - 0x58);
            *(s16 *)((unsigned char *)temp_v0_8 + (7110)) = (s16) ((temp_v1_7 >> 3) - 0x78);
            *(s16 *)((unsigned char *)temp_v0_8 + (7114)) = (s16) ((*(s16 *)((unsigned char *)temp_v0_8 + (7114)) - arg0) + 3);
        } else {
            temp_v1_8 = var_a1 - 0x380;
            *(s16 *)((unsigned char *)temp_v0_8 + (7114)) = (s16) ((temp_v1_8 >> 4) - 0x50);
            *(s16 *)((unsigned char *)temp_v0_8 + (7110)) = (s16) ((temp_v1_8 >> 2) - 0x68);
            *(s16 *)((unsigned char *)temp_v0_8 + (7114)) = (s16) ((*(s16 *)((unsigned char *)temp_v0_8 + (7114)) - arg0) + 3);
        }
        temp_v1_9 = *(s16 *)((unsigned char *)temp_v0_8 + (7110));
        if ((temp_v1_9 >= -0x67) && (*(u8 *)((unsigned char *)temp_v0_8 + (7075)) == 0)) {
            *(u8 *)((unsigned char *)temp_v0_8 + (7075)) = 0x85U;
            if (temp_v0_8 == ((DisplayActor44 *)D_8011ECD0)) {
                *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7096)) = 0x8A;
            } else {
                *(s16 *)((unsigned char *)temp_v0_8 + (7096)) = 0x86;
            }
        }
        if (temp_v1_9 < -0x68) {
            *(s16 *)((unsigned char *)temp_v0_8 + (7382)) = temp_v1_9;
            *(s16 *)((unsigned char *)temp_v0_8 + (7386)) = (s16) (*(s16 *)((unsigned char *)temp_v0_8 + (7114)) + 8);
            return;
        }
        *(s16 *)((unsigned char *)temp_v0_8 + (7382)) = (s16) (temp_v1_9 + 0x10);
        *(s16 *)((unsigned char *)temp_v0_8 + (7386)) = (s16) *(s16 *)((unsigned char *)temp_v0_8 + (7114));
    default:
        return;
    }
}
