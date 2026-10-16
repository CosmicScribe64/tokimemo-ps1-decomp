extern ? D_8015F9A0;
extern ? D_8015F9A8;
extern ? D_8015FB1C;
extern ? D_8015FB2C;
extern ? D_8015FB3C;

void func_801560E0(void) {
    ? sp98;
    ? sp88;
    ? sp68;
    s16 var_s2;
    s32 temp_t6;
    u8 *temp_s1;
    u8 *temp_s1_2;
    u8 *temp_s3;
    u8 temp_v0;
    u8 var_v0;
    void *temp_s0;

    sp98.unk0 = (s32) D_8015FB1C.unk0;
    *(s32 *)((unsigned char *)(&sp98) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8015FB1C) + (4));
    *(s32 *)((unsigned char *)(&sp98) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8015FB1C) + (12));
    *(s32 *)((unsigned char *)(&sp98) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8015FB1C) + (8));
    sp88.unk0 = (s32) D_8015FB2C.unk0;
    *(s32 *)((unsigned char *)(&sp88) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8015FB2C) + (4));
    *(s32 *)((unsigned char *)(&sp88) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8015FB2C) + (12));
    *(s32 *)((unsigned char *)(&sp88) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8015FB2C) + (8));
    sp68.unk0 = (s32) D_8015FB3C.unk0;
    *(s32 *)((unsigned char *)(&sp68) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8015FB3C) + (4));
    *(s32 *)((unsigned char *)(&sp68) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8015FB3C) + (12));
    *(s32 *)((unsigned char *)(&sp68) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8015FB3C) + (8));
    *(s32 *)((unsigned char *)(&sp68) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_8015FB3C) + (16));
    *(s32 *)((unsigned char *)(&sp68) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_8015FB3C) + (20));
    var_s2 = 0;
    *(s32 *)((unsigned char *)(&sp68) + (28)) = (s32) *(s32 *)((unsigned char *)(&D_8015FB3C) + (28));
    *(s32 *)((unsigned char *)(&sp68) + (24)) = (s32) *(s32 *)((unsigned char *)(&D_8015FB3C) + (24));
    do {
        temp_s3 = var_s2 + &D_8015F9A0;
        temp_v0 = *temp_s3;
        temp_s0 = D_8011ECD0 + (var_s2 * 0x44);
        *(u8 *)((unsigned char *)temp_s0 + (6671)) = (u8) D_80120657;
        if (temp_v0 == 0) {
            temp_s1 = &D_8015F9A8 + var_s2;
            *temp_s1 += 1;
        }
        temp_s1_2 = &D_8015F9A8 + var_s2;
        if ((temp_v0 == 0) && ((u8) *temp_s1_2 >= 0x79U) && ((u32) (0xB4 - *temp_s1_2) < (u32) (func_800AE0D0() % 60))) {
            *(s8 *)((unsigned char *)temp_s0 + (6666)) = 5;
            *(u8 *)((unsigned char *)temp_s0 + (6667)) = (u8) (*(u8 *)((unsigned char *)temp_s0 + (6667)) | 0x80);
            *temp_s3 = 1;
        }
        var_v0 = *temp_s3;
        if (var_s2 == 0) {
            if ((var_v0 == 1) && (*(s16 *)((unsigned char *)temp_s0 + (6688)) == 7) && (*(s16 *)((unsigned char *)temp_s0 + (6672)) == 0)) {
                *(s32 *)((unsigned char *)temp_s0 + (6712)) = 0;
                *temp_s3 = 2;
                goto block_16;
            }
        } else if ((var_v0 == 1) && (*(s16 *)((unsigned char *)temp_s0 + (6688)) == 3) && (*(s16 *)((unsigned char *)temp_s0 + (6672)) == 7)) {
            *(s32 *)((unsigned char *)temp_s0 + (6712)) = 0;
            *temp_s3 = 2;
block_16:
            var_v0 = 2 & 0xFF;
        }
        if (var_v0 == 2) {
            temp_t6 = *(s32 *)((unsigned char *)temp_s0 + (6712)) + 0x4000;
            *(s32 *)((unsigned char *)temp_s0 + (6712)) = temp_t6;
            *(s32 *)((unsigned char *)temp_s0 + (6704)) = (s32) (*(s32 *)((unsigned char *)temp_s0 + (6704)) + temp_t6);
            if ((*(s16 *)((unsigned char *)(sp + (var_s2 * 4)) + (106)) - 0x78) < *(s16 *)((unsigned char *)temp_s0 + (6706))) {
                *(s8 *)((unsigned char *)temp_s0 + (6666)) = 0;
                *(u8 *)((unsigned char *)temp_s0 + (6667)) = (u8) (*(u8 *)((unsigned char *)temp_s0 + (6667)) & 0x7F);
                *(s16 *)((unsigned char *)temp_s0 + (6706)) = (s16) (*(s16 *)((unsigned char *)(sp + (var_s2 * 2)) + (136)) - 0x78);
                *(s32 *)((unsigned char *)temp_s0 + (6704)) = 0;
                *(s16 *)((unsigned char *)temp_s0 + (6688)) = 0;
                *(s16 *)((unsigned char *)temp_s0 + (6672)) = 0;
                *temp_s3 = 0;
                *(&D_8015F9A8 + var_s2) = 0;
            }
        }
        var_s2 += 1;
    } while (var_s2 < 8);
}
