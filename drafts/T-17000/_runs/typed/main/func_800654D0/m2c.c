extern s16 D_80125C48;
extern s16 D_80125C4A;

void func_800654D0(s32 arg0) {
    s32 spE0;
    DisplayActor44 *var_v0_3;
    DisplayActor44 *var_v0_4;
    DisplayActor44 *var_v0_5;
    DisplayActor44 *var_v0_6;
    s16 *var_a0;
    s16 *var_a0_2;
    s16 *var_a1;
    s16 *var_a1_2;
    s16 *var_a2;
    s16 *var_a2_2;
    s16 *var_v0;
    s16 *var_v0_2;
    s16 temp_t4;
    s16 temp_t4_2;
    s16 temp_t5;
    s16 temp_t5_2;
    s16 temp_t5_3;
    s16 temp_t6;
    s16 temp_t6_2;
    s16 temp_t6_3;
    s16 temp_t7;
    s16 temp_t7_2;
    s16 temp_t8;
    s16 temp_t8_2;
    s16 temp_t8_3;
    s16 temp_t9;
    s16 temp_t9_2;
    s16 temp_t9_3;
    s32 temp_a3;
    s32 temp_a3_2;
    s32 temp_a3_3;
    s32 var_s0;
    s32 var_t0;
    s32 var_t0_2;
    void *var_v1;
    void *var_v1_2;
    void *var_v1_3;
    void *var_v1_4;
    void *var_v1_5;
    void *var_v1_6;

    var_s0 = arg0;
    if (arg0 != 0xF) {
        var_s0 = 0xE;
        D_800E6280.unk_F6D = 0xE;
    }
    var_t0 = 0;
    if (var_s0 > 0) {
        temp_a3 = var_s0 & 3;
        if (temp_a3 != 0) {
            var_v0 = &(&sp60[0])[0];
            var_a2 = &(&sp80[0])[0];
            var_a1 = &(&spA0[0])[0];
            var_v1 = (0 * 4) + D_80125C10;
            var_a0 = &(&spC0[0])[0];
            do {
                var_t0 += 1;
                *var_a1 = *(s16 *)((unsigned char *)var_v1 + (2)) - 0xA;
                *var_a0 = var_v1->unk0 - 0xA;
                *var_a2 = 0x14;
                *var_v0 = 0x10;
                var_a0 += 2;
                var_v1 += 4;
                var_a1 += 2;
                var_a2 += 2;
                var_v0 += 2;
            } while (temp_a3 != var_t0);
            if (var_t0 != var_s0) {
                goto block_7;
            }
        } else {
block_7:
            var_v1_2 = (var_t0 * 4) + D_80125C10;
            var_a0_2 = &(&spC0[0])[var_t0];
            var_v0_2 = &(&sp60[0])[var_t0];
            var_a2_2 = &(&sp80[0])[var_t0];
            var_a1_2 = &(&spA0[0])[var_t0];
            do {
                var_a0_2->unk0 = var_v1_2->unk0 - 0xA;
                var_a1_2->unk0 = *(s16 *)((unsigned char *)var_v1_2 + (2)) - 0xA;
                *(s16 *)((unsigned char *)var_a0_2 + (2)) = (s16) (*(s16 *)((unsigned char *)var_v1_2 + (4)) - 0xA);
                *(s16 *)((unsigned char *)var_a1_2 + (2)) = (s16) (*(s16 *)((unsigned char *)var_v1_2 + (6)) - 0xA);
                *(s16 *)((unsigned char *)var_a0_2 + (4)) = (s16) (*(s16 *)((unsigned char *)var_v1_2 + (8)) - 0xA);
                var_v0_2 += 8;
                *(s16 *)((unsigned char *)var_a1_2 + (6)) = (s16) (*(s16 *)((unsigned char *)var_v1_2 + (14)) - 0xA);
                *(s16 *)((unsigned char *)var_a0_2 + (6)) = (s16) (*(s16 *)((unsigned char *)var_v1_2 + (12)) - 0xA);
                *(s16 *)((unsigned char *)var_a1_2 + (4)) = (s16) (*(s16 *)((unsigned char *)var_v1_2 + (10)) - 0xA);
                *(s16 *)((unsigned char *)var_a2_2 + (2)) = 0x14;
                *(s16 *)((unsigned char *)var_v0_2 + (-6)) = 0x10;
                *(s16 *)((unsigned char *)var_a2_2 + (4)) = 0x14;
                *(s16 *)((unsigned char *)var_v0_2 + (-4)) = 0x10;
                *(s16 *)((unsigned char *)var_a2_2 + (6)) = 0x14;
                *(s16 *)((unsigned char *)var_v0_2 + (-2)) = 0x10;
                var_a0_2 += 8;
                var_v1_2 += 0x10;
                var_a1_2 += 8;
                var_a2_2 += 8;
                *(s16 *)((unsigned char *)var_a2_2 + (-8)) = 0x14;
                *(s16 *)((unsigned char *)var_v0_2 + (-8)) = 0x10;
            } while (var_v0_2 != &(&sp60[0])[var_s0]);
        }
    }
    spE0 = 0;
    menu_set(0, var_s0 & 0xFF, (s32) &spC0[0], (s32) &spA0[0], (s32) &sp80[0], (s32) &sp60[0]);
    var_t0_2 = spE0;
    if (var_s0 > 0) {
        temp_a3_2 = var_s0 & 3;
        if (temp_a3_2 != 0) {
            var_v1_3 = (var_t0_2 * 4) + D_80125C10;
            var_v0_3 = &D_8011ECD0[var_t0_2];
            do {
                temp_t5 = var_v1_3->unk0;
                temp_t6 = *(s16 *)((unsigned char *)var_v1_3 + (2));
                *(s16 *)((unsigned char *)var_v0_3 + (5462)) = (s16) var_t0_2;
                var_t0_2 += 1;
                var_v1_3 += 4;
                var_v0_3 += 0x44;
                *(s16 *)((unsigned char *)var_v0_3 + (5410)) = temp_t5;
                *(s16 *)((unsigned char *)var_v0_3 + (5414)) = temp_t6;
            } while (temp_a3_2 != var_t0_2);
            if (var_t0_2 != var_s0) {
                goto block_15;
            }
        } else {
block_15:
            var_v1_4 = (var_t0_2 * 4) + D_80125C10;
            var_v0_4 = &D_8011ECD0[var_t0_2];
            do {
                *(s16 *)((unsigned char *)var_v0_4 + (5530)) = (s16) (var_t0_2 + 1);
                *(s16 *)((unsigned char *)var_v0_4 + (5546)) = (s16) *(s16 *)((unsigned char *)var_v1_4 + (4));
                *(s16 *)((unsigned char *)var_v0_4 + (5550)) = (s16) *(s16 *)((unsigned char *)var_v1_4 + (6));
                *(s16 *)((unsigned char *)var_v0_4 + (5478)) = (s16) var_v1_4->unk0;
                *(s16 *)((unsigned char *)var_v0_4 + (5482)) = (s16) *(s16 *)((unsigned char *)var_v1_4 + (2));
                temp_t6_2 = *(s16 *)((unsigned char *)var_v1_4 + (10));
                temp_t5_2 = *(s16 *)((unsigned char *)var_v1_4 + (8));
                temp_t9 = *(s16 *)((unsigned char *)var_v1_4 + (14));
                temp_t8 = *(s16 *)((unsigned char *)var_v1_4 + (12));
                temp_t7 = var_t0_2 + 3;
                temp_t4 = var_t0_2 + 2;
                *(s16 *)((unsigned char *)var_v0_4 + (5462)) = (s16) var_t0_2;
                var_t0_2 += 4;
                *(s16 *)((unsigned char *)var_v0_4 + (5598)) = temp_t4;
                *(s16 *)((unsigned char *)var_v0_4 + (5666)) = temp_t7;
                var_v1_4 += 0x10;
                var_v0_4 += 0x110;
                *(s16 *)((unsigned char *)var_v0_4 + (5346)) = temp_t6_2;
                *(s16 *)((unsigned char *)var_v0_4 + (5342)) = temp_t5_2;
                *(s16 *)((unsigned char *)var_v0_4 + (5414)) = temp_t9;
                *(s16 *)((unsigned char *)var_v0_4 + (5410)) = temp_t8;
            } while (var_t0_2 != var_s0);
        }
        var_t0_2 = 0;
    }
    temp_a3_3 = var_s0 & 3;
    if (var_s0 > 0) {
        if (temp_a3_3 != 0) {
            var_v1_5 = (var_t0_2 * 4) + D_80125C10;
            var_v0_5 = &D_8011ECD0[var_t0_2];
            do {
                temp_t8_2 = var_v1_5->unk0;
                temp_t9_2 = *(s16 *)((unsigned char *)var_v1_5 + (2));
                var_t0_2 += 1;
                *(s16 *)((unsigned char *)var_v0_5 + (4376)) = 2;
                var_v1_5 += 4;
                var_v0_5 += 0x44;
                *(s16 *)((unsigned char *)var_v0_5 + (4322)) = temp_t8_2;
                *(s16 *)((unsigned char *)var_v0_5 + (4326)) = temp_t9_2;
            } while (temp_a3_3 != var_t0_2);
            if (var_t0_2 != var_s0) {
                goto block_23;
            }
        } else {
block_23:
            var_v1_6 = (var_t0_2 * 4) + D_80125C10;
            var_v0_6 = &D_8011ECD0[var_t0_2];
            do {
                *(s16 *)((unsigned char *)var_v0_6 + (4390)) = (s16) var_v1_6->unk0;
                *(s16 *)((unsigned char *)var_v0_6 + (4394)) = (s16) *(s16 *)((unsigned char *)var_v1_6 + (2));
                temp_t4_2 = *(s16 *)((unsigned char *)var_v1_6 + (14));
                temp_t9_3 = *(s16 *)((unsigned char *)var_v1_6 + (12));
                temp_t5_3 = *(s16 *)((unsigned char *)var_v1_6 + (4));
                temp_t6_3 = *(s16 *)((unsigned char *)var_v1_6 + (6));
                temp_t8_3 = *(s16 *)((unsigned char *)var_v1_6 + (8));
                temp_t7_2 = *(s16 *)((unsigned char *)var_v1_6 + (10));
                var_v0_6 += 0x110;
                *(s16 *)((unsigned char *)var_v0_6 + (4172)) = 2;
                *(s16 *)((unsigned char *)var_v0_6 + (4240)) = 2;
                *(s16 *)((unsigned char *)var_v0_6 + (4308)) = 2;
                var_v1_6 += 0x10;
                *(s16 *)((unsigned char *)var_v0_6 + (4104)) = 2;
                *(s16 *)((unsigned char *)var_v0_6 + (4326)) = temp_t4_2;
                *(s16 *)((unsigned char *)var_v0_6 + (4322)) = temp_t9_3;
                *(s16 *)((unsigned char *)var_v0_6 + (4186)) = temp_t5_3;
                *(s16 *)((unsigned char *)var_v0_6 + (4190)) = temp_t6_3;
                *(s16 *)((unsigned char *)var_v0_6 + (4254)) = temp_t8_3;
                *(s16 *)((unsigned char *)var_v0_6 + (4258)) = temp_t7_2;
            } while (var_v0_6 != &D_8011ECD0[var_s0]);
        }
    }
    if (var_s0 == 0xF) {
        *(s16 *)((unsigned char *)D_8011ECD0 + (6414)) = 0xF;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6430)) = (s16) D_80125C48;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6434)) = (s16) D_80125C4A;
        *(s16 *)((unsigned char *)D_8011ECD0 + (5328)) = 2;
        *(s16 *)((unsigned char *)D_8011ECD0 + (5342)) = (s16) D_80125C48;
        *(s16 *)((unsigned char *)D_8011ECD0 + (5346)) = (s16) D_80125C4A;
        return;
    }
    *(s8 *)((unsigned char *)D_8011ECD0 + (5307)) = 0;
    *(s8 *)((unsigned char *)D_8011ECD0 + (6395)) = 0;
}
