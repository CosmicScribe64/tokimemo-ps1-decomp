s32 func_80133330(void) {
    s32 sp2C[16];
    s8 temp_v1;

    menu_check(0, D_8011ECF6, D_8011ECFA);
    func_8004FC10(0);
    temp_v1 = D_800E7313;
    if (temp_v1 == -1) {
        D_80120697 &= 0xFF7F;
    } else {
        func_801325A0((s32) &sp2C[0]);
        D_80120697 |= 0x80;
        D_801206BA = (&sp2C[0])[D_800E7313].unk0 + 0x30;
        D_801206BE = *(s16 *)((unsigned char *)(&(&sp2C[0])[D_800E7313]) + (16)) + 6;
    }
    if (D_800E7208 & 0x20) {
        switch (temp_v1) {
        case 0:
            func_80044750(0x501);
            func_80042908(1);
            break;
        case 1:
            func_80044750(0x501);
            func_80042908(2);
            break;
        case 2:
            if (D_800E71F4 == 0) {
                func_80044750(0x501);
                func_80042908(3);
            }
            break;
        case 3:
            if (D_800E71F4 == 1) {
                func_80044750(0x501);
                func_80042908(4);
            }
            break;
        case 4:
            func_80044750(0x501);
            func_80042908(5);
            break;
        case 5:
            func_80044750(0x501);
            func_80042908(6);
            break;
        case 6:
            func_80044750(0x501);
            func_80042908(7);
            break;
        case 7:
            func_80044750(0x501);
            func_80042908(8);
            break;
        }
    }
    if ((D_800B3220 == 1) && ((D_800E7200 & 0xF) == 0xF)) {
        main_state_set(0x21);
    }
    if ((D_800B3220 == 1) && ((D_800E7200 & 0xF0000) == 0xF0000)) {
        main_state_set(0x22);
    }
    if (D_800E7208 & 0x40) {
        D_800E7313 = 7;
    }
}
