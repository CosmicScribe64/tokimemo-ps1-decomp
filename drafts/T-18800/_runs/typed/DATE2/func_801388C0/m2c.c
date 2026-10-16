? func_8004F984(?, s16, s16);                       /* extern */

void func_801388C0(void) {
    s32 var_t0;
    s32 var_v0_2;
    u8 temp_t7;
    u8 var_v0;

    func_8004F984(0, D_8011ECD0->unk_26, D_8011ECD0->unk_2A);
    func_8004FC10(0);
    func_80050B54(0);
    var_v0 = D_800B3CFC;
    if ((var_v0 != 0) && ((u16) D_8013B5D0 != var_v0)) {
        func_80044750(0x601);
        temp_t7 = (D_8013B5CC + 1) & 0xFF;
        D_8013B5CC = temp_t7;
        if (temp_t7 >= 3U) {
            D_8013B5CC = 1;
        }
        D_800E6280.unk_110D = 0;
        var_v0 = 0;
        D_800B3CF8 = 0;
    }
    D_8013B5D0 = (s16) var_v0;
    D_800B3CFC = var_v0;
    if (D_800E6280.unk_F88 & 0x600060) {
        var_v0_2 = 0;
        var_t0 = 0 * 0x10;
loop_7:
        if ((*(u8 *)((unsigned char *)(&D_800E6280 + var_t0) + (3990)) != 0) && (D_8013B5E4[var_v0_2] != 0)) {
            D_80122CDC = var_v0_2;
            D_8013B5C4 = (u8) var_v0_2;
            D_80122D30 = 0;
            D_8011ECD0->unk_03 &= 0xFF7F;
            func_8004E9F4(1);
            D_800E6280.unk_1104.w += 1;
            D_800E6280.unk_110D = 0;
            return;
        }
        var_v0_2 = (s32) (s16) (var_v0_2 + 1);
        var_t0 = var_v0_2 * 0x10;
        if (var_v0_2 >= 4) {

        } else {
            goto loop_7;
        }
    }
}
