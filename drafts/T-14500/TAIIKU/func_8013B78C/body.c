s32 func_8013B78C(void) {
    s32 sp2C;
    s32 var_t7;
    u16 temp_v1;
    u16 var_v1;

    var_v1 = D_801499A2;
    if (var_v1 >= 0x230U) {
        D_801499B2 = 0;
        func_80044750(0x802);
        var_v1 = 0;
    }
    D_801499A2 = var_v1;
    func_8013C5C8(0x15000);
    func_8013C694();
    temp_v1 = D_801499A2;
    var_t7 = temp_v1 & 0xC;
    if (!(temp_v1 & 4)) {
        func_8013C4E4(0xF000);
        var_t7 = D_801499A2 & 0xC;
    }
    if (var_t7 == 0) {
        if ((temp_v1 >= 0x227U) && (temp_v1 < 0x258U)) {
            if ((D_801206BA + 0x1E) < D_80120676) {
                D_801499A4 = D_80120676 - 0x1E;
            } else if (D_80120676 < (D_801206BA - 0x1E)) {
                D_801499A4 = D_80120676 + 0x1E;
            }
            D_801499A6 = (s16) D_8012067A;
        } else {
            sp2C = 0;
            *(s16 *)((unsigned char *)(&sp2C) + (2)) = (s16) ((s16) *D_80149984 >> 3);
            sp2C += D_80149974.unk_00 * 0x50;
            D_801499A6 = (s16) (((s16) D_80149986 >> 3) + 0x10);
            *(s16 *)((unsigned char *)(&D_801499A6) + (-2)) = (s16) *(s16 *)((unsigned char *)(&sp2C) + (2));
        }
    }
    D_801499A2 = temp_v1;
    if (temp_v1 != 0) {
        D_801499A2 = temp_v1;
        if ((temp_v1 % 35U) == 0) {
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
