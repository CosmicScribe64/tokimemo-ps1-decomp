void func_8013EA98(void) {
    s16 i;
    s16 count;
    u32 status;
    s32 flag;
    ShoShorts7 sp50;
    ShoShorts7 sp40;


    sp50 = D_8014610C;
    sp40 = D_8014611C;
    count = 0;
    for (i = 0; i < 11;) {
        count += D_800E6280.unk_1BC[i++].unk_0C.f.b1 >= 1;
    }
    status = 0;
    do {
        D_800E6280.unk_F5F = (D_800E6280.unk_10F8 + status) % 10U + 1;
        status = (s16)(status + 1);
        flag = D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.f.b1;
        if (flag || D_800E6280.unk_F5F < 9 + D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.f.b1 * 0) {
            if (*(u8 *)&D_800E6280.unk_F5F != 5 || (D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) != 8) {
                if (*(u8 *)&D_800E6280.unk_F5F != 6 || (D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) != 9) {
                    if ((*(u8 *)&D_80146054) != *(u8 *)&D_800E6280.unk_F5F && *(u8 *)&D_800E6280.unk_F5F != 11) {
                        if (flag) {
                            break;
                        }
                        if (count < 3) {
                            break;
                        }
                    }
                }
            }
        }
    } while (1);
    if (((*(u8 *)&D_80145FE4) == 7 || (*(u8 *)&D_80145FE4) == 9) && (*(u8 *)&D_80146054) != (*(u8 *)&D_80145FE4)) {
        D_800E6280.unk_F5F = (*(u8 *)&D_80145FE4);
    }
    D_80146058 = D_800E6280.unk_F5F;
    D_80145F2C = 0;
    status = (u8)func_80051A68(*(u8 *)&D_800E6280.unk_F5F);
    if (D_800E71DF == 6 && (status & 0x7F) >= 2U) {
        D_80145F2C++;
    }
    if ((status & 0x7F) == 4) {
        D_80145F2C++;
    }
    func_8013EE30();
    func_800847B8(D_800E71DF);
    if (!D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.f.b1) {
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.f.b1 = 1;
        D_800E6280.unk_720 = D_800E6280.unk_1108;
        D_800E6280.unk_721 = D_800E6280.unk_1109;
        D_800E6280.unk_722 = 0x18;
        D_80122D48 = 3;
        func_80085B3C(4, 0);
        func_80042908(5);
    } else {
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 += 4;
    for (i = 0; i < 7; i++) {
        if (D_800E6280.unk_114.unk_02 < sp50.v[i]) {
            break;
        }
    }
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_02 += sp40.v[i];
    func_80085B3C(4, 0);
    func_8004284C();
    }
    check_para_limit();
}
