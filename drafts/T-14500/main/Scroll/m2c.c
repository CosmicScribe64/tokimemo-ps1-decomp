extern u8 D_800E62BA;

void Scroll(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 sp68;
    u32 sp60;
    s32 sp50;
    s32 temp_v0;
    s32 var_s2;
    s32 var_s4;
    s32 var_s5;
    s8 temp_fp;
    s8 temp_t9;
    s8 temp_v1;

    sp50 = arg1 * 4;
    sp68 = arg0 * 2;
    var_s2 = 0;
    var_s4 = 0;
    var_s5 = 0;
    temp_fp = (s16) arg2 + (s16) arg3;
    sp60 = (u32) (s16) arg4;
    do {
        temp_v0 = GetWorkBase(0x28, D_8011ECA0);
        func_8009F040(temp_v0);
        SetSemiTrans(temp_v0, 0);
        SetShadeTex(temp_v0, 0);
        *(u8 *)((unsigned char *)temp_v0 + (4)) = (u8) D_800E62BA;
        *(u8 *)((unsigned char *)temp_v0 + (5)) = (u8) D_800E62BA;
        *(u8 *)((unsigned char *)temp_v0 + (6)) = (u8) D_800E62BA;
        temp_t9 = (sp68 + ((var_s4 % 2) << 6)) & 0x7F;
        temp_v1 = temp_t9 + 0x40;
        *(s8 *)((unsigned char *)temp_v0 + (20)) = temp_v1;
        *(s8 *)((unsigned char *)temp_v0 + (36)) = temp_v1;
        *(s8 *)((unsigned char *)temp_v0 + (12)) = temp_t9;
        *(s8 *)((unsigned char *)temp_v0 + (28)) = temp_t9;
        *(s8 *)((unsigned char *)temp_v0 + (37)) = temp_fp;
        *(s8 *)((unsigned char *)temp_v0 + (29)) = temp_fp;
        *(s8 *)((unsigned char *)temp_v0 + (21)) = (s8) (s16) arg2;
        *(s8 *)((unsigned char *)temp_v0 + (13)) = (s8) (s16) arg2;
        *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) ((D_800E6280.unk_014[D_8011ECA0] + var_s2) - 0xA0);
        *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) ((D_800E6280.unk_018[D_8011ECA0] + (s16) arg2) - 0x78);
        *(s16 *)((unsigned char *)temp_v0 + (16)) = (s16) ((D_800E6280.unk_014[D_8011ECA0] + var_s2) - 0x60);
        *(s16 *)((unsigned char *)temp_v0 + (18)) = (s16) ((D_800E6280.unk_018[D_8011ECA0] + (s16) arg2) - 0x78);
        *(s16 *)((unsigned char *)temp_v0 + (24)) = (s16) ((D_800E6280.unk_014[D_8011ECA0] + var_s2) - 0xA0);
        *(s16 *)((unsigned char *)temp_v0 + (26)) = (s16) ((D_800E6280.unk_018[D_8011ECA0] + (s16) arg2 + (s16) arg3) - 0x78);
        *(s16 *)((unsigned char *)temp_v0 + (32)) = (s16) ((D_800E6280.unk_014[D_8011ECA0] + var_s2) - 0x60);
        *(s16 *)((unsigned char *)temp_v0 + (34)) = (s16) ((D_800E6280.unk_018[D_8011ECA0] + (s16) arg2 + (s16) arg3) - 0x78);
        *(s16 *)((unsigned char *)temp_v0 + (22)) = func_8009ECB0(1, 0, ((u32) (((u32) (var_s5 + arg0) >> 6) << 6) % sp60) + 0x140, 0);
        *(s16 *)((unsigned char *)temp_v0 + (14)) = func_8009ED74(0, 0x1F0);
        AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + sp50], temp_v0);
        var_s4 += 1;
        var_s5 += 0x20;
        var_s2 += 0x40;
    } while (var_s4 != 5);
}
