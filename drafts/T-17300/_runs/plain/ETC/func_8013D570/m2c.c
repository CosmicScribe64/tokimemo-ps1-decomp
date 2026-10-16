? func_8009EFB0(void *, ?);                         /* extern */

void func_8013D570(s32 arg0, s32 arg1) {
    void *sp84;
    s32 sp54;
    s32 sp50;
    s16 temp_a1;
    s16 temp_a1_2;
    s16 temp_a2;
    s16 temp_a2_2;
    s16 temp_v1;
    s16 temp_v1_3;
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_a0_3;
    s32 temp_a0_4;
    s32 temp_lo;
    s32 temp_lo_10;
    s32 temp_lo_2;
    s32 temp_lo_3;
    s32 temp_lo_4;
    s32 temp_lo_5;
    s32 temp_lo_6;
    s32 temp_lo_7;
    s32 temp_lo_8;
    s32 temp_lo_9;
    s32 temp_v1_2;
    s32 temp_v1_4;
    s32 var_fp;
    s32 var_s4;
    void **var_s0;
    void *temp_v0;

    sp50 = 0xA0;
    sp54 = 0x18;
    var_s0 = &sp84;
    var_s4 = 0;
    var_fp = 0x140;
    do {
        temp_v0 = func_800490F0(0x28, D_8011ECA0);
        *var_s0 = temp_v0;
        func_8009F040((s32) temp_v0);
        func_8009EF84(*var_s0, 0);
        func_8009EFB0(*var_s0, 1);
        *(s16 *)((unsigned char *)(*var_s0) + (22)) = func_8009ECB0(2, 0, var_fp, 0x100);
        switch (arg1) {                             /* irregular */
        case 0:
            temp_lo = (s32) (arg0 * 0xB) / 80;
            temp_lo_2 = var_s4 * 0x88;
            temp_lo_3 = (s32) (arg0 * var_s4 * 0x44) / 80;
            temp_v1 = ((temp_lo_2 - temp_lo) - temp_lo_3) + 0x18;
            *(s16 *)((unsigned char *)(*var_s0) + (8)) = temp_v1;
            *(s16 *)((unsigned char *)(*var_s0) + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x65);
            temp_lo_4 = (s32) (arg0 * 0x44) / 80;
            *(s16 *)((unsigned char *)(*var_s0) + (16)) = (s16) ((temp_v1 - temp_lo_4) + 0x88);
            *(s16 *)((unsigned char *)(*var_s0) + (18)) = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x65);
            *(s16 *)((unsigned char *)(*var_s0) + (24)) = temp_v1;
            temp_a0 = 0xA0 - arg0;
            *(s16 *)((unsigned char *)(*var_s0) + (26)) = (s16) ((D_800E6280.unk_018[D_8011ECA0] + temp_a0) - 0x65);
            *(s16 *)((unsigned char *)(*var_s0) + (32)) = (s16) ((((0xA0 - temp_lo) + temp_lo_2) - temp_lo_3) - temp_lo_4);
            *(s16 *)((unsigned char *)(*var_s0) + (34)) = (s16) ((D_800E6280.unk_018[D_8011ECA0] + temp_a0) - 0x65);
            break;
        case 1:
            temp_v1_2 = (((s32) (arg0 * 0x8D) / 80) + (var_s4 * 0x88)) - ((s32) (arg0 * var_s4 * 0x44) / 80);
            temp_a1 = temp_v1_2 + 0x18;
            *(s16 *)((unsigned char *)(*var_s0) + (8)) = temp_a1;
            *(s16 *)((unsigned char *)(*var_s0) + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x65);
            temp_a2 = (temp_v1_2 - ((s32) (arg0 * 0x44) / 80)) + 0xA0;
            *(s16 *)((unsigned char *)(*var_s0) + (16)) = temp_a2;
            *(s16 *)((unsigned char *)(*var_s0) + (18)) = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x65);
            *(s16 *)((unsigned char *)(*var_s0) + (24)) = temp_a1;
            temp_a0_2 = 0xA0 - arg0;
            *(s16 *)((unsigned char *)(*var_s0) + (26)) = (s16) ((D_800E6280.unk_018[D_8011ECA0] + temp_a0_2) - 0x65);
            *(s16 *)((unsigned char *)(*var_s0) + (32)) = temp_a2;
            *(s16 *)((unsigned char *)(*var_s0) + (34)) = (s16) ((D_800E6280.unk_018[D_8011ECA0] + temp_a0_2) - 0x65);
            break;
        case 2:
            temp_lo_5 = (s32) (arg0 * 0xB) / 80;
            temp_lo_6 = var_s4 * 0x88;
            temp_lo_7 = (s32) (arg0 * var_s4 * 0x44) / 80;
            temp_v1_3 = ((temp_lo_6 - temp_lo_5) - temp_lo_7) + 0x18;
            temp_lo_8 = (s32) (arg0 * 0x70) / 80;
            *(s16 *)((unsigned char *)(*var_s0) + (8)) = temp_v1_3;
            *(s16 *)((unsigned char *)(*var_s0) + (10)) = (s16) ((D_800E6280.unk_018[D_8011ECA0] + temp_lo_8) - 0x65);
            temp_lo_9 = (s32) (arg0 * 0x44) / 80;
            *(s16 *)((unsigned char *)(*var_s0) + (16)) = (s16) ((temp_v1_3 - temp_lo_9) + 0x88);
            *(s16 *)((unsigned char *)(*var_s0) + (18)) = (s16) ((D_800E6280.unk_018[D_8011ECA0] + temp_lo_8) - 0x65);
            *(s16 *)((unsigned char *)(*var_s0) + (24)) = temp_v1_3;
            temp_a0_3 = 0xA0 - arg0;
            *(s16 *)((unsigned char *)(*var_s0) + (26)) = (s16) ((D_800E6280.unk_018[D_8011ECA0] + temp_lo_8 + temp_a0_3) - 0x65);
            *(s16 *)((unsigned char *)(*var_s0) + (32)) = (s16) ((((0xA0 - temp_lo_5) + temp_lo_6) - temp_lo_7) - temp_lo_9);
            *(s16 *)((unsigned char *)(*var_s0) + (34)) = (s16) ((D_800E6280.unk_018[D_8011ECA0] + temp_lo_8 + temp_a0_3) - 0x65);
            break;
        case 3:
            temp_v1_4 = (((s32) (arg0 * 0x8D) / 80) + (var_s4 * 0x88)) - ((s32) (arg0 * var_s4 * 0x44) / 80);
            temp_a1_2 = temp_v1_4 + 0x18;
            temp_lo_10 = (s32) (arg0 * 0x70) / 80;
            *(s16 *)((unsigned char *)(*var_s0) + (8)) = temp_a1_2;
            *(s16 *)((unsigned char *)(*var_s0) + (10)) = (s16) ((D_800E6280.unk_018[D_8011ECA0] + temp_lo_10) - 0x65);
            temp_a2_2 = (temp_v1_4 - ((s32) (arg0 * 0x44) / 80)) + 0xA0;
            *(s16 *)((unsigned char *)(*var_s0) + (16)) = temp_a2_2;
            *(s16 *)((unsigned char *)(*var_s0) + (18)) = (s16) ((D_800E6280.unk_018[D_8011ECA0] + temp_lo_10) - 0x65);
            *(s16 *)((unsigned char *)(*var_s0) + (24)) = temp_a1_2;
            temp_a0_4 = 0xA0 - arg0;
            *(s16 *)((unsigned char *)(*var_s0) + (26)) = (s16) ((D_800E6280.unk_018[D_8011ECA0] + temp_lo_10 + temp_a0_4) - 0x65);
            *(s16 *)((unsigned char *)(*var_s0) + (32)) = temp_a2_2;
            *(s16 *)((unsigned char *)(*var_s0) + (34)) = (s16) ((D_800E6280.unk_018[D_8011ECA0] + temp_lo_10 + temp_a0_4) - 0x65);
            break;
        }
        *(s8 *)((unsigned char *)(*var_s0) + (12)) = (s8) sp54;
        *(s8 *)((unsigned char *)(*var_s0) + (13)) = 0x13;
        *(s8 *)((unsigned char *)(*var_s0) + (20)) = (s8) sp50;
        *(s8 *)((unsigned char *)(*var_s0) + (21)) = 0x13;
        *(s8 *)((unsigned char *)(*var_s0) + (28)) = (s8) sp54;
        *(s8 *)((unsigned char *)(*var_s0) + (29)) = 0xB3;
        *(s8 *)((unsigned char *)(*var_s0) + (36)) = (s8) sp50;
        *(s8 *)((unsigned char *)(*var_s0) + (37)) = 0xB3;
        *(s8 *)((unsigned char *)(*var_s0) + (4)) = 0x80;
        *(s8 *)((unsigned char *)(*var_s0) + (5)) = 0x80;
        *(s8 *)((unsigned char *)(*var_s0) + (6)) = 0x80;
        func_8009EED0((D_8011ECA0 << 0xA) + 0x28 + D_800E8CA0, *var_s0, 0x80, 0x13);
        var_s4 += 1;
        sp50 += 8;
        sp54 += 8;
        var_s0 += 4;
        var_fp += 0x80;
    } while (var_s4 != 2);
}
