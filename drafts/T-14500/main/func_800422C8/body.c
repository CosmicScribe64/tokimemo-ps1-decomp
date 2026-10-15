void func_800422C8(void) {
    s32 id;

    id = func_80098370();
    if (D_800E6280.unk_1108 == 0x90) {
        D_8011ECA0 = id;
        func_800985E0((s32) D_800FD4A0 + id * 0x10C00);
        SetWorkBase((s32) D_800E94A0 + D_8011ECA0 * 0xA000, D_8011ECA0);
    } else {
        D_8011ECA0 = id;
        func_800985E0((s32) D_800E94A0 + id * 0xA000);
        SetWorkBase((s32) D_800FD4A0 + D_8011ECA0 * 0x10C00, D_8011ECA0);
    }
    func_800985F0(0, 0, (s32) &D_800E8C70 + D_8011ECA0 * 0x14);
    D_8011ECA4 = 0;
}
