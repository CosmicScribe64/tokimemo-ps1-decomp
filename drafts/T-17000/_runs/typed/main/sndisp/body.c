void sndisp(s32 arg0, s32 arg1, s32 arg2) {
    u8 sp14C;
    u8 sp4C;
    s32 var_s1;
    s32 var_s3;
    s32 var_v1;
    u8 *var_a0;
    u8 *var_s0;
    u8 var_s2;
    u8 var_v0;

    sp14C = (s32) D_800B40F8;
    *(u8 *)((unsigned char *)(&sp14C) + (4)) = (u8) *(u8 *)((unsigned char *)(&D_800B40F8) + (4));
    strncpy((s32) &sp4C, arg0, 0x100);
    var_v1 = 0;
    var_s3 = 0;
    var_s2 = 0;
    if (sp4C != 0) {
        var_a0 = &sp4C;
        var_v0 = *var_a0;
        do {
            if ((var_v0 == 0x20) && !(var_v1 & 1)) {
                (&sp14C)[var_s3] = var_s2;
                (*(s32 *)var_a0) = 0;
                var_v1 = -1;
                var_s3 += 1;
                var_v0 = 0 & 0xFF;
            }
            var_s2 += 1;
            if ((var_v0 == 0x50) && !(var_v1 & 1)) {
                var_v1 = -1;
            }
            if ((var_v0 == 0x47) && !(var_v1 & 1)) {
                var_v1 = -1;
            }
            var_v0 = *(u8 *)((unsigned char *)var_a0 + (1));
            var_a0 += 1;
            var_v1 += 1;
        } while (var_v0 != 0);
        var_s2 = 0;
    }
    D_800E6280.unk_F61 = check_set_k();
    sndi(&sp4C, arg1, arg2);
    if (var_s3 > 0) {
        var_s0 = &sp14C;
        var_s1 = arg1 + 1;
        do {
            sndi(&(&sp4C)[*var_s0] + 1, var_s1, arg2);
            var_s2 += 1;
            var_s0 += 1;
            var_s1 += 1;
        } while (var_s2 != var_s3);
    }
    D_800E6280.unk_F60 = check_set_k();
    D_800E6280.unk_F62 = 0x80;
}
