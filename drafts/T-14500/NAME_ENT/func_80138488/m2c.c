extern s32 D_800E7384;
extern u8 D_800E738A;

void func_80138488(void) {
    u8 temp_v0;

    temp_v0 = D_800E738A;
    D_800E7384 += 1;
    switch (temp_v0) {
    case 0:
        func_801385A8();
        break;
    case 1:
        func_80138634();
        break;
    case 2:
        func_801387F4();
        break;
    case 3:
        func_80138A7C();
        break;
    case 4:
        func_80138BCC();
        break;
    case 5:
        func_80139684();
        break;
    }
    if (temp_v0 < 4U) {
        if (temp_v0 < 2U) {
            func_8013621C(1);
            func_80136808(1);
            return;
        }
        func_80138268(1);
        func_80138330();
    }
}
