void *func_80019D00(?, s32);                        /* extern */
? func_800674F4(void *, void *, void *);            /* extern */
? func_800675A8(void *, s32);                       /* extern */
? func_80067628(void *);                            /* extern */
extern ? D_800B2580;
extern s32 D_800E9600;

void func_800FFA4C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u32 arg6, u32 arg7, s32 arg8, s32 arg9) {
    void *sp28;
    u32 temp_t2;
    u32 temp_t3;
    void *temp_v0;

    temp_v0 = func_80019D00(0x1C, D_800E9600);
    sp28 = temp_v0;
    func_80067628(temp_v0);
    func_800675A8(sp28, arg9);
    temp_t2 = arg7 >> 0x10;
    temp_t3 = arg7 >> 8;
    *(s8 *)((unsigned char *)sp28 + (5)) = (s8) (arg6 >> 8);
    *(s8 *)((unsigned char *)sp28 + (4)) = (s8) (arg6 >> 0x10);
    *(s8 *)((unsigned char *)sp28 + (6)) = (s8) arg6;
    *(s8 *)((unsigned char *)sp28 + (13)) = (s8) temp_t3;
    *(s8 *)((unsigned char *)sp28 + (12)) = (s8) temp_t2;
    *(s8 *)((unsigned char *)sp28 + (14)) = (s8) arg7;
    *(s8 *)((unsigned char *)sp28 + (21)) = (s8) temp_t3;
    *(s8 *)((unsigned char *)sp28 + (20)) = (s8) temp_t2;
    *(s8 *)((unsigned char *)sp28 + (22)) = (s8) arg7;
    *(s16 *)((unsigned char *)sp28 + (8)) = (s16) (*(s16 *)((unsigned char *)(D_800B0860 + (D_800E9600 * 2)) + (20)) + arg0);
    *(s16 *)((unsigned char *)sp28 + (10)) = (s16) (*(s16 *)((unsigned char *)(D_800B0860 + (D_800E9600 * 2)) + (24)) + arg1);
    *(s16 *)((unsigned char *)sp28 + (16)) = (s16) (*(s16 *)((unsigned char *)(D_800B0860 + (D_800E9600 * 2)) + (20)) + arg2);
    *(s16 *)((unsigned char *)sp28 + (18)) = (s16) (*(s16 *)((unsigned char *)(D_800B0860 + (D_800E9600 * 2)) + (24)) + arg3);
    *(s16 *)((unsigned char *)sp28 + (24)) = (s16) (*(s16 *)((unsigned char *)(D_800B0860 + (D_800E9600 * 2)) + (20)) + arg4);
    *(s16 *)((unsigned char *)sp28 + (26)) = (s16) (*(s16 *)((unsigned char *)(D_800B0860 + (D_800E9600 * 2)) + (24)) + arg5);
    func_800674F4((D_800E9600 << 6) + (arg8 * 4) + &D_800B2580, sp28, sp28);
}
