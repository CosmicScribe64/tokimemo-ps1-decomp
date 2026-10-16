u8 func_80144198(s32 arg0) {
    s32 sp18[16];
    s16 sp1A;
    s16 temp_t0;
    s32 var_v0;
    s32 var_v1;
    u8 var_a0;
    u8 var_a1;

    sp18[0] = (s32) D_80147C54.unk0;
    *(s32 *)((unsigned char *)(&sp18[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80147C54) + (4));
    *(s32 *)((unsigned char *)(&sp18[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80147C54) + (8));
    *(s32 *)((unsigned char *)(&sp18[0]) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80147C54) + (12));
    *(s32 *)((unsigned char *)(&sp18[0]) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80147C54) + (16));
    *(s32 *)((unsigned char *)(&sp18[0]) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_80147C54) + (20));
    if (D_800E62BE == 0x60) {
        sp1A = 0x1D;
    }
    temp_t0 = *(s16 *)((unsigned char *)(&(&sp18[0])[D_800E62BF]) + (-2));
    var_v1 = (D_800E62C0 - D_800E62C1) + 6;
    var_a1 = D_800E62BE;
    var_a0 = D_800E62BF;
    if (temp_t0 < var_v1) {
        var_a0 = D_800E62BF + 1;
        var_v1 -= temp_t0;
        if ((s32) var_a0 >= 0xD) {
            var_a0 = 1;
            var_a1 = D_800E62BE + 1;
        }
    }
    if (arg0 == 0) {
        return var_a1;
    }
    var_v0 = var_v1;
    if (arg0 == 1) {
        var_v0 = (s32) var_a0;
    }
    return (u8) var_v0;
}
