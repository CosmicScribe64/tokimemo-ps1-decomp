extern ? D_800E6294;
extern ? D_800E6298;

void func_8013AD64(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 sp3C;
    s32 sp38;
    s32 sp34;
    s32 sp30;
    s16 var_v1_2;
    s32 temp_t3;
    s32 temp_t6;
    s32 var_t0;
    s32 var_t1;
    s32 var_v1;
    u8 *temp_v0;

    temp_t6 = (arg2 >> 0x10) & 0xFF;
    var_t0 = (arg2 >> 8) & 0xFF;
    temp_t3 = arg2 & 0xFF;
    var_v1 = temp_t6;
    var_t1 = temp_t3;
    if (arg3 < 2) {
        if (arg1 < 0x80) {
            var_v1 = (s32) (temp_t6 * arg1) / 128;
            var_t0 = (s32) (var_t0 * arg1) / 128;
            var_t1 = (s32) (temp_t3 * arg1) / 128;
        }
        var_v1 = (s32) (var_v1 * D_8015F4F0) / 128;
        var_t0 = (s32) (var_t0 * D_8015F4F0) / 128;
        var_t1 = (s32) (var_t1 * D_8015F4F0) / 128;
    }
    sp3C = var_v1;
    sp38 = var_t0;
    sp34 = var_t1;
    temp_v0 = func_800490F0(0xC, D_8011ECA0);
    func_8009F0B8(temp_v0);
    func_8009EF84(temp_v0, 0);
    *(s8 *)((unsigned char *)temp_v0 + (4)) = (s8) var_v1;
    *(s8 *)((unsigned char *)temp_v0 + (5)) = (s8) var_t0;
    *(s8 *)((unsigned char *)temp_v0 + (6)) = (s8) var_t1;
    sp30 = *(&D_800E6294 + (D_8011ECA0 * 2)) + ((s32) (arg1 * func_800A0070(arg0)) >> 0xC);
    if (arg3 == 0) {
        var_v1_2 = (*(&D_800E6298 + (D_8011ECA0 * 2)) + ((s32) (arg1 * func_800A0140(arg0)) >> 0xC)) - 0x28;
    } else {
        var_v1_2 = *(&D_800E6298 + (D_8011ECA0 * 2)) + ((s32) (arg1 * func_800A0140(arg0)) >> 0xC);
    }
    *(s16 *)((unsigned char *)temp_v0 + (10)) = var_v1_2;
    *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) sp30;
    if (arg3 < 2) {
        func_8009EED0((D_8011ECA0 << 0xA) + 0x3F8 + D_800E8CA0, temp_v0);
        return;
    }
    func_8009EED0((D_8011ECA0 << 0xA) + 0x40 + D_800E8CA0, temp_v0);
}
