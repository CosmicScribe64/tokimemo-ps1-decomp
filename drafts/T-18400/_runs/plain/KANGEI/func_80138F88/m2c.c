s32 func_80052648(u8);                              /* extern */
s32 func_80052C88(u8);                              /* extern */
? func_800639D8();                                  /* extern */
? func_80065048(s16, ?);                            /* extern */
extern ? D_800E6448;
extern u8 D_800E71DF;
extern s8 D_800E7322;
extern s32 D_800E7368;
extern u8 D_800E738A;
extern KObj *D_80139A74;
extern s32 D_80139A94;
extern s32 D_80139AB4;
extern KObj *D_8013A19C;
extern s32 D_8013A1D0;
extern s32 D_8013A204;

s32 func_80138F88(void) {
    s16 var_a0;
    s16 var_s0_2;
    s32 temp_s2;
    s32 temp_s3;
    s32 temp_s4;
    s32 temp_v1;
    s32 var_t6;
    u32 temp_a0;
    u32 temp_t5;
    u32 var_s6;
    u8 var_s0;
    u8 var_s7;

    var_s7 = 0xFF;
    var_s6 = 4;
    var_s0 = 0;
    var_t6 = 0 * 8;
    do {
        if ((*(&D_800E6448 + ((var_t6 - var_s0) * 8)) & 2) && ((temp_v1 = func_80051A68(var_s0) & 0x7F, temp_a0 = temp_v1 & 0xFF, ((temp_a0 < var_s6) != 0)) || ((temp_a0 == var_s6) && (temp_s2 = func_80052C88(var_s0) & 0xFF, temp_s3 = func_80052648(var_s0) & 0xFF, temp_s4 = func_80052648(var_s7) & 0xFF, (((u32) (func_80052C88(var_s7) + temp_s4) < (u32) (temp_s3 + temp_s2)) != 0))))) {
            var_s7 = var_s0 & 0xFF;
            var_s6 = temp_v1 & 0xFF & 0xFF;
        }
        var_s0 = (u8) (s16) (var_s0 + 1);
        var_t6 = var_s0 * 8;
    } while ((s32) var_s0 < 0xB);
    temp_t5 = func_80051A68(var_s7) & 0x7F;
    if ((var_s7 != 0xFF) && (temp_t5 < 2U)) {
        D_800E71DF = var_s7;
        func_80085B3C(0xD, 0x2A);
        D_800E71DF = var_s7;
        func_800438DC(1, 0);
        func_80048DAC(1);
        func_80041584();
        func_800438F0(1);
        func_80048EB8(0);
        func_80064E84();
        func_800649D4();
        func_80048390();
        func_8004E58C();
        D_800E7322 = 0;
        D_800E7368 = 1;
        D_800B593C = 0;
        func_8008585C();
        func_80063930(0);
        func_80064FA4(0);
        D_80139ADC = 0;
        D_80139AE0 = 0;
        func_8007C740();
        func_80084E4C();
        func_800847B8(D_800E71DF);
        func_80132E74();
        D_80139AD0 = D_80139A74;
        D_80139AD4 = D_80139A94;
        D_80139AD8 = D_80139AB4;
        D_80139AC0 = 0;
        D_80139ADC = 0;
        D_80139AE0 = 0;
        func_800847B8(D_800E71DF);
        func_801392D4();
        if (((u8) D_80139AC4 != 0) && ((u8) D_8013A2A8 == 0)) {
            D_800E738A += 2;
        }
        return func_8004284C();
    }
    func_8008585C();
    D_80139ADC = 0;
    D_80139AE0 = 0;
    D_80139AD0 = D_8013A19C;
    D_80139AD4 = D_8013A1D0;
    D_80139AD8 = D_8013A204;
    func_8006509C();
    var_s0_2 = 0;
    var_a0 = 0;
    do {
        func_80065048(var_a0, 0);
        var_s0_2 += 1;
        var_a0 = var_s0_2;
    } while (var_s0_2 < 0xF);
    func_80065B0C(1);
    func_800639D8();
    func_800639D8();
    func_800847B8(0U);
    D_80139AC0 = 1;
    D_800E738A += 0x1E;
    return 0;
}
