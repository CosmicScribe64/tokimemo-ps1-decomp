? func_8004F984(?, s16, s16);                       /* extern */
extern u8 D_800E62BD;
extern u8 D_800E62BE;
extern u8 D_800E62BF;
extern u8 D_800E62C0;
extern u8 D_800E62C1;
extern u16 D_800E62C2;
extern u8 D_800E6375;
extern u8 D_800E6378;
extern u8 D_800E644C;
extern s32 D_800E7200;
extern s32 D_800E7208;
extern u8 D_800E7313;
extern s32 D_8013E744;

void func_8013A7EC(void) {
    s32 *var_s2;
    s32 var_s0;
    s32 var_s1;
    u8 temp_s2;
    u8 var_s0_2;

    func_8004F984(0, D_8011ECF6, D_8011ECFA);
    func_8004FC10(0);
    func_80050B54(0);
    var_s2 = &D_8013E744;
    var_s1 = -0x60;
    do {
        func_8004B338(*var_s2, -0x20, var_s1);
        var_s1 += 0xC;
        var_s2 += 4;
    } while (var_s1 < 0x60);
    func_8004AE28((s32) D_800E62BE, 0x20, -0x60);
    func_8004AE28((s32) D_800E62BF, 0x20, -0x54);
    func_8004AE28((s32) D_800E62C0, 0x20, -0x48);
    func_8004AE28((s32) D_800E62C1, 0x20, -0x3C);
    func_8004AE28((s32) D_800E62BD, 0x20, -0x30);
    func_8004AE28((s32) D_800E62C2, 0x20, -0x24);
    func_8004AE28(D_800E6280.unk_0F8 & 0xF, 0x20, 0xC);
    func_8004AE28((s32) ((u32) (D_800E6280.unk_0F8 << 0x17) >> 0x1B), 0x20, 0x18);
    func_8004AE28((s32) ((u32) (D_800E6280.unk_0F8 << 0x15) >> 0x1E), 0x20, 0x24);
    func_8004AE28((s32) ((u16) D_800E6280.unk_0F4.h >> 0xC), 0x20, 0x30);
    func_8004AE28(D_800E6280.unk_1BC[0].unk_10.w & 0xF, 0x20, 0x3C);
    func_8004AE28((s32) ((u32) (D_800E6280.unk_1BC[0].unk_10.w << 0x17) >> 0x1B), 0x20, 0x48);
    func_8004AE28((s32) ((u32) (D_800E6280.unk_1BC[0].unk_10.w << 0x15) >> 0x1E), 0x20, 0x54);
    if (D_800E7200 & 0x90) {
        switch (D_800E7313) {
        case 2:
            if (D_800E7208 & 0x90) {
                temp_s2 = D_800E62BF;
                var_s0 = 0;
                if (D_800E62C1 != 6) {
                    var_s0_2 = D_800E62C1;
                    if ((s32) D_800E62C1 < 6) {
                        do {
                            func_80072338();
                            var_s0_2 += 1;
                        } while ((s32) var_s0_2 < 6);
                    }
                } else {
                    do {
                        func_80072338();
                        var_s0 += 1;
                    } while (var_s0 != 7);
                }
                if (temp_s2 != D_800E62BF) {
                    func_80042940(0xFE);
                }
            }
            break;
        case 4:
            D_800E62BD = func_8013A550((s32) D_800E62BD, 3, 0);
            break;
        case 9:
            D_800E6378 = (func_8013A550(D_800E6280.unk_0F8 & 0xF, 0xC, 1) & 0xF) | (D_800E6378 & 0xFFF0);
            break;
        case 10:
            *(s32 *)((unsigned char *)(&D_800E6280) + (248)) = (s16) (((func_8013A550((s32) ((u32) (D_800E6280.unk_0F8 << 0x17) >> 0x1B), 0x1F, 1) * 0x10) & 0x1F0) | ((u16) *(s32 *)((unsigned char *)(&D_800E6280) + (248)) & 0xFE0F));
            break;
        case 11:
            *(u8 *)((unsigned char *)(&D_800E6280) + (249)) = (u8) (((func_8013A550((s32) ((u32) (D_800E6280.unk_0F8 << 0x15) >> 0x1E), 3, 0) * 2) & 6) | (*(u8 *)((unsigned char *)(&D_800E6280) + (249)) & 0xFFF9));
            break;
        case 12:
            D_800E6375 = (func_8013A550((s32) ((u16) D_800E6280.unk_0F4.h >> 0xC), 0xB, 0) * 0x10) | (D_800E6375 & 0xFF0F);
            break;
        case 13:
            D_800E644C = (func_8013A550(D_800E6280.unk_1BC[0].unk_10.w & 0xF, 0xC, 1) & 0xF) | (D_800E644C & 0xFFF0);
            break;
        case 14:
            D_800E6280.unk_1BC[0].unk_10.h[0] = ((func_8013A550((s32) ((u32) (D_800E6280.unk_1BC[0].unk_10.w << 0x17) >> 0x1B), 0x1F, 1) * 0x10) & 0x1F0) | (D_800E6280.unk_1BC[0].unk_10.h[0] & 0xFE0F);
            break;
        case 15:
            D_800E6280.unk_1BC[0].unk_10.b[1] = ((func_8013A550((s32) ((u32) (D_800E6280.unk_1BC[0].unk_10.w << 0x15) >> 0x1E), 3, 0) * 2) & 6) | (D_800E6280.unk_1BC[0].unk_10.b[1] & 0xFFF9);
            break;
        }
    }
    if (D_800E7208 & 0x800) {
        func_80041878();
        func_80042940(0);
    }
}
