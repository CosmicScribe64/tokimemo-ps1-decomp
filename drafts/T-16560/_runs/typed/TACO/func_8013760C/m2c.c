? func_80043A00(?, ?, ?, ?, s32, s32, s32);         /* extern */
extern s8 D_800E62B6;
extern s8 D_800E62B7;
extern s8 D_800E62B8;
extern s32 D_800E7378;
extern u8 D_800E738D;

void func_8013760C(void) {
    s32 temp_t9;
    s32 var_s0;

    switch (D_800E738D) {                           /* switch 1; irregular */
    case 0:                                         /* switch 1 */
        func_8013AFDC();
        D_8015F4F0 = 0x80;
        D_8015EDCC = 0;
        D_8015EDF0 = 0;
        D_8015EDB0 = 0x80;
        D_8015EDEC = 0x4650;
        func_80134884();
        D_8015EDBC = 0x10;
        D_800E62B6 = 0;
        D_800E62B7 = 0;
        D_800E62B8 = 0;
        func_800591D8(1);
        func_8004E9F4(0);
        func_800438F0(1);
        func_80143A74();
        func_8014394C();
        func_80059048();
        func_801347A8();
        func_80143AF4();
        var_s0 = 0;
        do {
            func_80099E30(0, &D_8015EDB4->unk0[var_s0] + 0x14);
            func_801438F0(0x801AC000, 0, (s32) &D_8015EDB4->unk0[var_s0]);
            var_s0 += 0x88;
        } while (var_s0 != 0x29F8);
        func_801322AC();
        func_80132220();
        func_801334BC();
        D_8011ED17 &= 0x7F;
        D_800E738D += 1;
        break;
    case 1:                                         /* switch 1 */
        if (D_8015EDBC == 0) {
            func_8015ABF0();
            D_8015EDB0 = 0x80;
            D_8015EDB4->unk84[1] = 0xFF;
            *(s8 *)((unsigned char *)D_8015EDB4 + (1221)) = 0xFF;
            temp_t9 = D_800E7378 & 3;
            switch (temp_t9) {                      /* switch 2; irregular */
            case 0:                                 /* switch 2 */
                D_8015EDB4->unk72 = 0;
                break;
            case 1:                                 /* switch 2 */
                D_8015EDB4->unk72 = 1;
                break;
            }
            func_80043A00(0x80162000, 0xE, 1, 0, 0, 0x100, 0x100);
            func_80043A00(0x80172000, 0xC, 1, 0, 0, 0x100, 0x100);
            func_80043A00(0x80182000, 0xA, 1, 0, 0, 0x80, 0x80);
            func_8004284C();
        }
        break;
    }
}
