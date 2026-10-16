s32 func_80139F30(void) {
    NameMetricWork *sp34;                           /* compiler-managed */
    DisplayActor44 *var_v0;
    DisplayActor44 *var_v0_3;
    NameMetricWork *var_v1;
    NameMetricWork *var_v1_4;
    NameRecordF00 *temp_a2;
    NameRecordF00 *var_v0_2;
    NameRecordF00 *var_v1_5;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s0_3;
    s32 var_s0_4;
    s32 var_s0_5;
    s32 var_s0_6;
    s32 var_t3;
    s32 var_t7;
    s32 var_t8;
    u8 *temp_v0;
    u8 *var_v1_2;
    u8 *var_v1_3;
    u8 temp_a0;
    u8 temp_a0_10;
    u8 temp_a0_11;
    u8 temp_a0_12;
    u8 temp_a0_13;
    u8 temp_a0_14;
    u8 temp_a0_15;
    u8 temp_a0_16;
    u8 temp_a0_17;
    u8 temp_a0_18;
    u8 temp_a0_19;
    u8 temp_a0_20;
    u8 temp_a0_21;
    u8 temp_a0_2;
    u8 temp_a0_3;
    u8 temp_a0_4;
    u8 temp_a0_5;
    u8 temp_a0_6;
    u8 temp_a0_7;
    u8 temp_a0_8;
    u8 temp_a0_9;
    u8 temp_t1;
    u8 temp_t1_2;
    u8 temp_t2;
    u8 temp_t7;
    u8 temp_t8;
    u8 temp_t9;
    u8 var_a3;
    u8 var_t0;

    if (D_800E8BEE != 0) {
        temp_a0 = ((NameMetricWork *)&D_800E7D10)->flags[0];
        if (temp_a0 != 0) {
            func_80139740((s32) temp_a0, -4, 0x1E);
        }
        temp_a0_2 = ((NameMetricWork *)&D_800E7D10)->flags[1];
        if (temp_a0_2 != 0) {
            func_80139740((s32) temp_a0_2, -4, 0x2E);
        }
        temp_a0_3 = ((NameMetricWork *)&D_800E7D10)->flags[2];
        if (temp_a0_3 != 0) {
            func_80139740((s32) temp_a0_3, -4, 0x3E);
        }
        temp_a0_4 = ((NameMetricWork *)&D_800E7D10)->flags[3];
        if (temp_a0_4 != 0) {
            func_80139740((s32) temp_a0_4, -4, 0x4E);
        }
        temp_a0_5 = ((NameMetricWork *)&D_800E7D10)->flags[4];
        if (temp_a0_5 != 0) {
            func_80139740((s32) temp_a0_5, 0x3C, 0x1E);
        }
        temp_a0_6 = ((NameMetricWork *)&D_800E7D10)->flags[5];
        if (temp_a0_6 != 0) {
            func_80139740((s32) temp_a0_6, 0x3C, 0x2E);
        }
        temp_a0_7 = ((NameMetricWork *)&D_800E7D10)->flags[6];
        if (temp_a0_7 != 0) {
            func_80139740((s32) temp_a0_7, 0x3C, 0x3E);
        }
        temp_a0_8 = ((NameMetricWork *)&D_800E7D10)->flags[7];
        if (temp_a0_8 != 0) {
            func_80139740((s32) temp_a0_8, 0x3C, 0x4E);
        }
        temp_t7 = ((NameMetricWork *)&D_800E7D10)->flags[0xC];
        if (temp_t7 != 0) {
            func_80139740((s32) temp_t7, 0x88, 0xE);
        }
        temp_t8 = ((NameMetricWork *)&D_800E7D10)->flags[8];
        if (temp_t8 != 0) {
            func_80139740((s32) temp_t8, 0x88, 0x1E);
        }
        temp_t9 = ((NameMetricWork *)&D_800E7D10)->flags[9];
        if (temp_t9 != 0) {
            func_80139740((s32) temp_t9, 0x88, 0x2E);
        }
        temp_t1 = ((NameMetricWork *)&D_800E7D10)->flags[0xA];
        if (temp_t1 != 0) {
            func_80139740((s32) temp_t1, 0x88, 0x3E);
        }
        temp_t2 = ((NameMetricWork *)&D_800E7D10)->flags[0xB];
        if (temp_t2 != 0) {
            func_80139740((s32) temp_t2, 0x88, 0x4E);
        }
        var_v1 = ((NameMetricWork *)&D_800E7D10);
        var_s0 = 0;
        do {
            if (var_v1->flags[0] != 0) {
                sp34 = var_v1;
                func_80139AD8(var_s0, -0x23, ((var_s0 % 4) * 0x10) + 0x1D);
            }
            var_s0 += 1;
            var_v1 += 1;
        } while (var_s0 < 4);
        var_v1_2 = ((NameMetricWork *)&D_800E7D10)->slot_status;
        var_s0_2 = 4;
        do {
            if (*(u8 *)((unsigned char *)var_v1_2 + (20)) != 0) {
                sp34 = var_v1_2;
                func_80139AD8(var_s0_2, 0x1E, ((var_s0_2 % 4) * 0x10) + 0x1D);
            }
            var_s0_2 += 1;
            var_v1_2 += 1;
        } while (var_s0_2 < 8);
        var_v1_3 = &((NameMetricWork *)&D_800E7D10)->slot_status[4];
        var_s0_3 = 8;
        do {
            if (*(u8 *)((unsigned char *)var_v1_3 + (20)) != 0) {
                sp34 = var_v1_3;
                func_80139AD8(var_s0_3, 0x69, ((var_s0_3 % 4) * 0x10) + 0x1D);
            }
            var_s0_3 += 1;
            var_v1_3 += 1;
        } while (var_s0_3 < 0xC);
        if (((NameMetricWork *)&D_800E7D10)->flags[0xC] != 0) {
            func_80139AD8(0xC, 0x69, 0xD);
        }
        var_v1_4 = ((NameMetricWork *)&D_800E7D10);
        var_v0 = ((DisplayActor44 *)D_8011ECD0);
        do {
            temp_t1_2 = var_v1_4->flags[0];
            var_v1_4 += 1;
            if (temp_t1_2 == 0) {
                *(s8 *)((unsigned char *)var_v0 + (8705)) = 4;
                *(u8 *)((unsigned char *)var_v0 + (8707)) = (u8) (*(u8 *)((unsigned char *)var_v0 + (8707)) | 0x80);
            } else {
                *(u8 *)((unsigned char *)var_v0 + (8707)) = (u8) (*(u8 *)((unsigned char *)var_v0 + (8707)) & 0x7F);
            }
            var_v0 += 1;
        } while ((u32) var_v1_4 < (u32) &((NameMetricWork *)&D_800E7D10)->slot_status[4]);
        if (((NameMetricWork *)&D_800E7D10)->flags[8] == 0) {
            ((DisplayActor44 *)D_8011ECD0)[137].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[137].unk_03 | 0x80);
            ((DisplayActor44 *)D_8011ECD0)[137].unk_01 = 4;
        } else {
            ((DisplayActor44 *)D_8011ECD0)[137].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[137].unk_03 & 0x7F);
        }
        if (((NameMetricWork *)&D_800E7D10)->flags[9] == 0) {
            ((DisplayActor44 *)D_8011ECD0)[138].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[138].unk_03 | 0x80);
            ((DisplayActor44 *)D_8011ECD0)[138].unk_01 = 4;
        } else {
            ((DisplayActor44 *)D_8011ECD0)[138].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[138].unk_03 & 0x7F);
        }
        if (((NameMetricWork *)&D_800E7D10)->flags[0xA] == 0) {
            ((DisplayActor44 *)D_8011ECD0)[139].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[139].unk_03 | 0x80);
            ((DisplayActor44 *)D_8011ECD0)[139].unk_01 = 4;
        } else {
            ((DisplayActor44 *)D_8011ECD0)[139].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[139].unk_03 & 0x7F);
        }
        if (((NameMetricWork *)&D_800E7D10)->flags[0xB] == 0) {
            ((DisplayActor44 *)D_8011ECD0)[140].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[140].unk_03 | 0x80);
            ((DisplayActor44 *)D_8011ECD0)[140].unk_01 = 4;
        } else {
            ((DisplayActor44 *)D_8011ECD0)[140].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[140].unk_03 & 0x7F);
        }
        if (((NameMetricWork *)&D_800E7D10)->flags[0xC] == 0) {
            ((DisplayActor44 *)D_8011ECD0)[136].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[136].unk_03 | 0x80);
            ((DisplayActor44 *)D_8011ECD0)[136].unk_01 = 4;
            return;
        }
        ((DisplayActor44 *)D_8011ECD0)[136].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[136].unk_03 & 0x7F);
        return;
    }
    var_a3 = D_800E6280.unk_1117;
    var_v0_2 = &((NameRecordF00 *)D_80123120)[var_a3];
    temp_a0_9 = var_v0_2->flags[0];
    if (temp_a0_9 != 0) {
        func_80139740((s32) temp_a0_9, -4, 0x1E);
        var_a3 = D_800E6280.unk_1117;
        var_v0_2 = &((NameRecordF00 *)D_80123120)[var_a3];
    }
    temp_a0_10 = var_v0_2->flags[1];
    if (temp_a0_10 != 0) {
        func_80139740((s32) temp_a0_10, -4, 0x2E);
        var_a3 = D_800E6280.unk_1117;
        var_v0_2 = &((NameRecordF00 *)D_80123120)[var_a3];
    }
    temp_a0_11 = var_v0_2->flags[2];
    if (temp_a0_11 != 0) {
        func_80139740((s32) temp_a0_11, -4, 0x3E);
        var_a3 = D_800E6280.unk_1117;
        var_v0_2 = &((NameRecordF00 *)D_80123120)[var_a3];
    }
    temp_a0_12 = var_v0_2->flags[3];
    if (temp_a0_12 != 0) {
        func_80139740((s32) temp_a0_12, -4, 0x4E);
        var_a3 = D_800E6280.unk_1117;
        var_v0_2 = &((NameRecordF00 *)D_80123120)[var_a3];
    }
    temp_a0_13 = var_v0_2->flags[4];
    if (temp_a0_13 != 0) {
        func_80139740((s32) temp_a0_13, 0x3C, 0x1E);
        var_a3 = D_800E6280.unk_1117;
        var_v0_2 = &((NameRecordF00 *)D_80123120)[var_a3];
    }
    temp_a0_14 = var_v0_2->flags[5];
    if (temp_a0_14 != 0) {
        func_80139740((s32) temp_a0_14, 0x3C, 0x2E);
        var_a3 = D_800E6280.unk_1117;
        var_v0_2 = &((NameRecordF00 *)D_80123120)[var_a3];
    }
    temp_a0_15 = var_v0_2->flags[6];
    if (temp_a0_15 != 0) {
        func_80139740((s32) temp_a0_15, 0x3C, 0x3E);
        var_a3 = D_800E6280.unk_1117;
        var_v0_2 = &((NameRecordF00 *)D_80123120)[var_a3];
    }
    temp_a0_16 = var_v0_2->flags[7];
    if (temp_a0_16 != 0) {
        func_80139740((s32) temp_a0_16, 0x3C, 0x4E);
        var_a3 = D_800E6280.unk_1117;
        var_v0_2 = &((NameRecordF00 *)D_80123120)[var_a3];
    }
    temp_a0_17 = var_v0_2->flags[0xC];
    if (temp_a0_17 != 0) {
        func_80139740((s32) temp_a0_17, 0x88, 0xE);
        var_a3 = D_800E6280.unk_1117;
        var_v0_2 = &((NameRecordF00 *)D_80123120)[var_a3];
    }
    temp_a0_18 = var_v0_2->flags[8];
    if (temp_a0_18 != 0) {
        func_80139740((s32) temp_a0_18, 0x88, 0x1E);
        var_a3 = D_800E6280.unk_1117;
        var_v0_2 = &((NameRecordF00 *)D_80123120)[var_a3];
    }
    temp_a0_19 = var_v0_2->flags[9];
    if (temp_a0_19 != 0) {
        func_80139740((s32) temp_a0_19, 0x88, 0x2E);
        var_a3 = D_800E6280.unk_1117;
        var_v0_2 = &((NameRecordF00 *)D_80123120)[var_a3];
    }
    temp_a0_20 = var_v0_2->flags[0xA];
    if (temp_a0_20 != 0) {
        func_80139740((s32) temp_a0_20, 0x88, 0x3E);
        var_a3 = D_800E6280.unk_1117;
        var_v0_2 = &((NameRecordF00 *)D_80123120)[var_a3];
    }
    temp_a0_21 = var_v0_2->flags[0xB];
    var_s0_4 = 0;
    if (temp_a0_21 != 0) {
        func_80139740((s32) temp_a0_21, 0x88, 0x4E);
        var_a3 = D_800E6280.unk_1117;
        var_s0_4 = 0;
    }
    var_t8 = var_a3 * 0x10;
    do {
        if (((NameRecordF00 *)D_80123120)->flags[((var_t8 - var_a3) << 8) + var_s0_4] != 0) {
            func_80139AD8(var_s0_4, -0x23, ((var_s0_4 % 4) * 0x10) + 0x1D);
            var_a3 = D_800E6280.unk_1117;
        }
        var_s0_4 += 1;
        var_t8 = var_a3 * 0x10;
    } while (var_s0_4 < 4);
    var_s0_5 = 4;
    var_t3 = var_a3 * 0x10;
    do {
        if (((NameRecordF00 *)D_80123120)->flags[((var_t3 - var_a3) << 8) + var_s0_5] != 0) {
            func_80139AD8(var_s0_5, 0x1E, ((var_s0_5 % 4) * 0x10) + 0x1D);
            var_a3 = D_800E6280.unk_1117;
        }
        var_s0_5 += 1;
        var_t3 = var_a3 * 0x10;
    } while (var_s0_5 < 8);
    var_s0_6 = 8;
    var_t7 = var_a3 * 0x10;
    do {
        if (((NameRecordF00 *)D_80123120)->flags[((var_t7 - var_a3) << 8) + var_s0_6] != 0) {
            func_80139AD8(var_s0_6, 0x69, ((var_s0_6 % 4) * 0x10) + 0x1D);
            var_a3 = D_800E6280.unk_1117;
        }
        var_s0_6 += 1;
        var_t7 = var_a3 * 0x10;
    } while (var_s0_6 < 0xC);
    var_t0 = *(&((NameRecordF00 *)D_80123120)->flags[0xC] + (var_a3 * 0xF00));
    if (var_t0 != 0) {
        func_80139AD8(0xC, 0x69, 0xD);
        var_a3 = D_800E6280.unk_1117;
        var_t0 = *(&((NameRecordF00 *)D_80123120)->flags[0xC] + (var_a3 * 0xF00));
    }
    temp_a2 = &((NameRecordF00 *)D_80123120)[var_a3];
    var_v0_3 = ((DisplayActor44 *)D_8011ECD0);
    var_v1_5 = temp_a2;
    do {
        if (var_v1_5->flags[0] == 0) {
            *(s8 *)((unsigned char *)var_v0_3 + (8705)) = 4;
            *(u8 *)((unsigned char *)var_v0_3 + (8707)) = (u8) (*(u8 *)((unsigned char *)var_v0_3 + (8707)) | 0x80);
        } else {
            *(u8 *)((unsigned char *)var_v0_3 + (8707)) = (u8) (*(u8 *)((unsigned char *)var_v0_3 + (8707)) & 0x7F);
        }
        var_v0_3 += 1;
        var_v1_5 += 1;
    } while ((u32) var_v0_3 < (u32) (&((DisplayActor44 *)D_8011ECD0)[8]));
    temp_v0 = &temp_a2->unk_000[8];
    if (*(u8 *)((unsigned char *)temp_v0 + (1672)) == 0) {
        ((DisplayActor44 *)D_8011ECD0)[137].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[137].unk_03 | 0x80);
        ((DisplayActor44 *)D_8011ECD0)[137].unk_01 = 4;
    } else {
        ((DisplayActor44 *)D_8011ECD0)[137].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[137].unk_03 & 0x7F);
    }
    if (*(u8 *)((unsigned char *)temp_v0 + (1673)) == 0) {
        ((DisplayActor44 *)D_8011ECD0)[138].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[138].unk_03 | 0x80);
        ((DisplayActor44 *)D_8011ECD0)[138].unk_01 = 4;
    } else {
        ((DisplayActor44 *)D_8011ECD0)[138].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[138].unk_03 & 0x7F);
    }
    if (*(u8 *)((unsigned char *)temp_v0 + (1674)) == 0) {
        ((DisplayActor44 *)D_8011ECD0)[139].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[139].unk_03 | 0x80);
        ((DisplayActor44 *)D_8011ECD0)[139].unk_01 = 4;
    } else {
        ((DisplayActor44 *)D_8011ECD0)[139].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[139].unk_03 & 0x7F);
    }
    if (*(u8 *)((unsigned char *)temp_v0 + (1675)) == 0) {
        ((DisplayActor44 *)D_8011ECD0)[140].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[140].unk_03 | 0x80);
        ((DisplayActor44 *)D_8011ECD0)[140].unk_01 = 4;
    } else {
        ((DisplayActor44 *)D_8011ECD0)[140].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[140].unk_03 & 0x7F);
    }
    if (var_t0 == 0) {
        ((DisplayActor44 *)D_8011ECD0)[136].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[136].unk_03 | 0x80);
        ((DisplayActor44 *)D_8011ECD0)[136].unk_01 = 4;
        return;
    }
    ((DisplayActor44 *)D_8011ECD0)[136].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[136].unk_03 & 0x7F);
}
