? func_8007E99C();                                  /* extern */
extern s16 D_800E658E;
extern s16 D_800E6592;
extern s8 D_800E71DF;
extern u8 D_800E738A;
extern s32 D_8014A0C0;

s32 func_80141CDC(void) {
    u8 *var_v1;
    u8 temp_t0;

    func_80083808();
    switch (D_800E738A) {
    case 0:
        func_80044890(0, 0xBF98, 0xBF79, 0xD712, 0xD6C3, 0xD6B7);
        D_8014A0C0 = 0;
        func_8008585C();
        func_8004E58C();
        func_80048390();
        func_800573AC();
        func_80048E78();
        func_8004E9F4(0);
        func_8006612C((s32) "中庭");
        func_80065F34(0);
        func_80064E84();
        D_800B593C = 0;
        func_8004284C();
        break;
    case 1:
        if (func_80044E8C() == 1) {
            func_8005742C(0x4071, 1);
            func_8004284C();
        }
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80057418(1, func_8005751C(1));
            func_80062CD0(0x6476);
            func_8004284C();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_8007C740();
            D_800E71DF = 6;
            func_80083474();
            cd_read_sectors(1U, 0x801E8000, 0xB7BF);
        }
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            func_80084E4C();
            func_800847B8(6U);
            func_80142140();
            D_800CA148 = 0;
            D_800CA14C = 0;
            D_800CA160 = D_8014A0D0;
            D_800CA164 = D_8014A0D4;
            D_800CA168 = D_8014A0D8;
            func_8004284C();
        }
        break;
    case 5:
        if (func_8004ECB4() != 0) {
            func_80065F34(1);
        }
        /* fallthrough */
    case 7:
    case 9:
    case 10:
    case 12:
    case 18:
        func_80085A60();
        break;
    case 6:
        func_8007E81C();
        if (D_800E738A == 7) {
            func_8004500C(0, 0x202);
        }
        break;
    case 8:
        func_80044750(0x60A);
        func_80047550();
        func_8004284C();
        break;
    case 11:
        func_8007E390();
        break;
    case 13:
        cd_read_sectors(0x3DU, 0x80197000, 0xBB5A);
        func_8004284C();
        break;
    case 14:
        func_8007E99C();
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            func_80145210();
            func_80043914(D_8014A3A0, 0x11, 1, 2, 0);
            func_80084E90(D_8014A3A4, D_8014A3A8, D_8014A3AC, D_8014A3B0, D_8014A3B4, D_8014A3B8);
            func_800850D4(D_8014A394, D_8014A398, D_8014A390, (s32) D_8014A39C);
            var_v1 = D_8011ECD0;
            do {
                temp_t0 = *(u8 *)((unsigned char *)var_v1 + (6532));
                var_v1 += 0x44;
                *(s8 *)((unsigned char *)var_v1 + (6464)) = (s8) (temp_t0 | 8);
            } while (var_v1 != D_8011ED58);
            func_8005742C(0x4728, 1);
            func_8004284C();
        }
        break;
    case 16:
        if (cd_read_sync_status() & 1) {
            func_80057418(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 17:
        func_80085368();
        break;
    case 19:
        func_80044750(0xD1);
        func_8004284C();
        break;
    case 20:
        func_800853FC();
        break;
    case 21:
        D_8014A0C0 = 1;
        func_80085B3C(2, 0x2D);
        D_800E6592 += 2;
        D_800E658E += 1;
        break;
    default:
        func_80046500();
        break;
    }
    func_80086424();
    return D_8014A0C0;
}
