void func_80132000(void) {
    u8 t = D_800E6280.unk_110A;
    u8 on;

    switch (t) {
    case 0x21:
    case 0x22:
    case 0x23:
        break;
    default:
        on = D_800E6280.unk_03A;
        switch (on) {
        case 0:
            break;
        default:
            if (D_801220E0 >= 0x7F) {
                func_80049A40(D_80120676 + 0x18, D_8012067A + 0x10, 0x7C, 0xBC, 0xA, 0x10907, 0x83);
                func_800494BC(0, 0, 0xA, 0, 0);
            }
            break;
        }
        break;
    }
}
