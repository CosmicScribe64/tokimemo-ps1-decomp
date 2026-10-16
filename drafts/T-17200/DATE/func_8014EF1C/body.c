void func_8014EF1C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10, s32 arg11) {
    u8 *sp28;
    u32 temp_t6;
    u32 temp_t7;
    u32 temp_t8;
    u32 temp_t9;
    void *temp_v0;

    temp_v0 = GetWorkBase(0x24, D_8011ECA0);
    sp28 = temp_v0;
    func_8009F054((s32) temp_v0);
    SetSemiTrans(sp28, arg11);
    temp_t6 = (u32) arg8 >> 0x10;
    temp_t7 = (u32) arg8 >> 8;
    temp_t8 = (u32) arg9 >> 0x10;
    temp_t9 = (u32) arg9 >> 8;
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
    *(s16 *)((unsigned char *)sp28 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg0);
    *(s16 *)((unsigned char *)sp28 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg1);
    *(s16 *)((unsigned char *)sp28 + (16)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg2);
    *(s16 *)((unsigned char *)sp28 + (18)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg3);
    *(s16 *)((unsigned char *)sp28 + (24)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg4);
    *(s16 *)((unsigned char *)sp28 + (26)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg5);
    *(s16 *)((unsigned char *)sp28 + (32)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg6);
    *(s16 *)((unsigned char *)sp28 + (34)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg7);
    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + (arg10 * 4)], sp28);
}
