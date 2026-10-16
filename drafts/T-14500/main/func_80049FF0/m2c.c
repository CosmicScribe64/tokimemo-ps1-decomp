void func_80049FF0(s16 arg0, s16 arg1, s16 arg2, s16 arg3, u32 arg4, u32 arg5, u8 arg6) {
    s32 temp_v0;

    temp_v0 = GetWorkBase(0x14, D_8011ECA0);
    func_8009F11C(temp_v0);
    if (arg6 & 0x80) {
        SetSemiTrans(temp_v0, 1);
    } else {
        SetSemiTrans(temp_v0, 0);
    }
    *(s8 *)((unsigned char *)temp_v0 + (5)) = (s8) (arg4 >> 8);
    *(s8 *)((unsigned char *)temp_v0 + (4)) = (s8) (arg4 >> 0x10);
    *(s8 *)((unsigned char *)temp_v0 + (13)) = (s8) (arg5 >> 8);
    *(s8 *)((unsigned char *)temp_v0 + (12)) = (s8) (arg5 >> 0x10);
    *(s8 *)((unsigned char *)temp_v0 + (6)) = (s8) arg4;
    *(s8 *)((unsigned char *)temp_v0 + (14)) = (s8) arg5;
    *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg0);
    *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg1);
    *(s16 *)((unsigned char *)temp_v0 + (16)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg2);
    *(s16 *)((unsigned char *)temp_v0 + (18)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg3);
    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + ((arg6 & 0x7F) * 4)], temp_v0);
}
