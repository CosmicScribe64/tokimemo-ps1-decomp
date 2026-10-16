s32 strNext(void *arg0) {
    u8 *sp54;
    u8 *sp50;
    s16 sp4A;
    s16 sp48;
    s16 sp46;
    s16 sp44;
    s32 *var_s0;
    s16 temp_v1;
    s32 temp_t3;
    s32 var_a0;

    var_s0 = &D_7FFFFF;
    if (func_80088D70((s32) &sp54, (s32) &sp50) != 0) {
loop_1:
        if (var_s0 == NULL) {
            return NULL;
        }
        var_s0 -= 1;
        if (func_80088D70((s32) &sp54, (s32) &sp50) == 0) {
            goto block_4;
        }
        goto loop_1;
    }
block_4:
    if ((u32) *(u32 *)((unsigned char *)sp50 + (8)) >= (u32) D_800B58F4) {
        *(s32 *)((unsigned char *)D_80125C58 + (52)) = 1;
    }
    if ((*(s32 *)((unsigned char *)sp50 + (20)) != (*(s32 *)sp54)) || (*(s32 *)((unsigned char *)sp50 + (24)) != *(s32 *)((unsigned char *)sp54 + (4)))) {
        func_80088CB0((s32) sp54);
        return NULL;
    }
    D_800B58F0 = *(u32 *)((unsigned char *)sp50 + (8));
    var_a0 = *(s32 *)((unsigned char *)D_80125C58 + (60));
    if ((*(u16 *)((unsigned char *)sp50 + (16)) != var_a0) || (*(u16 *)((unsigned char *)sp50 + (18)) != *(s32 *)((unsigned char *)D_80125C58 + (64)))) {
        sp44 = 0;
        sp46 = 0;
        sp48 = 0x280;
        sp4A = 0x1E0;
        func_8009C7F8((RECT *) &sp44, 0, 0, 0);
        *(s32 *)((unsigned char *)D_80125C58 + (60)) = (s32) *(u16 *)((unsigned char *)sp50 + (16));
        *(s32 *)((unsigned char *)D_80125C58 + (64)) = (s32) *(u16 *)((unsigned char *)sp50 + (18));
        var_a0 = *(s32 *)((unsigned char *)D_80125C58 + (60));
    }
    if (D_800B5900 & 1) {
        temp_t3 = (s32) (var_a0 * 3) / 2;
        *(s16 *)((unsigned char *)arg0 + (28)) = (s16) temp_t3;
        *(s16 *)((unsigned char *)arg0 + (20)) = (s16) temp_t3;
    } else {
        *(s16 *)((unsigned char *)arg0 + (28)) = (s16) var_a0;
        *(s16 *)((unsigned char *)arg0 + (20)) = (s16) var_a0;
    }
    temp_v1 = (s16) *(s32 *)((unsigned char *)D_80125C58 + (64));
    *(s16 *)((unsigned char *)arg0 + (30)) = temp_v1;
    *(s16 *)((unsigned char *)arg0 + (22)) = temp_v1;
    *(s16 *)((unsigned char *)arg0 + (42)) = (s16) *(s32 *)((unsigned char *)D_80125C58 + (64));
    return sp54;
}
