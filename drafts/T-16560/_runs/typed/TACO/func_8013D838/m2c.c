? func_800A1070(? *);                               /* extern */
extern ? D_800E6298;

void func_8013D838(s16 arg0, s16 arg1, s16 arg2, s32 arg3) {
    ? sp98;
    s16 sp94;
    s16 sp92;
    s16 sp90;
    s16 sp80;
    s32 sp78;
    ? sp74;
    ? sp70;
    s32 sp64;
    s32 sp58;
    s16 temp_v0;
    s16 var_s1;
    s32 temp_s7;
    s32 temp_t8;
    s32 temp_t9;
    s32 temp_v0_2;
    s32 var_fp;
    s32 var_s0;
    s32 var_s1_2;
    s8 temp_a2;
    s8 temp_a3;
    s8 temp_t0;
    s8 temp_t5;
    void *var_s2;

    sp98.unk0 = (s32) D_80129C40.unk0;
    *(s32 *)((unsigned char *)(&sp98) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (4));
    *(s32 *)((unsigned char *)(&sp98) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (8));
    *(s32 *)((unsigned char *)(&sp98) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (12));
    var_fp = 0x100;
    *(s32 *)((unsigned char *)(&sp98) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (16));
    *(s32 *)((unsigned char *)(&sp98) + (20)) = (s32) D_80129C40.t[0];
    *(s32 *)((unsigned char *)(&sp98) + (24)) = (s32) D_80129C40.t[1];
    *(s32 *)((unsigned char *)(&sp98) + (28)) = (s32) D_80129C40.t[2];
    func_800A1100((s32) &sp98);
    func_800A1070(&sp98);
    if ((arg3 < 0) || (arg3 == 6)) {
        var_fp = 0x200;
    }
    sp94 = arg2;
    sp78 = 0;
    temp_s7 = var_fp / 2;
    sp64 = (s32) arg1;
    sp58 = 0;
    do {
        var_s1 = arg0;
        sp92 = sp64 - temp_s7;
        var_s0 = 0;
        var_s2 = sp58 + &sp80;
loop_5:
        sp90 = var_s1 - temp_s7;
        func_800AB170((s32) &sp90, (s32) var_s2, (s32) &sp74, (s32) &sp70);
        var_s0 += 1;
        var_s1 += var_fp;
        var_s2 += 8;
        if (var_s0 != 2) {
            goto loop_5;
        }
        temp_t9 = sp78 + 1;
        sp58 += 4;
        sp64 += var_fp;
        sp78 = temp_t9;
    } while (temp_t9 != 2);
    if ((sp88 >= -0x2C0) && (sp80 < 0x2C1)) {
        temp_v0 = *(&D_800E6298 + (D_8011ECA0 * 2));
        if (sp86 >= (temp_v0 - 0x200)) {
            if ((temp_v0 + 0x200) >= sp8E) {
                var_s1_2 = (s32) (D_80122754 - arg2) / 127;
                if (var_s1_2 >= 0x100) {
                    var_s1_2 = 0xFF;
                } else if (var_s1_2 <= 0) {
                    var_s1_2 = 1;
                }
                temp_v0_2 = func_800490F0(0x28, D_8011ECA0);
                func_8009F040(temp_v0_2);
                func_8009EF84(temp_v0_2, 1);
                *(s8 *)((unsigned char *)temp_v0_2 + (4)) = 0x80;
                *(s8 *)((unsigned char *)temp_v0_2 + (5)) = 0x80;
                *(s8 *)((unsigned char *)temp_v0_2 + (6)) = 0x80;
                *(s16 *)((unsigned char *)temp_v0_2 + (22)) = func_8009ECB0(0, 1, 0x380, 0x100);
                *(s16 *)((unsigned char *)temp_v0_2 + (14)) = func_8009ED74(0, 0x1E5);
                temp_t5 = arg3 << 6;
                *(s8 *)((unsigned char *)temp_v0_2 + (12)) = temp_t5;
                temp_a3 = temp_t5 + 0x3F;
                temp_t8 = (arg3 & 4) * 0x10;
                temp_a2 = temp_t8 + 0x80;
                temp_t0 = temp_t8 + 0xBF;
                *(s8 *)((unsigned char *)temp_v0_2 + (29)) = temp_t0;
                *(s8 *)((unsigned char *)temp_v0_2 + (37)) = temp_t0;
                *(s8 *)((unsigned char *)temp_v0_2 + (13)) = temp_a2;
                *(s8 *)((unsigned char *)temp_v0_2 + (21)) = temp_a2;
                *(s8 *)((unsigned char *)temp_v0_2 + (20)) = temp_a3;
                *(s8 *)((unsigned char *)temp_v0_2 + (36)) = temp_a3;
                *(s8 *)((unsigned char *)temp_v0_2 + (28)) = temp_t5;
                *(s16 *)((unsigned char *)temp_v0_2 + (8)) = sp80;
                *(s16 *)((unsigned char *)temp_v0_2 + (10)) = sp82;
                *(s16 *)((unsigned char *)temp_v0_2 + (16)) = sp84;
                *(s16 *)((unsigned char *)temp_v0_2 + (18)) = sp86;
                *(s16 *)((unsigned char *)temp_v0_2 + (24)) = sp88;
                *(s16 *)((unsigned char *)temp_v0_2 + (26)) = sp8A;
                *(s16 *)((unsigned char *)temp_v0_2 + (32)) = sp8C;
                *(s16 *)((unsigned char *)temp_v0_2 + (34)) = sp8E;
                func_8009EED0(&D_800E8CA0[(D_8011ECA0 << 0xA) + (var_s1_2 * 4)], temp_v0_2, temp_a2, temp_a3);
            }
        }
    }
}
