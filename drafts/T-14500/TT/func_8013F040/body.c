void func_8013F040(void) {
    u8 *sp9C;
    u8 *sp98;
    s16 sp72;
    s16 sp70;
    s8 sp6D;
    s8 sp6C;
    s16 sp6A;
    s16 sp68;
    u8 *sp58;
    s32 var_fp;
    s32 var_fp_2;
    s32 var_fp_3;
    s32 var_s1;
    u8 *var_s0;
    u8 *var_v0;
    u8 *var_v0_2;
    u8 *temp_v0;

    sp9C = D_80158A8C;
    sp98 = D_80158A74;
    var_fp = 0;
    var_v0 = D_80158A8C;
    if (*D_80158A8C != 0) {
        do {
            var_fp += 1;
            var_v0 += 0x14;
            *(s8 *)((unsigned char *)var_v0 + (46)) = 0;
        } while (var_fp < 0x18);
        if ((*(u8 *)((unsigned char *)sp9C + (834)) == 0) && (func_801427A8() != 0)) {
            func_80133AD0(0x503);
        }
        var_fp_2 = 0x10;
        var_v0_2 = sp9C + 0x10;
        do {
            if (*(u8 *)((unsigned char *)var_v0_2 + (784)) != 0) {
                *(s8 *)((unsigned char *)(sp9C + (var_fp_2 * 0x14)) + (66)) = 0;
            }
            var_fp_2 += 1;
            var_v0_2 += 1;
        } while (var_fp_2 < 0x13);
        if (*(u8 *)((unsigned char *)sp9C + (835)) == 0) {
            var_fp_3 = 0x10;
            sp58 = sp9C + 0x10;
            do {
                temp_v0 = sp9C + (var_fp_3 * 2);
                if (*(u8 *)((unsigned char *)sp58 + (784)) != 0) {
                    sp70 = *(s16 *)((unsigned char *)temp_v0 + (584)) + *(s16 *)((unsigned char *)sp9C + (20));
                    sp72 = *(s16 *)((unsigned char *)temp_v0 + (632)) + *(s16 *)((unsigned char *)sp9C + (22));
                    if ((*(u8 *)((unsigned char *)sp98 + (82)) == 0) && ((*(s32 *)sp98) != 0) && (func_80145508((s32) (sp98 + 0x14), (s32) D_80155B30, (s32) &sp70, (s32) &D_80155D44) != 0)) {
                        func_80144094();
                        *(s16 *)((unsigned char *)sp98 + (4)) = 0x60;
                        *(s16 *)((unsigned char *)sp98 + (10)) = 0;
                        *(s16 *)((unsigned char *)sp98 + (8)) = 0;
                        *(s16 *)((unsigned char *)sp98 + (6)) = 0;
                        *(s8 *)((unsigned char *)sp98 + (15)) = 0;
                        *(s8 *)((unsigned char *)sp98 + (14)) = 0;
                        *(s8 *)((unsigned char *)sp98 + (13)) = 0;
                    }
                    var_s0 = D_80158A80;
                    var_s1 = 0;
                    do {
                        if (((*(s32 *)var_s0) != 0) && (*(u16 *)((unsigned char *)var_s0 + (4)) == 0x40) && (func_80145508((s32) (var_s0 + 0x14), (s32) &D_80155B36, (s32) &sp70, (s32) &D_80155D44) != 0)) {
                            *(u16 *)((unsigned char *)var_s0 + (4)) = 0x80U;
                            *(s16 *)((unsigned char *)var_s0 + (10)) = 0;
                            *(s16 *)((unsigned char *)var_s0 + (8)) = 0;
                            *(s16 *)((unsigned char *)var_s0 + (6)) = 0;
                            *(s8 *)((unsigned char *)var_s0 + (15)) = 0;
                            *(s8 *)((unsigned char *)var_s0 + (14)) = 0;
                            *(s8 *)((unsigned char *)var_s0 + (13)) = 0;
                        }
                        var_s1 += 1;
                        var_s0 += 0x54;
                    } while (var_s1 != 0xC);
                }
                var_fp_3 += 1;
                sp58 += 1;
            } while (var_fp_3 != 0x13);
        }
        if (*(u8 *)((unsigned char *)sp9C + (836)) == 0) {
            *(s8 *)((unsigned char *)sp9C + (526)) = 0;
            if ((*(s32 *)sp98) != 0) {
                sp70 = *(s16 *)((unsigned char *)sp9C + (630)) + *(s16 *)((unsigned char *)sp9C + (20));
                sp68 = 0;
                sp72 = *(s16 *)((unsigned char *)sp9C + (678)) + *(s16 *)((unsigned char *)sp9C + (22));
                sp6A = 0;
                sp6C = (s8) ((s32) (*(s16 *)((unsigned char *)sp9C + (528)) * 0x30) / 4096);
                sp6D = (s8) ((s32) (*(s16 *)((unsigned char *)sp9C + (530)) * 0x30) / 4096);
                if (*(u8 *)((unsigned char *)sp98 + (82)) == 0) {
                    if ((*(s32 *)sp98) != 0) {
                        if (func_80145508((s32) (sp98 + 0x14), (s32) D_80155B30, (s32) &sp70, (s32) &sp68) != 0) {
                            func_80144094();
                            *(s16 *)((unsigned char *)sp98 + (4)) = 0x60;
                            *(s16 *)((unsigned char *)sp98 + (10)) = 0;
                            *(s16 *)((unsigned char *)sp98 + (8)) = 0;
                            *(s16 *)((unsigned char *)sp98 + (6)) = 0;
                            *(s8 *)((unsigned char *)sp98 + (15)) = 0;
                            *(s8 *)((unsigned char *)sp98 + (14)) = 0;
                            *(s8 *)((unsigned char *)sp98 + (13)) = 0;
                        }
                    }
                }
            }
        }
    }
}
