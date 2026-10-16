? func_8004F984(?, s16, s16);                       /* extern */
extern u8 D_800E6375;
extern u8 D_800E6376;
extern s16 D_800E637E;
extern s16 D_800E6382;
extern s16 D_800E6386;
extern s16 D_800E638A;
extern s16 D_800E638E;
extern s16 D_800E6392;
extern s16 D_800E6396;
extern s16 D_800E639A;
extern s16 D_800E639E;
extern s32 D_800E7200;
extern u8 D_800E7313;
extern s32 D_8013E784;

void func_8013AF10(void) {
    s32 *var_s1;
    s32 var_s0;

    func_8004F984(0, D_8011ECF6, D_8011ECFA);
    func_8004FC10(0);
    func_80050B54(0);
    var_s1 = &D_8013E784;
    var_s0 = -0x60;
    do {
        func_8004B338(*var_s1, -0x20, var_s0);
        var_s0 += 0xC;
        var_s1 += 4;
    } while (var_s0 != 0x60);
    func_8004AE28(D_800E6280.unk_0F4.b[1] & 0xF, 0x20, -0x60);
    func_8004AE28((s32) ((u16) D_800E6280.unk_0F4.h >> 0xC), 0x20, -0x54);
    func_8004AE28(D_800E6280.unk_0F6.h & 1, 0x20, -0x48);
    func_8004AE28((s32) ((u32) ((s32) *(u16 *)((unsigned char *)(&D_800E6280) + (244)) << 0xE) >> 0x1F), 0x20, -0x3C);
    func_8004AE28((s32) ((u32) ((s32) *(u16 *)((unsigned char *)(&D_800E6280) + (244)) << 0xD) >> 0x1F), 0x20, -0x30);
    func_8004AE28((s32) ((u32) ((s32) *(u16 *)((unsigned char *)(&D_800E6280) + (244)) << 0xC) >> 0x1F), 0x20, -0x24);
    func_8004AE28((s32) ((u32) ((s32) *(u16 *)((unsigned char *)(&D_800E6280) + (244)) << 0xB) >> 0x1F), 0x20, -0x18);
    func_8004AE28((s32) D_800E637E, 0x20, -0xC);
    func_8004AE28((s32) D_800E6382, 0x20, 0);
    func_8004AE28((s32) D_800E6386, 0x20, 0xC);
    func_8004AE28((s32) D_800E638A, 0x20, 0x18);
    func_8004AE28((s32) D_800E638E, 0x20, 0x24);
    func_8004AE28((s32) D_800E6392, 0x20, 0x30);
    func_8004AE28((s32) D_800E6396, 0x20, 0x3C);
    func_8004AE28((s32) D_800E639A, 0x20, 0x48);
    func_8004AE28((s32) D_800E639E, 0x20, 0x54);
    if (D_800E7200 & 0x90) {
        switch (D_800E7313) {
        case 0:
            D_800E6280.unk_0F4.b[1] = (func_8013A550(D_800E6280.unk_0F4.b[1] & 0xF, 3, 0) & 0xF) | (D_800E6280.unk_0F4.b[1] & 0xFFF0);
            break;
        case 1:
            D_800E6375 = (func_8013A550((s32) ((u16) D_800E6280.unk_0F4.h >> 0xC), 0xB, 0) * 0x10) | (D_800E6375 & 0xFF0F);
            break;
        case 2:
            D_800E6376 = (func_8013A550(D_800E6280.unk_0F6.h & 1, 1, 0) & 1) | (D_800E6376 & 0xFFFE);
            break;
        case 3:
            D_800E6280.unk_0F6.b[0] = ((func_8013A550((s32) ((u32) ((s32) *(u16 *)((unsigned char *)(&D_800E6280) + (244)) << 0xE) >> 0x1F), 1, 0) * 2) & 2) | (D_800E6280.unk_0F6.b[0] & 0xFFFD);
            break;
        case 4:
            D_800E6280.unk_0F6.b[0] = ((func_8013A550((s32) ((u32) ((s32) *(u16 *)((unsigned char *)(&D_800E6280) + (244)) << 0xD) >> 0x1F), 1, 0) * 4) & 4) | (D_800E6280.unk_0F6.b[0] & 0xFFFB);
            break;
        case 5:
            D_800E6280.unk_0F6.b[0] = ((func_8013A550((s32) ((u32) ((s32) *(u16 *)((unsigned char *)(&D_800E6280) + (244)) << 0xC) >> 0x1F), 1, 0) * 8) & 8) | (D_800E6280.unk_0F6.b[0] & 0xFFF7);
            break;
        case 6:
            D_800E6280.unk_0F6.b[0] = ((func_8013A550((s32) ((u32) ((s32) *(u16 *)((unsigned char *)(&D_800E6280) + (244)) << 0xB) >> 0x1F), 1, 0) * 0x10) & 0x10) | (D_800E6280.unk_0F6.b[0] & 0xFFEF);
            break;
        case 7:
            D_800E637E = func_8013A550((s32) D_800E637E, 0x3E7, 0);
            break;
        case 8:
            D_800E6382 = func_8013A550((s32) D_800E6382, 0x3E7, 0);
            break;
        case 9:
            D_800E6386 = func_8013A550((s32) D_800E6386, 0x3E7, 0);
            break;
        case 10:
            D_800E638A = func_8013A550((s32) D_800E638A, 0x3E7, 0);
            break;
        case 11:
            D_800E638E = func_8013A550((s32) D_800E638E, 0x3E7, 0);
            break;
        case 12:
            D_800E6392 = func_8013A550((s32) D_800E6392, 0x3E7, 0);
            break;
        case 13:
            D_800E6396 = func_8013A550((s32) D_800E6396, 0x3E7, 0);
            break;
        case 14:
            D_800E639A = func_8013A550((s32) D_800E639A, 0x3E7, 0);
            break;
        case 15:
            D_800E639E = func_8013A550((s32) D_800E639E, 0x3E7, 0);
            break;
        }
    }
}
