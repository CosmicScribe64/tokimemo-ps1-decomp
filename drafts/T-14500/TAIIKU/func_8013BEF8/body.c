s32 func_8013BEF8(void) {
    s32 sp2C;
    s32 var_t7;
    u16 var_v1;

    func_8013C5C8(0x15000);
    func_8013C694();
    var_v1 = D_801499A2;
    var_t7 = var_v1 & 0xC;
    if (!(var_v1 & 4)) {
        func_8013C4E4(0xF000);
        var_v1 = D_801499A2;
        var_t7 = var_v1 & 0xC;
    }
    if (var_t7 == 0) {
        sp2C = 0;
        *(s16 *)((unsigned char *)(&sp2C) + (2)) = (s16) ((s16) *D_80149984 >> 3);
        sp2C += D_80149974.unk_00 * 0x50;
        D_801499A6 = (s16) (((s16) D_80149986 >> 3) + 0x10);
        *(s16 *)((unsigned char *)(&D_801499A6) + (-2)) = (s16) *(s16 *)((unsigned char *)(&sp2C) + (2));
    }
    if ((D_801499B2 != 0) && (var_v1 >= 0x41U)) {
        D_801499B2 = 0;
    }
    if ((D_80149990.unk_00 == 0) && !(var_v1 & 0x1F) && (D_801499B2 == 0)) {
        if (D_80149974.unk_00 > 0) {
            if (D_801206BA < (((s16) *D_80149984 >> 3) - 0x18)) {
                D_801499B2 = 1;
                D_801499A2 = 0;
                func_80044750(0x802);
            } else {
                func_80044750(0x801);
            }
        } else if (D_80149974.unk_00 < 0) {
            if ((((s16) *D_80149984 >> 3) + 0x18) < D_801206BA) {
                D_801499B2 = 1;
                D_801499A2 = 0;
                func_80044750(0x802);
            } else {
                func_80044750(0x800);
            }
        }
        var_v1 = (var_v1 + 1) & 0xFFFF;
    }
    D_801499A2 = var_v1;
    if (var_v1 >= 0x81U) {
        D_801499A2 = 0x40;
    }
}
