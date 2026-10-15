extern ? D_800E736C;
extern ? D_80122120;
extern ? D_80122140;
extern ? D_80122540;

void func_8004A1A0(s32 arg0, s32 arg1, ? arg2, s16 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9) {
    s32 sp64;
    void *sp58;
    void *sp54;
    s16 *var_s4;
    s16 var_s2_2;
    s32 *var_s2;
    s32 var_s0;
    s32 var_s1;
    s32 var_s1_2;
    s32 var_s3;
    s32 var_v1;
    void *temp_a0;
    void *temp_t2;
    void *temp_t6;
    void *temp_v0;
    void *var_s0_2;

    temp_v0 = (arg1 * 0x24) + &D_801220D0;
    *(s16 *)((unsigned char *)temp_v0 + (4)) = (s16) arg6;
    temp_a0 = &D_80122120 + (arg1 * 0x10);
    temp_v0->unk0 = 0x01000000;
    *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) (arg4 * 0x78);
    *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) (arg3 << 7);
    *(void **)((unsigned char *)temp_v0 + (20)) = (void *) (&D_80122120 + (arg1 * 0x10));
    *(s16 *)((unsigned char *)temp_v0 + (28)) = 0x1000;
    *(s16 *)((unsigned char *)temp_v0 + (30)) = 0x1000;
    temp_t6 = (arg1 << 9) + &D_80122140;
    temp_t2 = (arg1 << 7) + &D_80122540;
    *(s16 *)((unsigned char *)temp_v0 + (12)) = 0;
    *(s16 *)((unsigned char *)temp_v0 + (14)) = 0;
    *(s8 *)((unsigned char *)temp_v0 + (16)) = 0x80;
    *(s8 *)((unsigned char *)temp_v0 + (17)) = 0x80;
    *(s8 *)((unsigned char *)temp_v0 + (18)) = 0x80;
    *(s16 *)((unsigned char *)temp_v0 + (24)) = 0;
    *(s16 *)((unsigned char *)temp_v0 + (26)) = 0;
    *(s32 *)((unsigned char *)temp_v0 + (32)) = 0;
    temp_a0->unk0 = 0x80;
    *(s8 *)((unsigned char *)temp_a0 + (1)) = 0x78;
    *(s16 *)((unsigned char *)temp_a0 + (2)) = arg3;
    sp58 = temp_t6;
    *(void **)((unsigned char *)temp_a0 + (8)) = temp_t6;
    sp54 = temp_t2;
    *(void **)((unsigned char *)temp_a0 + (12)) = temp_t2;
    var_v1 = 0;
    *(s16 *)((unsigned char *)temp_a0 + (4)) = (s16) arg4;
    *(s16 *)((unsigned char *)temp_v0 + (6)) = (s16) arg7;
    if (arg4 > 0) {
        do {
            var_s1 = 0;
            if (arg3 > 0) {
                sp64 = var_v1;
                var_s0 = arg9 + (var_v1 << 5);
                var_s2 = arg0 + (var_v1 * arg3 * 4);
                do {
                    load_tpage_buf_lock(var_s0, *var_s2, 1);
                    var_s1 += 1;
                    var_s0 += 1;
                    var_s2 += 4;
                } while (var_s1 != arg3);
                var_v1 = sp64;
            }
            var_v1 += 1;
        } while (var_v1 != arg4);
        var_v1 = 0;
    }
    if (arg4 > 0) {
        do {
            var_s1_2 = 0;
            if (arg3 > 0) {
                var_s3 = arg9;
                sp64 = var_v1;
                var_s2_2 = var_v1 * arg3;
                var_s4 = sp54 + (var_s2_2 * 2);
                var_s0_2 = sp58 + (var_s2_2 * 8);
                do {
                    *var_s4 = var_s2_2;
                    var_s0_2->unk0 = 0;
                    *(s8 *)((unsigned char *)var_s0_2 + (1)) = (s8) (var_v1 * 0x78);
                    *(s16 *)((unsigned char *)var_s0_2 + (6)) = (s16) var_s3;
                    *(s16 *)((unsigned char *)var_s0_2 + (2)) = func_8009ED74(0, arg5);
                    *(s16 *)((unsigned char *)var_s0_2 + (4)) = 0;
                    if (var_s2_2 >= 0x41) {
                        printf("BG_ERROR:p_line.c \n");
                    }
                    var_s1_2 += 1;
                    var_s3 += 1;
                    var_s2_2 += 1;
                    var_s4 += 2;
                    var_s0_2 += 8;
                } while (var_s1_2 != arg3);
                var_v1 = sp64;
            }
            var_v1 += 1;
        } while (var_v1 != arg4);
    }
    *(&D_800E736C + arg1) = (s8) arg8;
}
