/* T-24301 draft, 57 rows (gstate view; ECF6 arm loads and the hoisted ECFA lui differ). */
void func_8006BA40(void) {
    GameState *g = &D_800E6280;
    u8 speed;
    s32 step;
    s16 x;
    s16 v;

    if ((u8)D_8011ECA4 == 0) {
        speed = g->unk_F6F;
        D_8011ECA4 = (u8)D_8011ECA4 + 1;
        if (speed == 0) {
            speed = 1;
        }
        g->unk_F6F = speed;
        step = speed;
        if (!(g->unk_1092 & 3) && (g->unk_F80 & 0xFF)) {
            step = speed * 2;
        }
        if (g->unk_F80 & 0xF) {
            x = D_8011ECF6;
        } else {
            switch (g->unk_F80 & 0x5000) {
            case 0x1000:
                D_8011ECFA -= step;
                break;
            case 0x4000:
                D_8011ECFA += step;
                break;
            }
            switch (g->unk_F80 & 0xA000) {
            case 0x2000:
                x = D_8011ECF6 + step;
                break;
            case 0x8000:
                x = D_8011ECF6 - step;
                break;
            default:
                x = D_8011ECF6;
                break;
            }
        }
        if (x >= 0x8D) {
            x = 0x8C;
        }
        D_8011ECF6 = x;
        if (x < -0x96) {
            D_8011ECF6 = -0x96;
        }
        if (D_8011ECFA < -0x64) {
            D_8011ECFA = -0x64;
        }
        if (D_8011ECFA >= 0x65) {
            D_8011ECFA = 0x64;
        }
        if (g->unk_F88 & 0x80) {
            if (D_800B6708 == 0) {
                v = 0x9B;
            } else if (D_800B6708 == 0xAC) {
                v = 0;
            } else {
                v = (D_800B6708 + 1) & 0xFF;
            }
            D_8011ECE8 = v;
            D_800B6708 = v;
        }
    }
}
