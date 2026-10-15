void func_801330D0(void) {
    s32 sel;
    s32 v;
    s32 d;
    volatile s16 *px;
    volatile s16 *py;

    sel = D_800E6280.unk_F80;
    if (sel & 0x40) {
        v = 3;
    } else {
        v = 2;
    }
    if (D_800E6280.unk_F74 == 1) {
        switch (sel & ~0x60) {
        case 0x1000:
            (&D_8011ECF4)[1] -= v << 16;
            break;
        case 0x2000:
            D_8011ECF4 += v << 16;
            break;
        case 0x4000:
            (&D_8011ECF4)[1] += v << 16;
            break;
        case 0x8000:
            D_8011ECF4 -= v << 16;
            break;
        case 0x3000:
            d = v * 0xB505;
            D_8011ECF4 += d;
            (&D_8011ECF4)[1] -= d;
            break;
        case 0x6000:
            d = v * 0xB505;
            D_8011ECF4 += d;
            (&D_8011ECF4)[1] += d;
            break;
        case 0xC000:
            d = v * 0xB505;
            D_8011ECF4 -= d;
            (&D_8011ECF4)[1] += d;
            break;
        case 0x9000:
            d = v * 0xB505;
            D_8011ECF4 -= d;
            (&D_8011ECF4)[1] -= d;
            break;
        }
    }
    px = (volatile s16 *)(D_8011ECD0 + 0x26);
    py = (volatile s16 *)(D_8011ECD0 + 0x2A);
    if (*px < -0x8F) {
        *px = -0x90;
    }
    if (*px >= 0x90) {
        *px = 0x90;
    }
    if (*py < -0x67) {
        *py = -0x68;
    }
    if (*py >= 0x68) {
        *py = 0x68;
    }
    D_80121556 = *px;
    D_8012155A = *py;
}
