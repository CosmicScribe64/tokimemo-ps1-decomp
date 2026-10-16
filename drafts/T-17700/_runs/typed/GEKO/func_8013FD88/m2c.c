void func_8013FD88(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u32 arg4, s32 arg5, s32 arg6) {
    void *sp28;
    u32 temp_t6;
    u32 temp_t7;
    void *temp_v0;

    temp_v0 = func_800490F0(0x24, D_8011ECA0);
    sp28 = temp_v0;
    func_8009F054((s32) temp_v0);
    func_8009EF84(sp28, arg6);
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
    *(s16 *)((unsigned char *)sp28 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg0);
    *(s16 *)((unsigned char *)sp28 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg1);
    *(s16 *)((unsigned char *)sp28 + (16)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg0 + arg2);
    *(s16 *)((unsigned char *)sp28 + (18)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg1);
    *(s16 *)((unsigned char *)sp28 + (24)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg0);
    *(s16 *)((unsigned char *)sp28 + (26)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg1 + arg3);
    *(s16 *)((unsigned char *)sp28 + (32)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg0 + arg2);
    *(s16 *)((unsigned char *)sp28 + (34)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg1 + arg3);
    func_8009EED0(&D_800E8CA0[(D_8011ECA0 << 0xA) + (arg5 * 4)], sp28);
}
