void func_80144E3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    s32 sp8C[1];
    void *sp80;
    u8 sp7B;
    u8 sp7A;
    u8 sp79;
    u8 sp78;
    u8 sp77;
    u8 sp76;
    s32 sp70;
    s16 temp_v1;
    s16 temp_v1_2;
    s16 temp_v1_3;
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_lo_3;
    s32 temp_t1;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v0_5;
    s32 var_a2;
    s32 var_a3;
    s32 var_at;
    s32 var_t0;
    u8 var_fp;
    u8 var_s2;
    u8 var_s3;
    u8 var_s4;
    void **var_s0;
    void **var_s0_2;
    void *temp_s0;
    void *temp_v0;

    var_a2 = arg2;
    var_a3 = arg3;
    var_s2 = sp79;
    var_s3 = sp7A;
    var_s4 = sp7B;
    var_fp = sp77;
    var_s0 = &sp80;
    var_t0 = 0;
    do {
        sp70 = var_t0;
        temp_v0 = GetWorkBase(0x24, D_8011ECA0, (u8) var_a2, (u8) var_a3);
        *var_s0 = temp_v0;
        func_8009F054((s32) temp_v0);
        SetSemiTrans(*var_s0, 1);
        SetShadeTex(*var_s0, 1);
        var_a2 = (s32) sp78;
        var_a3 = (s32) sp76;
        switch (arg5) {
        case 0:
            var_s2 = 0xFF;
            var_s3 = 0xFF;
            var_s4 = 0xFF;
            var_fp = 0x20;
            var_a2 = 0x20;
            var_a3 = 0x50;
            if ((u8) arg4 < 0xF8U) {
                temp_v0_2 = (u8) arg4 + 8;
                var_s4 = temp_v0_2 & 0xFF;
                var_s3 = temp_v0_2 & 0xFF;
            }
            temp_v0_3 = (u8) arg4 + 8;
            if ((u8) arg4 < 0xF0U) {
                var_s2 = ((u8) arg4 + 0x10) & 0xFF;
            }
            var_at = (u8) arg4 < 0x40U;
            if ((u8) arg4 < 0x18U) {
                var_a2 = temp_v0_3 & 0xFF;
                var_fp = temp_v0_3 & 0xFF;
                var_at = (u8) arg4 < 0x40U;
            }
            if (var_at != 0) {
                var_a3 = ((u8) arg4 + 0x10) & 0xFF;
            }
            break;
        case 1:
            var_fp = 8;
            var_a2 = 8;
            var_s3 = 8;
            var_s4 = 8;
            var_a3 = 0x10;
            var_s2 = 0x10;
            if ((u8) arg4 < 0xF8U) {
                temp_v0_4 = 0xFF - (u8) arg4;
                var_s4 = temp_v0_4 & 0xFF;
                var_s3 = temp_v0_4 & 0xFF;
            }
            if ((u8) arg4 < 0xF0U) {
                var_s2 = (0xFF - (u8) arg4) & 0xFF;
            }
            if ((u8) arg4 < 0x18U) {
                temp_v0_5 = 0x20 - (u8) arg4;
                var_a2 = temp_v0_5 & 0xFF;
                var_fp = temp_v0_5 & 0xFF;
            }
            if ((u8) arg4 < 0x40U) {
                var_a3 = (0x50 - (u8) arg4) & 0xFF;
            }
            break;
        case 2:
            var_s2 = 0xFF;
            var_s3 = 0xFF;
            var_s4 = 0xFF;
            var_fp = 0x20;
            var_a2 = 0x20;
            var_a3 = 0x50;
            break;
        }
        switch (var_t0) {
        case 0:
            *(s16 *)((unsigned char *)(*var_s0) + (8)) = (s16) arg0;
            temp_v1 = arg0 + 0x140;
            *(s16 *)((unsigned char *)(*var_s0) + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg1);
            *(s16 *)((unsigned char *)(*var_s0) + (16)) = temp_v1;
            *(s16 *)((unsigned char *)(*var_s0) + (18)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg1);
            *(s16 *)((unsigned char *)(*var_s0) + (24)) = (s16) arg0;
            temp_lo = arg2 / 3;
            *(s16 *)((unsigned char *)(*var_s0) + (26)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg1 + temp_lo);
            *(s16 *)((unsigned char *)(*var_s0) + (32)) = temp_v1;
            *(s16 *)((unsigned char *)(*var_s0) + (34)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg1 + temp_lo);
            *(u8 *)((unsigned char *)(*var_s0) + (4)) = (u8) var_a2;
            *(u8 *)((unsigned char *)(*var_s0) + (5)) = var_fp;
            *(u8 *)((unsigned char *)(*var_s0) + (6)) = (u8) var_a3;
            *(u8 *)((unsigned char *)(*var_s0) + (12)) = (u8) var_a2;
            *(u8 *)((unsigned char *)(*var_s0) + (13)) = var_fp;
            *(u8 *)((unsigned char *)(*var_s0) + (14)) = (u8) var_a3;
            *(u8 *)((unsigned char *)(*var_s0) + (20)) = var_s4;
            *(u8 *)((unsigned char *)(*var_s0) + (21)) = var_s3;
            *(u8 *)((unsigned char *)(*var_s0) + (22)) = var_s2;
            *(u8 *)((unsigned char *)(*var_s0) + (28)) = var_s4;
            *(u8 *)((unsigned char *)(*var_s0) + (29)) = var_s3;
            *(u8 *)((unsigned char *)(*var_s0) + (30)) = var_s2;
            break;
        case 1:
            temp_lo_2 = arg2 / 3;
            *(s16 *)((unsigned char *)(*var_s0) + (8)) = (s16) arg0;
            temp_v1_2 = arg0 + 0x140;
            temp_a0 = arg2 / 2;
            *(s16 *)((unsigned char *)(*var_s0) + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg1 + temp_lo_2 + temp_a0);
            *(s16 *)((unsigned char *)(*var_s0) + (16)) = temp_v1_2;
            *(s16 *)((unsigned char *)(*var_s0) + (18)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg1 + temp_lo_2 + temp_a0);
            *(s16 *)((unsigned char *)(*var_s0) + (24)) = (s16) arg0;
            *(s16 *)((unsigned char *)(*var_s0) + (26)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg1 + temp_lo_2 + temp_a0 + temp_lo_2);
            *(s16 *)((unsigned char *)(*var_s0) + (32)) = temp_v1_2;
            *(s16 *)((unsigned char *)(*var_s0) + (34)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg1 + temp_lo_2 + temp_a0 + temp_lo_2);
            *(u8 *)((unsigned char *)(*var_s0) + (4)) = var_s4;
            *(u8 *)((unsigned char *)(*var_s0) + (5)) = var_s3;
            *(u8 *)((unsigned char *)(*var_s0) + (6)) = var_s2;
            *(u8 *)((unsigned char *)(*var_s0) + (12)) = var_s4;
            *(u8 *)((unsigned char *)(*var_s0) + (13)) = var_s3;
            *(u8 *)((unsigned char *)(*var_s0) + (14)) = var_s2;
            *(u8 *)((unsigned char *)(*var_s0) + (20)) = (u8) var_a2;
            *(u8 *)((unsigned char *)(*var_s0) + (21)) = var_fp;
            *(u8 *)((unsigned char *)(*var_s0) + (22)) = (u8) var_a3;
            *(u8 *)((unsigned char *)(*var_s0) + (28)) = (u8) var_a2;
            *(u8 *)((unsigned char *)(*var_s0) + (29)) = var_fp;
            *(u8 *)((unsigned char *)(*var_s0) + (30)) = (u8) var_a3;
            break;
        case 2:
            temp_lo_3 = arg2 / 3;
            *(s16 *)((unsigned char *)(*var_s0) + (8)) = (s16) arg0;
            temp_v1_3 = arg0 + 0x140;
            *(s16 *)((unsigned char *)(*var_s0) + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg1 + temp_lo_3);
            *(s16 *)((unsigned char *)(*var_s0) + (16)) = temp_v1_3;
            *(s16 *)((unsigned char *)(*var_s0) + (18)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg1 + temp_lo_3);
            *(s16 *)((unsigned char *)(*var_s0) + (24)) = (s16) arg0;
            temp_a0_2 = arg2 / 2;
            *(s16 *)((unsigned char *)(*var_s0) + (26)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg1 + temp_lo_3 + temp_a0_2);
            *(s16 *)((unsigned char *)(*var_s0) + (32)) = temp_v1_3;
            *(s16 *)((unsigned char *)(*var_s0) + (34)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg1 + temp_lo_3 + temp_a0_2);
            *(u8 *)((unsigned char *)(*var_s0) + (4)) = var_s4;
            *(u8 *)((unsigned char *)(*var_s0) + (5)) = var_s3;
            *(u8 *)((unsigned char *)(*var_s0) + (6)) = var_s2;
            *(u8 *)((unsigned char *)(*var_s0) + (12)) = var_s4;
            *(u8 *)((unsigned char *)(*var_s0) + (13)) = var_s3;
            *(u8 *)((unsigned char *)(*var_s0) + (14)) = var_s2;
            *(u8 *)((unsigned char *)(*var_s0) + (20)) = var_s4;
            *(u8 *)((unsigned char *)(*var_s0) + (21)) = var_s3;
            *(u8 *)((unsigned char *)(*var_s0) + (22)) = var_s2;
            *(u8 *)((unsigned char *)(*var_s0) + (28)) = var_s4;
            *(u8 *)((unsigned char *)(*var_s0) + (29)) = var_s3;
            *(u8 *)((unsigned char *)(*var_s0) + (30)) = var_s2;
            break;
        }
        var_t0 += 1;
        var_s0 += 1;
        sp78 = (u8) var_a2;
        sp76 = (u8) var_a3;
    } while (var_t0 < 3);
    sp77 = var_fp;
    sp7B = var_s4;
    sp7A = var_s3;
    sp79 = var_s2;
    if (arg6 > 0) {
        func_80145650((s32) &sp80, arg6, arg2);
    }
    temp_t1 = arg3 * 4;
    var_s0_2 = &sp80;
    do {
        AddPrim(D_800E8CA0 + (D_8011ECA0 << 0xA) + temp_t1, *var_s0_2);
        var_s0_2 += 1;
    } while (var_s0_2 != &sp8C);
    temp_s0 = GetWorkBase(0xC, D_8011ECA0);
    func_8009D294((s32) temp_s0, 0, 0, func_8009ECB0(1, 1, 0, 0), 0);
    AddPrim(D_800E8CA0 + (D_8011ECA0 << 0xA) + temp_t1, temp_s0);
    safe_env(arg3);
}
