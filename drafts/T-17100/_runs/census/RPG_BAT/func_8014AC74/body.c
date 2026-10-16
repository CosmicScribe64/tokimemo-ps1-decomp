void func_8014AC74(void) {
    s32 i;
    s32 *p;
    u8 *rec;
    s32 t;
    s32 t5;
    u8 *ent;
    s32 v;

    p = &D_8015EBAC;
    i = 0;
    do {
        if (p[0x32] == 1) {
            t = i * 3;
            rec = D_8011ECD0 + t * 0x44;
            t5 = t + 5;
            ent = (u8 *)&D_8015EBAC + i * 8;
            if (!(rec[0x1A92] & 1)) {
                func_8013E810(t + 4, 7, 1, 1);
            }
            v = *(s16 *)(rec + 0x1B30);
            if (v >= 0x10) {
                v = 0x1E - v;
            }
            func_8013E97C(t5, *(s32 *)(ent + 0xDC), *(s32 *)(ent + 0xE0) - v * 8);
            if (!(rec[0x1B1A] & 1)) {
                func_8013E810(t + 4, 0xFF, 1, 0);
                func_8013E810(t5, 0xFF, 1, 0);
                func_8013E810(t + 6, 0xFF, 1, 0);
            }
        }
        i++;
        p++;
    } while (i != 4);
}
