s32 func_8013BCEC(void) {
    s16 var_v0;
    s32 temp_v1;
    s32 var_at;
    s32 var_t4;

    if (D_801499A2 == 0x12C) {
        D_801499B0->unk0 = 1;
        if (D_801499B2 == 0) {
            D_801499B2 = 1;
            func_80044750(0x800);
        } else {
            D_801499B2 = 0;
            func_80044750(0x801);
        }
        D_801499A8 = 0;
        *(s32 *)((unsigned char *)(&D_801499A8) + (4)) = 0;
        D_801206AC = 0x7C;
        D_801499A2 += 1;
        func_8013C694();
        return;
    }
    if (D_801499A2 == 0x12F) {
        D_801206AC = 0x7D;
        func_8013C694();
        return;
    }
    if (D_801499A2 == 0x131) {
        D_801206AC = 0x7E;
        func_8013C694();
        return;
    }
    if ((u16) D_801499A2 < 0x1A5U) {
        var_at = (u16) D_801499A2 < 0x1A5U;
        if ((u16) D_801499A2 >= 0x12DU) {
            func_8013C694();
            return;
        }
        goto block_13;
    }
    var_at = (u16) D_801499A2 < 0x1A5U;
block_13:
    if (var_at == 0) {
        func_8013C694();
        D_801499B0->unk0 = 0;
        D_801499A2 = 0;
        return;
    }
    D_801499A2 = D_801499A2;
    func_8013C5C8(0x12000);
    func_8013C694();
    var_t4 = D_801499A2 & 0xF;
    if (!(D_801499A2 & 8)) {
        func_8013C4E4(0xE000);
        var_t4 = D_801499A2 & 0xF;
    }
    if (var_t4 == 0) {
        temp_v1 = (s16) *D_80149984 >> 3;
        var_v0 = temp_v1 + ((s32) (temp_v1 * 0x4F) / 48);
        if ((((s32) D_80122EC4 % 3) != 0) && ((*D_80149984 >= 0) || (D_80149974.unk_00 > 0))) {
            var_v0 = temp_v1 + 0x20;
        }
        D_801499A4 = var_v0;
        D_801499A4 = ((s16) D_80149986 >> 3) + 0x10;
    }
}
