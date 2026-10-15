void srn_vram_set(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 sp94;
    s32 sp84;
    s32 sp80;
    s32 sp74;
    s32 sp70;
    s32 sp58;
    s32 *var_s2;
    s32 temp_lo;
    s32 temp_t9;
    s32 var_s0;
    s32 var_s1;
    s32 var_s3;
    s32 var_t1;
    s32 var_t8;
    s32 var_v1;
    u8 temp_a1;
    u8 temp_fp;
    u8 temp_s4;
    u8 temp_s5;
    u8 temp_t8;
    u8 var_t0;
    void *temp_v0;

    func_8009C674(0);
    temp_v0 = (arg0 * 0x28) + D_80121750;
    temp_t8 = *(u8 *)((unsigned char *)temp_v0 + (14));
    temp_fp = *(u8 *)((unsigned char *)temp_v0 + (15));
    temp_s4 = *(u8 *)((unsigned char *)temp_v0 + (16));
    temp_s5 = *(u8 *)((unsigned char *)temp_v0 + (17));
    var_t0 = *(u8 *)((unsigned char *)temp_v0 + (19));
    temp_a1 = *(u8 *)((unsigned char *)temp_v0 + (21));
    var_s2 = *(s32 **)((unsigned char *)temp_v0 + (8));
    sp94 = (s32) temp_t8;
    sp84 = (s32) *(u8 *)((unsigned char *)temp_v0 + (18));
    if ((temp_fp == 0) || (temp_t8 == 0)) {
        printf("ERROR", temp_a1);
        return;
    }
    var_s3 = *(s32 *)((unsigned char *)temp_v0 + (28));
    var_s1 = 0;
    temp_lo = (s32) (*(u8 *)((unsigned char *)temp_v0 + (20)) * temp_s4) / sp94;
    var_t1 = temp_lo;
    var_v1 = (s32) (temp_a1 * temp_s5) / (s32) temp_fp;
    sp70 = var_v1;
    if (var_v1 > 0) {
        do {
            var_s0 = 0;
            if (temp_lo > 0) {
                var_t8 = -1;
                do {
                    if ((((var_t8 * temp_s4) + (sp84 * sp94)) < arg2) && ((((var_s1 - 1) * temp_s5) + (var_t0 * temp_fp)) < arg3)) {
                        temp_t9 = *var_s2 & 0xFFFF;
                        if (temp_t9 != 0) {
                            sp58 = var_v1;
                            sp80 = (s32) var_t0;
                            sp74 = var_t1;
                            mod_trans_vram(0, temp_t9, arg1, temp_s4 * var_s0, temp_s5 * var_s1);
                        }
                    }
                    var_s3 -= 1;
                    var_s2 += 4;
                    if (var_s3 == 0) {
                        var_s1 = var_v1;
                        var_s0 = temp_lo;
                    }
                    var_s0 += 1;
                    var_t8 = var_s0 - 1;
                } while (var_s0 < var_t1);
            }
            var_s1 += 1;
        } while (var_s1 < sp70);
    }
    if ((u8) D_800B3DB0 != 0) {
        move_255_line(arg1, arg1 + 0xB, 0xF);
    }
}
