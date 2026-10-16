void func_8013A9C8(void) {
    DisplayActor44 *var_v0;
    DisplayActor44 *var_v0_2;
    DisplayActor44 *var_v0_3;
    DisplayActor44 *var_v0_4;
    DisplayActor44 *var_v0_5;
    NameRecordF00 *temp_t0;
    NameRecordF00 *var_a2;
    s32 temp_t2;
    s32 temp_t8;
    s32 var_a1;
    s32 var_a1_2;
    u8 *var_a2_2;
    u8 temp_t6;
    u8 temp_v1;
    u8 temp_v1_2;
    u8 temp_v1_3;
    u8 temp_v1_4;
    u8 *temp_a2;
    u8 *var_a0;
    u8 *var_a0_2;

    k_sub_reset_point_set();
    k_sub_disp_start(0);
    var_v0 = ((DisplayActor44 *)D_8011ECD0);
    do {
        temp_t6 = *(u8 *)((unsigned char *)var_v0 + (8707));
        var_v0 += 1;
        *(s8 *)((unsigned char *)var_v0 + (8639)) = (s8) (temp_t6 | 0x80);
    } while ((u32) var_v0 < (u32) (&((DisplayActor44 *)D_8011ECD0)[13]));
    var_a1 = 0;
    if (D_800E8BEE != 0) {
        temp_a2 = (((NameMetricWork *)&D_800E7D10)->selected_slot * 0x10) + &D_800E7D10;
        var_v0_2 = ((DisplayActor44 *)D_8011ECD0);
        var_a0 = temp_a2;
        do {
            temp_v1 = *(u8 *)((unsigned char *)var_a0 + (3272));
            if (temp_v1 == 0) {
                *(u8 *)((unsigned char *)var_v0_2 + (8707)) = (u8) (*(u8 *)((unsigned char *)var_v0_2 + (8707)) | 0x80);
                *(s8 *)((unsigned char *)var_v0_2 + (8705)) = 4;
                *(u8 *)((unsigned char *)var_v0_2 + (9591)) = (u8) (*(u8 *)((unsigned char *)var_v0_2 + (9591)) & 0x7F);
            } else {
                *(u8 *)((unsigned char *)var_v0_2 + (8707)) = (u8) (*(u8 *)((unsigned char *)var_v0_2 + (8707)) & 0x7F);
                *(u8 *)((unsigned char *)var_v0_2 + (9591)) = (u8) (*(u8 *)((unsigned char *)var_v0_2 + (9591)) | 0x80);
                *(s16 *)((unsigned char *)var_v0_2 + (9612)) = (s16) ((temp_v1 & 7) + 0x58);
            }
            var_v0_2 += 1;
            var_a0 += 1;
        } while ((u32) var_v0_2 < (u32) (&((DisplayActor44 *)D_8011ECD0)[8]));
        var_v0_3 = &((DisplayActor44 *)D_8011ECD0)[8];
        var_a0_2 = temp_a2 + 8;
        do {
            temp_v1_2 = *(u8 *)((unsigned char *)var_a0_2 + (3272));
            if (temp_v1_2 == 0) {
                var_v0_3[129].unk_03 = (u8) (var_v0_3[129].unk_03 | 0x80);
                var_v0_3[129].unk_01 = 4;
                var_v0_3[141].unk_03 = (u8) (var_v0_3[141].unk_03 & 0x7F);
            } else {
                var_v0_3[129].unk_03 = (u8) (var_v0_3[129].unk_03 & 0x7F);
                var_v0_3[141].unk_03 = (u8) (var_v0_3[141].unk_03 | 0x80);
                var_v0_3[141].unk_18 = (s16) ((temp_v1_2 & 7) + 0x58);
            }
            var_v0_3 += 1;
            var_a0_2 += 1;
        } while (var_v0_3 != (&((DisplayActor44 *)D_8011ECD0)[11]));
    } else {
        temp_t0 = &((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117];
        var_v0_4 = ((DisplayActor44 *)D_8011ECD0);
        var_a2 = temp_t0;
        do {
            temp_t2 = *(s32 *)((unsigned char *)var_a2 + (788));
            var_a2 += 0x38;
            if (!(temp_t2 & 2) || (temp_v1_3 = *(u8 *)((unsigned char *)(&temp_t0->unk_000[var_a1]) + (1636)), (temp_v1_3 == 0))) {
                *(u8 *)((unsigned char *)var_v0_4 + (8707)) = (u8) (*(u8 *)((unsigned char *)var_v0_4 + (8707)) | 0x80);
                *(s8 *)((unsigned char *)var_v0_4 + (8705)) = 4;
                *(u8 *)((unsigned char *)var_v0_4 + (9591)) = (u8) (*(u8 *)((unsigned char *)var_v0_4 + (9591)) & 0x7F);
            } else {
                *(u8 *)((unsigned char *)var_v0_4 + (8707)) = (u8) (*(u8 *)((unsigned char *)var_v0_4 + (8707)) & 0x7F);
                *(u8 *)((unsigned char *)var_v0_4 + (9591)) = (u8) (*(u8 *)((unsigned char *)var_v0_4 + (9591)) | 0x80);
                if (temp_v1_3 != 0) {
                    *(s16 *)((unsigned char *)var_v0_4 + (9612)) = (s16) ((temp_v1_3 & 7) + 0x58);
                }
            }
            var_a1 += 1;
            var_v0_4 += 1;
        } while (var_a1 < 8);
        var_v0_5 = &((DisplayActor44 *)D_8011ECD0)[8];
        var_a1_2 = 8;
        var_a2_2 = &temp_t0->unk_000[0x1C0];
        do {
            temp_t8 = *(s32 *)((unsigned char *)var_a2_2 + (788));
            var_a2_2 += 0x38;
            if (!(temp_t8 & 2) || (temp_v1_4 = *(u8 *)((unsigned char *)(&temp_t0->unk_000[var_a1_2]) + (1636)), (temp_v1_4 == 0))) {
                var_v0_5[129].unk_03 = (u8) (var_v0_5[129].unk_03 | 0x80);
                var_v0_5[129].unk_01 = 4;
                var_v0_5[141].unk_03 = (u8) (var_v0_5[141].unk_03 & 0x7F);
            } else {
                var_v0_5[129].unk_03 = (u8) (var_v0_5[129].unk_03 & 0x7F);
                var_v0_5[141].unk_03 = (u8) (var_v0_5[141].unk_03 | 0x80);
                if (temp_v1_4 != 0) {
                    var_v0_5[141].unk_18 = (s16) ((temp_v1_4 & 7) + 0x58);
                }
            }
            var_a1_2 += 1;
            var_v0_5 += 1;
        } while (var_a1_2 != 0xB);
    }
    ((DisplayActor44 *)D_8011ECD0)[140].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[140].unk_03 | 0x80);
    ((DisplayActor44 *)D_8011ECD0)[140].unk_01 = 4;
    ((DisplayActor44 *)D_8011ECD0)[136].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[136].unk_03 | 0x80);
    ((DisplayActor44 *)D_8011ECD0)[136].unk_01 = 4;
}
