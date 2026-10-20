/* T-24301 draft, ~131 rows (with the func_8013E7C0 implicit int view of 8014ADD0.c): x stays in its
 * home slot in the original and the loop is strength-reduced differently (pointer, index, x - 9i, 9i counter). */
void func_8014BFE4(s32 slot, s32 x, s32 y, s32 value, s32 kind) {
    s32 i;
    s32 div;
    s32 digit;
    s32 base;

    x -= 0xA0;
    y -= 0x78;
    if (value >= 10) {
        if (value < 100) {
            x += 4;
        } else if (value < 1000) {
            x += 9;
        } else {
            x += 13;
        }
    }
    div = 1;
    switch (slot) {
    case 0:
        D_8015EE5C = 0;
        D_8015EE60 = 1;
        D_8015EE64 = 0;
        break;
    case 1:
        D_8015EE68 = 0;
        D_8015EE6C = 1;
        D_8015EE70 = 0;
        break;
    case 2:
        D_8015EE74 = 0;
        D_8015EE78 = 1;
        D_8015EE7C = 0;
        break;
    case 3:
        D_8015EE80 = 0;
        D_8015EE84 = 1;
        D_8015EE88 = 0;
        break;
    }
    base = slot * 12;
    for (i = 0; i < 4; i++) {
        ((DisplayActor44 *)D_8011ECD0)[base + i + 64].unk_26 = x - i * 9;
        ((DisplayActor44 *)D_8011ECD0)[base + i + 64].unk_2A = y;
        ((DisplayActor44 *)D_8011ECD0)[base + i + 68].unk_26 = x - i * 9;
        ((DisplayActor44 *)D_8011ECD0)[base + i + 68].unk_2A = y;
        ((DisplayActor44 *)D_8011ECD0)[base + i + 72].unk_26 = x - i * 9;
        ((DisplayActor44 *)D_8011ECD0)[base + i + 72].unk_2A = y;
        if (value / div != 0) {
            digit = value / div % 10;
            func_8013E864(base + i + 68, digit + 0x27, 0);
            func_8013E864(base + i + 72, digit + 0x27, 0);
            if (kind == 0) {
                func_8013E864(base + i + 64, digit + 0x1D, 0);
            } else {
                func_8013E864(base + i + 64, digit + 0x13, 0);
            }
        } else {
            ((DisplayActor44 *)D_8011ECD0)[base + i + 64].unk_26 = 0xA0;
            ((DisplayActor44 *)D_8011ECD0)[base + i + 64].unk_2A = 0x78;
            ((DisplayActor44 *)D_8011ECD0)[base + i + 68].unk_26 = 0xA0;
            ((DisplayActor44 *)D_8011ECD0)[base + i + 68].unk_2A = 0x78;
            ((DisplayActor44 *)D_8011ECD0)[base + i + 72].unk_26 = 0xA0;
            ((DisplayActor44 *)D_8011ECD0)[base + i + 72].unk_2A = 0x78;
        }
        div *= 10;
    }
    if (value == 0) {
        ((DisplayActor44 *)D_8011ECD0)[base + 64].unk_2A = y;
        ((DisplayActor44 *)D_8011ECD0)[base + 68].unk_2A = y;
        ((DisplayActor44 *)D_8011ECD0)[base + 72].unk_2A = y;
        ((DisplayActor44 *)D_8011ECD0)[base + 64].unk_26 = x;
        ((DisplayActor44 *)D_8011ECD0)[base + 68].unk_26 = x;
        ((DisplayActor44 *)D_8011ECD0)[base + 72].unk_26 = x;
        func_8013E864(base + 68, 0x27, 0);
        func_8013E864(base + 72, 0x27, 0);
        if (kind == 0) {
            func_8013E864(base + 64, 0x1D, 0);
        } else {
            func_8013E864(base + 64, 0x13, 0);
        }
    }
}
