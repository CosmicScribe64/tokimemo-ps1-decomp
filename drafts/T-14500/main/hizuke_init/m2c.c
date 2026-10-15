extern u8 D_800E62BE;
extern u8 D_800E62BF;
extern u8 D_800E62C0;
extern u8 D_800E62C1;
extern ? D_800E62C4;
extern s16 D_8011F128;
extern s16 D_8011F136;
extern s16 D_8011F13A;
extern s16 D_8011F16C;
extern s16 D_8011F17A;
extern s16 D_8011F17E;
extern s16 D_8011F1B0;
extern s16 D_8011F1BE;
extern s16 D_8011F1C2;
extern s8 D_8011F1DF;
extern s16 D_8011F1F4;
extern s16 D_8011F202;
extern s16 D_8011F206;
extern s16 D_8011F238;
extern s16 D_8011F246;
extern s16 D_8011F24A;
extern s8 D_8011F267;
extern s16 D_8011F27C;
extern s16 D_8011F28A;
extern s16 D_8011F28E;
extern s16 D_8011F2C0;
extern s16 D_8011F2CE;
extern s16 D_8011F2D2;
extern s16 D_8011F304;
extern s16 D_8011F312;
extern s16 D_8011F316;
extern s8 D_8011F32F;

void hizuke_init(void) {
    s32 var_s1;
    u8 *var_s0;
    u8 temp_v0;

    var_s0 = D_8011ECD0;
    var_s1 = 0;
    do {
        func_80048F64(var_s1 + 0x10);
        *(s8 *)((unsigned char *)var_s0 + (1089)) = 4;
        *(s8 *)((unsigned char *)var_s0 + (1090)) = 0x40;
        *(s32 *)((unsigned char *)var_s0 + (1144)) = 0x40000000;
        *(s8 *)((unsigned char *)var_s0 + (1091)) = 0x84;
        *(u8 **)((unsigned char *)var_s0 + (1100)) = sprite_frame_table;
        *(u8 **)((unsigned char *)var_s0 + (1104)) = D_800C9A14;
        *(u8 **)((unsigned char *)var_s0 + (1140)) = D_800C9730;
        var_s1 += 1;
        var_s0 += 0x44;
        *(s8 *)((unsigned char *)var_s0 + (1024)) = 8;
        *(s8 *)((unsigned char *)var_s0 + (1025)) = 1;
        *(s16 *)((unsigned char *)var_s0 + (1040)) = (s16) D_800C9A54;
    } while (var_s1 != 8);
    D_8011F16C = func_80046590((s16) D_800E62BE, 2U) + 0x44;
    D_8011F17A = 0x8C;
    D_8011F17E = -0x41;
    D_8011F1B0 = func_80046590((s16) D_800E62BE, 1U) + 0x44;
    D_8011F1BE = 0x94;
    D_8011F1C2 = -0x41;
    if ((u8) D_800E62BF >= 0xAU) {
        D_8011F1F4 = func_80046590((s16) D_800E62BF, 2U) + 0x44;
        D_8011F202 = 0x8C;
        D_8011F206 = -0x22;
        D_8011F238 = func_80046590((s16) D_800E62BF, 1U) + 0x44;
        D_8011F246 = 0x94;
        D_8011F24A = -0x22;
    } else {
        D_8011F1DF = 0;
        D_8011F1F4 = func_80046590((s16) D_800E62BF, 2U) + 0x44;
        D_8011F202 = 0xFF;
        D_8011F206 = 0xFF;
        D_8011F238 = func_80046590((s16) D_800E62BF, 1U) + 0x44;
        D_8011F246 = 0x90;
        D_8011F24A = -0x22;
    }
    if ((u8) D_800E62C0 >= 0xAU) {
        D_8011F27C = func_80046590((s16) D_800E62C0, 2U) + 0x44;
        D_8011F28A = 0x8C;
        D_8011F28E = -1;
        D_8011F2C0 = func_80046590((s16) D_800E62C0, 1U) + 0x44;
        D_8011F2CE = 0x94;
        D_8011F2D2 = -1;
    } else {
        D_8011F267 = 0;
        D_8011F27C = func_80046590((s16) D_800E62C0, 2U) + 0x44;
        D_8011F28A = 0xFF;
        D_8011F28E = 0xFF;
        D_8011F2C0 = func_80046590((s16) D_800E62C0, 1U) + 0x44;
        D_8011F2CE = 0x90;
        D_8011F2D2 = -1;
    }
    D_8011F304 = D_800E62C1 + 0x52;
    D_8011F312 = 0x92;
    D_8011F316 = 0x1F;
    if (D_800E62C1 == 5) {
        D_8011F32F = 0x43;
    } else if (D_800E62C1 == 6) {
        D_8011F32F = 0x44;
    } else {
        D_8011F32F = 0;
    }
    temp_v0 = *(&D_800E62C4 + D_800E62C0);
    if ((temp_v0 == 1) || (temp_v0 == 2)) {
        D_8011F32F = 0x44;
    }
    D_8011F128 = 0x3C;
    D_8011F136 = 0x90;
    D_8011F13A = -0x12;
}
