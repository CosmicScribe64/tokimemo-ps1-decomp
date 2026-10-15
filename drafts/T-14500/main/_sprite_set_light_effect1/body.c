void _sprite_set_light_effect1(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    s32 sp38;
    s32 sp34;
    s32 sp30;
    s32 sp2C;
    s32 sp28;
    s32 temp_lo;
    s32 temp_t6;
    s32 temp_v0;

    sp34 = ((s32) (rsin(D_800E7378 * 0xFF0) << 5) / 4096) + (((u32) arg7 >> 0x10) & 0xFF);
    sp30 = ((s32) (rsin(D_800E7378 * 0xFF0) << 5) / 4096) + (((u32) arg7 >> 8) & 0xFF);
    sp2C = ((s32) (rsin(D_800E7378 * 0xFF0) << 5) / 4096) + (arg7 & 0xFF);
    sp28 = abs(rsin((D_800E7378 + arg8) * 0xFF0));
    temp_v0 = GetWorkBase(0x1C, D_8011ECA0);
    sp38 = temp_v0;
    func_8009F004(temp_v0);
    SetSemiTrans(sp38, 1);
    *(s8 *)((unsigned char *)sp38 + (4)) = (s8) ((s32) (sp34 * 3) / 5);
    *(s8 *)((unsigned char *)sp38 + (5)) = (s8) ((s32) (sp30 * 3) / 5);
    *(s8 *)((unsigned char *)sp38 + (6)) = (s8) ((s32) (sp2C * 3) / 5);
    temp_lo = 0 / 5;
    *(s8 *)((unsigned char *)sp38 + (14)) = (s8) temp_lo;
    *(s8 *)((unsigned char *)sp38 + (13)) = (s8) temp_lo;
    *(s8 *)((unsigned char *)sp38 + (12)) = (s8) temp_lo;
    *(s8 *)((unsigned char *)sp38 + (22)) = (s8) temp_lo;
    *(s8 *)((unsigned char *)sp38 + (21)) = (s8) temp_lo;
    *(s8 *)((unsigned char *)sp38 + (20)) = (s8) temp_lo;
    if (arg4 == 0) {
        *(s16 *)((unsigned char *)sp38 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg0);
        *(s16 *)((unsigned char *)sp38 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg1);
        temp_t6 = (s32) (sp28 * 8) / 4096;
        *(s16 *)((unsigned char *)sp38 + (16)) = (s16) ((D_800E6280.unk_014[D_8011ECA0] + arg2) - temp_t6);
        *(s16 *)((unsigned char *)sp38 + (18)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg3);
        *(s16 *)((unsigned char *)sp38 + (24)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg2 + temp_t6);
        *(s16 *)((unsigned char *)sp38 + (26)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg3);
    } else {
        *(s16 *)((unsigned char *)sp38 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg0);
        *(s16 *)((unsigned char *)sp38 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg1);
        *(s16 *)((unsigned char *)sp38 + (16)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg2);
        *(s16 *)((unsigned char *)sp38 + (18)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg3);
        *(s16 *)((unsigned char *)sp38 + (24)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg4);
        *(s16 *)((unsigned char *)sp38 + (26)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg5);
    }
    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + (arg6 * 4)], sp38);
}
