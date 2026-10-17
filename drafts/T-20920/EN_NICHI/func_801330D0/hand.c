void func_801330D0(void) {
    s32 sel;
    s32 v;
    s32 d;
    DisplayActor44 *p;

    sel = D_800E6280.unk_F80;
    if (sel & 0x40) {
        v = 3;
    } else {
        v = 2;
    }
    if (D_800E6280.unk_F74 == 1) {
        switch (sel & ~0x60) {
        case 0x1000:
            ((DisplayFixActor44 *)D_8011ECD0)->unk_28.w -= v << 16;
            break;
        case 0x2000:
            ((DisplayFixActor44 *)D_8011ECD0)->unk_24.w += v << 16;
            break;
        case 0x4000:
            ((DisplayFixActor44 *)D_8011ECD0)->unk_28.w += v << 16;
            break;
        case 0x8000:
            ((DisplayFixActor44 *)D_8011ECD0)->unk_24.w -= v << 16;
            break;
        case 0x3000:
            d = v * 0xB505;
            ((DisplayFixActor44 *)D_8011ECD0)->unk_24.w += d;
            ((DisplayFixActor44 *)D_8011ECD0)->unk_28.w -= d;
            break;
        case 0x6000:
            d = v * 0xB505;
            ((DisplayFixActor44 *)D_8011ECD0)->unk_24.w += d;
            ((DisplayFixActor44 *)D_8011ECD0)->unk_28.w += d;
            break;
        case 0xC000:
            d = v * 0xB505;
            ((DisplayFixActor44 *)D_8011ECD0)->unk_24.w -= d;
            ((DisplayFixActor44 *)D_8011ECD0)->unk_28.w += d;
            break;
        case 0x9000:
            d = v * 0xB505;
            ((DisplayFixActor44 *)D_8011ECD0)->unk_24.w -= d;
            ((DisplayFixActor44 *)D_8011ECD0)->unk_28.w -= d;
            break;
        }
    }
    p = (DisplayActor44 *)D_8011ECD0;
    if (p->unk_26 < -0x8F) {
        p->unk_26 = -0x90;
    }
    if (p->unk_26 >= 0x90) {
        p->unk_26 = 0x90;
    }
    if (p->unk_2A < -0x67) {
        p->unk_2A = -0x68;
    }
    if (p->unk_2A >= 0x68) {
        p->unk_2A = 0x68;
    }
    p[152].unk_26 = p->unk_26;
    p[152].unk_2A = p->unk_2A;
}
