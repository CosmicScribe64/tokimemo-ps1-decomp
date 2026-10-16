/* Port of the TACO sibling func_80153E78 (T-17300). */
void func_8013E524(s32 a, s32 b) {
    POLY_F4 *poly;
    s32 sprite;

    poly = GetWorkBase(0x18, D_8011ECA0);
    func_8009F02C(poly);
    SetSemiTrans(poly, 1);
    SetShadeTex(poly, 0);
    poly->x0 = a;
    poly->y0 = D_800E6280.unk_018[D_8011ECA0] + 0x2B;
    poly->x1 = a + b;
    poly->y1 = D_800E6280.unk_018[D_8011ECA0] + 0x2B;
    poly->x2 = a;
    poly->y2 = D_800E6280.unk_018[D_8011ECA0] + 0x37;
    poly->x3 = a + b;
    poly->y3 = D_800E6280.unk_018[D_8011ECA0] + 0x37;
    poly->b0 = 0;
    poly->g0 = 0;
    poly->r0 = 0;
    func_8009EED0(D_8011ECA0 * 0x400 + 0xC + D_800E8CA0, poly);
    sprite = GetWorkBase(0xC, D_8011ECA0);
    func_8009D294(sprite, 0, 0, func_8009ECB0(1, 0, 0, 0), 0);
    func_8009EED0(D_8011ECA0 * 0x400 + 0xC + D_800E8CA0, sprite);
    safe_env(3);
}
