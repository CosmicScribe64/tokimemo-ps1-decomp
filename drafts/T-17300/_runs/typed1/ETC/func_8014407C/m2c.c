s32 func_800AE0D0();                                /* extern */
extern ? D_800E6280;
extern ? D_801217D0;

void func_8014407C(s32 arg0) {
    s16 temp_t8;
    s16 var_s2;
    s32 temp_hi;
    s32 var_s1;
    void *var_s0;

    var_s0 = &D_801217D0 + 0x48;
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
    *(s8 *)((unsigned char *)(&D_800E6280) + (4263)) = 0xFF;
    *(s8 *)((unsigned char *)(&D_800E6280) + (4264)) = 0xFE;
    *(s8 *)((unsigned char *)(&D_800E6280) + (4265)) = 0xFF;
    if (arg0 == 0xE) {
        *(s16 *)((unsigned char *)(&D_801217D0) + (78)) = -0x18;
        *(s16 *)((unsigned char *)(&D_801217D0) + (114)) = -0x64;
        *(s16 *)((unsigned char *)(&D_801217D0) + (150)) = -0x28;
    }
}
