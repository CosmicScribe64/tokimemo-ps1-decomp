void func_80114B0C(void) {
    u8 sel;
    s16 i;
    s16 *p;
    s16 *r;
    s16 *q;
    s16 v;
    u8 temp;

    sel = D_800B0F35[D_800B1746];
    for (i = 0; i < 3; i++) {
        p = &D_80124394 + i;
        r = &D_8012439C + i;
        q = &D_801243A4 + i;
        *p = v = *r;
        if (*p == 0) {
            *p = 1;
        }
        if (*p == 4) {
            *p = 3;
        }
        temp = D_800B0F2C;
        if (temp == sel) {
            *p = 3;
        }
        *q = v - 1;
        if (*q == -1) {
            *q = 0;
        }
        if (*q == 3) {
            *q = 2;
        }
    }
    func_80011DFC();
}
