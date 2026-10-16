void func_80139054(void) {
    s16 selector;

    selector = D_800CA2CC;
    switch (selector) {
    case 0:
        bg_read_sub2(0x425E);
        func_800AE0F0(D_800CA19C, "社寺巡り");
        break;
    case 1:
        bg_read_sub2(0x424B);
        func_800AE0F0(D_800CA19C, "守礼門");
        break;
    default:
        bg_read_sub2(0x4271);
        func_800AE0F0(D_800CA19C, "時計台前");
        break;
    }
    if (D_800CA2EC == 0) {
        func_80085B3C(3, D_800CA2CC);
    }
    func_8004284C();
}
