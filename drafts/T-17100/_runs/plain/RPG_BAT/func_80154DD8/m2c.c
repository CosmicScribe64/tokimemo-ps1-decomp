extern s32 D_8015EC5C;
extern s32 D_8015EC64;
extern s32 D_8015EC9C;

void func_80154DD8(void) {
    s32 sp48;
    s32 sp44;
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_a0_3;
    s32 temp_a1;
    s32 temp_a1_2;
    s32 temp_a2;
    s32 temp_a2_2;
    s32 temp_v0;

    func_8013F0F4(0x50A, 0, 1);
    temp_v0 = D_8015EDB0.unk_00;
    if (((temp_v0 == 1) && (D_8015EC9C >= 0x3D)) || (temp_v0 >= 2)) {
        func_8013F0F4(0x50C, 0xA, 7);
    }
    switch (temp_v0) {                              /* switch 1; irregular */
    case 0:                                         /* switch 1 */
        D_8015EC9C = 1;
        D_8015EC74 = 0xA;
        D_8015EC78 = 0;
        D_8015EC7C = 0xA;
        D_8015EC80 = 0;
        switch (D_8015EBAC) {                       /* switch 2; irregular */
        default:                                    /* switch 2 */
            D_8015EC68 = 0xB8;
            D_8015EC5C = 0x48;
            D_8015EC64 = -0x50;
            break;
        case 0:                                     /* switch 2 */
            D_8015EC68 = 0x70;
            D_8015EC5C = 0x10;
            D_8015EC64 = -0x30;
            break;
        case 1:                                     /* switch 2 */
            D_8015EC68 = 0xA8;
            D_8015EC5C = 0x24;
            D_8015EC64 = -0x1C;
            D_801215FD = 4;
            D_80121685 = 4;
            break;
        case 2:                                     /* switch 2 */
            D_8015EC68 = 0xB0;
            D_8015EC5C = -0x18;
            D_8015EC64 = -0x50;
            break;
        }
        func_8014EBD0();
        break;
    case 1:                                         /* switch 1 */
        temp_a1 = -((s32) (D_8015EC5C * D_8015EC9C) / (s32) D_8015EC68);
        sp48 = temp_a1;
        temp_a2 = -((s32) (D_8015EC64 * D_8015EC9C) / (s32) D_8015EC68);
        sp44 = temp_a2;
        func_8015528C(D_8015EC9C, temp_a1, temp_a2, 0x30, 5, 5, 0x34, 0x35, 0xBF);
        func_8015528C(D_8015EC9C, temp_a1, temp_a2, 0xC, 0x14, 4, 0x32, 0x33, 0xDF);
        temp_a0 = D_8015EC9C + 1;
        D_8015EC9C = temp_a0;
        if (D_8015EC68 < temp_a0) {
            func_8014EBD0();
            D_8015EC9C = 1;
        }
        break;
    case 2:                                         /* switch 1 */
        func_8015528C(D_8015EC68, -D_8015EC5C, -D_8015EC64, 0x30, 5, 5, 0x34, 0x35, 0xBF);
        func_8015528C(D_8015EC68, -D_8015EC5C, -D_8015EC64, 0xC, 0x14, 4, 0x32, 0x33, 0xDF);
        temp_a0_2 = D_8015EC9C + 1;
        D_8015EC9C = temp_a0_2;
        if (temp_a0_2 >= 0x79) {
            func_8014EBD0();
            D_8015EC9C = 1;
        }
        break;
    case 3:                                         /* switch 1 */
        temp_a2_2 = -(D_8015EC9C + D_8015EC64);
        temp_a1_2 = -(D_8015EC5C - D_8015EC9C);
        sp48 = temp_a1_2;
        sp44 = temp_a2_2;
        func_8015528C(D_8015EC68, temp_a1_2, temp_a2_2, 0x30, 5, 5, 0x34, 0x35, 0xBF);
        func_8015528C(D_8015EC68, temp_a1_2, temp_a2_2, 0xC, 0x14, 4, 0x32, 0x33, 0xDF);
        temp_a0_3 = D_8015EC9C + 1;
        D_8015EC9C = temp_a0_3;
        if ((D_8015EC5C - temp_a0_3) < (temp_a0_3 + D_8015EC64)) {
            func_8014EBA8();
        }
        break;
    }
}
