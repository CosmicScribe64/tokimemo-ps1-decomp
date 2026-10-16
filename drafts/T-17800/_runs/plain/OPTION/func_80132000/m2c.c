extern u8 D_800E62BA;
extern u8 D_800E738A;

void func_80132000(void) {
    if ((D_800E738A != 0x21) && (D_800E738A != 0x22) && (D_800E738A != 0x23)) {
        if (D_800E62BA != 0) {
            if ((u8) D_801220E0 >= 0x7FU) {
                func_80049A40(D_80120676 + 0x18, D_8012067A + 0x10, 0x7C, 0xBC, 0xA, 0x10907, 0x83);
                func_800494BC(0, 0, 0xA, 0, 0);
            }
        }
    }
}
