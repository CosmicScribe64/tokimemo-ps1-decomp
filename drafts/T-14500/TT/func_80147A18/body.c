void func_80147A18(void *arg0, void *arg1, s32 arg2) {
    s32 sp50;
    s16 var_a3;
    s16 var_t0;
    s32 temp_a3;
    s32 temp_v1_2;
    s32 var_t1;
    u16 temp_a0_2;
    u16 temp_s5;
    u16 var_s3;
    u8 *temp_s6;
    u8 *temp_v0_2;
    u8 temp_v0_3;
    u8 temp_v0_4;
    u8 temp_v1;
    u8 *temp_a0;
    u8 *temp_a1;
    u8 *temp_s4;
    void *temp_s7;
    u8 *temp_v0;
    u8 *var_s0;

    temp_a3 = (*(s32 *)arg1);
    temp_s6 = D_80158AB0;
    temp_s4 = (*(s32 *)((unsigned char *)D_80158A68 + (8)) * 0x80B8) + D_80158AB4;
    if (temp_a3 != 0) {
        temp_v0 = *(s32 *)(temp_a3 + (*(u8 *)((unsigned char *)arg1 + (5)) * 4));
        temp_a0 = *(s32 *)((unsigned char *)temp_v0 + (4)) + (*(s32 *)((*(s32 *)temp_v0) + (*(u8 *)((unsigned char *)arg1 + (6)) * 4)) * 4);
        var_t1 = *(s32 *)((unsigned char *)temp_v0 + (12));
        temp_s5 = *(u16 *)((unsigned char *)temp_v0 + (16));
        *(s16 *)((unsigned char *)temp_s6 + (4)) = (s16) (*(s32 *)arg0);
        *(s16 *)((unsigned char *)temp_s6 + (6)) = (s16) *(s16 *)((unsigned char *)arg0 + (2));
        var_s0 = (*(s32 *)((unsigned char *)temp_v0 + (8)) + (*(u16 *)((unsigned char *)temp_a0 + (2)) * 8) + ((*(s32 *)temp_a0) * 8)) - 8;
        if (*(u8 *)((unsigned char *)arg1 + (16)) != 0) {
            (*(s32 *)temp_s6) = (s16) *(s16 *)((unsigned char *)arg1 + (12));
            *(s16 *)((unsigned char *)temp_s6 + (2)) = (s16) *(s16 *)((unsigned char *)arg1 + (14));
        }
        var_s3 = (*(s32 *)temp_a0);
        if ((s32) var_s3 > 0) {
            temp_s7 = temp_s4 + ((arg2 & 0xFFFF) * 4) + 0x8008;
            do {
                temp_a1 = *(void **)((unsigned char *)temp_s4 + (4));
                *(void **)((unsigned char *)temp_s4 + (4)) = (void *) (temp_a1 + 0x30);
                *(s8 *)((unsigned char *)temp_a1 + (40)) = (s8) (*(u8 *)((unsigned char *)var_s0 + (7)) ^ *(u8 *)((unsigned char *)arg1 + (4)));
                temp_v1 = *(u8 *)((unsigned char *)arg1 + (4));
                if (temp_v1 & 1) {
                    var_t0 = (*(s16 *)((unsigned char *)arg0 + (2)) - *(s16 *)((unsigned char *)var_s0 + (4))) - 0x10;
                } else {
                    var_t0 = *(s16 *)((unsigned char *)arg0 + (2)) + *(s16 *)((unsigned char *)var_s0 + (4));
                }
                if (temp_v1 & 2) {
                    var_a3 = ((*(s32 *)arg0) - *(s16 *)((unsigned char *)var_s0 + (2))) - 0x10;
                } else {
                    var_a3 = (*(s32 *)arg0) + *(s16 *)((unsigned char *)var_s0 + (2));
                }
                *(s16 *)((unsigned char *)temp_a1 + (8)) = var_a3;
                *(s16 *)((unsigned char *)temp_a1 + (10)) = var_t0;
                temp_a0_2 = (*(s32 *)var_s0);
                temp_v0_2 = var_t1 + (temp_a0_2 >> 6);
                temp_v1_2 = temp_a0_2 & 0x3F;
                *(s8 *)((unsigned char *)temp_a1 + (12)) = (s8) ((*temp_v0_2 & 0x80) + ((temp_v1_2 & 7) * 0x10));
                *(s8 *)((unsigned char *)temp_a1 + (13)) = (s8) (((*temp_v0_2 & 0x40) * 2) + ((temp_v1_2 >> 3) * 0x10));
                *(s8 *)((unsigned char *)temp_a1 + (41)) = 0;
                *(s8 *)((unsigned char *)temp_a1 + (42)) = 1;
                *(s16 *)((unsigned char *)temp_a1 + (22)) = (s16) (*temp_v0_2 & 0x1F);
                if (*(u8 *)((unsigned char *)arg1 + (10)) != 0) {
                    temp_v0_3 = *(u8 *)((unsigned char *)arg1 + (11));
                    *(s16 *)((unsigned char *)temp_a1 + (14)) = (s16) (temp_s5 + (temp_v0_3 & 0xF) + ((temp_v0_3 & 0xF0) * 4));
                } else {
                    temp_v0_4 = *(u8 *)((unsigned char *)var_s0 + (6));
                    *(s16 *)((unsigned char *)temp_a1 + (14)) = (s16) (temp_s5 + (temp_v0_4 & 0xF) + ((temp_v0_4 & 0xF0) * 4));
                }
                *(s8 *)((unsigned char *)temp_a1 + (44)) = 0;
                *(u8 *)((unsigned char *)temp_a1 + (43)) = (u8) *(u8 *)((unsigned char *)arg1 + (16));
                sp50 = var_t1;
                func_80141C6C((s32) temp_s7, (s32) temp_a1, (s32) temp_s6);
                var_s3 -= 1;
                var_s0 -= 8;
            } while ((s32) var_s3 > 0);
        }
    }
}
