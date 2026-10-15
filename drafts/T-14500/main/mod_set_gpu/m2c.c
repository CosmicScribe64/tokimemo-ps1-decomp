void mod_set_gpu(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    u32 sp9C;
    s32 sp78;
    void *sp6C;
    s32 sp60;
    s32 sp5C;
    s16 var_s4;
    s32 *var_s6;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 var_s3;
    u32 temp_s1;
    u32 temp_s2;
    u32 temp_s7;
    u32 var_a2;
    u32 var_s5;
    u8 temp_t9;
    u8 var_v0;
    u8 var_v1;
    void *temp_t8;

    temp_v0 = func_8004901C();
    temp_t8 = (arg0 * 0x14) + D_801217A0;
    sp6C = temp_t8;
    temp_t9 = *(u8 *)((unsigned char *)temp_t8 + (8));
    switch (temp_t9) {
    case 0:
        sp78 = 1;
        break;
    case 1:
        sp78 = 2;
        break;
    case 2:
        sp78 = 2;
        break;
    case 3:
        sp78 = 1;
        break;
    case 4:
        sp78 = 1;
        break;
    case 5:
        sp78 = 1;
        break;
    default:
        sp78 = 1;
        break;
    }
    var_s3 = 0xFFFF;
    var_v1 = *(u8 *)((unsigned char *)sp6C + (13));
    var_v0 = *(u8 *)((unsigned char *)sp6C + (12));
    var_a2 = 0;
    var_s6 = (var_v1 * arg1 * var_v0 * 4) + *(s32 *)((unsigned char *)sp6C + (4));
    if (var_v1 != 0) {
        do {
            var_s5 = 0;
            if (var_v0 != 0) {
                sp5C = arg6 * 4;
                sp60 = (var_a2 * 8) + arg3 + 0x168;
                sp9C = var_a2;
                var_s4 = arg2 + 0xA0;
                do {
                    temp_v0_2 = *var_s6;
                    temp_s1 = (u32) (temp_v0_2 & 0xFF0000) >> 0x10;
                    temp_s2 = (u32) (temp_v0_2 & 0xFF000000) >> 0x18;
                    temp_s7 = (u32) (temp_v0_2 & 0xFF00) >> 8;
                    temp_v0_3 = GetWorkBase(0x10, D_8011ECA0);
                    func_8009F07C(temp_v0_3);
                    SetSemiTrans(temp_v0_3, 0);
                    *(s16 *)((unsigned char *)temp_v0_3 + (8)) = var_s4;
                    *(s16 *)((unsigned char *)temp_v0_3 + (10)) = (s16) (sp60 - (D_8011ECA0 * 0xF0));
                    *(s8 *)((unsigned char *)temp_v0_3 + (13)) = (s8) (((u32) (temp_s1 & 0xF0) >> 4) * 8);
                    *(s8 *)((unsigned char *)temp_v0_3 + (12)) = (s8) ((temp_s1 & 0xF) * 8);
                    *(s8 *)((unsigned char *)temp_v0_3 + (6)) = (s8) temp_v0;
                    *(s8 *)((unsigned char *)temp_v0_3 + (5)) = (s8) temp_v0;
                    *(s8 *)((unsigned char *)temp_v0_3 + (4)) = (s8) temp_v0;
                    temp_v0_4 = search_tpage_multi(*(sp6C->unk0 + (temp_s2 * 4)), sp78);
                    if (var_s3 != temp_v0_4) {
                        if (var_s3 != 0xFFFF) {
                            goto_tpage((var_s3 & 0xF) << 6, (var_s3 & 0x10) * 0x10, arg6 + 1, 0, 0);
                        }
                        var_s3 = temp_v0_4 & 0xFFFF & 0xFFFF;
                    }
                    *(s16 *)((unsigned char *)temp_v0_3 + (14)) = func_8009ED74(arg4 + (temp_s7 * 0x10), arg5);
                    AddPrim(&*(? *)((unsigned char *)(&D_800E8CA0[(D_8011ECA0 << 0xA) + sp5C]) + (4)), temp_v0_3);
                    var_s5 += 1;
                    var_v0 = *(u8 *)((unsigned char *)sp6C + (12));
                    var_s4 += 8;
                    var_s6 += 4;
                } while (var_s5 < var_v0);
                var_v1 = *(u8 *)((unsigned char *)sp6C + (13));
                var_a2 = sp9C;
            }
            var_a2 += 1;
        } while (var_a2 < var_v1);
    }
    goto_tpage((var_s3 & 0xF) << 6, (var_s3 & 0x10) * 0x10, arg6 + 1, 0, 0);
}
