s32 func_80139F30(void) {
    Work800E7D10 *sp34;                             /* compiler-managed */
    DisplayActor44 *var_v0;
    DisplayActor44 *var_v0_3;
    NameRecordF00 *temp_a2;
    NameRecordF00 *var_v0_2;
    u8 *var_v1_5;
    Work800E7D10 *var_v1;
    Work800E7D10 *var_v1_3;
    Work800E7D10 *var_v1_4;
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
    u8 temp_a0;
    u8 temp_a0_10;
    u8 temp_a0_11;
    u8 temp_a0_12;
    u8 temp_a0_13;
    u8 temp_a0_2;
    u8 temp_a0_3;
    u8 temp_a0_4;
    u8 temp_a0_5;
    u8 temp_a0_6;
    u8 temp_a0_7;
    u8 temp_a0_8;
    u8 temp_a0_9;
    u8 temp_t1;
    u8 var_t0;

    if (D_800E8BEE != 0) {
        if (D_800E7D24 != 0) {
            func_80139740((s32) D_800E7D24, -4, 0x1E);
        }
        if (D_800E7D25 != 0) {
            func_80139740((s32) D_800E7D25, -4, 0x2E);
        }
        if (D_800E7D26 != 0) {
            func_80139740((s32) D_800E7D26, -4, 0x3E);
        }
        if (D_800E7D27 != 0) {
            func_80139740((s32) D_800E7D27, -4, 0x4E);
        }
        if (D_800E7D28 != 0) {
            func_80139740((s32) D_800E7D28, 0x3C, 0x1E);
        }
        if (D_800E7D29 != 0) {
            func_80139740((s32) D_800E7D29, 0x3C, 0x2E);
        }
        if (D_800E7D2A != 0) {
            func_80139740((s32) D_800E7D2A, 0x3C, 0x3E);
        }
        if (D_800E7D2B != 0) {
            func_80139740((s32) D_800E7D2B, 0x3C, 0x4E);
        }
        if (D_800E7D30 != 0) {
            func_80139740((s32) D_800E7D30, 0x88, 0xE);
        }
        if (D_800E7D2C != 0) {
            func_80139740((s32) D_800E7D2C, 0x88, 0x1E);
        }
        if (D_800E7D2D != 0) {
            func_80139740((s32) D_800E7D2D, 0x88, 0x2E);
        }
        if (D_800E7D2E != 0) {
            func_80139740((s32) D_800E7D2E, 0x88, 0x3E);
        }
        if (D_800E7D2F != 0) {
            func_80139740((s32) D_800E7D2F, 0x88, 0x4E);
        }
        var_v1 = &D_800E7D10;
        var_s0 = 0;
        do {
            if (var_v1->flags[0] != 0) {
                sp34 = var_v1;
                func_80139AD8(var_s0, -0x23, ((var_s0 % 4) * 0x10) + 0x1D);
            }
            var_s0 += 1;
            var_v1 += 1;
        } while (var_s0 < 4);
        var_v1_2 = D_800E7D14;
        var_s0_2 = 4;
        do {
            if (*(u8 *)((unsigned char *)var_v1_2 + (20)) != 0) {
                sp34 = var_v1_2;
                func_80139AD8(var_s0_2, 0x1E, ((var_s0_2 % 4) * 0x10) + 0x1D);
            }
            var_s0_2 += 1;
            var_v1_2 += 1;
        } while (var_s0_2 < 8);
        var_v1_3 = &D_800E7D18;
        var_s0_3 = 8;
        do {
            if (var_v1_3->flags[0] != 0) {
                sp34 = var_v1_3;
                func_80139AD8(var_s0_3, 0x69, ((var_s0_3 % 4) * 0x10) + 0x1D);
            }
            var_s0_3 += 1;
            var_v1_3 += 1;
        } while (var_s0_3 < 0xC);
        if (D_800E7D30 != 0) {
            func_80139AD8(0xC, 0x69, 0xD);
        }
        var_v1_4 = &D_800E7D10;
        var_v0 = ((DisplayActor44 *)D_8011ECD0);
        do {
            temp_t1 = var_v1_4->flags[0];
            var_v1_4 += 1;
            if (temp_t1 == 0) {
                var_v0[128].unk_01 = 4;
                var_v0[128].unk_03 = (u8) (var_v0[128].unk_03 | 0x80);
            } else {
                var_v0[128].unk_03 = (u8) (var_v0[128].unk_03 & 0x7F);
            }
            var_v0 += 1;
        } while ((u32) var_v1_4 < (u32) &D_800E7D18);
        if (D_800E7D2C == 0) {
            ((DisplayActor44 *)D_8011ECD0)[137].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[137].unk_03 | 0x80);
            ((DisplayActor44 *)D_8011ECD0)[137].unk_01 = 4;
        } else {
            ((DisplayActor44 *)D_8011ECD0)[137].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)->unk_00 & 0x7F);
        }
        if (D_800E7D2D == 0) {
            ((DisplayActor44 *)D_8011ECD0)[138].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[138].unk_03 | 0x80);
            ((DisplayActor44 *)D_8011ECD0)[138].unk_01 = 4;
        } else {
            ((DisplayActor44 *)D_8011ECD0)[138].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)->unk_00 & 0x7F);
        }
        if (D_800E7D2E == 0) {
            ((DisplayActor44 *)D_8011ECD0)[139].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[139].unk_03 | 0x80);
            ((DisplayActor44 *)D_8011ECD0)[139].unk_01 = 4;
        } else {
            ((DisplayActor44 *)D_8011ECD0)[139].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)->unk_00 & 0x7F);
        }
        if (D_800E7D2F == 0) {
            ((DisplayActor44 *)D_8011ECD0)[140].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[140].unk_03 | 0x80);
            ((DisplayActor44 *)D_8011ECD0)[140].unk_01 = 4;
        } else {
            ((DisplayActor44 *)D_8011ECD0)[140].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)->unk_00 & 0x7F);
        }
        if (D_800E7D30 == 0) {
            ((DisplayActor44 *)D_8011ECD0)[136].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[136].unk_03 | 0x80);
            ((DisplayActor44 *)D_8011ECD0)[136].unk_01 = 4;
            return;
        }
        ((DisplayActor44 *)D_8011ECD0)[136].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)->unk_00 & 0x7F);
        return;
    }
    var_v0_2 = &((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117];
    temp_a0 = var_v0_2->flags[0];
    if (temp_a0 != 0) {
        func_80139740((s32) temp_a0, -4, 0x1E);
        var_v0_2 = &((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117];
    }
    temp_a0_2 = var_v0_2->flags[1];
    if (temp_a0_2 != 0) {
        func_80139740((s32) temp_a0_2, -4, 0x2E);
        var_v0_2 = &((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117];
    }
    temp_a0_3 = var_v0_2->flags[2];
    if (temp_a0_3 != 0) {
        func_80139740((s32) temp_a0_3, -4, 0x3E);
        var_v0_2 = &((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117];
    }
    temp_a0_4 = var_v0_2->flags[3];
    if (temp_a0_4 != 0) {
        func_80139740((s32) temp_a0_4, -4, 0x4E);
        var_v0_2 = &((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117];
    }
    temp_a0_5 = var_v0_2->flags[4];
    if (temp_a0_5 != 0) {
        func_80139740((s32) temp_a0_5, 0x3C, 0x1E);
        var_v0_2 = &((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117];
    }
    temp_a0_6 = var_v0_2->flags[5];
    if (temp_a0_6 != 0) {
        func_80139740((s32) temp_a0_6, 0x3C, 0x2E);
        var_v0_2 = &((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117];
    }
    temp_a0_7 = var_v0_2->flags[6];
    if (temp_a0_7 != 0) {
        func_80139740((s32) temp_a0_7, 0x3C, 0x3E);
        var_v0_2 = &((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117];
    }
    temp_a0_8 = var_v0_2->flags[7];
    if (temp_a0_8 != 0) {
        func_80139740((s32) temp_a0_8, 0x3C, 0x4E);
        var_v0_2 = &((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117];
    }
    temp_a0_9 = var_v0_2->flags[0xC];
    if (temp_a0_9 != 0) {
        func_80139740((s32) temp_a0_9, 0x88, 0xE);
        var_v0_2 = &((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117];
    }
    temp_a0_10 = var_v0_2->flags[8];
    if (temp_a0_10 != 0) {
        func_80139740((s32) temp_a0_10, 0x88, 0x1E);
        var_v0_2 = &((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117];
    }
    temp_a0_11 = var_v0_2->flags[9];
    if (temp_a0_11 != 0) {
        func_80139740((s32) temp_a0_11, 0x88, 0x2E);
        var_v0_2 = &((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117];
    }
    temp_a0_12 = var_v0_2->flags[0xA];
    if (temp_a0_12 != 0) {
        func_80139740((s32) temp_a0_12, 0x88, 0x3E);
        var_v0_2 = &((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117];
    }
    temp_a0_13 = var_v0_2->flags[0xB];
    var_s0_4 = 0;
    if (temp_a0_13 != 0) {
        func_80139740((s32) temp_a0_13, 0x88, 0x4E);
        var_s0_4 = 0;
    }
    var_t8 = D_800E6280.unk_1117 * 0x10;
    do {
        if (((NameRecordF00 *)D_80123120)->flags[((var_t8 - D_800E6280.unk_1117) << 8) + var_s0_4] != 0) {
            func_80139AD8(var_s0_4, -0x23, ((var_s0_4 % 4) * 0x10) + 0x1D);
        }
        var_s0_4 += 1;
        var_t8 = D_800E6280.unk_1117 * 0x10;
    } while (var_s0_4 < 4);
    var_s0_5 = 4;
    var_t3 = D_800E6280.unk_1117 * 0x10;
    do {
        if (((NameRecordF00 *)D_80123120)->flags[((var_t3 - D_800E6280.unk_1117) << 8) + var_s0_5] != 0) {
            func_80139AD8(var_s0_5, 0x1E, ((var_s0_5 % 4) * 0x10) + 0x1D);
        }
        var_s0_5 += 1;
        var_t3 = D_800E6280.unk_1117 * 0x10;
    } while (var_s0_5 < 8);
    var_s0_6 = 8;
    var_t7 = D_800E6280.unk_1117 * 0x10;
    do {
        if (((NameRecordF00 *)D_80123120)->flags[((var_t7 - D_800E6280.unk_1117) << 8) + var_s0_6] != 0) {
            func_80139AD8(var_s0_6, 0x69, ((var_s0_6 % 4) * 0x10) + 0x1D);
        }
        var_s0_6 += 1;
        var_t7 = D_800E6280.unk_1117 * 0x10;
    } while (var_s0_6 < 0xC);
    var_t0 = *(&((NameRecordF00 *)D_80123120)->flags[0xC] + (D_800E6280.unk_1117 * 0xF00));
    if (var_t0 != 0) {
        func_80139AD8(0xC, 0x69, 0xD);
        var_t0 = *(&((NameRecordF00 *)D_80123120)->flags[0xC] + (D_800E6280.unk_1117 * 0xF00));
    }
    temp_a2 = &((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117];
    var_v0_3 = ((DisplayActor44 *)D_8011ECD0);
    var_v1_5 = temp_a2;
    do {
        if (*(u8 *)(var_v1_5 + 0x688) == 0) {
            var_v0_3[128].unk_01 = 4;
            var_v0_3[128].unk_03 = (u8) (var_v0_3[128].unk_03 | 0x80);
        } else {
            var_v0_3[128].unk_03 = (u8) (var_v0_3[128].unk_03 & 0x7F);
        }
        var_v1_5 += 1;
        var_v0_3 += 1;
    } while ((u32) var_v0_3 < (u32) (&((DisplayActor44 *)D_8011ECD0)[8]));
    temp_v0 = &temp_a2->unk_000[8];
    if (*(u8 *)((unsigned char *)temp_v0 + (1672)) == 0) {
        ((DisplayActor44 *)D_8011ECD0)[137].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[137].unk_03 | 0x80);
        ((DisplayActor44 *)D_8011ECD0)[137].unk_01 = 4;
    } else {
        ((DisplayActor44 *)D_8011ECD0)[137].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)->unk_00 & 0x7F);
    }
    if (*(u8 *)((unsigned char *)temp_v0 + (1673)) == 0) {
        ((DisplayActor44 *)D_8011ECD0)[138].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[138].unk_03 | 0x80);
        ((DisplayActor44 *)D_8011ECD0)[138].unk_01 = 4;
    } else {
        ((DisplayActor44 *)D_8011ECD0)[138].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)->unk_00 & 0x7F);
    }
    if (*(u8 *)((unsigned char *)temp_v0 + (1674)) == 0) {
        ((DisplayActor44 *)D_8011ECD0)[139].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[139].unk_03 | 0x80);
        ((DisplayActor44 *)D_8011ECD0)[139].unk_01 = 4;
    } else {
        ((DisplayActor44 *)D_8011ECD0)[139].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)->unk_00 & 0x7F);
    }
    if (*(u8 *)((unsigned char *)temp_v0 + (1675)) == 0) {
        ((DisplayActor44 *)D_8011ECD0)[140].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[140].unk_03 | 0x80);
        ((DisplayActor44 *)D_8011ECD0)[140].unk_01 = 4;
    } else {
        ((DisplayActor44 *)D_8011ECD0)[140].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)->unk_00 & 0x7F);
    }
    if (var_t0 == 0) {
        ((DisplayActor44 *)D_8011ECD0)[136].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[136].unk_03 | 0x80);
        ((DisplayActor44 *)D_8011ECD0)[136].unk_01 = 4;
        return;
    }
    ((DisplayActor44 *)D_8011ECD0)[136].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)->unk_00 & 0x7F);
}
