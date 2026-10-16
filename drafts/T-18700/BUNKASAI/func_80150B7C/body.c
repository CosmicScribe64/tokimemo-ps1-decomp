void func_80150B7C(void) {
    u16 temp_v1_2;
    u32 temp_t2;
    u8 temp_v1;

    temp_v1 = D_800E6280.unk_110A;
    switch (temp_v1) {
    case 0:
        D_800CA148 = ((((u16) D_800E6280.unk_0F4.h >> 0xC) * 3) + D_800E6280.unk_03E) - 0x4D;
        func_8004284C();
        return;
    case 1:
        normal_date_speak(2);
        return;
    case 2:
        temp_v1_2 = D_800E6280.unk_0F4.h;
        D_80122EB8 = (s32) (temp_v1_2 >> 0xC);
        temp_t2 = temp_v1_2 >> 0xC;
        switch (temp_t2) {
        case 0:
            if (((u8) D_800E6280.unk_1BC[0].unk_0C.h[1] >> 4) == 0) {
                func_80042908(1);
                return;
            }
            if ((((u8) D_800E6280.unk_1BC[1].unk_0C.b[2] >> 4) == 0) && (((u32) (D_800E6280.unk_1BC[1].unk_0C.w << 0x1E) >> 0x1F) == 1)) {
                func_80042908(0x2A);
                return;
            }
            func_80042908(0xE);
            return;
        case 1:
            if (((u8) D_800E6280.unk_1BC[0].unk_0C.h[1] >> 4) == 1) {
                func_80042908(1);
                return;
            }
            if ((((u8) D_800E6280.unk_1BC[1].unk_0C.b[2] >> 4) == 1) && (((u32) (D_800E6280.unk_1BC[1].unk_0C.w << 0x1E) >> 0x1F) == 1)) {
                func_80042908(0x2A);
                return;
            }
            func_80042908(0xE);
            return;
        case 2:
            if (((u8) D_800E6280.unk_1BC[0].unk_0C.h[1] >> 4) == 2) {
                func_80042908(1);
                return;
            }
            if ((((u8) D_800E6280.unk_1BC[2].unk_0C.b[2] >> 4) == 2) && (((u32) (D_800E6280.unk_1BC[2].unk_0C.w << 0x1E) >> 0x1F) == 1)) {
                func_80042908(0x3B);
                return;
            }
            func_80042908(0xE);
            return;
        case 3:
            if (((u8) D_800E6280.unk_1BC[0].unk_0C.h[1] >> 4) == 3) {
                func_80042908(1);
                return;
            }
            if ((((u8) D_800E6280.unk_1BC[2].unk_0C.b[2] >> 4) == 3) && (((u32) (D_800E6280.unk_1BC[2].unk_0C.w << 0x1E) >> 0x1F) == 1)) {
                func_80042908(0x3B);
                return;
            }
            func_80042908(0xE);
            return;
        case 4:
            if (((u8) D_800E6280.unk_1BC[0].unk_0C.h[1] >> 4) == 4) {
                func_80042908(1);
                return;
            }
            if ((((u8) D_800E6280.unk_1BC[3].unk_0C.b[2] >> 4) == 4) && (((u32) (D_800E6280.unk_1BC[3].unk_0C.w << 0x1E) >> 0x1F) == 1)) {
                func_80042908(0x50);
                return;
            }
            func_80042908(0xE);
            return;
        case 5:
            if (((u8) D_800E6280.unk_1BC[0].unk_0C.h[1] >> 4) == 5) {
                func_80042908(1);
                return;
            }
            if ((((u8) D_800E6280.unk_1BC[3].unk_0C.b[2] >> 4) == 5) && (((u32) (D_800E6280.unk_1BC[3].unk_0C.w << 0x1E) >> 0x1F) == 1)) {
                func_80042908(0x50);
                return;
            }
            func_80042908(0xE);
            return;
        default:
            func_80046500();
            return;
        }
        break;
    default:
        func_80046500();
        return;
    }
}
