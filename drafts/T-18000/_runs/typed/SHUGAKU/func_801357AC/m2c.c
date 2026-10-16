? func_8004BBA8(s32 *, ?, ?, ?, s32);               /* extern */

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
    *(s8 *)((unsigned char *)D_8011ECD0 + (6600)) = 8;
    *(s8 *)((unsigned char *)D_8011ECD0 + (6532)) = 8;
    *(s8 *)((unsigned char *)D_8011ECD0 + (6597)) = 0xB;
    *(s8 *)((unsigned char *)D_8011ECD0 + (6529)) = 0xB;
    M2C_MEMCPY_ALIGNED(D_8011ECD0 + 0x1A08, D_8011ECD0 + 0x1980, 0x44);
    *(s8 *)((unsigned char *)D_8011ECD0 + (6666)) = 0x40;
    *(s16 *)((unsigned char *)D_8011ECD0 + (6688)) = 0x14;
    func_8004BBA8(D_800E6280.unk_163C, 0xFFFFFF, 0xFFFFFF, 0x10, 0);
    func_80043914((s32) D_800E6280.unk_163C, 0x1F, 1, 3, 0);
    D_800E6280.unk_1228[0x25] = 1;
    func_80046754(0x801C4000, 0x25, 0x80);
    func_8004284C();
}
