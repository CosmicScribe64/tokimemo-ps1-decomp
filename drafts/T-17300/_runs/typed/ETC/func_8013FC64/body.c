void func_8013FC64(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 sp64;
    s32 sp5C;
    s32 var_s1;
    s32 var_s2;
    s32 var_s5;
    SPRT *temp_v0;
    u8 *temp_v0_2;
    SPRT *temp_v0_3;

    if (arg0 < 0) {
        sp64 = 1;
        var_s1 = -arg0;
    } else {
        sp64 = 0;
        var_s1 = arg0;
    }
    var_s5 = 0;
    if (var_s1 >= 0) {
        sp5C = arg3 * 0x10;
        var_s2 = 0;
loop_5:
        temp_v0 = (SPRT *)GetWorkBase(0x14, D_8011ECA0);
        func_8009F0A4(temp_v0);
        temp_v0->w = 8;
        temp_v0->h = 8;
        SetSemiTrans(temp_v0, 0);
        temp_v0->r0 = 0x80;
        temp_v0->g0 = 0x80;
        temp_v0->b0 = 0x80;
        temp_v0->x0 = (s16) ((*(s16 *)((unsigned char *)(&D_800E6280 + (D_8011ECA0 * 2)) + (20)) + arg1) - var_s2);
        temp_v0->y0 = (s16) (*(s16 *)((unsigned char *)(&D_800E6280 + (D_8011ECA0 * 2)) + (24)) + arg2);
        temp_v0_2 = &D_801500A0 + ((var_s1 % 10) * 2);
        temp_v0->u0 = (u8) (*(s32 *)temp_v0_2);
        temp_v0->v0 = (u8) *(u8 *)((unsigned char *)temp_v0_2 + (1));
        temp_v0->clut = func_8009ED74(sp5C, 0x1FD);
        AddPrim((D_8011ECA0 << 0xA) + 4 + &D_800E8CA0, temp_v0);
        if (var_s1 >= 0xA) {
            var_s5 += 1;
            var_s2 += 8;
            var_s1 = var_s1 / 10;
            if (var_s1 >= 0) {
                goto loop_5;
            }
        }
    }
    if (sp64 != 0) {
        sp5C = arg3 * 0x10;
        temp_v0_3 = (SPRT *)GetWorkBase(0x14, D_8011ECA0);
        func_8009F0A4(temp_v0_3);
        temp_v0_3->w = 8;
        temp_v0_3->h = 8;
        SetSemiTrans(temp_v0_3, 0);
        temp_v0_3->r0 = 0x80;
        temp_v0_3->g0 = 0x80;
        temp_v0_3->b0 = 0x80;
        temp_v0_3->x0 = (s16) ((*(s16 *)((unsigned char *)(&D_800E6280 + (D_8011ECA0 * 2)) + (20)) + arg1) - ((var_s5 + 1) * 8));
        temp_v0_3->y0 = (s16) (*(s16 *)((unsigned char *)(&D_800E6280 + (D_8011ECA0 * 2)) + (24)) + arg2);
        temp_v0_3->u0 = (u8) D_801500B4;
        temp_v0_3->v0 = (u8) D_801500B5;
        temp_v0_3->clut = func_8009ED74(sp5C, 0x1FD);
        AddPrim((D_8011ECA0 << 0xA) + 4 + &D_800E8CA0, temp_v0_3);
    }
    dtd_on_tpage(0x3C0, 0, 1, 0, 1);
}
