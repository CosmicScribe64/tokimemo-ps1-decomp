void func_80148F7C(u32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 sp5C;
    s32 var_fp;
    s32 var_s4;
    u32 var_s1;
    SPRT *temp_v0;

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
    temp_v0 = (SPRT *)GetWorkBase(0x14, D_8011ECA0);
    func_8009F0A4(temp_v0);
    SetSemiTrans(temp_v0, 0);
    temp_v0->r0 = 0x80;
    temp_v0->g0 = 0x80;
    temp_v0->b0 = 0x80;
    switch (var_fp) {
    case 0:
        temp_v0->x0 = (s16) (((*(s16 *)((unsigned char *)(&D_800E6280 + (D_8011ECA0 * 2)) + (20)) + arg1) - (var_s4 * 8)) - (arg4 * 2));
        temp_v0->y0 = (s16) (*(s16 *)((unsigned char *)(&D_800E6280 + (D_8011ECA0 * 2)) + (24)) + arg2);
        temp_v0->w = 8;
        temp_v0->h = 0xC;
        temp_v0->v0 = 0;
        temp_v0->u0 = (u8) *(&D_80150748 + (var_s1 % 10U));
        break;
    case 1:
        temp_v0->x0 = (s16) (((*(s16 *)((unsigned char *)(&D_800E6280 + (D_8011ECA0 * 2)) + (20)) + arg1) - (var_s4 * 8)) + (arg4 * 4));
        temp_v0->y0 = (s16) (*(s16 *)((unsigned char *)(&D_800E6280 + (D_8011ECA0 * 2)) + (24)) + arg2);
        temp_v0->w = 8;
        temp_v0->h = 0xC;
        temp_v0->v0 = 0x10;
        temp_v0->u0 = (u8) *(&D_80150748 + (var_s1 % 10U));
        break;
    case 2:
        temp_v0->x0 = (s16) (((*(s16 *)((unsigned char *)(&D_800E6280 + (D_8011ECA0 * 2)) + (20)) + arg1) - (var_s4 * 6)) + (arg4 * 4));
        temp_v0->y0 = (s16) (*(s16 *)((unsigned char *)(&D_800E6280 + (D_8011ECA0 * 2)) + (24)) + arg2);
        temp_v0->w = 6;
        temp_v0->h = 0xC;
        temp_v0->v0 = 0x20;
        temp_v0->u0 = (u8) *(&D_80150748 + (var_s1 % 10U));
        break;
    }
    temp_v0->clut = func_8009ED74(0x100, sp5C);
    AddPrim((D_8011ECA0 << 0xA) + 4 + &D_800E8CA0, temp_v0);
    if (var_s1 >= 0xAU) {
        var_s4 += 1;
        var_s1 = var_s1 / 10U;
        goto loop_7;
    }
    dtd_on_tpage(0x140, 0, 1, 0, 0);
}
