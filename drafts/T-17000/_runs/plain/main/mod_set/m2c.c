void mod_set(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u16 arg6) {
    u32 spD4;
    s32 spD0;
    s16 spCE;
    s16 spCC;
    s16 spCA;
    s16 spC8;
    s8 spC6;
    s8 spC5;
    s8 spC4;
    s16 spC2;
    s16 spC0;
    s8 spBF;
    s8 spBE;
    s16 spBC;
    s16 spBA;
    s16 spB8;
    s16 spB6;
    s16 spB4;
    s32 spB0;
    s32 sp70;
    s32 *var_s5;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 var_a1;
    s32 var_s1;
    u32 temp_t9;
    u32 var_a2;
    u32 var_s2;
    u8 temp_a0;
    u8 temp_s4;
    u8 temp_t8;
    void *temp_fp;

    temp_v0 = func_8004901C();
    temp_fp = (arg0 * 0x14) + D_801217A0;
    temp_t8 = *(u8 *)((unsigned char *)temp_fp + (8));
    var_a1 = 1;
    switch (temp_t8) {
    case 0:
        spB0 = 0x01000000;
        var_a1 = 1;
        break;
    case 1:
        sp70 = 2;
        spB0 = 0;
        var_a1 = 2;
        break;
    case 2:
        sp70 = 2;
        spB0 = 0;
        var_a1 = 2;
        break;
    case 3:
        sp70 = 1;
        spB0 = 0x01000000;
        var_a1 = 1;
        break;
    case 4:
        sp70 = 1;
        spB0 = 0x01000000;
        var_a1 = 1;
        break;
    case 5:
        sp70 = 1;
        spB0 = 0x01000000;
        var_a1 = 1;
        break;
    default:
        spB0 = 0x01000000;
        sp70 = 1;
        break;
    }
    temp_a0 = *(u8 *)((unsigned char *)temp_fp + (13));
    temp_s4 = *(u8 *)((unsigned char *)temp_fp + (14));
    spC8 = 0;
    spCA = 0;
    spC4 = (s8) temp_v0;
    spC5 = (s8) temp_v0;
    spC6 = (s8) temp_v0;
    spD0 = 0;
    spCC = 0x1000;
    spCE = 0x1000;
    var_a2 = 0;
    spB8 = (s16) temp_s4;
    spBA = (s16) *(u8 *)((unsigned char *)temp_fp + (15));
    var_s5 = (temp_a0 * arg1 * *(u8 *)((unsigned char *)temp_fp + (12)) * 4) + *(s32 *)((unsigned char *)temp_fp + (4));
    if (temp_a0 != 0) {
        do {
            var_s2 = 0;
            if (*(u8 *)((unsigned char *)temp_fp + (12)) != 0) {
                var_s1 = arg2;
                sp70 = var_a1;
                spD4 = var_a2;
                do {
                    spB4 = (s16) var_s1;
                    spB6 = (var_a2 * temp_s4) + arg3;
                    temp_v0_2 = *var_s5;
                    temp_t9 = (u32) (temp_v0_2 & 0xFF0000) >> 0x10;
                    spBC = search_tpage_multi(*(temp_fp->unk0 + (((u32) (temp_v0_2 & 0xFF000000) >> 0x18) * 4)), sp70);
                    if (temp_s4 != 8) {
                        if (temp_s4 != 0x10) {
                            spBF = ((u32) (temp_t9 & 0xF0) >> 4) * 8;
                            spBE = (temp_t9 & 0xF) * 8;
                        } else {
                            spBE = (temp_t9 & 7) * 0x10;
                            spBF = ((u32) (temp_t9 & 0xF8) >> 4) * 0x10;
                        }
                    } else {
                        spBE = (temp_t9 & 0xF) * 8;
                        spBF = ((u32) (temp_t9 & 0xF0) >> 4) * 8;
                    }
                    spC0 = arg4 + (((u32) (temp_v0_2 & 0xFF00) >> 8) * 0x10);
                    spC2 = (s16) arg5;
                    func_80098B80((s32) &spB0, (s32) ((D_8011ECA0 * 0x14) + &D_800E8C70), arg6 & 0xFFFF);
                    var_s2 += 1;
                    var_s1 += temp_s4;
                    var_s5 += 4;
                } while (var_s2 < (u8) *(u8 *)((unsigned char *)temp_fp + (12)));
                var_a2 = spD4;
                var_a1 = sp70;
            }
            var_a2 += 1;
        } while (var_a2 < *(u8 *)((unsigned char *)temp_fp + (13)));
    }
}
