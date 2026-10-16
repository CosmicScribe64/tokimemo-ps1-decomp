extern s16 D_8013B5E8;
extern s16 D_8013B5EA;
extern s32 D_8013B5E6;
extern s8 D_800E7313;
extern u8 D_800E738D;
void func_801385CC(void) {
    typedef struct { s32 w[21]; } M2cBlock54; /* m2c memcpy of 84 bytes */
    s32 sp8F[1];
    s32 sp7A[1];
    s32 sp65[1];
    s32 sp50[21];
    s16 sp4E;
    s16 sp4C;
    s16 sp4A;
    s16 sp48;
    s16 sp46;
    s16 sp44;
    s16 sp42;
    s16 sp40;
    s16 sp3E;
    s16 sp3C;
    s16 sp3A;
    s16 sp38;
    s32 var_t1;
    s8 var_v1;

    *(M2cBlock54 *)(&sp50) = *(M2cBlock54 *)(&D_8013B634);
    sp40 = 0x34;
    sp48 = -0x80;
    sp42 = 0x34;
    sp4A = 0;
    sp44 = 0x4C;
    sp4C = -0x80;
    sp46 = 0x4C;
    sp4E = -0x20;
    D_8013B5E6 = (s16) ((func_800AE0E0(&sp65) / 2) * 0x10);
    sp3C = 0x10;
    sp3A = 0x10;
    sp38 = 0x10;
    *(s16 *)((unsigned char *)(&D_8013B5E6) + (-2)) = 0;
    *(s16 *)((unsigned char *)(&D_8013B5E6) + (2)) = 0;
    *(s16 *)((unsigned char *)(&D_8013B5E6) + (4)) = 0;
    sp3E = 0x10;
    k_reset(1);
    if ((D_800E7D34 & 1) || (D_8013B5CC != 0)) {
        if ((u8) D_8013B5CC >= 2U) {
            func_800AE0F0(&sp50, "恋は…（歌付き）");
        }
        D_8013B5E4 = (func_800AE0E0(&sp50) / 2) * 0x10;
        k_disp_start(set_kanji_string(sp48, sp40, 0, (s32) &sp50, 0));
    }
    k_disp_start(set_kanji_string(sp4A, sp42, 0, (s32) &sp65, 0));
    if ((D_800E7D34 & 4) || (D_8013B5CC != 0)) {
        D_8013B5E8 = (func_800AE0E0(&sp7A) / 2) * 0x10;
        k_disp_start(set_kanji_string(sp4C, sp44, 0, (s32) &sp7A, 0));
    }
    if ((D_800E7D34 & 8) || (D_8013B5CC != 0)) {
        if (D_8013B5CC >= 2U) {
            func_800AE0F0(&sp8F, "女々しい…（歌付き）");
        }
        D_8013B5EA = (func_800AE0E0(&sp8F) / 2) * 0x10;
        k_disp_start(set_kanji_string(sp4E, sp46, 0, (s32) &sp8F, 0));
    }
    menu_set(0, 4, &sp48, &sp40, &D_8013B5E4, &sp38);
    D_80122D30 = 1;
    var_v1 = 0;
    var_t1 = 0 * 2;
loop_14:
    if (*(&D_8013B5E4 + var_t1) != 0) {
        D_800E7313 = var_v1;
    } else {
        var_v1 = (s8) (s16) (var_v1 + 1);
        var_t1 = var_v1 * 2;
        if (var_v1 < 4) {
            goto loop_14;
        }
    }
    D_800E738D += 1;
}
