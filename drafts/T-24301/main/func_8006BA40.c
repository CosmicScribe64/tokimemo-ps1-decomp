/* T-24301 draft, 41 rows: D_8011ECF6 written as the promoted global (empty F80 & 0xF arm, += / -= in the
 * switch, clamps on the global) as the original's v1 store-backs show; left: one ugen temporary short at the
 * start (lui t0 of D_8011ECFA hoisted in the original) and the D_800B6708 value in $v0 (here $v1). */
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
                D_8011ECF6 += step;
                break;
            case 0x8000:
                D_8011ECF6 -= step;
                break;
            }
        }
        if (D_8011ECF6 >= 0x8D) {
            D_8011ECF6 = 0x8C;
        }
        if (D_8011ECF6 < -0x96) {
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
