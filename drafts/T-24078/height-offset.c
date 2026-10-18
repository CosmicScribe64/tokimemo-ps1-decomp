/* NON_MATCHING: T-24078 signed lower-vertex offset control. */
/* FAKE: retain the current implicit-int caller view with s32 fallthrough;
 * original return type unproven and callers ignore result. T-24078. */
s32 func_8013D570(s32 arg0, s32 arg1) {
    /* FAKE: counter home above poly tests the unknown original slot; original declaration order unproven. T-24078. */
    s32 var_s4;
    POLY_FT4 *poly[2];
    s32 temp_a2;
    s32 temp_v1;
    s32 temp_a0;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_lo_3;
    s32 temp_lo_4;
    s32 temp_lo_8;
    s32 var_fp;
    POLY_FT4 **var_s0;

    var_s0 = poly;
    var_s4 = 0;
    var_fp = 0x140;
    do {
        *var_s0 = (POLY_FT4 *)GetWorkBase(0x28, D_8011ECA0);
        func_8009F040(*var_s0);
        SetSemiTrans(*var_s0, 0);
        SetShadeTex(*var_s0, 1);
        (*var_s0)->tpage = func_8009ECB0(2, 0, var_fp, 0x100);
        switch (arg1) {
        case 0:
            temp_lo = (s32) (arg0 * 0xB) / 80;
            temp_lo_2 = var_s4 * 0x88;
            temp_lo_3 = (s32) (arg0 * var_s4 * 0x44) / 80;
            temp_v1 = ((0x18 - temp_lo) + temp_lo_2) - temp_lo_3;
            (*var_s0)->x0 = temp_v1;
            (*var_s0)->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x65);
            temp_lo_4 = (s32) (arg0 * 0x44) / 80;
            (*var_s0)->x1 = (s16) ((temp_v1 - temp_lo_4) + 0x88);
            (*var_s0)->y1 = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x65);
            (*var_s0)->x2 = temp_v1;
            temp_a0 = arg0 - 0xA0; /* FAKE: shared negative height offset tests subtraction web; original spelling unproven. T-24078. */
            (*var_s0)->y2 = (s16) ((D_800E6280.unk_018[D_8011ECA0] - temp_a0) - 0x65);
            (*var_s0)->x3 = (s16) ((((0xA0 - temp_lo) + temp_lo_2) - temp_lo_3) - temp_lo_4);
            (*var_s0)->y3 = (s16) ((D_800E6280.unk_018[D_8011ECA0] - temp_a0) - 0x65);
            break;
        case 1:
            temp_a0 = (((s32) (arg0 * 0x8D) / 80) + (var_s4 * 0x88)) - ((s32) (arg0 * var_s4 * 0x44) / 80);
            temp_v1 = temp_a0 + 0x18;
            (*var_s0)->x0 = temp_v1;
            (*var_s0)->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x65);
            temp_a2 = (temp_a0 - ((s32) (arg0 * 0x44) / 80)) + 0xA0;
            (*var_s0)->x1 = temp_a2;
            (*var_s0)->y1 = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x65);
            (*var_s0)->x2 = temp_v1;
            temp_a0 = arg0 - 0xA0; /* FAKE: shared negative height offset tests subtraction web; original spelling unproven. T-24078. */
            (*var_s0)->y2 = (s16) ((D_800E6280.unk_018[D_8011ECA0] - temp_a0) - 0x65);
            (*var_s0)->x3 = temp_a2;
            (*var_s0)->y3 = (s16) ((D_800E6280.unk_018[D_8011ECA0] - temp_a0) - 0x65);
            break;
        case 2:
            temp_lo = (s32) (arg0 * 0xB) / 80;
            temp_lo_2 = var_s4 * 0x88;
            temp_lo_3 = (s32) (arg0 * var_s4 * 0x44) / 80;
            temp_v1 = ((0x18 - temp_lo) + temp_lo_2) - temp_lo_3;
            temp_lo_8 = (s32) (arg0 * 0x70) / 80;
            (*var_s0)->x0 = temp_v1;
            (*var_s0)->y0 = (s16) ((D_800E6280.unk_018[D_8011ECA0] + temp_lo_8) - 0x65);
            temp_lo_4 = (s32) (arg0 * 0x44) / 80;
            (*var_s0)->x1 = (s16) ((temp_v1 - temp_lo_4) + 0x88);
            (*var_s0)->y1 = (s16) ((D_800E6280.unk_018[D_8011ECA0] + temp_lo_8) - 0x65);
            (*var_s0)->x2 = temp_v1;
            temp_a0 = arg0 - 0xA0; /* FAKE: shared negative height offset tests subtraction web; original spelling unproven. T-24078. */
            (*var_s0)->y2 = (s16) ((D_800E6280.unk_018[D_8011ECA0] + temp_lo_8 - temp_a0) - 0x65);
            (*var_s0)->x3 = (s16) ((((0xA0 - temp_lo) + temp_lo_2) - temp_lo_3) - temp_lo_4);
            (*var_s0)->y3 = (s16) ((D_800E6280.unk_018[D_8011ECA0] + temp_lo_8 - temp_a0) - 0x65);
            break;
        case 3:
            temp_a0 = (((s32) (arg0 * 0x8D) / 80) + (var_s4 * 0x88)) - ((s32) (arg0 * var_s4 * 0x44) / 80);
            temp_v1 = temp_a0 + 0x18;
            temp_lo_8 = (s32) (arg0 * 0x70) / 80;
            (*var_s0)->x0 = temp_v1;
            (*var_s0)->y0 = (s16) ((D_800E6280.unk_018[D_8011ECA0] + temp_lo_8) - 0x65);
            temp_a2 = (temp_a0 - ((s32) (arg0 * 0x44) / 80)) + 0xA0;
            (*var_s0)->x1 = temp_a2;
            (*var_s0)->y1 = (s16) ((D_800E6280.unk_018[D_8011ECA0] + temp_lo_8) - 0x65);
            (*var_s0)->x2 = temp_v1;
            temp_a0 = arg0 - 0xA0; /* FAKE: shared negative height offset tests subtraction web; original spelling unproven. T-24078. */
            (*var_s0)->y2 = (s16) ((D_800E6280.unk_018[D_8011ECA0] + temp_lo_8 - temp_a0) - 0x65);
            (*var_s0)->x3 = temp_a2;
            (*var_s0)->y3 = (s16) ((D_800E6280.unk_018[D_8011ECA0] + temp_lo_8 - temp_a0) - 0x65);
            break;
        }
        temp_v1 = var_s4 * 8 + 0x18;
        (*var_s0)->u0 = temp_v1;
        (*var_s0)->v0 = 0x13;
        (*var_s0)->u1 = (temp_a0 = var_s4 * 8 + 0xA0);
        (*var_s0)->v1 = 0x13;
        (*var_s0)->u2 = temp_v1;
        (*var_s0)->v2 = 0xB3;
        (*var_s0)->u3 = temp_a0;
        (*var_s0)->v3 = 0xB3;
        (*var_s0)->r0 = 0x80;
        (*var_s0)->g0 = 0x80;
        (*var_s0)->b0 = 0x80;
        AddPrim(D_8011ECA0 * 0x400 + 0x28 + D_800E8CA0, *var_s0);
        var_s4 += 1;
        var_s0 += 1;
        var_fp += 0x80;
    } while (var_s4 != 2);
}
