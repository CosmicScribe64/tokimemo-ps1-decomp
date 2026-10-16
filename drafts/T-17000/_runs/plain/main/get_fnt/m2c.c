void get_fnt(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5) {
    ? sp8C;
    ? *var_s2;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_lo_3;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s1;
    s32 var_s1_2;
    s32 var_s3;
    u8 temp_t5;
    u8 temp_t7;
    void *var_a0;
    void *var_a1;
    void *var_v0;
    void *var_v1;

    var_s0 = 0;
    if (arg4 > 0) {
        temp_lo = 0x80 / arg4;
        var_s1 = 0;
        do {
            memcpy((var_s1 / arg3) + arg0, ((s32) ((arg2 % temp_lo) * arg4) / arg3) + arg1 + ((s32) ((((arg2 / temp_lo) * arg4) + var_s0) << 7) / arg3), arg4 / arg3);
            var_s0 += 1;
            var_s1 += arg4;
        } while (var_s0 != arg4);
    }
    if (arg5 & 1) {
        memcpy((s32) &sp8C, arg0, 0x100);
        var_s1_2 = arg4 - 1;
        if (arg4 > 0) {
            var_s2 = &sp8C;
            var_s3 = 1;
            do {
                memcpy((var_s1_2 * arg4) + arg0, (s32) var_s2, arg4);
                var_s1_2 -= 1;
                var_s2 += arg4;
                var_s3 += 1;
            } while (var_s3 != arg4);
        }
    }
    if (arg5 & 2) {
        var_s0_2 = 0;
        memcpy((s32) &sp8C, arg0, 0x100);
        if (arg4 > 0) {
            if (!(arg4 & 1) || (var_s0_2 = 1, *(u8 *)((unsigned char *)arg0 + (4)) = (u8) *(u8 *)((unsigned char *)(&sp8C) + (3)), *(u8 *)((unsigned char *)arg0 + (5)) = (u8) *(u8 *)((unsigned char *)(&sp8C) + (2)), *(u8 *)((unsigned char *)arg0 + (6)) = (u8) *(u8 *)((unsigned char *)(&sp8C) + (1)), *(u8 *)((unsigned char *)arg0 + (7)) = (u8) sp8C.unk0, arg0->unk0 = (u8) *(u8 *)((unsigned char *)(&sp8C) + (7)), *(u8 *)((unsigned char *)arg0 + (1)) = (u8) *(u8 *)((unsigned char *)(&sp8C) + (6)), *(u8 *)((unsigned char *)arg0 + (2)) = (u8) *(u8 *)((unsigned char *)(&sp8C) + (5)), *(u8 *)((unsigned char *)arg0 + (3)) = (u8) *(u8 *)((unsigned char *)(&sp8C) + (4)), (arg4 != 1))) {
                temp_lo_2 = var_s0_2 * arg4;
                var_v0 = arg0 + temp_lo_2;
                var_v1 = temp_lo_2 + &sp8C;
                temp_lo_3 = arg4 * (var_s0_2 + 1);
                var_a0 = arg0 + temp_lo_3;
                var_a1 = temp_lo_3 + &sp8C;
                do {
                    var_s0_2 += 2;
                    *(u8 *)((unsigned char *)var_v0 + (4)) = (u8) *(u8 *)((unsigned char *)var_v1 + (3));
                    *(u8 *)((unsigned char *)var_v0 + (5)) = (u8) *(u8 *)((unsigned char *)var_v1 + (2));
                    *(u8 *)((unsigned char *)var_v0 + (6)) = (u8) *(u8 *)((unsigned char *)var_v1 + (1));
                    *(u8 *)((unsigned char *)var_v0 + (7)) = (u8) var_v1->unk0;
                    var_v0->unk0 = (u8) *(u8 *)((unsigned char *)var_v1 + (7));
                    *(u8 *)((unsigned char *)var_v0 + (1)) = (u8) *(u8 *)((unsigned char *)var_v1 + (6));
                    *(u8 *)((unsigned char *)var_v0 + (2)) = (u8) *(u8 *)((unsigned char *)var_v1 + (5));
                    temp_t5 = *(u8 *)((unsigned char *)var_v1 + (4));
                    var_v1 += arg4 * 2;
                    *(u8 *)((unsigned char *)var_v0 + (3)) = temp_t5;
                    var_v0 += arg4 * 2;
                    *(u8 *)((unsigned char *)var_a0 + (4)) = (u8) *(u8 *)((unsigned char *)var_a1 + (3));
                    *(u8 *)((unsigned char *)var_a0 + (5)) = (u8) *(u8 *)((unsigned char *)var_a1 + (2));
                    *(u8 *)((unsigned char *)var_a0 + (6)) = (u8) *(u8 *)((unsigned char *)var_a1 + (1));
                    *(u8 *)((unsigned char *)var_a0 + (7)) = (u8) var_a1->unk0;
                    var_a0->unk0 = (u8) *(u8 *)((unsigned char *)var_a1 + (7));
                    *(u8 *)((unsigned char *)var_a0 + (1)) = (u8) *(u8 *)((unsigned char *)var_a1 + (6));
                    *(u8 *)((unsigned char *)var_a0 + (2)) = (u8) *(u8 *)((unsigned char *)var_a1 + (5));
                    temp_t7 = *(u8 *)((unsigned char *)var_a1 + (4));
                    var_a1 += arg4 * 2;
                    *(u8 *)((unsigned char *)var_a0 + (3)) = temp_t7;
                    var_a0 += arg4 * 2;
                } while (var_s0_2 != arg4);
            }
        }
    }
}
