s32 func_800AE0D0();                                /* extern */
extern s8 D_800E7327;
extern s8 D_800E7328;
extern s8 D_800E7329;
extern ? D_80121818;
extern s16 D_8012181E;
extern s16 D_80121842;
extern s16 D_80121866;

void func_8014407C(s32 arg0) {
    ? *var_s0;
    s16 temp_t8;
    s16 var_s2;
    s32 temp_hi;
    s32 var_s1;

    var_s0 = &D_80121818;
    var_s1 = 2;
    var_s2 = -0xB0;
    do {
        var_s0->unk0 = 0;
        *(s16 *)((unsigned char *)var_s0 + (4)) = var_s2;
        temp_hi = func_800AE0D0() % 112;
        temp_t8 = var_s1 + 3;
        var_s1 += 1;
        var_s0 += 0x24;
        var_s2 += 0x6C;
        *(s16 *)((unsigned char *)var_s0 + (-28)) = 0x80;
        *(s16 *)((unsigned char *)var_s0 + (-26)) = 0x80;
        *(s16 *)((unsigned char *)var_s0 + (-24)) = temp_t8;
        *(s8 *)((unsigned char *)var_s0 + (-22)) = 0;
        *(s8 *)((unsigned char *)var_s0 + (-21)) = 0;
        *(s16 *)((unsigned char *)var_s0 + (-20)) = 0;
        *(s16 *)((unsigned char *)var_s0 + (-18)) = 0x1F1;
        *(s8 *)((unsigned char *)var_s0 + (-16)) = 0;
        *(s8 *)((unsigned char *)var_s0 + (-15)) = 0;
        *(s8 *)((unsigned char *)var_s0 + (-14)) = 0;
        *(s16 *)((unsigned char *)var_s0 + (-12)) = 0;
        *(s16 *)((unsigned char *)var_s0 + (-10)) = 0;
        *(s16 *)((unsigned char *)var_s0 + (-8)) = 0x1000;
        *(s16 *)((unsigned char *)var_s0 + (-30)) = (s16) (temp_hi - 0x78);
        *(s16 *)((unsigned char *)var_s0 + (-6)) = 0x1000;
        *(s32 *)((unsigned char *)var_s0 + (-4)) = 0;
    } while (var_s1 != 5);
    D_800E7327 = 0xFF;
    D_800E7328 = 0xFE;
    D_800E7329 = 0xFF;
    if (arg0 == 0xE) {
        D_8012181E = -0x18;
        D_80121842 = -0x64;
        D_80121866 = -0x28;
    }
}
