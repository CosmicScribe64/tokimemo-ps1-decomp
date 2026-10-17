void func_80154DD8(void) {
    s32 pad0;
    s32 pad1;
    s32 sel;
    s32 x;
    s32 y;
    s32 pad3;

    func_8013F0F4(0x50A, 0, 1);
    if ((D_8015EDB0.unk_00 == 1 && D_8015EC9C >= 0x3D) || D_8015EDB0.unk_00 >= 2) {
        func_8013F0F4(0x50C, 0xA, 7);
    }
    switch (D_8015EDB0.unk_00) {
    case 0:
        sel = D_8015EBAC;
        D_8015EC9C = 1;
        D_8015EC74 = 0xA;
        D_8015EC78 = 0;
        D_8015EC7C = 0xA;
        D_8015EC80 = 0;
        switch (sel) {
        default:
            D_8015EC68 = 0xB8;
            D_8015EC5C = 0x48;
            D_8015EC64 = -0x50;
            break;
        case 0:
            D_8015EC68 = 0x70;
            D_8015EC5C = 0x10;
            D_8015EC64 = -0x30;
            break;
        case 1:
            D_8015EC68 = 0xA8;
            D_8015EC5C = 0x24;
            D_8015EC64 = -0x1C;
            D_801215FD = 4;
            D_80121685 = 4;
            break;
        case 2:
            D_8015EC68 = 0xB0;
            D_8015EC5C = -0x18;
            D_8015EC64 = -0x50;
            break;
        }
        func_8014EBD0();
        break;
    case 1:
        x = -(D_8015EC5C * D_8015EC9C / D_8015EC68);
        y = -(D_8015EC64 * D_8015EC9C / D_8015EC68);
        func_8015528C(D_8015EC9C, x, y, 0x30, 5, 5, 0x34, 0x35, 0xBF);
        func_8015528C(D_8015EC9C, x, y, 0xC, 0x14, 4, 0x32, 0x33, 0xDF);
        D_8015EC9C++;
        if (D_8015EC9C > D_8015EC68) {
            func_8014EBD0();
            D_8015EC9C = 1;
        }
        break;
    case 2:
        func_8015528C(D_8015EC68, -D_8015EC5C, -D_8015EC64, 0x30, 5, 5, 0x34, 0x35, 0xBF);
        func_8015528C(D_8015EC68, -D_8015EC5C, -D_8015EC64, 0xC, 0x14, 4, 0x32, 0x33, 0xDF);
        D_8015EC9C++;
        if (D_8015EC9C >= 0x79) {
            func_8014EBD0();
            D_8015EC9C = 1;
        }
        break;
    case 3:
        x = -(D_8015EC5C - D_8015EC9C);
        y = -(D_8015EC9C + D_8015EC64);
        func_8015528C(D_8015EC68, x, y, 0x30, 5, 5, 0x34, 0x35, 0xBF);
        func_8015528C(D_8015EC68, x, y, 0xC, 0x14, 4, 0x32, 0x33, 0xDF);
        D_8015EC9C++;
        if (D_8015EC5C - D_8015EC9C < D_8015EC9C + D_8015EC64) {
            func_8014EBA8();
        }
        break;
    }
}
