/* NON_MATCHING: T-25114, 34 funcdiff rows, all aligned: frame 0x60 against 0x58 (the two kept quotient
 * webs cost two frame words more than the original's), the case 0 selector load placed before the
 * stores in the original, and the case 3 counter store placed after the test loads here. Notes:
 * drafts/T-25114/RPG_BAT/func_80154DD8/NOTES.md */
extern s32 D_8015EC9C;
void func_8015528C(s32, s32, s32, s32, s32, s32, s32, s32, s32);

s32 func_80154DD8(void) {
    s32 x;
    s32 y;
    s32 nx;
    s32 ny;

    func_8013F0F4(0x50A, 0, 1);
    if (((D_8015EDB0.unk_00 == 1) && (D_8015EC9C >= 0x3D)) || (D_8015EDB0.unk_00 >= 2)) {
        func_8013F0F4(0x50C, 0xA, 7);
    }
    switch (D_8015EDB0.unk_00) {
    case 0:
        D_8015EC9C = 1;
        D_8015EC74 = 0xA;
        D_8015EC78 = 0;
        D_8015EC7C = 0xA;
        D_8015EC80 = 0;
        /* FAKE: scalar-address view keeps the selector in $v1 and the constant 1 in $a3 (T-17114, T-24910). */
        switch (*(s32 *)&D_8015EBAC) {
        default:
            D_8015EC58.unk_10 = 0xB8;
            D_8015EC58.unk_04 = 0x48;
            D_8015EC58.unk_0C = -0x50;
            break;
        case 0:
            D_8015EC58.unk_10 = 0x70;
            D_8015EC58.unk_04 = 0x10;
            D_8015EC58.unk_0C = -0x30;
            break;
        case 1:
            D_8015EC58.unk_10 = 0xA8;
            D_8015EC58.unk_04 = 0x24;
            D_8015EC58.unk_0C = -0x1C;
            D_801215FD = 4;
            D_80121685 = 4;
            break;
        case 2:
            D_8015EC58.unk_10 = 0xB0;
            D_8015EC58.unk_04 = -0x18;
            D_8015EC58.unk_0C = -0x50;
            break;
        }
        func_8014EBD0();
        break;
    case 1:
        /* FAKE: the address views keep x and y as their own webs ($v0/$v1) instead of being folded into
         * nx/ny; plain reads compute the quotient straight into $a1/$a2. T-25114 */
        x = (*(s32 *)&D_8015EC58.unk_04 * D_8015EC9C) / D_8015EC58.unk_10;
        y = (*(s32 *)&D_8015EC58.unk_0C * D_8015EC9C) / D_8015EC58.unk_10;
        nx = -x;
        ny = -y;
        func_8015528C(D_8015EC9C, nx, ny, 0x30, 5, 5, 0x34, 0x35, 0xBF);
        func_8015528C(D_8015EC9C, nx, ny, 0xC, 0x14, 4, 0x32, 0x33, 0xDF);
        D_8015EC9C++;
        if (D_8015EC58.unk_10 < D_8015EC9C) {
            func_8014EBD0();
            D_8015EC9C = 1;
        }
        break;
    case 2:
        func_8015528C(D_8015EC58.unk_10, -D_8015EC58.unk_04, -D_8015EC58.unk_0C, 0x30, 5, 5, 0x34, 0x35, 0xBF);
        func_8015528C(D_8015EC58.unk_10, -D_8015EC58.unk_04, -D_8015EC58.unk_0C, 0xC, 0x14, 4, 0x32, 0x33, 0xDF);
        D_8015EC9C++;
        if (D_8015EC9C >= 0x79) {
            func_8014EBD0();
            D_8015EC9C = 1;
        }
        break;
    case 3:
        /* FAKE: same web-keeping view as case 1; the view on the counter also keeps it as the first addu
         * operand of y. T-25114 */
        x = D_8015EC58.unk_04 - *(s32 *)&D_8015EC9C;
        y = *(s32 *)&D_8015EC9C + D_8015EC58.unk_0C;
        nx = -x;
        ny = -y;
        func_8015528C(D_8015EC58.unk_10, nx, ny, 0x30, 5, 5, 0x34, 0x35, 0xBF);
        func_8015528C(D_8015EC58.unk_10, nx, ny, 0xC, 0x14, 4, 0x32, 0x33, 0xDF);
        D_8015EC9C++;
        if ((D_8015EC58.unk_04 - D_8015EC9C) < (D_8015EC9C + D_8015EC58.unk_0C)) {
            func_8014EBA8();
        }
        break;
    }
}
