s32 func_80144A04(u8 *arg0) {
    u8 *p;
    u8 *w;
    s32 i;
    u8 *q;

    p = D_80158A74;
    switch (*(u16 *)(arg0 + 4)) {
    case 0x40:
    case 0x60:
        switch (arg0[0x58]) {
        case 0:
            func_80133AD0(0x60A);
            arg0[0] = 0;
            break;
        case 1:
            func_80133AD0(0x605);
            func_8013BE08(arg0, 0x28);
            p[0x55] = 9;
            D_80158A78[0x92] = 9;
            *(s32 *)(arg0 + 0x34) = 0;
            *(s32 *)(arg0 + 0x2C) = 0;
            *(s32 *)(arg0 + 0x28) = 0;
            *(s16 *)(arg0 + 0x62) = 0;
            *(u16 *)(arg0 + 4) = 0x90;
            arg0[0xD] = 0;
            arg0[0xE] = 0;
            arg0[0xF] = 0;
            *(s16 *)(arg0 + 6) = 0;
            *(s16 *)(arg0 + 8) = 0;
            *(s16 *)(arg0 + 0xA) = 0;
            break;
        case 2:
            func_80133AD0(0x602);
            func_8013BE08(arg0, 0x29);
            p[0x54] += 1;
            *(s32 *)(arg0 + 0x34) = 0;
            *(s32 *)(arg0 + 0x2C) = 0;
            *(s32 *)(arg0 + 0x28) = 0;
            *(s16 *)(arg0 + 0x62) = 0;
            *(u16 *)(arg0 + 4) = 0x90;
            arg0[0xD] = 0;
            arg0[0xE] = 0;
            arg0[0xF] = 0;
            *(s16 *)(arg0 + 6) = 0;
            *(s16 *)(arg0 + 8) = 0;
            *(s16 *)(arg0 + 0xA) = 0;
            break;
        case 3:
            func_80133AD0(0x604);
            if (p[0x53] == 0) {
                func_8013BE08(arg0, 0x2A);
                p[0x53] = 1;
                q = D_80158A78;
                for (i = 0; i != 3; i++, q += 0xA4) {
                    func_80147FD0(q);
                }
            }
            *(s32 *)(arg0 + 0x34) = 0;
            *(s32 *)(arg0 + 0x2C) = 0;
            *(s32 *)(arg0 + 0x28) = 0;
            *(s16 *)(arg0 + 0x62) = 0;
            *(u16 *)(arg0 + 4) = 0x90;
            *(s16 *)(arg0 + 0xA) = 0;
            *(s16 *)(arg0 + 8) = 0;
            *(s16 *)(arg0 + 6) = 0;
            arg0[0xF] = 0;
            arg0[0xE] = 0;
            arg0[0xD] = 0;
            break;
        case 6:
            func_80133AD0(0x603);
            func_8013BE08(arg0, 0x2B);
            p[0x54] -= 1;
            if (p[0x54] == 0) {
                p[0x54] = 1;
            }
            *(s32 *)(arg0 + 0x34) = 0;
            *(s32 *)(arg0 + 0x2C) = 0;
            *(s32 *)(arg0 + 0x28) = 0;
            *(s16 *)(arg0 + 0x62) = 0;
            *(u16 *)(arg0 + 4) = 0x90;
            *(s16 *)(arg0 + 0xA) = 0;
            *(s16 *)(arg0 + 8) = 0;
            *(s16 *)(arg0 + 6) = 0;
            arg0[0xF] = 0;
            arg0[0xE] = 0;
            arg0[0xD] = 0;
            break;
        }
        break;
    case 0x20:
        break;
    case 0x70:
        if (D_80158A74[0x52] == 0) {
            w = D_80158A6C;
            if (*(u16 *)((u8 *)D_80158A70 + 2) < 0x401) {
                *(s32 *)(w + 4) = 0x8000;
                *(s32 *)w = 0x8000;
            }
            func_80144094();
            *(u16 *)(p + 4) = 0x60;
            *(s16 *)(p + 0xA) = 0;
            *(s16 *)(p + 8) = 0;
            *(s16 *)(p + 6) = 0;
            p[0xF] = 0;
            p[0xE] = 0;
            p[0xD] = 0;
            arg0[0] = 0;
        }
        break;
    }
}
