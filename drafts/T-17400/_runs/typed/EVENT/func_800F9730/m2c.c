void *func_80019D00(?, s32);                        /* extern */
? func_800674F4(void *, void *);                    /* extern */
? func_800675A8(void *, s32);                       /* extern */
? func_80067678(void *);                            /* extern */
extern ? D_800B2580;
extern s32 D_800E9600;

void func_800F9730(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, u32 arg8, u32 arg9, s32 arg10, s32 arg11) {
    void *sp28;
    u32 temp_t6;
    u32 temp_t7;
    u32 temp_t8;
    u32 temp_t9;
    void *temp_v0;

    temp_v0 = func_80019D00(0x24, D_800E9600);
    sp28 = temp_v0;
    func_80067678(temp_v0);
    func_800675A8(sp28, arg11);
    temp_t6 = arg8 >> 0x10;
    temp_t7 = arg8 >> 8;
    temp_t8 = arg9 >> 0x10;
    temp_t9 = arg9 >> 8;
    *(s8 *)((unsigned char *)sp28 + (5)) = (s8) temp_t7;
    *(s8 *)((unsigned char *)sp28 + (4)) = (s8) temp_t6;
    *(s8 *)((unsigned char *)sp28 + (6)) = (s8) arg8;
    *(s8 *)((unsigned char *)sp28 + (13)) = (s8) temp_t7;
    *(s8 *)((unsigned char *)sp28 + (12)) = (s8) temp_t6;
    *(s8 *)((unsigned char *)sp28 + (14)) = (s8) arg8;
    *(s8 *)((unsigned char *)sp28 + (21)) = (s8) temp_t9;
    *(s8 *)((unsigned char *)sp28 + (20)) = (s8) temp_t8;
    *(s8 *)((unsigned char *)sp28 + (22)) = (s8) arg9;
    *(s8 *)((unsigned char *)sp28 + (29)) = (s8) temp_t9;
    *(s8 *)((unsigned char *)sp28 + (28)) = (s8) temp_t8;
    *(s8 *)((unsigned char *)sp28 + (30)) = (s8) arg9;
    *(s16 *)((unsigned char *)sp28 + (8)) = (s16) (*(s16 *)((unsigned char *)(D_800B0860 + (D_800E9600 * 2)) + (20)) + arg0);
    *(s16 *)((unsigned char *)sp28 + (10)) = (s16) (*(s16 *)((unsigned char *)(D_800B0860 + (D_800E9600 * 2)) + (24)) + arg1);
    *(s16 *)((unsigned char *)sp28 + (16)) = (s16) (*(s16 *)((unsigned char *)(D_800B0860 + (D_800E9600 * 2)) + (20)) + arg2);
    *(s16 *)((unsigned char *)sp28 + (18)) = (s16) (*(s16 *)((unsigned char *)(D_800B0860 + (D_800E9600 * 2)) + (24)) + arg3);
    *(s16 *)((unsigned char *)sp28 + (24)) = (s16) (*(s16 *)((unsigned char *)(D_800B0860 + (D_800E9600 * 2)) + (20)) + arg4);
    *(s16 *)((unsigned char *)sp28 + (26)) = (s16) (*(s16 *)((unsigned char *)(D_800B0860 + (D_800E9600 * 2)) + (24)) + arg5);
    *(s16 *)((unsigned char *)sp28 + (32)) = (s16) (*(s16 *)((unsigned char *)(D_800B0860 + (D_800E9600 * 2)) + (20)) + arg6);
    *(s16 *)((unsigned char *)sp28 + (34)) = (s16) (*(s16 *)((unsigned char *)(D_800B0860 + (D_800E9600 * 2)) + (24)) + arg7);
    func_800674F4((D_800E9600 << 6) + (arg10 * 4) + &D_800B2580, sp28);
}
