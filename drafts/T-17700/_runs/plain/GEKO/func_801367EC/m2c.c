extern u16 D_800E6374;
extern u8 D_800E699E;
extern u8 D_800E71DF;
extern u8 D_80145090;

void func_801367EC(void) {
    u32 temp_t9;

    func_8004500C(1, 0x200);
    if (D_80145088 == 0) {
        if (D_80145090 == 0) {
            func_800AE0F0(D_800CA19C, "廊下");
            func_8007ED84(0x4045);
            func_80085B3C(0xA, 0x25);
        } else {
            temp_t9 = (u16) D_800E6374 >> 0xC;
            switch (temp_t9) {
            case 0:
                func_800AE0F0(D_800CA19C, "図書室");
                func_8007ED84(0x40BC);
                break;
            case 1:
                func_800AE0F0(D_800CA19C, "演劇部");
                func_8007ED84(0x40B2);
                break;
            case 2:
                func_800AE0F0(D_800CA19C, "実験室");
                func_8007ED84(0x40C6);
                break;
            case 3:
                func_800AE0F0(D_800CA19C, "電脳部室");
                func_8007ED84(0x40D0);
                break;
            case 4:
                func_800AE0F0(D_800CA19C, "美術室");
                func_8007ED84(0x40D9);
                break;
            case 5:
                func_800AE0F0(D_800CA19C, "音楽室");
                func_8007ED84(0x40E2);
                break;
            case 6:
                func_800AE0F0(D_800CA19C, "グランド");
                func_8007ED84(0x4296);
                break;
            case 7:
                func_800AE0F0(D_800CA19C, "グランド");
                func_8007ED84(0x4296);
                break;
            case 8:
                func_800AE0F0(D_800CA19C, "グランド");
                func_8007ED84(0x4296);
                break;
            case 9:
                func_800AE0F0(D_800CA19C, "プール");
                func_8007ED84(0x40A0);
                break;
            default:
                func_800AE0F0(D_800CA19C, "体育館");
                func_8007ED84(0x40A9);
                break;
            }
            func_80085B3C(0xA, ((u16) D_800E6374 >> 0xC) + 0x33);
        }
    } else if ((D_80145074 != 0) || (D_800E699E & 0x20)) {
        if (D_800E71DF == 0) {
            func_800AE0F0(D_800CA19C, "自宅前");
            func_8007ED84(0x4156);
            func_80085B3C(0xA, 0x28);
        } else {
            func_800AE0F0(D_800CA19C, "近所の公園");
            func_8007ED84(0x42E2);
            func_80085B3C(0xA, 0x27);
        }
    } else if (D_800E71DF == 0) {
        func_800AE0F0(D_800CA19C, "自宅前");
        func_8007ED84(0x414C);
        func_80085B3C(0xA, 0x2F);
    } else {
        func_800AE0F0(D_800CA19C, "近所の公園");
        func_8007ED84(0x42D8);
        func_80085B3C(0xA, 0x26);
    }
    func_8004284C();
}
