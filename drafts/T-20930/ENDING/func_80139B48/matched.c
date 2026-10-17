    func_80139B48(-0x64, -0x50, 0xC8, 0x10, arg0, 0, 1);
    func_80139B48(-0x64, 0x40, 0xC8, 0x10, arg0, 0, 1);
    dtd_on(0);
    func_80139B48(-0x64, -0x40, 0xC8, 0x80, arg0, 0xB, 1);
    dtd_on(0xB);
}

/* FAKE: the explicit "& 0xFF" masks and one source line per colour trio, as in the matched main sibling
 * _sprite_set_box_shade_tarao (T-17001; one colour instead of two): the masks leave the shifts in ugen
 * temporaries (t6, t7) and as1 schedules the stores by source line. The real source is unknown. T-20930 */
void func_80139B48(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u32 arg4, s32 arg5, s32 arg6) {
    GameState *gs;
    POLY_G4 *p;

    p = (POLY_G4 *)GetWorkBase(0x24, D_8011ECA0);
    func_8009F054(p);
    SetSemiTrans(p, arg6);
    gs = &D_800E6280;
    p->r0 = (arg4 >> 16) & 0xFF; p->g0 = (arg4 >> 8) & 0xFF; p->b0 = arg4 & 0xFF;
    p->r1 = (arg4 >> 16) & 0xFF; p->g1 = (arg4 >> 8) & 0xFF; p->b1 = arg4 & 0xFF;
    p->r2 = (arg4 >> 16) & 0xFF; p->g2 = (arg4 >> 8) & 0xFF; p->b2 = arg4 & 0xFF;
    p->r3 = (arg4 >> 16) & 0xFF; p->g3 = (arg4 >> 8) & 0xFF; p->b3 = arg4 & 0xFF;
    p->x0 = gs->unk_014[D_8011ECA0] + arg0;
    p->y0 = gs->unk_018[D_8011ECA0] + arg1;
    p->x1 = gs->unk_014[D_8011ECA0] + arg0 + arg2;
    p->y1 = gs->unk_018[D_8011ECA0] + arg1;
    p->x2 = gs->unk_014[D_8011ECA0] + arg0;
    p->y2 = gs->unk_018[D_8011ECA0] + arg1 + arg3;
    p->x3 = gs->unk_014[D_8011ECA0] + arg0 + arg2;
    p->y3 = gs->unk_018[D_8011ECA0] + arg1 + arg3;
    AddPrim(D_800E8CA0 + (D_8011ECA0 << 10) + arg5 * 4, (s32)p);
}
