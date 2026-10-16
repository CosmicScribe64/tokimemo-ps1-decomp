void func_80154220(void *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 temp_v0;
    s32 temp_v1;
    s32 var_v0;

    temp_v0 = GetWorkBase(0x28, D_8011ECA0);
    func_8009F040(temp_v0);
    SetShadeTex(temp_v0, 0);
    if (arg3 != 0) {
        SetSemiTrans(temp_v0, 1);
    } else {
        SetSemiTrans(temp_v0, 0);
    }
    *(s8 *)((unsigned char *)temp_v0 + (4)) = (s8) arg2;
    *(s8 *)((unsigned char *)temp_v0 + (5)) = (s8) arg2;
    *(s8 *)((unsigned char *)temp_v0 + (6)) = (s8) arg2;
    if (arg3 != 0) {
        var_v0 = func_8009ED74(0, 0x1F7);
    } else {
        var_v0 = func_8009ED74(0, 0x1F5);
    }
    *(s16 *)((unsigned char *)temp_v0 + (14)) = (s16) var_v0;
    *(s16 *)((unsigned char *)temp_v0 + (22)) = func_8009ECB0(1, arg3 + 4, (arg4 << 6) + 0x140, 0x100);
    *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) (*(s32 *)arg0);
    temp_v1 = (D_8011ECA0 * -0xF0) + 0xF0;
    *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) (*(s32 *)((unsigned char *)arg0 + (4)) + temp_v1);
    *(s16 *)((unsigned char *)temp_v0 + (16)) = (s16) (*(s32 *)((unsigned char *)arg0 + (8)) + 1);
    *(s16 *)((unsigned char *)temp_v0 + (18)) = (s16) (*(s32 *)((unsigned char *)arg0 + (12)) + temp_v1);
    *(s16 *)((unsigned char *)temp_v0 + (24)) = (s16) *(s32 *)((unsigned char *)arg0 + (16));
    *(s16 *)((unsigned char *)temp_v0 + (26)) = (s16) (*(s32 *)((unsigned char *)arg0 + (20)) + temp_v1 + 1);
    *(s16 *)((unsigned char *)temp_v0 + (32)) = (s16) (*(s32 *)((unsigned char *)arg0 + (24)) + 1);
    *(s16 *)((unsigned char *)temp_v0 + (34)) = (s16) (*(s32 *)((unsigned char *)arg0 + (28)) + temp_v1 + 1);
    *(s8 *)((unsigned char *)temp_v0 + (12)) = (s8) (*(s32 *)arg1);
    *(s8 *)((unsigned char *)temp_v0 + (13)) = (s8) *(s32 *)((unsigned char *)arg1 + (4));
    *(s8 *)((unsigned char *)temp_v0 + (20)) = (s8) (*(s32 *)((unsigned char *)arg1 + (8)) + 1);
    *(s8 *)((unsigned char *)temp_v0 + (21)) = (s8) *(s32 *)((unsigned char *)arg1 + (12));
    *(s8 *)((unsigned char *)temp_v0 + (28)) = (s8) *(s32 *)((unsigned char *)arg1 + (16));
    *(s8 *)((unsigned char *)temp_v0 + (29)) = (s8) (*(s32 *)((unsigned char *)arg1 + (20)) + 1);
    *(s8 *)((unsigned char *)temp_v0 + (36)) = (s8) (*(s32 *)((unsigned char *)arg1 + (24)) + 1);
    *(s8 *)((unsigned char *)temp_v0 + (37)) = (s8) (*(s32 *)((unsigned char *)arg1 + (28)) + 1);
    AddPrim((D_8011ECA0 << 0xA) + 0x20 + D_800E8CA0, temp_v0);
}
