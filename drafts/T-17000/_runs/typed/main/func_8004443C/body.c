void func_8004443C(void) {
    s32 sp74[1];
    s32 sp70;
    s32 sp6C;
    s32 sp68;
    s32 sp64;
    s16 sp5E;
    s16 sp5C;
    s16 sp5A;
    s16 sp58;
    s16 var_s1;
    s32 *var_s0;
    s32 *var_s0_2;
    s32 *var_s2;
    s32 temp_lo;
    s32 temp_v0;
    s32 var_s3;

    sp64 = (s32) D_800B3D10 / 264;
    var_s0 = &sp64;
    var_s2 = &D_800B3D10;
    var_s3 = 0;
    var_s1 = 0x8C;
    sp68 = (s32) D_800B3D14 / 264;
    sp6C = (s32) D_800B3D18 / 264;
    sp70 = (s32) D_800B3D1C / 264;
    do {
        temp_v0 = *var_s0;
        if (temp_v0 >= 3) {
            *var_s0 = 0x800000;
        } else if (temp_v0 >= 2) {
            *var_s0 = 0x808000;
        } else if (temp_v0 > 0) {
            *var_s0 = 0x8000;
        } else {
            *var_s0 = 0x80;
        }
        var_s0 += 1;
    } while ((u32) var_s0 < (u32) &sp74);
    sp5C = 4;
    var_s0_2 = &sp64;
    do {
        sp58 = var_s1;
        temp_lo = (s32) (*var_s2 * 0x10) / 264;
        sp5E = (s16) temp_lo;
        sp5A = 0x4C - temp_lo;
        func_8004A14C(var_s3 + 6, subroutine_arg1, subroutine_arg2, *var_s0_2);
        var_s3 += 1;
        var_s0_2 += 1;
        var_s1 += 4;
        var_s2 += 1;
    } while (var_s3 != 4);
    sp58 = 0x88;
    sp5E = 0x10;
    sp5A = 0x3C;
    func_8004A14C(var_s3 + 6, subroutine_arg1, subroutine_arg2, 0x808080);
}
