? func_800AE100(u8 *, ? *);                         /* extern */
extern ? D_800E6354;
extern s32 D_8015EDA4;

void func_8013AD50(void) {
    s32 temp_lo;

    if (!(D_8015EDF0 & 0xFF)) {
        if (D_8015ED98 & 0x100) {
            func_8013E7C0(0x30, 3, 1, 1);
        } else {
            func_8013D030();
        }
    }
    D_8015EDB4 += 1;
    switch (D_8015EDAC) {
    case 0:
        if (!(D_8015EDC4 & 0xFF) && !(D_8015EDF0 & 0xFF)) {
            if (D_8015EDB4 >= 0x97) {
                D_8015ED98 = 0x104;
                temp_lo = (s32) D_8015EDA0 / 20;
                D_8015EDA4 = temp_lo;
                if (temp_lo == 0) {
                    D_8015EDA4 = 1;
                }
                func_800AE0F0(D_8015E74C, &D_800E6354);
                func_800AE100(D_8015E74C, "の体力が５％回復した");
                func_8014B738((s32) &D_8015E814, 1, 0x3C);
                func_8014EBA8();
            }
        default:
        }
        break;
    case 1:
        func_8014EE3C(1);
        break;
    case 2:
        func_8013EA60(0, 0x21, D_8015EDA4, 0);
        func_8013F15C(0x502, 1, 0);
        func_8014EBA8();
        break;
    case 3:
        func_8014ED44(0x14);
        break;
    case 4:
        func_8014EBEC();
        D_8015ED9C = func_801445E8(1);
        break;
    }
}
