void func_80132E80(void) {
    u8 temp_v0;

    temp_v0 = D_800E738A;
    if (temp_v0 != 0) {
        if (temp_v0 < 6U) {
            func_80135434(0);
            func_80134EB8();
        } else if (temp_v0 < 8U) {
            func_8013621C(0);
            func_80136808(0);
        } else {
            func_80138268(0);
            func_80138330();
        }
    }
    switch (temp_v0) {
    case 0:
        func_80135168();
        return;
    case 1:
        func_801357F4();
        return;
    case 2:
        func_80135A98();
        return;
    case 3:
        func_80135BB4();
        return;
    case 4:
        func_80135E58();
        return;
    case 5:
        func_80135F74();
        return;
    case 6:
        func_801362D0();
        return;
    case 7:
        func_80137708();
        return;
    case 8:
        func_80137FC4();
        return;
    case 9:
        func_80138338();
        /* fallthrough */
    default:
        return;
    }
}
