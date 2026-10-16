void func_80108B24(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u32 arg4, s32 arg5, s32 arg6) {
    u8 *sp28;
    u32 temp_t6;
    u32 temp_t7;
    void *temp_v0;

    temp_v0 = func_80019D00(0x24, D_800E9600);
    sp28 = temp_v0;
    func_80067678(temp_v0);
    func_800675A8(sp28, arg6);
    temp_t6 = arg4 >> 0x10;
    temp_t7 = arg4 >> 8;
    *(s8 *)((unsigned char *)sp28 + (5)) = (s8) temp_t7;
    *(s8 *)((unsigned char *)sp28 + (4)) = (s8) temp_t6;
    *(s8 *)((unsigned char *)sp28 + (6)) = (s8) arg4;
    *(s8 *)((unsigned char *)sp28 + (13)) = (s8) temp_t7;
    *(s8 *)((unsigned char *)sp28 + (12)) = (s8) temp_t6;
    *(s8 *)((unsigned char *)sp28 + (14)) = (s8) arg4;
    *(s8 *)((unsigned char *)sp28 + (21)) = (s8) temp_t7;
    *(s8 *)((unsigned char *)sp28 + (20)) = (s8) temp_t6;
    *(s8 *)((unsigned char *)sp28 + (22)) = (s8) arg4;
    *(s8 *)((unsigned char *)sp28 + (28)) = (s8) temp_t6;
    *(s8 *)((unsigned char *)sp28 + (30)) = (s8) arg4;
    *(s8 *)((unsigned char *)sp28 + (29)) = (s8) temp_t7;
    *(s16 *)((unsigned char *)sp28 + (8)) = (s16) (*(s16 *)((unsigned char *)(D_800B0860 + (D_800E9600 * 2)) + (20)) + arg0);
    *(s16 *)((unsigned char *)sp28 + (10)) = (s16) (*(s16 *)((unsigned char *)(D_800B0860 + (D_800E9600 * 2)) + (24)) + arg1);
    *(s16 *)((unsigned char *)sp28 + (16)) = (s16) (*(s16 *)((unsigned char *)(D_800B0860 + (D_800E9600 * 2)) + (20)) + arg0 + arg2);
    *(s16 *)((unsigned char *)sp28 + (18)) = (s16) (*(s16 *)((unsigned char *)(D_800B0860 + (D_800E9600 * 2)) + (24)) + arg1);
    *(s16 *)((unsigned char *)sp28 + (24)) = (s16) (*(s16 *)((unsigned char *)(D_800B0860 + (D_800E9600 * 2)) + (20)) + arg0);
    *(s16 *)((unsigned char *)sp28 + (26)) = (s16) (*(s16 *)((unsigned char *)(D_800B0860 + (D_800E9600 * 2)) + (24)) + arg1 + arg3);
    *(s16 *)((unsigned char *)sp28 + (32)) = (s16) (*(s16 *)((unsigned char *)(D_800B0860 + (D_800E9600 * 2)) + (20)) + arg0 + arg2);
    *(s16 *)((unsigned char *)sp28 + (34)) = (s16) (*(s16 *)((unsigned char *)(D_800B0860 + (D_800E9600 * 2)) + (24)) + arg1 + arg3);
    func_800674F4((D_800E9600 << 6) + (arg5 * 4) + &D_800B2580, sp28);
}
