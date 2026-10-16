/* NON_MATCHING: T-11810, 16 lines: the 0x40 argument of the t+0xC handler call is rematerialized (li a0) where the original reuses the hoisted 0x40 in a0, and the pad word *(u16 *)(g + 0x14) is a uopt CSE in v0 where the original keeps a ugen temp. */
void func_8013A0DC(void) {
    u8 *g;
    u8 *s;
    u8 *c;
    u8 *t;
    u8 *q;
    u8 *e;

    g = D_80158A60;
    c = D_80158A6C;
    t = (u8 *)D_80158A70;
    s = D_80158A64;
    *(s32 *)t += *(s32 *)c;
    switch (*(u16 *)(s + 4)) {
    case 0:
        if (s[0x22] != 0 && g[0x52] == 0xFF) {
            func_80044750(0xB1);
        } else {
            func_80044750(0x22);
            func_80044750(0x2F);
            func_80044750(4);
            func_80044750(0x203);
            c[0xF] = 1;
        }
        s[0x11] = 0;
        q = D_80158AA8 + 0x9D8;
        *(s16 *)(q + 0xA) = -0x24;
        *(s16 *)(q + 0xC) = 0x74;
        *(s16 *)(q + 0xE) = 9;
        q[2] = 2;
        q[0] = 1;
        *(s16 *)(q + 6) = 0;
        *(s16 *)(q + 0x10) = 0;
        if (*(u16 *)(t + 2) > 0x400) {
            *(s32 *)(c + 4) = 0;
        } else {
            *(s32 *)(c + 4) = 0x8000;
        }
        *(s16 *)(s + 0x26) = 0;
        *(u16 *)(s + 4) = 0x40;
        break;
    case 0x40:
        switch (*(u16 *)(s + 2)) {
        case 0x40:
        default:
            if (*(u16 *)(t + 2) > 0x400) {
                e = D_80158A8C;
                *(s16 *)(e + 4) = 0;
                e[0xC] = 0;
                *(s16 *)(e + 0xA) = 0;
                *(s16 *)(e + 8) = 0;
                *(s16 *)(e + 6) = 0;
                e[0xF] = 0;
                e[0xE] = 0;
                e[0xD] = 0;
                *(u16 *)(s + 2) = 0x50;
                }
            break;
        case 0x50:
            if (*(u16 *)(t + 0x1E) == 0) {
                (*(void (**)(s32, u8 *))(t + 0xC))(0x40, s);
            }
            break;
        }
        func_8013A040();
        if (s[9] != 0) {
            if (s[0x22] != 0) {
                if (g[0x52] == 0xFF) {
                    *(s16 *)s = 1;
                } else {
                    *(s16 *)s = 8;
                }
            } else {
                *(s16 *)s = 7;
            }
            *(s16 *)(s + 6) = 0;
            *(s16 *)(s + 4) = 0;
            *(s16 *)(s + 2) = 0;
            s[0xB] = 0;
            s[0xA] = 0;
            s[9] = 0;
        }
        break;
    }
    if (s[0x22] != 0 && g[0x52] == 0xFF && ((*(u16 *)(g + 0x14) & 0x800) || (*(u16 *)(g + 0x14) & 0x20))) {
        *(s16 *)s = 1;
        *(s16 *)(s + 6) = 0;
        *(s16 *)(s + 4) = 0;
        *(s16 *)(s + 2) = 0;
        s[0xB] = 0;
        s[0xA] = 0;
        s[9] = 0;
    }
    func_8014AB48();
    func_80147074();
    func_80149108();
    func_8013C0A8();
    func_801475C8();
    func_8014C680();
    func_801333D4();
    func_80134BBC();
    func_8013D150();
    func_80136830();
}
