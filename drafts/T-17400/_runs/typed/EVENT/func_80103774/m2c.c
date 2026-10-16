void func_80103774(void) {
    ? sp68;
    DisplayActor44 *temp_s0;
    s16 var_s2;
    s32 temp_t8;
    u8 *temp_s1;
    u8 *temp_s1_2;
    u8 *temp_s3;
    u8 temp_v0;
    u8 var_v0;

    sp68.unk0 = (s32) D_80122990.unk0;
    *(s32 *)((unsigned char *)(&sp68) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80122990) + (4));
    *(s32 *)((unsigned char *)(&sp68) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80122990) + (12));
    *(s32 *)((unsigned char *)(&sp68) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80122990) + (8));
    *(s32 *)((unsigned char *)(&sp68) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80122990) + (16));
    *(s32 *)((unsigned char *)(&sp68) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_80122990) + (20));
    var_s2 = 0;
    *(s32 *)((unsigned char *)(&sp68) + (28)) = (s32) *(s32 *)((unsigned char *)(&D_80122990) + (28));
    *(s32 *)((unsigned char *)(&sp68) + (24)) = (s32) *(s32 *)((unsigned char *)(&D_80122990) + (24));
    do {
        temp_s3 = var_s2 + &D_8012274C;
        temp_v0 = *temp_s3;
        temp_s0 = &D_800E9620[var_s2];
        *(u8 *)((unsigned char *)temp_s0 + (6671)) = (u8) *(u8 *)((unsigned char *)D_800E9620 + (6535));
        if (temp_v0 == 0) {
            temp_s1 = &D_80122774 + var_s2;
            *temp_s1 += 1;
        }
        temp_s1_2 = &D_80122774 + var_s2;
        if ((temp_v0 == 0) && ((u8) *temp_s1_2 >= 0x79U) && ((u32) (0xB4 - *temp_s1_2) < (u32) (func_800789A0() % 60))) {
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
            temp_t8 = *(s32 *)((unsigned char *)temp_s0 + (6712)) + 0x4000;
            *(s32 *)((unsigned char *)temp_s0 + (6712)) = temp_t8;
            *(s32 *)((unsigned char *)temp_s0 + (6704)) = (s32) (*(s32 *)((unsigned char *)temp_s0 + (6704)) + temp_t8);
            if ((*(s16 *)((unsigned char *)(sp + (var_s2 * 4)) + (106)) - 0x78) < *(s16 *)((unsigned char *)temp_s0 + (6706))) {
                *(u8 *)((unsigned char *)temp_s0 + (6667)) = (u8) (*(u8 *)((unsigned char *)temp_s0 + (6667)) & 0x7F);
                *(s8 *)((unsigned char *)temp_s0 + (6666)) = 0;
                *(s16 *)((unsigned char *)temp_s0 + (6706)) = (s16) (*(&D_80122764 + (var_s2 * 2)) - 0x78);
                *(s32 *)((unsigned char *)temp_s0 + (6704)) = 0;
                *(s16 *)((unsigned char *)temp_s0 + (6688)) = 0;
                *(s16 *)((unsigned char *)temp_s0 + (6672)) = 0;
                *temp_s3 = 0;
                *(&D_80122774 + var_s2) = 0;
            }
        }
        var_s2 += 1;
    } while (var_s2 < 8);
}
