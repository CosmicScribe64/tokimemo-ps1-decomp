void func_801416E8(Tc14 *arg0, s32 arg1) {
    s32 sp98;
    s32 sp92;
    s32 sp9E;
    s32 sp96;
    s32 sp9C;
    s32 sp9A;
    s32 sp94;
    s32 spA8[8];
    s16 spA4;
    s16 spA2;
    s16 spA0;
    s16 sp90;
    s32 sp80;
    s32 sp7C[1];
    s32 sp78[1];
    s32 sp70;
    u16 sp6C;
    s32 sp54;
    Tc14 *temp_fp;
    s16 temp_a0;
    s16 temp_v0;
    s32 temp_s7;
    s32 temp_t5;
    s32 temp_t9;
    s32 temp_v0_2;
    s32 var_s0;
    s32 var_s6;
    s32 var_t0;
    u8 temp_s2;
    u8 *var_s1;

    sp6C = D_8015F584;
    (*(s32 *)&spA8) = (s32) D_80129C40.unk0;
    *(s32 *)((unsigned char *)(&spA8) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (4));
    *(s32 *)((unsigned char *)(&spA8) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (8));
    *(s32 *)((unsigned char *)(&spA8) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (12));
    *(s32 *)((unsigned char *)(&spA8) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (16));
    *(s32 *)((unsigned char *)(&spA8) + (20)) = (s32) D_80129C40.t[0];
    *(s32 *)((unsigned char *)(&spA8) + (24)) = (s32) D_80129C40.t[1];
    *(s32 *)((unsigned char *)(&spA8) + (28)) = (s32) D_80129C40.t[2];
    func_800A1100((s32) &spA8);
    func_800A1070(&spA8);
    temp_fp = &arg0[arg1];
    temp_a0 = temp_fp->unk4;
    var_t0 = (s32) (D_80122754 - temp_a0) / 127;
    if (var_t0 >= 0x100) {
        var_t0 = 0xFF;
        goto block_4;
    }
    var_s6 = var_t0 * 4;
    if (var_t0 <= 0) {
        var_t0 = 1;
block_4:
        var_s6 = var_t0 * 4;
    }
    spA4 = temp_a0;
    temp_t5 = (s32) (((var_s6 + var_t0) * 8) + 0x280) / 16;
    temp_s7 = temp_t5 / 2;
    sp80 = 0;
    sp54 = 0;
    sp70 = var_t0;
    do {
        var_s0 = 0;
        var_s1 = sp54 + &sp90;
        spA2 = (temp_fp->unk2 + (sp80 * temp_t5)) - temp_s7;
loop_7:
        spA0 = (temp_fp->unk0 + (var_s0 * temp_t5)) - temp_s7;
        func_800AB170((s32) &spA0, (s32) var_s1, (s32) &sp7C, (s32) &sp78);
        var_s0 += 1;
        var_s1 += 8;
        if (var_s0 != 2) {
            goto loop_7;
        }
        temp_t9 = sp80 + 1;
        sp54 += 4;
        sp80 = temp_t9;
    } while (temp_t9 != 2);
    if ((sp98 >= -0x2C0) && (sp90 < 0x2C1)) {
        temp_v0 = *(&D_800E6298 + (D_8011ECA0 * 2));
        if (sp96 >= (temp_v0 - 0x200)) {
            if ((temp_v0 + 0x200) >= sp9E) {
                temp_s2 = temp_fp->unk12;
                temp_v0_2 = GetWorkBase(0x28, D_8011ECA0);
                func_8009F040(temp_v0_2);
                SetSemiTrans(temp_v0_2, 1);
                *(s8 *)((unsigned char *)temp_v0_2 + (4)) = 0x80;
                *(s8 *)((unsigned char *)temp_v0_2 + (5)) = 0x80;
                *(s8 *)((unsigned char *)temp_v0_2 + (6)) = 0x80;
                *(s16 *)((unsigned char *)temp_v0_2 + (22)) = func_8009ECB0(0, 1, 0x380, 0x100);
                *(s16 *)((unsigned char *)temp_v0_2 + (14)) = func_8009ED74(((s32) temp_s2 % 14) * 0x10, 0x1EB);
                if (sp70 < 0x20) {
                    *(s8 *)((unsigned char *)temp_v0_2 + (20)) = 0x9E;
                    *(s8 *)((unsigned char *)temp_v0_2 + (36)) = 0x9E;
                    *(s8 *)((unsigned char *)temp_v0_2 + (29)) = 0x3E;
                    *(s8 *)((unsigned char *)temp_v0_2 + (37)) = 0x3E;
                    *(s8 *)((unsigned char *)temp_v0_2 + (12)) = 0x80;
                    *(s8 *)((unsigned char *)temp_v0_2 + (13)) = 0x20;
                    *(s8 *)((unsigned char *)temp_v0_2 + (21)) = 0x20;
                    *(s8 *)((unsigned char *)temp_v0_2 + (28)) = 0x80;
                } else if (sp70 < 0x60) {
                    *(s8 *)((unsigned char *)temp_v0_2 + (20)) = 0xBE;
                    *(s8 *)((unsigned char *)temp_v0_2 + (36)) = 0xBE;
                    *(s8 *)((unsigned char *)temp_v0_2 + (12)) = 0xA0;
                    *(s8 *)((unsigned char *)temp_v0_2 + (28)) = 0xA0;
                    *(s8 *)((unsigned char *)temp_v0_2 + (29)) = 0x3E;
                    *(s8 *)((unsigned char *)temp_v0_2 + (37)) = 0x3E;
                    *(s8 *)((unsigned char *)temp_v0_2 + (13)) = 0x20;
                    *(s8 *)((unsigned char *)temp_v0_2 + (21)) = 0x20;
                } else if (sp70 < 0xA0) {
                    *(s8 *)((unsigned char *)temp_v0_2 + (20)) = 0xDE;
                    *(s8 *)((unsigned char *)temp_v0_2 + (36)) = 0xDE;
                    *(s8 *)((unsigned char *)temp_v0_2 + (12)) = 0xC0;
                    *(s8 *)((unsigned char *)temp_v0_2 + (28)) = 0xC0;
                    *(s8 *)((unsigned char *)temp_v0_2 + (29)) = 0x3E;
                    *(s8 *)((unsigned char *)temp_v0_2 + (37)) = 0x3E;
                    *(s8 *)((unsigned char *)temp_v0_2 + (13)) = 0x20;
                    *(s8 *)((unsigned char *)temp_v0_2 + (21)) = 0x20;
                } else {
                    *(s8 *)((unsigned char *)temp_v0_2 + (20)) = 0xFE;
                    *(s8 *)((unsigned char *)temp_v0_2 + (36)) = 0xFE;
                    *(s8 *)((unsigned char *)temp_v0_2 + (12)) = 0xE0;
                    *(s8 *)((unsigned char *)temp_v0_2 + (28)) = 0xE0;
                    *(s8 *)((unsigned char *)temp_v0_2 + (13)) = 0x20;
                    *(s8 *)((unsigned char *)temp_v0_2 + (21)) = 0x20;
                    *(s8 *)((unsigned char *)temp_v0_2 + (29)) = 0x3E;
                    *(s8 *)((unsigned char *)temp_v0_2 + (37)) = 0x3E;
                }
                *(s16 *)((unsigned char *)temp_v0_2 + (8)) = sp90;
                *(s16 *)((unsigned char *)temp_v0_2 + (10)) = sp92;
                *(s16 *)((unsigned char *)temp_v0_2 + (16)) = sp94;
                *(s16 *)((unsigned char *)temp_v0_2 + (18)) = sp96;
                *(s16 *)((unsigned char *)temp_v0_2 + (24)) = sp98;
                *(s16 *)((unsigned char *)temp_v0_2 + (26)) = sp9A;
                *(s16 *)((unsigned char *)temp_v0_2 + (32)) = sp9C;
                *(s16 *)((unsigned char *)temp_v0_2 + (34)) = sp9E;
                AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + (sp70 * 4)], temp_v0_2);
            }
        }
    }
}
