? func_80042940(?);                                 /* extern */
s32 func_800460EC();                                /* extern */
? func_8004B19C(u32, ?, ?, void *);                 /* extern */
? func_801436E4(u8);                                /* extern */
? func_80143968();                                  /* extern */
? func_8014407C(u32);                               /* extern */
? func_801441DC(?, s32, s32, u32);                  /* extern */
extern ? D_800E6280;
extern ? D_801217D0;
extern s32 D_80150148;
extern ? D_8015014C;

void func_80144224(void) {
    u32 sp30;
    s32 sp28;
    s32 temp_a2;
    s32 temp_a2_2;
    s32 temp_v1;
    s32 var_a0;
    u32 temp_a3;
    u32 temp_hi;
    u32 temp_hi_2;
    void *temp_v0;

    D_801217D0.unk0 = 0x80000000;
    *(s32 *)((unsigned char *)(&D_801217D0) + (36)) = 0x80000000;
    func_80143968();
    temp_a3 = *(u32 *)((unsigned char *)(&D_800E6280) + (4356));
    temp_hi = temp_a3 % 701U;
    sp30 = temp_a3 / 701U;
    if (temp_hi == 1) {
        temp_v1 = D_80150148;
        if (temp_v1 == 0) {
            func_801436E4(*(u8 *)((unsigned char *)(&D_800E6280) + (3935)));
        } else if (temp_v1 < 0xF) {
            temp_v0 = (temp_v1 * 4) + (&D_8015014C + 4);
            var_a0 = *(s32 *)((unsigned char *)temp_v0 + (-4));
            D_80150148 = temp_v1;
            if (*(u8 *)((unsigned char *)(&D_800E6280) + (3935)) == var_a0) {
                var_a0 = temp_v0->unk0;
                D_80150148 = temp_v1 + 1;
            }
            func_801436E4((u8) var_a0);
        } else {
            D_80150148 = temp_v1;
            func_801436E4(D_800E6280.unk0);
        }
        D_80150148 += 1;
    } else if ((s32) sp30 < 0xF) {
        if (temp_hi == 2) {
            func_8014407C(sp30);
        } else if (temp_hi < 0xA0U) {
            temp_a2 = temp_hi * 3;
            sp28 = temp_a2;
            func_801441DC(2, temp_a2 / 4, temp_a2, temp_a3);
            func_801441DC(3, (s32) (sp28 - 0x3C) / 4);
            func_801441DC(4, (s32) (sp28 - 0x78) / 4);
        }
    }
    if (D_80150148 < 0x10) {
        temp_hi_2 = temp_a3 % 701U;
        if (temp_hi_2 >= 0x21EU) {
            temp_a2_2 = (0x2BD - temp_hi_2) * 3;
            sp28 = temp_a2_2;
            func_801441DC(2, (s32) (temp_a2_2 - 0x78) / 4, temp_a2_2, temp_a3);
            func_801441DC(3, (s32) (sp28 - 0x3C) / 4);
            func_801441DC(4, sp28 / 4);
        }
    }
    if ((temp_a3 >= 0x259U) && (func_800460EC() & 2)) {
        func_80042940(2);
    }
    func_8004B19C(*(u32 *)((unsigned char *)(&D_800E6280) + (4356)), 0, -0xA, &D_800E6280 + 0x1104);
    func_8004B19C(sp30, 0, 0);
    func_8004B19C((u32) D_80150148, 0, 0xA);
    func_8004B19C(*(&D_8015014C + (D_80150148 * 4)), 0, 0x14);
}
