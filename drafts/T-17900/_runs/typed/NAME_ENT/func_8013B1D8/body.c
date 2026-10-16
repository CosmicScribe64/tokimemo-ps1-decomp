void func_8013B1D8(void) {
    s32 sp28;
    DisplayActor44 *temp_t6;
    DisplayActor44 *temp_t8;
    DisplayActor44 *var_v0_2;
    DisplayActor44 *var_v0_4;
    NameMetricWork *var_v0;
    NameRecordF00 *var_v0_3;
    s32 var_a3;
    s32 var_a3_2;
    s32 var_v1;
    s8 temp_v0;
    s8 temp_v0_2;
    u8 temp_t0;
    u8 temp_t7;

    if (D_800E8BEE != 0) {
        var_a3 = 0;
        var_v0 = ((NameMetricWork *)&D_800E7D10);
        do {
            temp_t7 = var_v0->flags[0];
            var_v0 += 1;
            if (temp_t7 != 0) {
                var_a3 += 1;
            }
        } while ((u32) var_v0 < (u32) &((NameMetricWork *)&D_800E7D10)->slot_status[9]);
        temp_t8 = &((DisplayActor44 *)D_8011ECD0)[96];
        var_v0_2 = temp_t8;
        do {
            if (var_v0_2 == &temp_t8[((NameMetricWork *)&D_800E7D10)->selected_slot - 1]) {
                *(s8 *)((unsigned char *)var_v0_2 + (339)) = 0x11;
            } else {
                *(s8 *)((unsigned char *)var_v0_2 + (339)) = 0;
            }
            var_v0_2 += 1;
        } while (var_v0_2 != (&((DisplayActor44 *)D_8011ECD0)[110]));
        temp_v0 = D_800E6280.unk_1093;
        if (temp_v0 != -1) {
            ((DisplayActor44 *)D_8011ECD0)[119].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[119].unk_03 | 0x80);
            ((DisplayActor44 *)D_8011ECD0)[119].unk_26 = (s16) (((temp_v0 / 7) * 0x1F) - 0x82);
            ((DisplayActor44 *)D_8011ECD0)[119].unk_2A = (s16) (((temp_v0 % 7) * 0x10) - 0xB);
        }
        sp28 = var_a3;
        func_80139740((s32) ((NameMetricWork *)&D_800E7D10)->year[((NameMetricWork *)&D_800E7D10)->selected_slot], 0x30, -0x60);
        func_80139740((s32) ((NameMetricWork *)&D_800E7D10)->month[((NameMetricWork *)&D_800E7D10)->selected_slot], 0x50, -0x60);
        func_80139740((s32) ((NameMetricWork *)&D_800E7D10)->day[((NameMetricWork *)&D_800E7D10)->selected_slot], 0x70, -0x60);
        func_80139740(sp28, 0x5A, -0x1D);
        return;
    }
    temp_t0 = D_800E6280.unk_1117;
    var_a3_2 = 0;
    var_v0_3 = &((NameRecordF00 *)D_80123120)[temp_t0];
    var_v1 = 0;
    do {
        var_v1 += 1;
        if (var_v0_3->flags[0] != 0) {
            var_a3_2 += 1;
        }
        var_v0_3 += 1;
    } while (var_v1 < 0xD);
    temp_t6 = &((DisplayActor44 *)D_8011ECD0)[96];
    var_v0_4 = temp_t6;
    do {
        if (var_v0_4 == &temp_t6[D_800E6280.unk_1118 - 1]) {
            *(s8 *)((unsigned char *)var_v0_4 + (339)) = 0x11;
        } else {
            *(s8 *)((unsigned char *)var_v0_4 + (339)) = 0;
        }
        var_v0_4 += 1;
    } while (var_v0_4 != (&((DisplayActor44 *)D_8011ECD0)[110]));
    temp_v0_2 = D_800E6280.unk_1093;
    if (temp_v0_2 != -1) {
        ((DisplayActor44 *)D_8011ECD0)[119].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[119].unk_03 | 0x80);
        ((DisplayActor44 *)D_8011ECD0)[119].unk_26 = (s16) (((temp_v0_2 / 7) * 0x1F) - 0x82);
        ((DisplayActor44 *)D_8011ECD0)[119].unk_2A = (s16) (((temp_v0_2 % 7) * 0x10) - 0xB);
    }
    sp28 = var_a3_2;
    func_80139740((s32) *(&((NameRecordF00 *)D_80123120)->year + (temp_t0 * 0xF00)), 0x30, -0x60);
    func_80139740((s32) *(&((NameRecordF00 *)D_80123120)->month + (D_800E6280.unk_1117 * 0xF00)), 0x50, -0x60);
    func_80139740((s32) *(&((NameRecordF00 *)D_80123120)->day + (D_800E6280.unk_1117 * 0xF00)), 0x70, -0x60);
    func_80139740(sp28, 0x5A, -0x1D);
}
