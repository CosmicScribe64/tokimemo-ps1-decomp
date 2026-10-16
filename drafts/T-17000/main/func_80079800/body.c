void func_80079800(void) {
    switch ((u8) D_800E738D) {
    case 0x0:
        func_80053CE0();
        D_800E738D += 1;
        break;
    case 0x1:
        if (func_80053F04() != 0) {
            if (D_800E7395 == 1) {
                D_800E738D += 1;
            } else if (D_800E7395 == 0xF1) {
                D_800E738D += 1;
            } else if (D_800E7395 == 0xFF) {
                D_800E738D += 1;
            }
        } else if ((u32) D_800E73A0 >= 0x201U) {
            D_800E738D += 1;
        }
        break;
    case 0x2:
        func_8005C4CC(0);
        func_80053D10();
        if (D_800E7395 == 1) {
            if (func_80054AF4(0) == 1) {
                D_800E738D += 1;
                func_80053CE0();
            } else {
                D_800E738D = 0xFF;
            }
        } else {
            D_800E738D = 0xFF;
        }
        break;
    case 0x3:
        if (func_80054284() == 3) {
            func_80053D10();
            D_800E738D += 1;
        } else if (((u32) D_800E73A0 >= 0x201U) || (D_800E7395 == 0xF0)) {
            func_80053D10();
            D_800E738D = 0xFF;
        }
        break;
    case 0x4:
        if (func_800552DC(0) == -1) {
            D_800E738D = 0xFF;
        } else {
            D_800E738D += 1;
        }
        func_8005C4CC(1);
        break;
    case 0x5:
        func_80053CE0();
        D_800E7D36 = 0;
        D_800E738D += 1;
        break;
    case 0x6:
        if (func_80054388() == 0x13) {
            func_80044750(0x50F);
            func_80053D10();
            D_800E738D += 1;
        } else if (((u32) D_800E73A0 >= 0x201U) || (D_800E7395 == 0xF0)) {
            func_80044750(0x502);
            func_80053D10();
            D_800E738D += 1;
        }
        break;
    default:
        func_80042808();
        break;
    }
}
