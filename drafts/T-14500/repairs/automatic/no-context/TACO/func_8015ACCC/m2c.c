? func_80099E30(s32, void *);                       /* extern */
? func_8009B3C0(s32, s32);                          /* extern */
? func_8009B430(s32, void *, s32);                  /* extern */
? func_80143B34(s32);                               /* extern */
? func_8015B1B8(s32, s32, s32, ?);                  /* extern */
? func_8015B2AC(s32, s32);                          /* extern */
extern ? D_80127080;
extern ? D_80127480;
extern s32 D_8015EDB4;
extern u8 D_80161D9C;
extern ? D_80161DDC;
extern ? D_80161E20;
extern ? D_80161E60;
extern ? D_80161EA4;
extern ? D_80161EC8;

s32 func_8015ACCC(s32 arg0, s32 arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5, s16 arg6, s16 arg7, s32 arg8, s32 arg9) {
    s32 sp64;
    s32 sp50;
    s16 *temp_v0_3;
    s32 *temp_s3;
    s32 temp_fp;
    s32 temp_fp_2;
    s32 temp_s5;
    s32 temp_v0;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_v1;
    u32 var_s2;
    u8 *temp_s7;
    u8 *var_s1;
    u8 *var_s1_2;
    u8 temp_t8;
    u8 var_s2_2;
    void *temp_s4;
    void *temp_v0_2;
    void *temp_v1;
    void *var_v0;

    var_v1 = 0;
    var_s2 = 0;
    var_s0 = 0x10;
    var_v0 = D_8015EDB4 + 0x880;
loop_1:
    if (!(*(u8 *)((unsigned char *)var_v0 + (1)) & 0x80)) {
        var_v1 = var_s0;
    } else {
        var_s0 += 1;
        var_v0 += 0x88;
        if (var_s0 < 0x3F) {
            goto loop_1;
        }
    }
    if (var_v1 == 0) {
        return -1;
    }
    var_s1 = &D_80161D9C;
    do {
        temp_t8 = *var_s1;
        var_s1 += 1;
        if (temp_t8 == 0) {
            var_s2 += 1;
        }
    } while ((u32) var_s1 < (u32) &D_80161DDC);
    temp_s7 = arg0 + &D_80161E20;
    temp_v0 = arg0 * 4;
    if (var_s2 < (u8) *temp_s7) {
        return -1;
    }
    sp50 = temp_v0;
    temp_fp = *(&D_80161DDC + temp_v0) + 4;
    sp64 = var_v1;
    func_8009B3C0(temp_fp, arg0);
    temp_fp_2 = temp_fp + 8;
    func_80143B34(sp64);
    temp_s5 = sp64 * 0x88;
    func_80099E30(0, temp_s5 + D_8015EDB4 + 0x14);
    if (arg0 == 0xA) {
        func_8009B430(temp_fp_2, temp_s5 + D_8015EDB4 + 4, 1);
    } else {
        func_8009B430(temp_fp_2, temp_s5 + D_8015EDB4 + 4, 0);
    }
    var_s2_2 = *temp_s7;
    var_s0_2 = 0;
    if (var_s2_2 != 0) {
        var_s1_2 = &D_80161D9C;
        do {
            if (*var_s1_2 == 0) {
                temp_s4 = (var_s0_2 * 0x10) + &D_80127080;
                temp_s3 = sp50 + &D_80161E60;
                func_80099E30(temp_s5 + D_8015EDB4 + 0x14, (var_s0_2 * 0x50) + &D_80127480);
                *var_s1_2 = (u8) sp64;
                if ((arg0 == 0xF) || (arg0 == 0x10)) {
                    temp_v0_2 = *temp_s3 + ((*temp_s7 - var_s2_2) * 8);
                    func_8015B1B8(var_s0_2, temp_v0_2->unk0, *(s32 *)((unsigned char *)temp_v0_2 + (4)), 0);
                } else {
                    func_8015B2AC(var_s0_2, *(*temp_s3 + (*temp_s7 * 4) + -(var_s2_2 * 4)));
                }
                func_8009B430(temp_fp_2, temp_s4, *temp_s7 - var_s2_2);
                var_s2_2 -= 1;
                *(void **)((unsigned char *)temp_s4 + (4)) = (void *) ((var_s0_2 * 0x50) + &D_80127480);
                temp_s4->unk0 = 0x80000000;
            }
            var_s0_2 += 1;
            var_s1_2 += 1;
        } while (var_s2_2 != 0);
    }
    *(s32 *)((unsigned char *)(D_8015EDB4 + temp_s5) + (4)) = 0x80000000;
    *(s8 *)((unsigned char *)(D_8015EDB4 + temp_s5) + (2)) = (s8) arg1;
    *(s16 *)((unsigned char *)(D_8015EDB4 + temp_s5) + (122)) = 0x7FFF;
    *(s8 *)((unsigned char *)(D_8015EDB4 + temp_s5) + (134)) = (s8) arg0;
    *(s8 *)((unsigned char *)(D_8015EDB4 + temp_s5) + (3)) = 1;
    *(s16 *)((unsigned char *)(D_8015EDB4 + temp_s5) + (130)) = 0;
    *(s16 *)((unsigned char *)(D_8015EDB4 + temp_s5) + (114)) = 0;
    *(s16 *)((unsigned char *)(D_8015EDB4 + temp_s5) + (124)) = arg2;
    *(s16 *)((unsigned char *)(D_8015EDB4 + temp_s5) + (126)) = arg2;
    *(s16 *)((unsigned char *)(D_8015EDB4 + temp_s5) + (128)) = arg3;
    *(s8 *)((unsigned char *)(D_8015EDB4 + temp_s5) + (1)) = 0x80;
    *(s16 *)((unsigned char *)(D_8015EDB4 + temp_s5) + (116)) = arg4;
    *(s16 *)((unsigned char *)(D_8015EDB4 + temp_s5) + (118)) = arg4;
    *(s16 *)((unsigned char *)(D_8015EDB4 + temp_s5) + (120)) = arg5;
    *(s16 *)((unsigned char *)(D_8015EDB4 + temp_s5) + (100)) = arg6;
    *(s16 *)((unsigned char *)(D_8015EDB4 + temp_s5) + (102)) = arg6;
    *(s16 *)((unsigned char *)(D_8015EDB4 + temp_s5) + (104)) = arg7;
    *(s16 *)((unsigned char *)(D_8015EDB4 + temp_s5) + (106)) = arg7;
    temp_v0_3 = (arg0 * 2) + &D_80161EA4;
    *(s16 *)((unsigned char *)(D_8015EDB4 + temp_s5) + (108)) = (s16) *temp_v0_3;
    *(s16 *)((unsigned char *)(D_8015EDB4 + temp_s5) + (110)) = (s16) *temp_v0_3;
    *(s16 *)((unsigned char *)(D_8015EDB4 + temp_s5) + (112)) = (s16) *temp_v0_3;
    *(s8 *)((unsigned char *)(D_8015EDB4 + temp_s5) + (133)) = (s8) arg8;
    *(u8 *)((unsigned char *)(D_8015EDB4 + temp_s5) + (132)) = (u8) *(&D_80161EC8 + arg0);
    *(s8 *)((unsigned char *)(D_8015EDB4 + temp_s5) + (135)) = (s8) (arg9 | 1);
    temp_v1 = temp_s5 + D_8015EDB4;
    *(s32 *)((unsigned char *)temp_v1 + (8)) = (s32) (temp_v1 + 0x14);
    return sp64;
}
