extern u8 D_800E7389;

void func_80075320(void) {
    switch ((u8) D_800E7389) {                      /* irregular */
    case 0x0:
        func_80075468();
        break;
    case 0x1:
        func_80075BE8();
        break;
    case 0x5:
        func_80076F48();
        break;
    case 0x6:
        holiday_club();
        break;
    case 0x7:
        func_8007894C();
        break;
    case 0x10:
        holiday_tel();
        break;
    case 0x20:
        func_80077BE0();
        break;
    case 0x30:
        func_8005B798();
        break;
    case 0x40:
        func_8005D9B4();
        break;
    case 0xFE:
        func_80076EC0();
        break;
    }
    if (*D_8011F3FF & 0x80) {
        func_80067870();
    }
    message_window_show();
    func_800578F4(0);
    hizuke_show();
    parameter_show();
    func_8006BA40();
    func_80066C08(1);
}
