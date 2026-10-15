? func_8004E93C(s32, ?);                            /* extern */
? func_800AE100(u8 *, ? *);                         /* extern */
extern ? D_800E6354;

void func_80140CC8(void) {
    s32 temp_v0;

    switch (D_8015EDD8) {
    case 0:
        func_800AE0F0(D_8015E828, &D_800E6354);
        func_800AE100(D_8015E828, "「よせっ！　紐緒さん！");
        func_8014B738((s32) D_8015E8F0, 1, 0x5A);
        func_8014EF3C();
        return;
    case 1:
        func_8014F178(0);
        return;
    case 2:
        func_8013E810(0x3B, 1, 1, 1);
        func_8014EF3C();
        return;
    case 3:
        func_8013F4F0();
        return;
    case 4:
        func_8013E810(0x3B, 0, 1, 1);
        func_8014EF3C();
        return;
    case 5:
        func_8014F080(0x1E);
        return;
    case 6:
        func_8013E97C(0x3B, 0x48 - D_8015EDE0, 0x70);
        temp_v0 = D_8015EDE0 + 0x10;
        D_8015EDE0 = temp_v0;
        if (temp_v0 >= 0x61) {
            func_8004E93C(D_8015EE14, 0);
            func_8014EF3C();
            return;
        }
    default:
        return;
    case 7:
        func_8014F080(0x1E);
        return;
    case 8:
        func_80140E70();
        return;
    case 9:
        func_8014EF8C();
        func_8004E93C(D_8015EDE8, 1);
        D_8015EDC4 = 0x08000001;
        break;
    }
}
