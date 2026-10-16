? func_8004BBA8(? *, ?, ?, ?, s32);                 /* extern */
extern s32 D_800E753C;
extern ? D_800E78BC;

void func_801357AC(void) {
    s16 var_v0;
    s32 temp_lo;

    func_80135430();
    func_80043914(D_8013C2E0, 0x11, 1, 2, 0);
    var_v0 = 0;
    do {
        temp_lo = var_v0 * 0x24;
        var_v0 += 1;
        *(s16 *)((unsigned char *)(D_801217D0 + temp_lo) + (2096)) = 0;
    } while (var_v0 < 6);
    func_800850D4(D_8013C2D4, D_8013C2D8, D_8013C2D0, (s32) D_8013C2DC);
    D_80120698 = 8;
    D_80120654 = 8;
    D_80120695 = 0xB;
    D_80120651 = 0xB;
    M2C_MEMCPY_ALIGNED(D_801206D8, D_80120650, 0x44);
    D_801206DA = 0x40;
    D_801206F0 = 0x14;
    func_8004BBA8(&D_800E78BC, 0xFFFFFF, 0xFFFFFF, 0x10, 0);
    func_80043914((s32) &D_800E78BC, 0x1F, 1, 3, 0);
    D_800E753C = 1;
    func_80046754(0x801C4000, 0x25, 0x80);
    func_8004284C();
}
