? func_8004EBEC(?);                                 /* extern */
s32 func_8004EC14();                                /* extern */
extern s8 D_800E7314;
extern u8 D_800E738D;
extern ? D_8012071C;
extern s16 D_80120910;
extern s16 D_80120914;

void func_80133718(void) {
    s16 sp74;
    s16 sp68;
    ? sp5C;
    ? sp50;
    ? *var_a3;
    ? *var_t0;
    s16 *var_a0;
    s16 *var_v1;
    s16 temp_t6;
    s16 temp_t7;
    s16 temp_t8;
    s16 var_a2;
    s16 var_t1;
    s32 temp_v0;
    u8 *var_v0;
    u8 temp_t9;

    if (D_800E738D == 0) {
        D_80120910 = 1;
        D_8012091E = -0xA0;
        D_80120922 = -0x78;
        D_801208FB = 0;
        D_80120914 = 0x7FFF;
        D_800E738D = 1;
        *D_80120912 = 0x7FFF;
        return;
    }
    var_v1 = &sp74;
    var_a0 = &sp68;
    var_a2 = -0x58;
    var_a3 = &sp5C;
    var_t0 = &sp50;
    var_t1 = 0xE;
    temp_t6 = ((s16) *D_80120912 / 17) * 0x10;
    D_80120914 = ((s16) D_80120914 / 17) * 0x10;
    *D_80120912 = temp_t6;
    if (temp_t6 < 0x1001) {
        D_80120914 = 0x1000;
        *D_80120912 = 0x1000;
        var_v0 = D_80120650;
        do {
            temp_t9 = *(u8 *)((unsigned char *)var_v0 + (751));
            *var_v1 = 0x10;
            *var_a0 = var_a2;
            temp_t8 = *var_a0;
            temp_t7 = *var_v1;
            var_v0 += 0x44;
            *(s16 *)((unsigned char *)var_v0 + (704)) = var_t1;
            var_t1 += 2;
            var_a2 += 0x28;
            var_v1 += 2;
            var_a0 += 2;
            var_a3 += 2;
            var_t0 += 2;
            *(s16 *)((unsigned char *)var_a3 + (-2)) = 0x28;
            *(s16 *)((unsigned char *)var_t0 + (-2)) = 0x10;
            *(s8 *)((unsigned char *)var_v0 + (683)) = (s8) (temp_t9 | 0x80);
            *(s16 *)((unsigned char *)var_v0 + (722)) = temp_t8;
            *(s16 *)((unsigned char *)var_v0 + (718)) = temp_t7;
        } while (var_v0 != &D_8012071C);
        func_8004F870(1, 3, &sp74, &sp68, &sp5C, &sp50);
        temp_v0 = func_8004EC14();
        switch (temp_v0) {                          /* irregular */
        case 0:
block_13:
            D_800E7314 = 0;
            break;
        case 1:
            D_800E7314 = 1;
            break;
        case 2:
            D_800E7314 = 2;
            break;
        default:
            func_8004EBEC(0);
            goto block_13;
        }
        func_8004284C();
    }
}
