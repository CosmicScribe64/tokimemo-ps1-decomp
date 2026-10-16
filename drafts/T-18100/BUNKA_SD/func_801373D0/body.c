s32 func_801373D0(void) {
    BsdEvent *list;
    BsdEvent *ev;
    s32 i;
    s32 cmd;
    s32 slot;
    s32 img;

    if (D_800E6280.unk_1100 == 0) {
        D_8013CED8 = 0;
        if (D_8013CED4 != 0) {
            switch (D_8013CED4) {
            case 1:
                D_800E6280.unk_1104.w = 0;
                break;
            case 2:
                D_8013CECC++;
                D_8013CED0 = 0;
                D_8013CED4 = 0;
                D_8013CEC8 = 1;
                func_8004E9F4(1);
                func_8004284C();
                return 1;
            }
        }
    }
    list = D_8013CE8C[D_8013CECC];
    if (D_8013CED4 == 0) {
        D_8013CED4 = 1;
    }
    if (D_8013CED4 == 1) {
        for (i = D_8013CEC8; i < 0xFFFF; i++) {
            ev = &list[i];
            if (D_8013CED0 == ev->unk_00) {
                cmd = ev->unk_04;
                if (cmd == 0x0FFFFFFF) {
                    D_8013CED4 = 2;
                    i = 0xFFFF;
                } else {
                    slot = (cmd & 0xFF0000) >> 16;
                    img = cmd & 0xFF;
                    if (img == 0xFE) {
                        func_801377D4(slot, 0x50, 0);
                    } else {
                        func_801377D4(slot, img, 1);
                    }
                    if (ev->unk_04 & 0xF0000000) {
                        func_80137824(D_8013C814[D_8013CECC]);
                        D_800E6280.unk_1100 = 1;
                        D_8013CED8 = 1;
                    }
                }
            } else {
                D_8013CEC8 = i;
                i = 0xFFFF;
            }
        }
    }
    D_8013CED0++;
    if (D_8013CED8 == 1) {
        func_80137620();
    }
}
