void func_8013555C(void) {
    switch ((u8) D_8015EDE8) {
    case 0:
        func_801432F0((s32) "GAME START", -0x40, 0x48, 2);
        return;
    case 1:
        func_801432F0((s32) "  OPTION  ", -0x40, 0x48, 3);
        return;
    case 2:
        func_801432F0((s32) "  SCORES  ", -0x40, 0x48, 5);
        return;
    case 3:
        func_801432F0((s32) "OPERATIONS", -0x40, 0x48, 6);
        return;
    case 4:
        func_801432F0((s32) "   EXIT   ", -0x40, 0x48, 0);
        /* fallthrough */
    default:
        return;
    }
}
