s32 func_80135638(void) {
    if ((u32) D_800E7384 >= 0x41U) {
        switch ((u8) D_8015EDE8) {
        case 0:
            func_80042908(0xA);
            break;
        case 1:
            func_80042908(4);
            break;
        case 2:
            func_80042908(5);
            break;
        case 3:
            func_80042908(7);
            break;
        case 4:
            func_80042908(0x2E);
            break;
        }
    }
    if (D_800E7389 != 3) {
        func_80048DAC(0);
    }
    if (D_800E7384 & 4) {
        func_8013555C();
    }
}
