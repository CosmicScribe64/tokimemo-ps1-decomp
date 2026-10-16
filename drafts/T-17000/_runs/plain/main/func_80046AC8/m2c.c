void func_80046AC8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    s32 sp7C;
    s32 sp70;
    s32 sp68;
    s32 sp64;
    s32 sp5C;
    s32 sp58;
    s16 temp_v0_3;
    s16 temp_v0_4;
    s16 temp_v1;
    s16 temp_v1_2;
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_a0_3;
    s32 temp_t2;
    s32 temp_t3;
    s32 temp_t4;
    s32 temp_t6;
    s32 temp_t6_2;
    s32 temp_t8;
    s32 temp_t8_2;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 var_s6;
    s32 var_s6_2;
    s8 temp_a1;
    s8 temp_a1_2;
    s8 temp_a1_3;
    s8 temp_a1_4;
    s8 temp_fp;
    s8 temp_t7;

    temp_t6 = arg7 & 0xF;
    temp_t7 = (arg0 & 0x20) * 4;
    sp68 = (arg0 & 0xF) << 6;
    sp58 = (arg6 >> 0x1C) & 0xF;
    var_s6 = 0;
    sp64 = temp_t7 + ((arg0 & 0x10) * 0x10);
    sp5C = temp_t6 * 4;
    sp7C = temp_t6;
    do {
        temp_v0 = GetWorkBase(0x34, D_8011ECA0);
        func_8009F068(temp_v0);
        if (sp58 != 0) {
            SetSemiTrans(temp_v0, 1);
        } else {
            SetSemiTrans(temp_v0, 0);
        }
        if (sp58 != 0) {
            temp_fp = (arg6 >> 0x10) & 0xFF;
            temp_t3 = arg6 >> 8;
            *(s8 *)((unsigned char *)temp_v0 + (5)) = (s8) temp_t3;
            *(s8 *)((unsigned char *)temp_v0 + (4)) = temp_fp;
            *(s8 *)((unsigned char *)temp_v0 + (6)) = (s8) arg6;
            *(s8 *)((unsigned char *)temp_v0 + (17)) = (s8) temp_t3;
            *(s8 *)((unsigned char *)temp_v0 + (16)) = temp_fp;
            *(s8 *)((unsigned char *)temp_v0 + (18)) = (s8) arg6;
            *(s8 *)((unsigned char *)temp_v0 + (29)) = (s8) temp_t3;
            *(s8 *)((unsigned char *)temp_v0 + (28)) = temp_fp;
            *(s8 *)((unsigned char *)temp_v0 + (30)) = (s8) arg6;
            *(s8 *)((unsigned char *)temp_v0 + (41)) = (s8) temp_t3;
            *(s8 *)((unsigned char *)temp_v0 + (40)) = temp_fp;
            *(s8 *)((unsigned char *)temp_v0 + (42)) = (s8) arg6;
        } else {
            *(s8 *)((unsigned char *)temp_v0 + (4)) = 0x80;
            *(s8 *)((unsigned char *)temp_v0 + (5)) = 0x80;
            *(s8 *)((unsigned char *)temp_v0 + (6)) = 0x80;
            *(s8 *)((unsigned char *)temp_v0 + (16)) = 0x80;
            *(s8 *)((unsigned char *)temp_v0 + (17)) = 0x80;
            *(s8 *)((unsigned char *)temp_v0 + (18)) = 0x80;
            *(s8 *)((unsigned char *)temp_v0 + (28)) = 0x80;
            *(s8 *)((unsigned char *)temp_v0 + (29)) = 0x80;
            *(s8 *)((unsigned char *)temp_v0 + (30)) = 0x80;
            *(s8 *)((unsigned char *)temp_v0 + (40)) = 0x80;
            *(s8 *)((unsigned char *)temp_v0 + (41)) = 0x80;
            *(s8 *)((unsigned char *)temp_v0 + (42)) = 0x80;
        }
        switch (var_s6) {                           /* switch 1; irregular */
        case 0:                                     /* switch 1 */
            *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg4);
            temp_a1 = temp_t7 + 0x7F;
            *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg5);
            temp_t6_2 = (s32) (arg3 << 7) / 4096;
            *(s16 *)((unsigned char *)temp_v0 + (20)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg4 + temp_t6_2);
            *(s16 *)((unsigned char *)temp_v0 + (22)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg5);
            *(s16 *)((unsigned char *)temp_v0 + (32)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg4);
            *(s16 *)((unsigned char *)temp_v0 + (34)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg5 + temp_t6_2);
            *(s16 *)((unsigned char *)temp_v0 + (44)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg4 + temp_t6_2);
            *(s8 *)((unsigned char *)temp_v0 + (49)) = temp_a1;
            *(s16 *)((unsigned char *)temp_v0 + (46)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg5 + temp_t6_2);
            *(s8 *)((unsigned char *)temp_v0 + (48)) = 0x7F;
            *(s8 *)((unsigned char *)temp_v0 + (37)) = temp_a1;
            *(s8 *)((unsigned char *)temp_v0 + (36)) = 0;
            *(s8 *)((unsigned char *)temp_v0 + (25)) = temp_t7;
            *(s8 *)((unsigned char *)temp_v0 + (24)) = 0x7F;
            *(s8 *)((unsigned char *)temp_v0 + (13)) = temp_t7;
block_16:
            *(s8 *)((unsigned char *)temp_v0 + (12)) = 0;
            break;
        case 1:                                     /* switch 1 */
            *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg4);
            temp_a1_2 = temp_t7 + 0x7F;
            *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg5);
            temp_t2 = (s32) (arg3 << 7) / 4096;
            *(s16 *)((unsigned char *)temp_v0 + (20)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg4 + temp_t2);
            *(s16 *)((unsigned char *)temp_v0 + (22)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg5);
            temp_a0 = -temp_t2;
            *(s16 *)((unsigned char *)temp_v0 + (32)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg4);
            *(s16 *)((unsigned char *)temp_v0 + (34)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg5 + temp_a0);
            *(s16 *)((unsigned char *)temp_v0 + (44)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg4 + temp_t2);
            *(s8 *)((unsigned char *)temp_v0 + (49)) = temp_a1_2;
            *(s16 *)((unsigned char *)temp_v0 + (46)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg5 + temp_a0);
            *(s8 *)((unsigned char *)temp_v0 + (48)) = 0x7F;
            *(s8 *)((unsigned char *)temp_v0 + (37)) = temp_a1_2;
            *(s8 *)((unsigned char *)temp_v0 + (36)) = 0;
            *(s8 *)((unsigned char *)temp_v0 + (25)) = temp_t7;
            *(s8 *)((unsigned char *)temp_v0 + (24)) = 0x7F;
            *(s8 *)((unsigned char *)temp_v0 + (13)) = temp_t7;
            goto block_16;
        case 2:                                     /* switch 1 */
            *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg4);
            temp_a1_3 = temp_t7 + 0x7F;
            *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg5);
            temp_t8 = (s32) (arg3 << 7) / 4096;
            temp_a0_2 = -temp_t8;
            *(s16 *)((unsigned char *)temp_v0 + (20)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg4 + temp_a0_2);
            *(s16 *)((unsigned char *)temp_v0 + (22)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg5);
            *(s16 *)((unsigned char *)temp_v0 + (32)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg4);
            *(s16 *)((unsigned char *)temp_v0 + (34)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg5 + temp_t8);
            *(s16 *)((unsigned char *)temp_v0 + (44)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg4 + temp_a0_2);
            *(s8 *)((unsigned char *)temp_v0 + (49)) = temp_a1_3;
            *(s16 *)((unsigned char *)temp_v0 + (46)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg5 + temp_t8);
            *(s8 *)((unsigned char *)temp_v0 + (48)) = 0x7F;
            *(s8 *)((unsigned char *)temp_v0 + (37)) = temp_a1_3;
            *(s8 *)((unsigned char *)temp_v0 + (36)) = 0;
            *(s8 *)((unsigned char *)temp_v0 + (25)) = temp_t7;
            *(s8 *)((unsigned char *)temp_v0 + (24)) = 0x7F;
            *(s8 *)((unsigned char *)temp_v0 + (13)) = temp_t7;
            goto block_16;
        case 3:                                     /* switch 1 */
            *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg4);
            temp_a1_4 = temp_t7 + 0x7F;
            *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg5);
            temp_a0_3 = -((s32) (arg3 << 7) / 4096);
            *(s16 *)((unsigned char *)temp_v0 + (20)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg4 + temp_a0_3);
            *(s16 *)((unsigned char *)temp_v0 + (22)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg5);
            *(s16 *)((unsigned char *)temp_v0 + (32)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg4);
            *(s16 *)((unsigned char *)temp_v0 + (34)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg5 + temp_a0_3);
            *(s16 *)((unsigned char *)temp_v0 + (44)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg4 + temp_a0_3);
            *(s8 *)((unsigned char *)temp_v0 + (49)) = temp_a1_4;
            *(s16 *)((unsigned char *)temp_v0 + (46)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg5 + temp_a0_3);
            *(s8 *)((unsigned char *)temp_v0 + (48)) = 0x7F;
            *(s8 *)((unsigned char *)temp_v0 + (37)) = temp_a1_4;
            *(s8 *)((unsigned char *)temp_v0 + (36)) = 0;
            *(s8 *)((unsigned char *)temp_v0 + (25)) = temp_t7;
            *(s8 *)((unsigned char *)temp_v0 + (24)) = 0x7F;
            *(s8 *)((unsigned char *)temp_v0 + (13)) = temp_t7;
            goto block_16;
        }
        *(s16 *)((unsigned char *)temp_v0 + (26)) = func_8009ECB0(0, 0, sp68, sp64);
        *(s16 *)((unsigned char *)temp_v0 + (14)) = func_8009ED74(arg1, arg2);
        AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + sp5C], temp_v0);
        dtd_on_tpage(0, 0, sp7C, 1, 0);
        var_s6 += 1;
    } while (var_s6 < 4);
    var_s6_2 = 0;
    if ((arg6 >> 0x18) & 0xF) {
        sp70 = arg6 & 0xFF;
        do {
            temp_v0_2 = GetWorkBase(0x10, D_8011ECA0);
            func_8009F0F4(temp_v0_2);
            SetSemiTrans(temp_v0_2, 0);
            SetShadeTex(temp_v0_2, 0);
            *(s8 *)((unsigned char *)temp_v0_2 + (4)) = (s8) ((arg6 >> 0x10) & 0xFF);
            *(s8 *)((unsigned char *)temp_v0_2 + (5)) = (s8) ((arg6 >> 8) & 0xFF);
            *(s8 *)((unsigned char *)temp_v0_2 + (6)) = (s8) sp70;
            switch (var_s6_2) {                     /* switch 2; irregular */
            case 0:                                 /* switch 2 */
                *(s16 *)((unsigned char *)temp_v0_2 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0xA0);
                *(s16 *)((unsigned char *)temp_v0_2 + (12)) = 0x140;
                *(s16 *)((unsigned char *)temp_v0_2 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x78);
                temp_v0_3 = (arg5 - ((s32) (arg3 << 7) / 4096)) + 0x78;
                *(s16 *)((unsigned char *)temp_v0_2 + (14)) = temp_v0_3;
                if (temp_v0_3 > 0) {
                    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + sp5C], temp_v0_2);
                }
                break;
            case 1:                                 /* switch 2 */
                *(s16 *)((unsigned char *)temp_v0_2 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0xA0);
                *(s16 *)((unsigned char *)temp_v0_2 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x78);
                *(s16 *)((unsigned char *)temp_v0_2 + (14)) = 0xF0;
                temp_v0_4 = (arg4 - ((s32) (arg3 << 7) / 4096)) + 0xA0;
                *(s16 *)((unsigned char *)temp_v0_2 + (12)) = temp_v0_4;
                if (temp_v0_4 > 0) {
                    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + sp5C], temp_v0_2);
                }
                break;
            case 2:                                 /* switch 2 */
                *(s16 *)((unsigned char *)temp_v0_2 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0xA0);
                temp_t8_2 = (s32) (arg3 << 7) / 4096;
                temp_v1 = (-arg5 - temp_t8_2) + 0x78;
                *(s16 *)((unsigned char *)temp_v0_2 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg5 + temp_t8_2);
                *(s16 *)((unsigned char *)temp_v0_2 + (14)) = temp_v1;
                *(s16 *)((unsigned char *)temp_v0_2 + (12)) = 0x140;
                if (temp_v1 > 0) {
                    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + sp5C], temp_v0_2);
                }
                break;
            case 3:                                 /* switch 2 */
                temp_t4 = (s32) (arg3 << 7) / 4096;
                *(s16 *)((unsigned char *)temp_v0_2 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg4 + temp_t4);
                temp_v1_2 = (-arg4 - temp_t4) + 0xA0;
                *(s16 *)((unsigned char *)temp_v0_2 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x78);
                *(s16 *)((unsigned char *)temp_v0_2 + (12)) = temp_v1_2;
                *(s16 *)((unsigned char *)temp_v0_2 + (14)) = 0xF0;
                if (temp_v1_2 > 0) {
                    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + sp5C], temp_v0_2);
                }
                break;
            }
            var_s6_2 += 1;
        } while (var_s6_2 != 4);
    }
}
