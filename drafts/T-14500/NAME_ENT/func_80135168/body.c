void func_80135168(void) {
    u8 *p;

    if ((u8) D_8014D0A4 < 2) {
        if ((u8) D_8014D0A4 == 1) {
            func_800AE0A0(D_8014CC74, D_8014D070, 8);
            func_800AE0A0(D_8014CC7C, D_8014D078, 8);
            func_800AE0A0(D_8014CC84, D_8014D080, 0xC);
            D_8014CC90 = (u8) D_8014D08C % 12;
            D_8014CC94 = (u8) D_8014D090 % 31;
            D_8014CC98 = (u8) D_8014D094 & 3;
            D_8014CC9C = (u8) D_8014D098 % 12;
            D_8014CCA0 = (u8) D_8014D09C % 31;
            D_8014CCA4 = (u8) D_8014D0A0 & 3;
        } else {
            p = D_8014CC74;
            do {
                func_800AE0F0(p, D_800B3644);
                p += 2;
            } while (p < &D_8014CC7A);
            p = D_8014CC7C;
            do {
                func_800AE0F0(p, D_800B3644);
                p += 2;
            } while (p < &D_8014CC82);
            p = D_8014CC84;
            do {
                func_800AE0F0(p, D_800B3644);
                p += 2;
            } while (p != &D_8014CC8E);
            D_8014CC8E = 0;
            D_8014CC83 = 0;
            D_8014CC82 = 0;
            D_8014CC7B = 0;
            D_8014CC7A = 0;
            D_8014CC90 = 0xFF;
            D_8014CC94 = 0xFF;
            D_8014CC98 = 0xFF;
            D_8014CC9C = 0xFF;
            D_8014CCA0 = 0xFF;
            D_8014CCA4 = 0xFF;
            D_8014CC8F = 0;
            func_80134F6C(p);
        }
    }
    D_8014CC70 = 0;
    D_8014CCC8 = 0;
    func_801330E8(D_8014CCE4);
    D_8014CCB4 = 0;
    D_8014CCB8 = -1;
    D_8014CCBC = -1;
    func_801324AC();
    func_80046290(D_8014CC38, D_8014CC3C, 0);
    func_80044750(0x300);
    func_8004EA98();
    func_80134B18();
    D_8014D0A4 = 2;
    func_8004284C();
}
