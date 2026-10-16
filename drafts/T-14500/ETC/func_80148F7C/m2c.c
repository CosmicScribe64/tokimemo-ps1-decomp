extern ? D_80150748;

void func_80148F7C(u32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 sp5C;
    s32 temp_v0;
    s32 var_fp;
    s32 var_s4;
    u32 var_s1;

    var_fp = arg3;
    var_s4 = 0;
    if (arg4 == 1) {
        if (arg0 < 0xAU) {
            var_fp = 0;
        } else {
            var_fp = 2;
            if (arg0 < 0x64U) {
                var_fp = 1;
            }
        }
    }
    var_s1 = arg0;
    sp5C = arg4 + 0x1F0;
loop_7:
    temp_v0 = func_800490F0(0x14, D_8011ECA0);
    func_8009F0A4(temp_v0);
    func_8009EF84(temp_v0, 0);
    *(s8 *)((unsigned char *)temp_v0 + (4)) = 0x80;
    *(s8 *)((unsigned char *)temp_v0 + (5)) = 0x80;
    *(s8 *)((unsigned char *)temp_v0 + (6)) = 0x80;
    switch (var_fp) {                               /* irregular */
    case 0:
        *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) (((D_800E6280.unk_014[D_8011ECA0] + arg1) - (var_s4 * 8)) - (arg4 * 2));
        *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg2);
        *(s16 *)((unsigned char *)temp_v0 + (16)) = 8;
        *(s16 *)((unsigned char *)temp_v0 + (18)) = 0xC;
        *(s8 *)((unsigned char *)temp_v0 + (13)) = 0;
        *(u8 *)((unsigned char *)temp_v0 + (12)) = (u8) *(&D_80150748 + (var_s1 % 10U));
        break;
    case 1:
        *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) (((D_800E6280.unk_014[D_8011ECA0] + arg1) - (var_s4 * 8)) + (arg4 * 4));
        *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg2);
        *(s16 *)((unsigned char *)temp_v0 + (16)) = 8;
        *(s16 *)((unsigned char *)temp_v0 + (18)) = 0xC;
        *(s8 *)((unsigned char *)temp_v0 + (13)) = 0x10;
        *(u8 *)((unsigned char *)temp_v0 + (12)) = (u8) *(&D_80150748 + (var_s1 % 10U));
        break;
    case 2:
        *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) (((D_800E6280.unk_014[D_8011ECA0] + arg1) - (var_s4 * 6)) + (arg4 * 4));
        *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg2);
        *(s16 *)((unsigned char *)temp_v0 + (16)) = 6;
        *(s16 *)((unsigned char *)temp_v0 + (18)) = 0xC;
        *(s8 *)((unsigned char *)temp_v0 + (13)) = 0x20;
        *(u8 *)((unsigned char *)temp_v0 + (12)) = (u8) *(&D_80150748 + (var_s1 % 10U));
        break;
    }
    *(s16 *)((unsigned char *)temp_v0 + (14)) = func_8009ED74(0x100, sp5C);
    func_8009EED0((D_8011ECA0 << 0xA) + 4 + D_800E8CA0, temp_v0);
    if (var_s1 >= 0xAU) {
        var_s4 += 1;
        var_s1 = var_s1 / 10U;
        goto loop_7;
    }
    func_800494BC(0x140, 0, 1, 0, 0);
}
