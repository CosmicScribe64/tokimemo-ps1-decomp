? func_800AE100(u8 *, ? *);                         /* extern */
extern ? D_800E6354;
extern s16 D_80120E2A;
extern s16 D_80121336;

void func_80157640(void) {
    s32 temp_v0;

    switch (D_8015EDD8) {
    case 0:
        D_8015EC38 = 0x110 - D_8015EDE0;
        D_8015EDF0 = 0;
        D_8015EDE0 += 4;
        func_8013E97C(0x1D, 0, 0);
        func_8013E7C0(0x1D, 0x18, 1, 1);
        if (D_8015EDE0 >= 0xD1) {
            func_8014EF3C();
        default:
        }
        break;
    case 1:
        D_8015EDF0 = 0;
        func_8013E97C(0x1D, 0, 0);
        func_8013E7C0(0x1D, 0x18, 1, 1);
        func_8014EF3C();
        break;
    case 2:
        func_8014F080(0x1E);
        break;
    case 3:
        func_800AE0F0(D_8015E828, &D_800E6354);
        func_800AE100(D_8015E828, "達は逃げ出した。");
        func_8014B738((s32) D_8015E8F0, 1, 0x5A);
        func_8014EF3C();
        break;
    case 4:
        func_8014F178(0);
        break;
    case 5:
        D_80120E07 = 0x81;
        func_8013E7C0(0x30, 2, 1, 1);
        func_8013F15C(0x503, 1, 0);
        func_8014EF3C();
        break;
    case 6:
        D_80120E2A += 4;
        D_80121336 += 4;
        temp_v0 = D_8015EDE0 + 1;
        D_8015EDE0 = temp_v0;
        if (temp_v0 >= 0x21) {
            func_8014EF3C();
        }
        break;
    case 7:
        func_8014F080(0x3C);
        break;
    case 8:
        D_8015EC14 = 1;
        func_80042808();
        break;
    }
}
