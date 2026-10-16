s32 func_80152E34(s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, u32 c0, u32 c1, s32 z, s32 trans) {
    s32 pad; /* fills the word above poly (frame 0x30, poly at sp+0x28); the original's local there is unknown */
    POLY_G3 *poly;

    poly = func_800490F0(0x1C, D_8011ECA0);
    func_8009F004(poly);
    SetSemiTrans(poly, trans);
    poly->g0 = (c0 >> 8) & 0xFF;
    poly->r0 = (c0 >> 16) & 0xFF;
    poly->b0 = c0;
    poly->g1 = (c1 >> 8) & 0xFF;
    poly->r1 = (c1 >> 16) & 0xFF;
    poly->b1 = c1;
    poly->g2 = (c1 >> 8) & 0xFF;
    poly->r2 = (c1 >> 16) & 0xFF;
    poly->b2 = c1;
    poly->x0 = D_800E6280.unk_014[D_8011ECA0] + x0;
    poly->y0 = D_800E6280.unk_018[D_8011ECA0] + y0;
    poly->x1 = D_800E6280.unk_014[D_8011ECA0] + x1;
    poly->y1 = D_800E6280.unk_018[D_8011ECA0] + y1;
    poly->x2 = D_800E6280.unk_014[D_8011ECA0] + x2;
    poly->y2 = D_800E6280.unk_018[D_8011ECA0] + y2;
    func_8009EED0(&D_800E8CA0[(D_8011ECA0 << 10) + z * 4], poly);
}
