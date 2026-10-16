void func_801529B4(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v0_5;

    func_8014B79C();
    switch (D_800E6280.unk_110A) {                  /* switch 1; irregular */
    case 0:                                         /* switch 1 */
        func_8004500C(1, 0x200);
        temp_v0 = D_8015EB98;
        switch (temp_v0) {                          /* switch 2; irregular */
        case 0x10000:                               /* switch 2 */
            func_800AE0F0(D_8015E9E0, "ハブ・ロードが襲ってきた！");
            break;
        case 0x20000:                               /* switch 2 */
            func_800AE0F0(D_8015E9E0, "鹿・グレートが襲ってきた！");
            break;
        case 0x40000:                               /* switch 2 */
            func_800AE0F0(D_8015E9E0, "キラーヒグマが襲ってきた！");
            break;
        case 0x100000:                              /* switch 2 */
            func_800AE0F0(D_8015E9E0, "不良達が襲ってきた！");
            break;
        }
        if (temp_v0 != 0x08000000) {
            func_8014B738((s32) D_8015EB70, 1, 0x5A);
        }
        func_8013E810(0x30, 1, 1, 1);
        if ((D_8015EB98 != 0x10000) && (D_8015EB98 != 0x20000) && (D_8015EB98 != 0x40000)) {
            if (D_8015EB98 != 0x100000) {
                if (D_8015EB98 == 0x08000000) {
                    goto block_20;
                }
            } else {
                func_8013E810(0x3B, 0, 1, 1);
                func_8013E810(0x3C, 0, 1, 1);
                func_8013E810(0x3D, 0, 1, 1);
            }
        } else {
block_20:
            func_8013E810(0x3B, 0, 1, 1);
        }
        switch (D_8015EB9C) {                       /* switch 3; irregular */
        case 0:                                     /* switch 3 */
            func_8013E810(0x1D, 2, 1, 1);
            break;
        case 1:                                     /* switch 3 */
            func_8013E810(0x1D, 2, 1, 1);
            break;
        case 2:                                     /* switch 3 */
            func_8013E810(0x1D, 9, 1, 1);
            break;
        case 3:                                     /* switch 3 */
            func_8013E810(0x1D, 2, 1, 1);
            break;
        case 4:                                     /* switch 3 */
            func_8013E810(0x1D, 1, 1, 1);
            break;
        case 5:                                     /* switch 3 */
            func_8013E810(0x1D, 0, 1, 1);
            break;
        case 6:                                     /* switch 3 */
            func_8013E810(0x1D, 1, 1, 1);
            break;
        case 7:                                     /* switch 3 */
            func_8013E810(0x1D, 1, 1, 1);
            break;
        case 8:                                     /* switch 3 */
            func_8013E810(0x1D, 1, 1, 1);
            break;
        case 9:                                     /* switch 3 */
            func_8013E810(0x1D, 1, 1, 1);
            break;
        case 10:                                    /* switch 3 */
            func_8013E810(0x1D, 1, 1, 1);
            break;
        case 13:                                    /* switch 3 */
            func_8013E810(0x1D, 1, 1, 1);
            break;
        }
        func_8013E97C(0x30, 0x160, 0x58);
        func_8013E97C(0x1D, 0x168, 0x88);
        func_8013E97C(0x3B, -0x58, 0x60);
        temp_v0_2 = D_8015EB98;
        if (temp_v0_2 == 0x100000) {
            func_8013E97C(0x3B, -0x28, 0x68);
            func_8013E97C(0x3C, -0x60, 0x58);
            func_8013E97C(0x3D, -0x68, 0x88);
        }
        if (temp_v0_2 == 0x08000000) {
            func_8013E97C(0x3B, -0x58, 0x70);
        }
        func_8004284C();
        break;
    case 1:                                         /* switch 1 */
        func_8013E97C(0x30, 0x160 - D_800E6280.unk_1104.w, 0x58);
        func_8013E97C(0x1D, 0x168 - D_800E6280.unk_1104.w, 0x88);
        func_8013E97C(0x3B, (D_800E6280.unk_1104.w * 2) - 0x58, 0x60);
        temp_v0_3 = D_8015EB98;
        if (temp_v0_3 == 0x100000) {
            func_8013E97C(0x3B, (D_800E6280.unk_1104.w * 2) - 0x28, 0x68);
            func_8013E97C(0x3C, (D_800E6280.unk_1104.w * 2) - 0x60, 0x58);
            func_8013E97C(0x3D, (D_800E6280.unk_1104.w * 2) - 0x68, 0x88);
        }
        if (temp_v0_3 == 0x08000000) {
            func_8013E97C(0x3B, (D_800E6280.unk_1104.w * 2) - 0x58, 0x70);
        }
        temp_v0_4 = D_800E6280.unk_1104.w + 4;
        D_800E6280.unk_1104.w = temp_v0_4;
        if ((u32) temp_v0_4 >= 0x51U) {
            func_8013E97C(0x30, 0, 0);
            func_8013E7C0(0x30, 0, 1, 1);
            func_8013E97C(0x1D, 0, 0);
            if (D_8015EB9C != 0xE) {
                func_8013E7C0(0x1D, 0, 1, 1);
            }
            func_8004284C();
        }
        break;
    case 2:                                         /* switch 1 */
        temp_v0_5 = D_800E6280.unk_1104.w + 1;
        D_800E6280.unk_1104.w = temp_v0_5;
        if ((u32) temp_v0_5 >= 0x3DU) {
            func_80042808();
        }
        break;
    }
}
