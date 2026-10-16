s32 func_8013B460(s32 arg0) {
    s32 pad; /* FAKE: unused word gives the frame home slot of the original; real source unknown. T-19030 */
    POLY_FT4 *p;
    s16 gray; /* FAKE: folded 0x80 local reserves a home word; real source unknown. T-19030 */

    gray = 0x80;
    p = (POLY_FT4 *)GetWorkBase(0x28, D_8011ECA0);
    func_8009F040(p);
    SetSemiTrans(p, 0);
    SetShadeTex(p, 0);
    p->tpage = func_8009ECB0(2, 0, 0x200, 0);
    p->r0 = gray;
    p->g0 = gray;
    p->b0 = gray;
    p->x0 = D_800E6280.unk_014[D_8011ECA0] + (((0x64 - arg0) << 9) / 100 - ((0x64 - arg0) * 0x10) / 100) - 0x100;
    p->y0 = D_800E6280.unk_018[D_8011ECA0] + ((0x64 - arg0) << 6) / 100 - 0x78;
    p->x1 = D_800E6280.unk_014[D_8011ECA0] + (((0x64 - arg0) << 9) / 100 - ((0x64 - arg0) * 0x10) / 100) + (arg0 << 8) / 100 - 0x100;
    p->y1 = D_800E6280.unk_018[D_8011ECA0] + ((0x64 - arg0) << 6) / 100 - 0x78;
    p->x2 = D_800E6280.unk_014[D_8011ECA0] + (((0x64 - arg0) << 9) / 100 - ((0x64 - arg0) * 0x10) / 100) - 0x100;
    p->y2 = D_800E6280.unk_018[D_8011ECA0] + ((0x64 - arg0) << 6) / 100 + (arg0 * 0xF0) / 100 - 0x78;
    p->x3 = D_800E6280.unk_014[D_8011ECA0] + (((0x64 - arg0) << 9) / 100 - ((0x64 - arg0) * 0x10) / 100) + (arg0 << 8) / 100 - 0x100;
    p->y3 = D_800E6280.unk_018[D_8011ECA0] + ((0x64 - arg0) << 6) / 100 + (arg0 * 0xF0) / 100 - 0x78;
    p->v3 = 0xF0;
    p->u3 = 0xFF;
    p->v2 = 0xF0;
    p->u2 = 0;
    p->v1 = 0;
    p->u1 = 0xFF;
    p->v0 = 0;
    p->u0 = 0;
    AddPrim((D_8011ECA0 << 0xA) + 0x3FC + D_800E8CA0, p);
    p = (POLY_FT4 *)GetWorkBase(0x28, D_8011ECA0);
    func_8009F040(p);
    SetSemiTrans(p, 0);
    SetShadeTex(p, 0);
    p->tpage = func_8009ECB0(2, 0, 0x300, 0);
    p->r0 = gray;
    p->g0 = gray;
    p->b0 = gray;
    p->x0 = D_800E6280.unk_014[D_8011ECA0] + (((0x64 - arg0) << 8) / 100 - ((0x64 - arg0) * 0x10) / 100);
    p->y0 = D_800E6280.unk_018[D_8011ECA0] + ((0x64 - arg0) << 6) / 100 - 0x78;
    p->x1 = D_800E6280.unk_014[D_8011ECA0] + (((0x64 - arg0) << 8) / 100 - ((0x64 - arg0) * 0x10) / 100) + (arg0 << 8) / 100;
    p->y1 = D_800E6280.unk_018[D_8011ECA0] + ((0x64 - arg0) << 6) / 100 - 0x78;
    p->x2 = D_800E6280.unk_014[D_8011ECA0] + (((0x64 - arg0) << 8) / 100 - ((0x64 - arg0) * 0x10) / 100);
    p->y2 = D_800E6280.unk_018[D_8011ECA0] + ((0x64 - arg0) << 6) / 100 + (arg0 * 0xF0) / 100 - 0x78;
    p->x3 = D_800E6280.unk_014[D_8011ECA0] + (((0x64 - arg0) << 8) / 100 - ((0x64 - arg0) * 0x10) / 100) + (arg0 << 8) / 100;
    p->y3 = D_800E6280.unk_018[D_8011ECA0] + ((0x64 - arg0) << 6) / 100 + (arg0 * 0xF0) / 100 - 0x78;
    p->v3 = 0xF0;
    p->u3 = 0xFF;
    p->v2 = 0xF0;
    p->u2 = 0;
    p->v1 = 0;
    p->u1 = 0xFF;
    p->v0 = 0;
    p->u0 = 0;
    AddPrim((D_8011ECA0 << 0xA) + 0x3FC + D_800E8CA0, p);
}
