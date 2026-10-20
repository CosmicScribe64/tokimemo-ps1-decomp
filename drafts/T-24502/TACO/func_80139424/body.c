void func_80139424(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    s16 temp_v1;
    s32 temp_ra;
    s32 temp_t5;
    s32 temp_t7;
    s32 temp_t8;
    POLY_FT4 *temp_v0;
    s32 temp_v0_3;
    s32 var_t0;
    s32 var_t1;
    s32 var_t2;
    s32 var_t4;
    s32 rightU;
    s32 var_t8;
    u32 temp_s1;
    u8 *temp_v0_2;

    temp_s1 = arg0 & 0xFF;
    if (arg8 == 3) {
        var_t0 = 0xC;
    } else {
        var_t0 = 0xB;
    }
    temp_v0 = (POLY_FT4 *)GetWorkBase(sizeof(POLY_FT4), D_8011ECA0);
    func_8009F040((s32)temp_v0);
    SetShadeTex(temp_v0, 0);
    SetSemiTrans(temp_v0, 0);
    temp_v0->b0 = arg3;
    temp_v0->g0 = arg3;
    temp_v0->r0 = arg3;
    temp_v1 = D_800E6280.unk_014[D_8011ECA0];
    temp_t7 = (s32) (var_t0 * arg6) / 8192;
    temp_ra = (temp_v1 + arg4) - temp_t7;
    temp_t5 = temp_v1 + arg4 + temp_t7;
    if (arg8 != 1) {
        temp_v1 = D_800E6280.unk_018[D_8011ECA0];
        temp_t8 = (s32) (arg7 * 0xC) / 8192;
        var_t2 = (temp_v1 + arg5) - temp_t8;
        var_t4 = temp_v1 + arg5 + temp_t8;
        var_t1 = var_t2;
    } else {
        temp_v1 = D_800E6280.unk_018[D_8011ECA0];
        var_t2 = (temp_v1 + arg5) - ((s32) (arg7 * 0xC) / 4096);
        var_t4 = temp_v1 + arg5;
        var_t1 = var_t2;
    }
    temp_v0->tpage = func_8009ECB0(0, 0, 0x340, 0x180);
    temp_v0->y0 = var_t1;
    temp_v0->x0 = temp_ra;
    temp_v0->y1 = var_t2;
    temp_v0->x1 = temp_t5;
    temp_v0->y2 = var_t4;
    temp_v0->y3 = var_t4;
    temp_v0->x3 = temp_t5;
    temp_v0->x2 = temp_ra;
    if ((temp_s1 >= 0x30U) && (temp_s1 < 0x3AU)) {
        temp_v0_2 = (((temp_s1 - 0x30) & 0xFF) * 2) + D_8015F5A0;
        temp_v0->u0 = (temp_v0_2[0] + 0x80);
        temp_v0->v0 = temp_v0_2[1];
        temp_v0->u1 = (temp_v0_2[0] + var_t0 + 0x80);
        temp_v0->v1 = temp_v0_2[1];
        temp_v0->u2 = (temp_v0_2[0] + 0x80);
        temp_v0->v2 = (temp_v0_2[1] + 0xC);
        temp_v0->u3 = (temp_v0_2[0] + var_t0 + 0x80);
        temp_v0->v3 = (temp_v0_2[1] + 0xC);
    } else {
        temp_v0_3 = temp_s1 - 0x41;
        if ((u32) (temp_v0_3 & 0xFF) < 0x1AU) {
            temp_v0_2 = ((temp_v0_3 & 0xFF) * 2) + D_8015F5B4;
            temp_v0->u0 = (temp_v0_2[0] + 0x80);
            temp_v0->v0 = temp_v0_2[1];
            temp_v0->u1 = (temp_v0_2[0] + var_t0 + 0x80);
            temp_v0->v1 = temp_v0_2[1];
            temp_v0->u2 = (temp_v0_2[0] + 0x80);
            temp_v0->v2 = (temp_v0_2[1] + 0xC);
            temp_v0->u3 = (temp_v0_2[0] + var_t0 + 0x80);
            var_t8 = temp_v0_2[1] + 0xC;
            goto block_24;
        }
        if (temp_s1 == 0x2E) {
            temp_v0->u0 = (D_8015F5E8 + 0x80);
            temp_v0->v0 = D_8015F5E9;
            temp_v0->u1 = (D_8015F5E8 + var_t0 + 0x80);
            temp_v0->v1 = D_8015F5E9;
            temp_v0->u2 = (D_8015F5E8 + 0x80);
            temp_v0->v2 = (D_8015F5E9 + 0xC);
            temp_v0->u3 = (D_8015F5E8 + var_t0 + 0x80);
            var_t8 = D_8015F5E9 + 0xC;
            goto block_24;
        }
        if (temp_s1 == 0x2D) {
            temp_v0->u0 = (D_8015F5EA + 0x80);
            temp_v0->v0 = D_8015F5EB;
            temp_v0->u1 = (D_8015F5EA + var_t0 + 0x80);
            temp_v0->v1 = D_8015F5EB;
            temp_v0->u2 = (D_8015F5EA + 0x80);
            temp_v0->v2 = (D_8015F5EB + 0xC);
            temp_v0->u3 = (D_8015F5EA + var_t0 + 0x80);
            var_t8 = D_8015F5EB + 0xC;
            goto block_24;
        }
        if (temp_s1 == 0x21) {
            temp_v0->u0 = (D_8015F5EE + 0x80);
            temp_v0->v0 = D_8015F5EF;
            temp_v0->u1 = (D_8015F5EE + var_t0 + 0x80);
            temp_v0->v1 = D_8015F5EF;
            temp_v0->u2 = (D_8015F5EE + 0x80);
            temp_v0->v2 = (D_8015F5EF + 0xC);
            temp_v0->u3 = (D_8015F5EE + var_t0 + 0x80);
            var_t8 = D_8015F5EF + 0xC;
            goto block_24;
        }
        if (temp_s1 == 0x40) {
            rightU = var_t0 + 0xA6;
            temp_v0->u1 = rightU;
            temp_v0->u3 = rightU;
            temp_v0->u0 = 0xA6;
            temp_v0->u2 = 0xA6;
            temp_v0->v2 = 0xDF;
            temp_v0->v3 = 0xDF;
            temp_v0->v0 = 0xD3U;
            temp_v0->v1 = 0xD3U;
        } else if (temp_s1 == 0x2A) {
            rightU = var_t0 + 0xBE;
            temp_v0->u1 = rightU;
            temp_v0->u3 = rightU;
            temp_v0->u0 = 0xBE;
            temp_v0->u2 = 0xBE;
            temp_v0->v2 = 0xDF;
            temp_v0->v3 = 0xDF;
            temp_v0->v0 = 0xD3U;
            temp_v0->v1 = 0xD3U;
        } else if (temp_s1 == 0x23) {
            rightU = var_t0 + 0xEE;
            temp_v0->u1 = rightU;
            temp_v0->u3 = rightU;
            temp_v0->u0 = 0xEE;
            temp_v0->u2 = 0xEE;
            temp_v0->v2 = 0xDF;
            temp_v0->v3 = 0xDF;
            temp_v0->v0 = 0xD3U;
            temp_v0->v1 = 0xD3U;
        } else {
            temp_v0->u0 = (D_8015F5EC + 0x80);
            temp_v0->v0 = D_8015F5ED;
            temp_v0->u1 = (D_8015F5EC + var_t0 + 0x80);
            temp_v0->v1 = D_8015F5ED;
            temp_v0->u2 = (D_8015F5EC + 0x80);
            temp_v0->v2 = (D_8015F5ED + 0xC);
            temp_v0->u3 = (D_8015F5EC + var_t0 + 0x80);
            var_t8 = D_8015F5ED + 0xC;
block_24:
            temp_v0->v3 = var_t8;
        }
    }
    temp_v0->clut = func_8009ED74(arg1 * 0x10, 0x1EF);
    AddPrim(D_800E8CA0 + (D_8011ECA0 << 0xA) + arg2 * 4, (s32)temp_v0);
    dtd_on_tpage(0x340, 0x180, arg2, 0, 0);
}
