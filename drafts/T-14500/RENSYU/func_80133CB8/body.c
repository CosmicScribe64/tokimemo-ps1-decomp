void func_80133CB8(void) {
    s32 *var_v0;
    s16 temp_t0;
    s16 temp_t2;
    s16 temp_t5;
    s16 temp_t6;
    u32 temp_t8;

    switch (D_800E738A) {
    case 0:
        func_80044750(0xBF);
        func_800674B0();
        func_8004284C();
        break;
    case 1:
        if (func_80044E8C() == 1) {
            if ((u8) D_801342C4 >= 3U) {
                temp_t8 = (u16) D_800E6374 >> 0xC;
                D_800E6280.unk_66C[temp_t8].unk_02 = ((*(u16 *)((unsigned char *)(&D_800E6280.unk_66C[temp_t8]) + (2)) + 1) & 0xF) | (D_800E6280.unk_66C[temp_t8].unk_02 & 0xFFF0);
                D_800E643E += 2;
                var_v0 = &D_800E62B8;
                do {
                    temp_t2 = *(s16 *)((unsigned char *)var_v0 + (502));
                    temp_t5 = *(s16 *)((unsigned char *)var_v0 + (558));
                    temp_t6 = *(s16 *)((unsigned char *)var_v0 + (614));
                    temp_t0 = *(s16 *)((unsigned char *)var_v0 + (446));
                    var_v0 += 0xE0;
                    *(s16 *)((unsigned char *)var_v0 + (390)) = (s16) (temp_t6 + 2);
                    *(s16 *)((unsigned char *)var_v0 + (334)) = (s16) (temp_t5 + 2);
                    *(s16 *)((unsigned char *)var_v0 + (278)) = (s16) (temp_t2 + 2);
                    *(s16 *)((unsigned char *)var_v0 + (222)) = (s16) (temp_t0 + 2);
                } while (var_v0 != &D_800E6558);
            } else {
                D_800E639A += 0xA;
                D_800E639E += 0xA;
            }
            if (D_801342D0 != 0xFF) {
                D_800E6280.unk_1BC[D_801342D0].unk_06 += 2;
            }
            func_8008585C();
            set_dec_bri(0);
            func_80072B5C(1);
        }
        break;
    default:
        func_80046500();
        break;
    }
    Default_Disp();
}
