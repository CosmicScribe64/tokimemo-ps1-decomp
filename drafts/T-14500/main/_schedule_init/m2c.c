extern u8 D_800E738A;

void _schedule_init(void) {
    switch (D_800E738A) {
    case 0:
        func_8006CD14();
        break;
    case 1:
        func_8006D7EC();
        break;
    case 2:
        schedule_girls_gakko();
        break;
    case 3:
        schdeule_girls_param();
        break;
    case 4:
        func_80070ECC();
        break;
    }
    parameter_show();
    message_window_show();
    hizuke_show();
    func_80065B0C(0);
    func_80067870();
    func_80066C08(1);
}
