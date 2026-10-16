s32 func_800AE0D0(s16, u32);                        /* extern */
extern u8 D_800B593C;
extern ? D_800E7325;
extern ? D_800E7328;
extern ? D_801217D0;
extern s16 D_80121864;
extern s32 D_80122D04;
extern ? D_8014653C;
extern ? D_80146540;
extern ? D_80146608;
extern ? D_801466D0;
extern s16 D_80146798;
extern s16 D_801467FC;
extern u32 D_80146800;

void func_8014288C(void) {
    s16 *temp_s4;
    s16 *var_v1;
    s16 temp_a0;
    s16 temp_s2;
    s16 temp_v0;
    s16 temp_v0_4;
    s16 var_a0;
    s16 var_s2;
    s16 var_s2_2;
    s16 var_v1_2;
    s32 *temp_s0;
    s32 temp_at;
    s32 temp_hi;
    s32 temp_lo;
    s32 temp_s6;
    s32 temp_t7;
    s32 temp_v0_3;
    s32 temp_v1;
    s8 temp_t8;
    s8 temp_v0_2;
    void *temp_s0_2;
    void *temp_s1;
    void *temp_s3;
    void *temp_s5;

    var_a0 = D_801467FC;
    if (var_a0 == 0) {
        return;
    }
    if (D_80146800 == 0x1E) {
        D_80121864 = 0xB4;
        var_a0 += 1;
        D_80146798 = 1;
    } else if (D_80146800 == 0x5A) {
        var_a0 += 1;
        D_80121864 = 0x28;
    } else if (((u32) D_80146800 >= 0xC9U) && (((u32) D_80146800 % 20U) == 0) && (var_a0 < 0x32)) {
        D_801467FC = var_a0 + 1;
        temp_a0 = D_801467FC;
        temp_t8 = (func_800AE0D0(var_a0, D_80146800) % 2) * 8;
        *(s8 *)((unsigned char *)(&D_801217D0 + (temp_a0 * 0x24)) + (122)) = temp_t8;
        if ((temp_t8 & 0xFF) == 8) {
            *(&D_800E7328 + temp_a0) = 0xC;
            D_801467FC = temp_a0;
            var_a0 = D_801467FC;
            var_v1 = &(&D_80146798)[var_a0];
            *(s16 *)((unsigned char *)var_v1 + (-2)) = (s16) ((func_800AE0D0(temp_a0) % 2) + 3);
        } else {
            *(&D_800E7328 + temp_a0) = 9;
            D_801467FC = temp_a0;
            var_a0 = D_801467FC;
            var_v1 = &(&D_80146798)[var_a0];
            *(s16 *)((unsigned char *)var_v1 + (-2)) = (s16) ((func_800AE0D0(temp_a0) % 2) + 1);
        }
        temp_v0 = *(s16 *)((unsigned char *)var_v1 + (-2));
        *(&D_8014653C + (var_a0 * 4)) = 0x10000 / (s32) (((temp_v0 >= 3) * 2) + temp_v0 + 1);
    }
    if (D_80122D04 == 0) {
        D_801467FC = var_a0;
        var_v1_2 = var_a0;
    } else {
        D_801467FC = var_a0;
        var_v1_2 = 0xA;
        var_s2 = 0xE;
        do {
            temp_lo = var_s2 * 0x24;
            temp_s2 = var_s2 + 1;
            temp_at = temp_s2 < 0x36;
            var_s2 = temp_s2;
            temp_s0 = &D_801217D0 + temp_lo;
            *temp_s0 |= 0x80000000;
        } while (temp_at != 0);
    }
    temp_s6 = var_v1_2 + 4;
    var_s2_2 = 4;
    if (temp_s6 >= 5) {
        do {
            temp_s4 = &(&D_80146798)[var_s2_2];
            temp_v0_2 = D_800B593C & 0xFF;
            temp_s0_2 = &D_801217D0 + (var_s2_2 * 0x24);
            *(s8 *)((unsigned char *)temp_s0_2 + (21)) = temp_v0_2;
            *(s8 *)((unsigned char *)temp_s0_2 + (20)) = temp_v0_2;
            *(u8 *)((unsigned char *)temp_s0_2 + (22)) = (u8) D_800B593C;
            temp_v0_3 = var_s2_2 * 4;
            temp_s0_2->unk0 = (s32) (temp_s0_2->unk0 & 0x7FFFFFFF);
            temp_s5 = temp_v0_3 + &D_80146540;
            temp_s1 = temp_v0_3 + &D_80146608;
            temp_s3 = temp_v0_3 + &D_801466D0;
            if ((D_80146800 % (u32) *(s16 *)((unsigned char *)temp_s4 + (-8))) == 0) {
                *(s16 *)((unsigned char *)temp_s0_2 + (6)) = (s16) (*(s16 *)((unsigned char *)temp_s0_2 + (6)) + 1);
            }
            temp_v1 = *(s32 *)((unsigned char *)temp_s5 + (-16));
            temp_t7 = *(s32 *)((unsigned char *)temp_s1 + (-16)) + temp_v1;
            *(s32 *)((unsigned char *)temp_s1 + (-16)) = temp_t7;
            *(s32 *)((unsigned char *)temp_s5 + (-16)) = (s32) (temp_v1 + ((s32) (*(s32 *)((unsigned char *)temp_s3 + (-16)) - temp_t7) >> 8));
            *(s16 *)((unsigned char *)temp_s0_2 + (4)) = (s16) *(s16 *)((unsigned char *)temp_s1 + (-14));
            if (*(s16 *)((unsigned char *)temp_s0_2 + (6)) >= 0x29) {
                temp_hi = func_800AE0D0() % 320;
                *(s32 *)((unsigned char *)temp_s1 + (-16)) = 0;
                *(s16 *)((unsigned char *)temp_s0_2 + (6)) = -0x78;
                *(s16 *)((unsigned char *)temp_s0_2 + (4)) = (s16) (temp_hi - 0x9F);
                *(s16 *)((unsigned char *)temp_s1 + (-14)) = (s16) *(s16 *)((unsigned char *)temp_s0_2 + (4));
                *(s32 *)((unsigned char *)temp_s3 + (-16)) = (s32) *(s32 *)((unsigned char *)temp_s1 + (-16));
                *(u8 *)((unsigned char *)temp_s0_2 + (14)) = (u8) ((func_800AE0D0() % 2) * 8);
                if (D_80122D04 == 1) {
                    *(u8 *)((unsigned char *)temp_s0_2 + (14)) = 0U;
                }
                if (*(u8 *)((unsigned char *)temp_s0_2 + (14)) == 8) {
                    *(&D_800E7325 + var_s2_2) = 0xC;
                    *(s16 *)((unsigned char *)temp_s4 + (-8)) = (s16) ((func_800AE0D0() % 2) + 3);
                } else {
                    *(&D_800E7325 + var_s2_2) = 9;
                    *(s16 *)((unsigned char *)temp_s4 + (-8)) = (s16) ((func_800AE0D0() % 2) + 1);
                }
                temp_v0_4 = *(s16 *)((unsigned char *)temp_s4 + (-8));
                *(s32 *)((unsigned char *)temp_s5 + (-16)) = (s32) (0x10000 / (s32) (((temp_v0_4 >= 3) * 2) + temp_v0_4 + 1));
            }
            var_s2_2 += 1;
        } while (var_s2_2 < temp_s6);
    }
    D_80146800 += 1;
}
