void func_8013B200(void) {
    s32 sp2C;
    s32 temp_v0;
    s32 var_at;
    u16 var_a1;

    func_8013C5C8(0x18000);
    func_8013C694();
    var_a1 = D_801499A2;
    if (!(var_a1 & 4)) {
        func_8013C4E4(0x10000);
        var_a1 = D_801499A2;
    }
    var_at = var_a1 < 0x23AU;
    if ((var_a1 % 6U) == 0) {
        temp_v0 = (s16) D_80149984 >> 3;
        sp2C = 0;
        *(s16 *)((unsigned char *)(&sp2C) + (2)) = (s16) temp_v0;
        sp2C += D_80149974 * 0x28;
        *(s16 *)((unsigned char *)(&sp2C) + (2)) = (s16) (*(s16 *)((unsigned char *)(&sp2C) + (2)) + ((s32) (temp_v0 * 0x28) / 48));
        if ((((s32) D_80122EC4 % 3) != 0) && ((D_80149984 >= 0) || (D_80149974 > 0))) {
            *(s16 *)((unsigned char *)(&sp2C) + (2)) = (s16) (temp_v0 + 0x20);
        }
        D_801499A6 = ((s16) D_80149986 >> 3) + 0x10;
        D_801499A6 = *(s16 *)((unsigned char *)(&sp2C) + (2));
        var_at = var_a1 < 0x23AU;
    }
    if (var_at == 0) {
        D_801499B2 = 0;
        func_80044750(0x802);
        var_a1 = 0;
    }
    D_801499A2 = var_a1;
    if (var_a1 != 0) {
        D_801499A2 = var_a1;
        if ((var_a1 % 30U) == 0) {
            if (D_801499B2 == 0) {
                D_801499B2 = 1;
                func_80044750(0x800);
            } else {
                D_801499B2 = 0;
                func_80044750(0x801);
            }
            D_801499A2 += 1;
        }
    }
}
