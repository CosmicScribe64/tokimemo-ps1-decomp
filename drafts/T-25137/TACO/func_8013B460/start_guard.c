/* T-24705: the y3 store position is the only difference. Splitting the UV statement into 2, 4 or 8 lines (20 lines), the y3 store first or last in one comma chain (18/154), a same-line `p->y3` with the UV chain, and a y3 temporary in the pad slot (151) do not move it; the memory of the division already reaches the AddPrim block in the original. */
/* NON_MATCHING: T-24200, 4 differing lines (2 rows, one per quad): the p->y3 halfword store is scheduled before the eight
 * UV byte stores; the original computes y3 first but stores it after them (as1 order; a y3 temporary gets
 * that order but recolours the 0xFF/0xF0 constants). */
#ifdef NON_MATCHING
s32 func_8013B460(s32 arg0) {
    GameState *g = &D_800E6280;
    s32 pad; /* FAKE: unused word gives the frame home slot of the original; real source unknown. T-19030 */
    POLY_FT4 *p;

    p = (POLY_FT4 *)GetWorkBase(0x28, D_8011ECA0);
    func_8009F040(p);
    SetSemiTrans(p, 0);
    SetShadeTex(p, 0);
    p->tpage = func_8009ECB0(2, 0, 0x200, 0);
    p->r0 = 0x80;
    p->g0 = 0x80;
    p->b0 = 0x80;
    p->x0 = g->unk_014[D_8011ECA0] + (((0x64 - arg0) << 9) / 100 - ((0x64 - arg0) * 0x10) / 100) - 0x100;
    p->y0 = g->unk_018[D_8011ECA0] + ((0x64 - arg0) << 6) / 100 - 0x78;
    p->x1 = g->unk_014[D_8011ECA0] + (((0x64 - arg0) << 9) / 100 - ((0x64 - arg0) * 0x10) / 100) + (arg0 << 8) / 100 - 0x100;
    p->y1 = g->unk_018[D_8011ECA0] + ((0x64 - arg0) << 6) / 100 - 0x78;
    p->x2 = g->unk_014[D_8011ECA0] + (((0x64 - arg0) << 9) / 100 - ((0x64 - arg0) * 0x10) / 100) - 0x100;
    p->y2 = g->unk_018[D_8011ECA0] + ((0x64 - arg0) << 6) / 100 + (arg0 * 0xF0) / 100 - 0x78;
    p->x3 = g->unk_014[D_8011ECA0] + (((0x64 - arg0) << 9) / 100 - ((0x64 - arg0) * 0x10) / 100) + (arg0 << 8) / 100 - 0x100;
    p->y3 = g->unk_018[D_8011ECA0] + ((0x64 - arg0) << 6) / 100 + (arg0 * 0xF0) / 100 - 0x78;
    /* one comma statement, the shape of the libgpu setUV4 macro: gives the original's store order and constant registers. T-24200 */
    p->u0 = 0, p->v0 = 0, p->u1 = 0xFF, p->v1 = 0, p->u2 = 0, p->v2 = 0xF0, p->u3 = 0xFF, p->v3 = 0xF0;
    AddPrim((D_8011ECA0 << 0xA) + 0x3FC + D_800E8CA0, p);
    p = (POLY_FT4 *)GetWorkBase(0x28, D_8011ECA0);
    func_8009F040(p);
    SetSemiTrans(p, 0);
    SetShadeTex(p, 0);
    p->tpage = func_8009ECB0(2, 0, 0x300, 0);
    p->r0 = 0x80;
    p->g0 = 0x80;
    p->b0 = 0x80;
    p->x0 = g->unk_014[D_8011ECA0] + (((0x64 - arg0) << 8) / 100 - ((0x64 - arg0) * 0x10) / 100);
    p->y0 = g->unk_018[D_8011ECA0] + ((0x64 - arg0) << 6) / 100 - 0x78;
    p->x1 = g->unk_014[D_8011ECA0] + (((0x64 - arg0) << 8) / 100 - ((0x64 - arg0) * 0x10) / 100) + (arg0 << 8) / 100;
    p->y1 = g->unk_018[D_8011ECA0] + ((0x64 - arg0) << 6) / 100 - 0x78;
    p->x2 = g->unk_014[D_8011ECA0] + (((0x64 - arg0) << 8) / 100 - ((0x64 - arg0) * 0x10) / 100);
    p->y2 = g->unk_018[D_8011ECA0] + ((0x64 - arg0) << 6) / 100 + (arg0 * 0xF0) / 100 - 0x78;
    p->x3 = g->unk_014[D_8011ECA0] + (((0x64 - arg0) << 8) / 100 - ((0x64 - arg0) * 0x10) / 100) + (arg0 << 8) / 100;
    p->y3 = g->unk_018[D_8011ECA0] + ((0x64 - arg0) << 6) / 100 + (arg0 * 0xF0) / 100 - 0x78;
    p->u0 = 0, p->v0 = 0, p->u1 = 0xFF, p->v1 = 0, p->u2 = 0, p->v2 = 0xF0, p->u3 = 0xFF, p->v3 = 0xF0;
    AddPrim((D_8011ECA0 << 0xA) + 0x3FC + D_800E8CA0, p);
}
#else
INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013B170", func_8013B460);
#endif
