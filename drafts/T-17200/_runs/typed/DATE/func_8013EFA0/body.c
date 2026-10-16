void func_8013EFA0(void) {
    typedef struct { s32 w[181]; } M2cBlock2D4; /* m2c memcpy of 724 bytes */
    s32 sp2C[181];

    *(M2cBlock2D4 *)(&sp2C) = *(M2cBlock2D4 *)(&date_place_names);
    *(u16 *)((unsigned char *)(&sp2C + 0x2D0) + (4)) = (u16) *(u16 *)((unsigned char *)(&date_place_names + 0x2D0) + (4));
    func_80065F34(1);
    D_80122CD4 = (s32) D_800E6280.unk_F5F;
    if ((get_g_zyotai_s(D_800E6280.unk_F5F) & 0x7F) == 4) {
        D_8015BF3C = 8;
        D_800E6280.unk_110A += 0x24;
        return;
    }
    if (((D_800E6280.unk_724 == D_800E6280.unk0) && (date_arrival == 1)) || (D_800E6280.unk_726 == 0xFF) || ((D_800E6280.unk0 == D_800E6280.unk_726) && (D_800E6280.unk_727 == 0xFF)) || ((D_800E6280.unk0 == D_800E6280.unk_726) && (D_800E6280.unk0 == D_800E6280.unk_727))) {
        D_800E6280.unk_110A += 0x11;
        return;
    }
    if (D_800E6280.unk_724 == D_800E6280.unk_726) {
        func_800AE0F0(D_800CA25C, (D_800E6280.unk0 * 0x21) + &sp2C);
    } else {
        func_800AE0F0(D_800CA25C, (D_800E6280.unk_726 * 0x21) + &sp2C);
    }
    if (D_800E6280.unk_727 != 0xFF) {
        D_8015BF3C = 3;
        if (D_800E6280.unk_724 == D_800E6280.unk_727) {
            func_800AE0F0(&D_800CA280, (D_800E6280.unk_725 * 0x21) + &sp2C);
        } else {
            func_800AE0F0(&D_800CA280, (D_800E6280.unk_727 * 0x21) + &sp2C);
        }
    }
    func_8004284C();
}
