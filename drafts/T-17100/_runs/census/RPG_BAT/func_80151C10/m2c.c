void func_80151F38(void);
void func_80151C10(s32 arg0, s32 arg1) {
    switch (D_8015EE04) {
    case 0x0:
        func_800AE0F0(D_8015E904, "　　　　　ポコポコ乱れ撃ち　　　　　");
        func_8014B738((s32) D_8015E9CC, 1, 0x5A);
        func_8013E54C(0xFF);
        D_8015EC74 = func_8013E9A8(4) + 3;
        D_8015EC7C = 0;
        func_8014F230();
        return;
    case 0x1:
        func_8014F448(1);
        return;
    case 0x2:
    case 0x3:
    case 0x4:
    case 0x5:
    case 0x6:
    case 0x7:
    case 0x8:
    case 0x9:
    case 0xA:
    case 0xB:
    case 0xC:
    case 0xD:
    case 0xE:
    case 0xF:
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
    case 0x19:
        if (!(D_8015EE04 & 1)) {
            if (D_8015EE0C == 0) {
                func_80151F38(0x1D, 0);
                func_8013E7C0(0x1D, 5, 1, 1);
                D_8015EC78 = func_8013E9A8(0x38) - 0x30;
                D_8015EE0C += 1;
                return;
            }
            func_80143DA8(0x1D, 0, D_8015EC7C, -0xEF, D_8015EC78, 0x1E);
            return;
        }
        if (((s32) (D_8015EE04 - 3) / 2) == D_8015EC74) {
            func_80151F38(arg0, arg1);
            func_8013E7C0(0x1D, 6, 1, 1);
            D_8015EE04 = 0x32;
            return;
        }
        if (D_8015EE0C == 0) {
            func_80151F38(0x1D, -0xEF);
            func_8013E7C0(0x1D, 6, 1, 1);
            D_8015EC7C = func_8013E9A8(0x38) - 0x30;
            D_8015EE0C += 1;
            return;
        }
        func_80143DA8(0x1D, -0xEF, D_8015EC78, 0, D_8015EC7C, 0x1E);
        return;
    case 0x32:
        func_80143DA8(0x1D, -0xEF, D_8015EC78, 0, 0, 0x1E);
        return;
    case 0x33:
        func_8013E7C0(0x1D, 0, 1, 1);
        func_801477E0();
        if (D_8015EDC4 & 0x100000) {
            func_8013EA60(0, 0, D_8015EBB0, 1);
            func_8013EA60(1, 1, D_8015EBB4, 1);
            func_8013EA60(2, 2, D_8015EBB8, 1);
            func_8013EA60(3, 0x21, D_8015EBBC, 1);
        } else {
            func_8013EA60(0, 3, D_8015EBB0, 1);
            func_8013EA60(1, 0x21, D_8015EBBC, 1);
        }
        func_8014F230();
        return;
    case 0x34:
        func_8014F350(0x3C);
        return;
    case 0x35:
        func_8014F278();
        /* fallthrough */
    default:
        return;
    }
}
