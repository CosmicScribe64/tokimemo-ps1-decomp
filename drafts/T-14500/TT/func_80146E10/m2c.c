void func_80146E10(void *arg0, s32 arg1) {
    s16 temp_a2;
    s16 temp_v1;
    s16 temp_v1_2;
    s32 temp_a3;
    s32 temp_v1_4;
    s32 temp_v1_5;
    s32 temp_v1_6;
    s32 temp_v1_7;
    s32 var_a1;
    s32 var_a1_2;
    s32 var_a1_3;
    s32 var_a1_4;
    s32 var_a1_5;
    s32 var_a1_6;
    s32 var_a2;
    u16 temp_v1_3;
    u8 *temp_a0_2;
    u8 *var_a0;
    void *temp_a0;
    void *temp_v0;

    *(s32 *)((unsigned char *)arg0 + (32)) = (s32) (*(s32 *)((unsigned char *)arg0 + (32)) + *(s32 *)((unsigned char *)arg0 + (40)));
    temp_v0 = (arg1 * 0x58) + D_80158A88;
    *(s32 *)((unsigned char *)arg0 + (36)) = (s32) (*(s32 *)((unsigned char *)arg0 + (36)) + *(s32 *)((unsigned char *)arg0 + (44)));
    *(s16 *)((unsigned char *)arg0 + (20)) = (s16) *(s16 *)((unsigned char *)arg0 + (34));
    *(s16 *)((unsigned char *)arg0 + (22)) = (s16) *(s16 *)((unsigned char *)arg0 + (38));
    temp_a2 = *(s16 *)((unsigned char *)temp_v0 + (20));
    temp_v1 = *(s16 *)((unsigned char *)arg0 + (20));
    temp_a3 = temp_v1 - temp_a2;
    var_a1 = temp_a3;
    if (temp_a3 < 0) {
        var_a1 = -temp_a3;
    }
    if (var_a1 < 2) {
        *(s32 *)((unsigned char *)arg0 + (40)) = 0;
    } else {
        *(s32 *)((unsigned char *)arg0 + (40)) = (s32) ((temp_a2 - temp_v1) << 0xC);
        var_a1_2 = 0xFFFE0000;
        if (temp_v1 < *(s16 *)((unsigned char *)temp_v0 + (20))) {
            var_a1_2 = 0x20000;
        }
        *(s32 *)((unsigned char *)arg0 + (40)) = (s32) (*(s32 *)((unsigned char *)arg0 + (40)) + var_a1_2);
    }
    temp_v1_2 = *(s16 *)((unsigned char *)arg0 + (22));
    if ((temp_v1_2 < *(s16 *)((unsigned char *)temp_v0 + (22))) || (temp_v1_2 < 0)) {
        arg0->unk0 = 0;
        temp_v1_3 = *(u16 *)((unsigned char *)temp_v0 + (68));
        temp_v0->unk0 = 0;
        if (temp_v1_3 != 0xFFFF) {
            temp_a0 = (temp_v1_3 * 0x78) + D_80158A9C;
            if (*(u16 *)((unsigned char *)temp_a0 + (4)) == 0x40) {
                *(u16 *)((unsigned char *)temp_a0 + (4)) = 0x70U;
                *(s16 *)((unsigned char *)temp_a0 + (10)) = 0;
                *(s16 *)((unsigned char *)temp_a0 + (8)) = 0;
                *(s16 *)((unsigned char *)temp_a0 + (6)) = 0;
                *(s8 *)((unsigned char *)temp_a0 + (15)) = 0;
                *(s8 *)((unsigned char *)temp_a0 + (14)) = 0;
                *(s8 *)((unsigned char *)temp_a0 + (13)) = 0;
            }
        } else {
            var_a0 = D_80158A9C;
            var_a2 = 0;
            do {
                var_a2 += 2;
                if (*(u16 *)((unsigned char *)var_a0 + (4)) == 0x40) {
                    temp_v1_4 = *(s16 *)((unsigned char *)temp_v0 + (20)) - *(s16 *)((unsigned char *)var_a0 + (20));
                    var_a1_3 = temp_v1_4;
                    if (temp_v1_4 < 0) {
                        var_a1_3 = -temp_v1_4;
                    }
                    if (var_a1_3 < 8) {
                        temp_v1_5 = *(s16 *)((unsigned char *)temp_v0 + (22)) - *(s16 *)((unsigned char *)var_a0 + (22));
                        var_a1_4 = temp_v1_5;
                        if (temp_v1_5 < 0) {
                            var_a1_4 = -temp_v1_5;
                        }
                        if (var_a1_4 < 8) {
                            *(u16 *)((unsigned char *)var_a0 + (4)) = 0x70U;
                            *(s16 *)((unsigned char *)var_a0 + (10)) = 0;
                            *(s16 *)((unsigned char *)var_a0 + (8)) = 0;
                            *(s16 *)((unsigned char *)var_a0 + (6)) = 0;
                            *(s8 *)((unsigned char *)var_a0 + (15)) = 0;
                            *(s8 *)((unsigned char *)var_a0 + (14)) = 0;
                            *(s8 *)((unsigned char *)var_a0 + (13)) = 0;
                        }
                    }
                }
                temp_a0_2 = var_a0 + 0x78;
                if (*(u16 *)((unsigned char *)var_a0 + (124)) == 0x40) {
                    temp_v1_6 = *(s16 *)((unsigned char *)temp_v0 + (20)) - *(s16 *)((unsigned char *)temp_a0_2 + (20));
                    var_a1_5 = temp_v1_6;
                    if (temp_v1_6 < 0) {
                        var_a1_5 = -temp_v1_6;
                    }
                    if (var_a1_5 < 8) {
                        temp_v1_7 = *(s16 *)((unsigned char *)temp_v0 + (22)) - *(s16 *)((unsigned char *)temp_a0_2 + (22));
                        var_a1_6 = temp_v1_7;
                        if (temp_v1_7 < 0) {
                            var_a1_6 = -temp_v1_7;
                        }
                        if (var_a1_6 < 8) {
                            *(s16 *)((unsigned char *)temp_a0_2 + (4)) = 0x70;
                            *(s16 *)((unsigned char *)temp_a0_2 + (10)) = 0;
                            *(s16 *)((unsigned char *)temp_a0_2 + (8)) = 0;
                            *(s16 *)((unsigned char *)temp_a0_2 + (6)) = 0;
                            *(s8 *)((unsigned char *)temp_a0_2 + (15)) = 0;
                            *(s8 *)((unsigned char *)temp_a0_2 + (14)) = 0;
                            *(s8 *)((unsigned char *)temp_a0_2 + (13)) = 0;
                        }
                    }
                }
                var_a0 = temp_a0_2 + 0x78;
            } while (var_a2 != 0x20);
        }
    }
}
