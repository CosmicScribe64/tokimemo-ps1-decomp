extern u8 D_800E62BA;
extern u8 D_800E7389;
extern u8 D_800E738A;

void func_801320C0(void) {
    if ((D_800E738A != 0x21) && (D_800E738A != 0x22) && (D_800E738A != 0x23)) {
        if (D_800E62BA != 0) {
            switch (D_800E7389) {
            case 1:
                func_80049A40(8, -0x68, 0x40, 0x90, 8, 0x10907, 0x80);
                break;
            case 2:
            case 4:
                func_80049A40(-0x28, -0x48, 0xB0, 0x80, 8, 0x10907, 0x80);
                break;
            case 3:
                func_80049A40(-0x80, -0x30, 0x100, 0x80, 8, 0x10907, 0x80);
                break;
            case 5:
                func_80049A40(8, -0x20, 0x40, 0x80, 8, 0x10907, 0x80);
                break;
            case 6:
                func_80049A40(-0x10, 0x10, 0x90, 0x50, 8, 0x10907, 0x80);
                break;
            case 7:
                if ((u8) D_800E738A < 0xAU) {
                    func_80049A40(-0x50, 8, 0x80, 0x58, 8, 0x10907, 0x80);
                } else if ((u8) D_800E738A < 0x14U) {
                    func_80049A40(-0x58, -8, 0xDC, 0x40, 8, 0x10907, 0x80);
                } else {
                    func_80049A40(-0x90, -0x58, 0x130, 0x90, 8, 0x10907, 0x80);
                }
                break;
            case 8:
                func_80049A40(-0x50, 8, 0x80, 0x58, 8, 0x10907, 0x80);
                break;
            default:
                func_80049A40(8, -0x68, 0x40, 0x90, 8, 0x10907, 0x80);
                break;
            }
            func_800494BC(0, 0, 8, 0, 0);
        }
    }
}
