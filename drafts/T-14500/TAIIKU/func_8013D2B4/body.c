void func_8013D2B4(void) {
    s32 sp90;
    s16 sp8C;
    s16 sp8A;
    s16 sp88;
    s32 sp84;
    s32 sp80;
    s32 sp7C;
    s32 sp68[8];
    s32 sp48[8];
    s32 sp40;
    s32 sp3C;
    s32 sp38;
    s16 sp36;
    s16 var_v1;
    s32 temp_v0;

    temp_v0 = D_800E6375 & 0xF;
    var_v1 = D_800E638E;
    if ((temp_v0 & 1) || (temp_v0 & 2)) {
        var_v1 = (s16) ((s16) D_800E638E >> 1);
    }
    sp36 = var_v1;
    sp90 = func_800AE0C0(D_80149978 * 8);
    if ((D_80149998 > 0) && (D_801499AC > 0)) {
        if (var_v1 >= 0xA0) {
            sp90 = func_800AE0C0(D_80149978 * 0x10);
        } else if (var_v1 >= 0x64) {
            sp90 += func_800AE0C0(D_80149974.unk_00 * 2) * 3;
        } else if (var_v1 >= 0x4B) {
            sp90 += func_800AE0C0(D_80149974.unk_00 * 4);
        } else if (var_v1 >= 0x32) {
            sp90 += func_800AE0C0(D_80149974.unk_00 * 2);
        }
    } else if (D_80149940 == 2) {
        if (var_v1 >= 0x190) {
            sp90 = func_800AE0C0(D_80149978 * 0x10);
        } else if (var_v1 >= 0x12C) {
            sp90 += func_800AE0C0(D_80149974.unk_00 * 2) * 3;
        } else if (var_v1 >= 0xC8) {
            sp90 += func_800AE0C0(D_80149974.unk_00 * 4);
        } else if (var_v1 >= 0x64) {
            sp90 += func_800AE0C0(D_80149974.unk_00 * 2);
        }
    }
    if (D_80149990.unk_00 == 0) {
        if (D_80149940 == 6) {
            *(s16 *)((unsigned char *)(&sp90) + (2)) = (s16) (*(s16 *)((unsigned char *)(&sp90) + (2)) + 8);
        }
        if (D_80149940 == 0xA) {
            *(s16 *)((unsigned char *)(&sp90) + (2)) = (s16) (*(s16 *)((unsigned char *)(&sp90) + (2)) + 4);
        }
    }
    if (D_80149990.unk_00 == 0) {
        D_80149958 -= *(s16 *)((unsigned char *)(&sp90) + (2));
    } else if ((D_8014994C == 4) && (D_80149950 == 4)) {
        if (D_80149990.unk_00 == 1) {
            D_80149958 += 8;
            *(s16 *)((unsigned char *)(&D_80149958) + (2)) = 0x400;
        } else {
            D_80149958 += 8;
            *(s16 *)((unsigned char *)(&D_80149958) + (2)) = -0x400;
        }
    }
    (*(s32 *)&sp68) = (s32) (*(s32 *)D_80125C60);
    *(s32 *)((unsigned char *)(&sp68) + (4)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (4));
    *(s32 *)((unsigned char *)(&sp68) + (8)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (8));
    *(s32 *)((unsigned char *)(&sp68) + (12)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (12));
    *(s32 *)((unsigned char *)(&sp68) + (16)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (16));
    *(s32 *)((unsigned char *)(&sp68) + (20)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (20));
    *(s32 *)((unsigned char *)(&sp68) + (24)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (24));
    *(s32 *)((unsigned char *)(&sp68) + (28)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (28));
    sp88 = 0;
    sp8C = 0;
    sp8A = D_8014995A;
    func_800A09D0(&sp88, &sp68);
    (*(s32 *)&sp48) = (s32) (*(s32 *)D_80125C60);
    *(s32 *)((unsigned char *)(&sp48) + (4)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (4));
    *(s32 *)((unsigned char *)(&sp48) + (8)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (8));
    *(s32 *)((unsigned char *)(&sp48) + (12)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (12));
    *(s32 *)((unsigned char *)(&sp48) + (16)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (16));
    *(s32 *)((unsigned char *)(&sp48) + (20)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (20));
    *(s32 *)((unsigned char *)(&sp48) + (24)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (24));
    *(s32 *)((unsigned char *)(&sp48) + (28)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (28));
    sp8A = 0;
    sp8C = 0;
    sp88 = D_80149958;
    func_800A09D0(&sp88, &sp48);
    func_800A0C64(&sp68, &sp48);
    sp38 = 0x1200;
    sp3C = 0x1200;
    sp40 = 0x1200;
    func_800A0F4C(&sp48, &sp38);
    func_800A0C64(&sp68, &sp48);
    sp84 = 0x1B58;
    sp7C = (s32) *D_80149984;
    sp80 = (s32) D_80149986;
    (*(s32 *)D_801227A4) = (s32) (*(s32 *)&sp68);
    *(s32 *)((unsigned char *)D_801227A4 + (4)) = (s32) *(s32 *)((unsigned char *)(&sp68) + (4));
    *(s32 *)((unsigned char *)D_801227A4 + (12)) = (s32) *(s32 *)((unsigned char *)(&sp68) + (12));
    *(s32 *)((unsigned char *)D_801227A4 + (8)) = (s32) *(s32 *)((unsigned char *)(&sp68) + (8));
    *(s32 *)((unsigned char *)D_801227A4 + (16)) = (s32) *(s32 *)((unsigned char *)(&sp68) + (16));
    *(s32 *)((unsigned char *)D_801227A4 + (20)) = (s32) *(s32 *)((unsigned char *)(&sp68) + (20));
    *(s32 *)((unsigned char *)D_801227A4 + (28)) = (s32) *(s32 *)((unsigned char *)(&sp68) + (28));
    *(s32 *)((unsigned char *)D_801227A4 + (24)) = (s32) *(s32 *)((unsigned char *)(&sp68) + (24));
    D_801227A0->unk0 = 0;
}
