s32 func_8013842C(void) {
    BsdEvent *list;
    BsdEvent *ev;
    s32 i;
    s32 cmd;
    s32 slot;
    s32 img;

    if (D_800E6280.unk_1100 == 0) {
        D_8013D9A4 = 0;
        if (D_8013D9A0 != 0) {
            switch (D_8013D9A0) {
            case 1:
                D_800E6280.unk_1104.w = 0;
                break;
            case 2:
                D_8013D998++;
                D_8013D99C = 0;
                D_8013D9A0 = 0;
                D_8013D994 = 1;
                func_8004E9F4(1);
                func_8004284C();
                return 1;
            }
        }
    }
    list = D_8013D90C[D_8013D998];
    if (D_8013D9A0 == 0) {
        D_8013D9A0 = 1;
    }
    if (D_8013D9A0 == 1) {
        for (i = D_8013D994; i < 0xFFFF; i++) {
            ev = &list[i];
            if (D_8013D99C == ev->unk_00) {
                cmd = ev->unk_04;
                if (cmd == 0x0FFFFFFF) {
                    D_8013D9A0 = 2;
                    i = 0xFFFF;
                } else {
                    slot = (cmd & 0xFF0000) >> 16;
                    img = cmd & 0xFF;
                    if (img == 0xFE) {
                        func_80138810(slot, 0x50, 0);
                    } else {
                        func_80138810(slot, img, 1);
                    }
                    if (ev->unk_04 & 0xF0000000) {
                        func_80138860(D_8013D0C0[D_8013D998]);
                        D_800E6280.unk_1100 = 1;
                        D_8013D9A4 = 1;
                    }
                }
            } else {
                D_8013D994 = i;
                i = 0xFFFF;
            }
        }
    }
    D_8013D99C++;
    if (D_8013D9A4 == 1) {
        func_8013867C();
    }
}
