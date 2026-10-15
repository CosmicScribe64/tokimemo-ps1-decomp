? func_80085ED0();                                  /* extern */
s32 func_80085EEC();                                /* extern */
? func_800862D0();                                  /* extern */
s32 func_800862F0();                                /* extern */
? func_80086350();                                  /* extern */
s32 func_80086370();                                /* extern */
extern ? D_800E62B8;
extern s8 D_800E62BA;
extern s16 D_800E643E;
extern ? D_800E6558;
extern u8 D_800E738A;
extern u8 D_801499D5;

void func_80139610(void) {
    s32 sp38;
    u8 *sp34;
    ? *var_v0_2;
    ? *var_v0_3;
    s16 temp_t0;
    s16 temp_t0_2;
    s16 temp_t2;
    s16 temp_t2_2;
    s16 temp_t4;
    s16 temp_t4_2;
    s16 temp_t6;
    s16 temp_t8;
    s32 var_v1;
    u8 *var_v0;

    switch (D_800E738A) {
    case 0:
        if (D_80149954 < 0x80) {
            func_80048E78();
            D_80149954 = 0;
            func_800862D0();
            D_800E62BA = 0;
            func_8004284C();
        }
        func_8013FEF0();
        func_8013DDB8();
        func_8013D788();
        return;
    case 1:
        func_801400D8();
        func_80086350();
        func_80133258();
        func_8004284C();
        return;
    case 2:
        if (func_80086370() != 0) {
            if ((*D_801499D4 == 0) || (D_801499D5 == 0)) {
                func_80044750(0x205);
            } else {
                func_80044750(0x206);
            }
            func_80085ED0();
            func_8004284C();
        }
        func_80085F0C(0, 0xF, 0, 0xF0, 0xC0);
        return;
    case 3:
        var_v1 = 0;
        if (func_80085EEC() >= 0x5B) {
            var_v0 = D_801499D4;
            do {
                if (*var_v0 == 0) {
                    sp34 = var_v0;
                    sp38 = var_v1;
                    func_8013384C(D_80149940, var_v1 & 0xFF);
                }
                var_v1 += 1;
                var_v0 += 1;
            } while (var_v1 != 4);
            func_80140338();
            func_8014038C();
            D_801499D8 = 0;
            func_80085ED0();
            func_8004284C();
        }
        func_80085F0C(0, 0xF, 0, 0xF0, 0xC0);
        return;
    case 4:
        D_801499D8 += 1;
        func_80140414();
        if (func_80085EEC() >= 0xB5) {
            func_80085ED0();
            func_8004284C();
        }
        func_80085F0C(0, 0xF, 0, 0xF0, 0xC0);
        return;
    case 5:
        if (func_80085EEC() >= 0xF1) {
            func_800862D0();
            func_8004284C();
        }
        func_80085F0C(0, 0xF, 0, 0xF0, 0xC0);
        return;
    case 6:
        if (func_800862F0() != 0) {
            if (*D_801499D4 == 0) {
                D_800E643E += 4;
                var_v0_2 = &D_800E62B8;
                do {
                    temp_t2 = *(s16 *)((unsigned char *)var_v0_2 + (502));
                    temp_t4 = *(s16 *)((unsigned char *)var_v0_2 + (558));
                    temp_t6 = *(s16 *)((unsigned char *)var_v0_2 + (614));
                    temp_t0 = *(s16 *)((unsigned char *)var_v0_2 + (446));
                    var_v0_2 += 0xE0;
                    *(s16 *)((unsigned char *)var_v0_2 + (390)) = (s16) (temp_t6 + 4);
                    *(s16 *)((unsigned char *)var_v0_2 + (334)) = (s16) (temp_t4 + 4);
                    *(s16 *)((unsigned char *)var_v0_2 + (278)) = (s16) (temp_t2 + 4);
                    *(s16 *)((unsigned char *)var_v0_2 + (222)) = (s16) (temp_t0 + 4);
                } while (var_v0_2 != &D_800E6558);
                D_800E6280.unk_1BC[D_80149940].unk_02 += 2;
                D_800E6280.unk_1BC[D_80149940].unk_06 += 0xA;
            } else if (D_801499D5 == 0) {
                D_800E643E += 2;
                var_v0_3 = &D_800E62B8;
                do {
                    temp_t0_2 = *(s16 *)((unsigned char *)var_v0_3 + (502));
                    temp_t2_2 = *(s16 *)((unsigned char *)var_v0_3 + (558));
                    temp_t4_2 = *(s16 *)((unsigned char *)var_v0_3 + (614));
                    temp_t8 = *(s16 *)((unsigned char *)var_v0_3 + (446));
                    var_v0_3 += 0xE0;
                    *(s16 *)((unsigned char *)var_v0_3 + (390)) = (s16) (temp_t4_2 + 2);
                    *(s16 *)((unsigned char *)var_v0_3 + (334)) = (s16) (temp_t2_2 + 2);
                    *(s16 *)((unsigned char *)var_v0_3 + (278)) = (s16) (temp_t0_2 + 2);
                    *(s16 *)((unsigned char *)var_v0_3 + (222)) = (s16) (temp_t8 + 2);
                } while (var_v0_3 != &D_800E6558);
                D_800E6280.unk_1BC[D_80149940].unk_02 += 1;
                D_800E6280.unk_1BC[D_80149940].unk_06 += 5;
            }
            func_80042808();
        }
        func_80085F0C(0, 0xF, 0, 0xF0, 0xC0);
        return;
    default:
        func_80046500();
        return;
    }
}
