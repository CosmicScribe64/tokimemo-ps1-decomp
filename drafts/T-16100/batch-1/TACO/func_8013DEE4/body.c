void func_8013DEE4(s32 arg0) {
    s16 screen_xy[8];
    SVECTOR stack6C;
    s32 spB0[8];
    s16 spAC;
    s16 spAA;
    s16 spA8;
    s32 sp90;
    s32 sp8C[1];
    s32 sp88[1];
    u32 sp80;
    u32 sp64;
    s32 sp60;
    s32 sp54;
    Tc14 *temp_v0;
    s16 temp_v0_2;
    s32 temp_fp;
    s32 temp_t3;
    s32 temp_v0_3;
    s32 var_s0;
    s32 var_s1;
    s32 var_s2_2;
    s32 var_s7;
    s32 var_v0_2;
    s32 var_v0_3;
    s8 var_v0;
    s8 var_v0_4;
    u8 *var_s2;

    temp_v0 = &D_8015EB30[arg0];
    stack6C.vx = (s32) temp_v0->unk0;
    *(s32 *)((unsigned char *)(&stack6C.vx) + (4)) = (s32) *(s32 *)((unsigned char *)temp_v0 + (4));
    sp80 = temp_v0->padE[3] & 0xF;
    (*(s32 *)&spB0) = (s32) D_80129C40;
    *(s32 *)((unsigned char *)(&spB0) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (4));
    *(s32 *)((unsigned char *)(&spB0) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (8));
    *(s32 *)((unsigned char *)(&spB0) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (12));
    *(s32 *)((unsigned char *)(&spB0) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (16));
    *(s32 *)((unsigned char *)(&spB0) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (20));
    *(s32 *)((unsigned char *)(&spB0) + (24)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (24));
    *(s32 *)((unsigned char *)(&spB0) + (28)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (28));
    stack6C.vz -= 0x200;
    func_800A1100((s32) &spB0);
    func_800A1070(&spB0);
    switch (sp80) {
    case 3:
    case 5:
        var_s7 = 0x100;
        break;
    case 0:
        var_s7 = 0x40;
        break;
    default:
    case 1:
    case 2:
    case 4:
        var_s7 = 0x80;
        break;
    }
    sp60 = 0;
    sp90 = 0;
    sp54 = 0;
    sp64 = sp80;
    spAC = stack6C.vz;
    temp_fp = var_s7 / 2;
    do {
        spAA = (stack6C.vy + sp60) - temp_fp;
        var_s0 = 0;
        var_s1 = 0;
        var_s2 = sp54 + &screen_xy[0];
loop_7:
        spA8 = (stack6C.vx + var_s1) - temp_fp;
        func_800AB170((s32) &spA8, (s32) var_s2, (s32) &sp8C, (s32) &sp88);
        var_s0 += 1;
        var_s1 += var_s7;
        var_s2 += 8;
        if (var_s0 != 2) {
            goto loop_7;
        }
        temp_t3 = sp90 + 1;
        sp54 += 4;
        sp60 += var_s7;
        sp90 = temp_t3;
    } while (temp_t3 != 2);
    if ((screen_xy[4] >= -0x2C0) && (screen_xy[0] < 0x2C1)) {
        temp_v0_2 = *(&D_800E6298 + (D_8011ECA0 * 2));
        if (screen_xy[3] >= (temp_v0_2 - 0x200)) {
            if ((temp_v0_2 + 0x200) >= screen_xy[7]) {
                var_s2_2 = (s32) (D_80122754 - stack6C.vz) / 127;
                if (var_s2_2 >= 0x100) {
                    var_s2_2 = 0xFF;
                    goto block_17;
                }
                if (var_s2_2 <= 0) {

                } else {
block_17:
                    temp_v0_3 = GetWorkBase(0x28, D_8011ECA0);
                    func_8009F040(temp_v0_3);
                    if (sp80 != 5) {
                        var_v0 = 0xFF;
                        if (sp80 != 6) {
                            var_v0 = 0x80;
                        }
                    } else {
                        var_v0 = 0xFF;
                    }
                    *(s8 *)((unsigned char *)temp_v0_3 + (4)) = var_v0;
                    *(s8 *)((unsigned char *)temp_v0_3 + (5)) = var_v0;
                    *(s8 *)((unsigned char *)temp_v0_3 + (6)) = var_v0;
                    switch (sp64) {
                    case 3:
                    case 5:
                    case 6:
                        SetSemiTrans(temp_v0_3, 1);
                        break;
                    default:
                    case 0:
                    case 1:
                    case 2:
                    case 4:
                        SetSemiTrans(temp_v0_3, 0);
                        break;
                    }
                    if (sp80 != 4) {
                        var_v0_2 = func_8009ECB0(1, 1, 0x3C0, 0x100);
                    } else {
                        var_v0_2 = func_8009ECB0(0, 1, 0x380, 0x100);
                    }
                    *(s16 *)((unsigned char *)temp_v0_3 + (22)) = (s16) var_v0_2;
                    if (sp80 != 4) {
                        if (sp80 == 5) {
                            var_v0_3 = func_8009ED74(0, 0x1EA);
                        } else {
                            var_v0_3 = func_8009ED74(0x100, 0x1F8);
                        }
                    } else {
                        var_v0_3 = func_8009ED74(0, 0x1E5);
                    }
                    *(s16 *)((unsigned char *)temp_v0_3 + (14)) = (s16) var_v0_3;
                    switch (sp64) {
                    default:
                    case 0:
                    case 1:
                        var_v0_4 = 1;
                        *(s8 *)((unsigned char *)temp_v0_3 + (29)) = 0x5F;
                        *(s8 *)((unsigned char *)temp_v0_3 + (37)) = 0x5F;
                        *(s8 *)((unsigned char *)temp_v0_3 + (13)) = 0x41;
                        *(s8 *)((unsigned char *)temp_v0_3 + (21)) = 0x41;
                        *(s8 *)((unsigned char *)temp_v0_3 + (20)) = 0x1F;
                        *(s8 *)((unsigned char *)temp_v0_3 + (36)) = 0x1F;
                        *(s8 *)((unsigned char *)temp_v0_3 + (12)) = 1;
block_39:
                        *(s8 *)((unsigned char *)temp_v0_3 + (28)) = var_v0_4;
                        break;
                    case 2:
                        *(s8 *)((unsigned char *)temp_v0_3 + (20)) = 0x7F;
                        *(s8 *)((unsigned char *)temp_v0_3 + (36)) = 0x7F;
                        *(s8 *)((unsigned char *)temp_v0_3 + (12)) = 0x41;
                        *(s8 *)((unsigned char *)temp_v0_3 + (28)) = 0x41;
                        *(s8 *)((unsigned char *)temp_v0_3 + (29)) = 0x3F;
                        *(s8 *)((unsigned char *)temp_v0_3 + (37)) = 0x3F;
                        *(s8 *)((unsigned char *)temp_v0_3 + (13)) = 1;
                        *(s8 *)((unsigned char *)temp_v0_3 + (21)) = 1;
                        break;
                    case 3:
                        var_v0_4 = 1;
                        *(s8 *)((unsigned char *)temp_v0_3 + (20)) = 0x3F;
                        *(s8 *)((unsigned char *)temp_v0_3 + (29)) = 0x3F;
                        *(s8 *)((unsigned char *)temp_v0_3 + (36)) = 0x3F;
                        *(s8 *)((unsigned char *)temp_v0_3 + (37)) = 0x3F;
                        *(s8 *)((unsigned char *)temp_v0_3 + (12)) = 1;
                        *(s8 *)((unsigned char *)temp_v0_3 + (13)) = 1;
                        *(s8 *)((unsigned char *)temp_v0_3 + (21)) = 1;
                        goto block_39;
                    case 4:
                        var_v0_4 = 0x40;
                        *(s8 *)((unsigned char *)temp_v0_3 + (20)) = 0x7F;
                        *(s8 *)((unsigned char *)temp_v0_3 + (36)) = 0x7F;
                        *(s8 *)((unsigned char *)temp_v0_3 + (29)) = 0xBF;
                        *(s8 *)((unsigned char *)temp_v0_3 + (37)) = 0xBF;
                        *(s8 *)((unsigned char *)temp_v0_3 + (13)) = 0x80;
                        *(s8 *)((unsigned char *)temp_v0_3 + (21)) = 0x80;
                        *(s8 *)((unsigned char *)temp_v0_3 + (12)) = 0x40;
                        goto block_39;
                    }
                    *(s16 *)((unsigned char *)temp_v0_3 + (8)) = screen_xy[4];
                    *(s16 *)((unsigned char *)temp_v0_3 + (10)) = screen_xy[5];
                    *(s16 *)((unsigned char *)temp_v0_3 + (16)) = screen_xy[0];
                    *(s16 *)((unsigned char *)temp_v0_3 + (18)) = screen_xy[1];
                    *(s16 *)((unsigned char *)temp_v0_3 + (24)) = screen_xy[6];
                    *(s16 *)((unsigned char *)temp_v0_3 + (26)) = screen_xy[7];
                    *(s16 *)((unsigned char *)temp_v0_3 + (32)) = screen_xy[2];
                    *(s16 *)((unsigned char *)temp_v0_3 + (34)) = screen_xy[3];
                    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + (var_s2_2 * 4)], temp_v0_3);
                }
            }
        }
    }
}
