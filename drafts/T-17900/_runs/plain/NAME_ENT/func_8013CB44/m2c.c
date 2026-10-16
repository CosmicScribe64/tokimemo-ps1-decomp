extern s32 D_800E7208;
extern u8 D_800E738D;
extern u8 D_800E7395;
extern u32 D_800E73A0;

void func_8013CB44(void) {
    s32 sp30;
    s32 var_a2;
    s32 var_v1_2;
    u8 *var_v0;
    u8 var_v1;

    switch (D_800E738D) {
    case 0:
        func_80053CE0();
        D_800E738D += 1;
    default:
        break;
    case 1:
        if (func_80054284() == 3) {
            func_80053D10();
            D_800E738D += 1;
        } else if (((u32) D_800E73A0 >= 0x201U) || (D_800E7395 == 0xF0)) {
            func_80053D10();
            func_80056284();
            func_80042908(1);
        }
        break;
    case 2:
        if (func_800552DC(0) == -1) {
            D_800E738D += 1;
        } else {
            func_8004E58C();
            func_8013BB10();
            var_a2 = 0xFF;
            if (D_800E7D14[D_800E7D36] == 1) {
                var_a2 = D_800E7D36 - 1;
            } else {
                var_v0 = D_800E7D11;
                var_v1 = 1;
loop_13:
                if (*(u8 *)((unsigned char *)var_v0 + (4)) == 1) {
                    D_800E7D36 = var_v1;
                    var_a2 = var_v1 - 1;
                } else {
                    var_v1 += 1;
                    var_v0 += 1;
                    if (var_v1 != 0xE) {
                        goto loop_13;
                    }
                }
            }
            if (var_a2 != 0xFF) {
                func_8013B51C(var_a2);
            }
            func_80139CB4();
            func_8004EA98();
            func_8004EAD4(0);
            D_8014D0D4 = 1;
            func_8004284C();
        }
        break;
    case 3:
        func_8004E58C();
        func_8004E788(-0x40, 0x20, 0xF, (s32) "システムファイルは", 0);
        func_8004E788(-0x40, 0x30, 0xF, (s32) "壊れています。", 0);
        func_8004E884(2);
        func_80053D10();
        D_800E738D += 1;
        break;
    case 4:
        if (D_800E7208 & 0x60) {
            D_800E738D += 1;
        }
        break;
    case 5:
        func_8004E58C();
        func_8004E788(-0x40, 0x20, 0xF, (s32) "システムファイルを", 0);
        func_8004E788(-0x40, 0x30, 0xF, (s32) "作り直しますか？", 0);
        func_8004E788(-0x40, 0x40, 0xF, (s32) D_800B3C70, 0);
        func_8004E788(0x14, 0x40, 1, func_8006CA9C(), 0);
        func_8004E788(-0x40, 0x50, 0xF, (s32) D_800B3C88, 0);
        func_8004E788(0x14, 0x50, 2, func_8006CAE0(), 0);
        func_8004E884(6);
        D_800E738D += 1;
        break;
    case 6:
        if (D_800E7208 & 0x20) {
            D_800E738D += 1;
        } else {
            var_v1_2 = 0;
            if (D_800E7208 & 0x40) {
loop_27:
                sp30 = var_v1_2;
                var_v1_2 += 1;
                if (func_80054704(0) != 1) {
                    if (var_v1_2 != 4) {
                        goto loop_27;
                    }
                }
                func_80042908(1);
            }
        }
        break;
    case 7:
        func_80042908(0xD0);
        break;
    }
}
