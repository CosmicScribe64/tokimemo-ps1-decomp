void func_8013FBF0(void) {
    SPRT *sprite;
    u8 r;
    u8 g;
    u8 b;
    s32 value;
    u8 state;

    sprite = GetWorkBase(0x14, D_8011ECA0);
    func_8009F0A4(sprite);
    sprite->w = 0x20, sprite->h = 0x40;
    SetSemiTrans(sprite, 1);
    sprite->r0 = 0xFF;
    sprite->g0 = 0xFF;
    sprite->b0 = 0xFF;
    sprite->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0xC8);
    sprite->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] + 0x28);
    sprite->u0 = 0x80;
    sprite->v0 = 0x40;
    sprite->clut = func_8009ED74(0x10, 0x1EF);
    AddPrim((D_8011ECA0 << 0xA) + 4 + D_800E8CA0, sprite);
    dtd_on_tpage(0x340, 0x100, 1, 0, 0);
    state = D_8015EDB4->unk84[1];
    if (state >= 0xC1U) {
        r = 0;
        g = 0;
        b = 1;
    } else if (state >= 0x81U) {
        r = 0;
        g = 1;
        b = 0;
    } else if (state >= 0x41U) {
        r = 1;
        g = 1;
        b = 0;
    } else {
        r = 1;
        g = 0;
        b = 0;
    }
    value = ((s32) (0x1000 - func_800A0070(D_800E6280.unk_10F8 * 0xF0)) / 128) + 0x40;
    r *= value;
    g *= value;
    b *= value;
    func_80049A40(-0xBF, 0x68, 0xE, ((u32) -(s32) D_8015EDB4->unk84[1] >> 2), 2, ((r & 0xFF) << 0x10) + ((g & 0xFF) << 8) + (b & 0xFF), 0x81);
    dtd_on_tpage(0, 0, 2, 1, 0);
    sprite = GetWorkBase(0x14, D_8011ECA0);
    func_8009F0A4(sprite);
    sprite->w = 0x20;
    sprite->h = 0x20;
    SetSemiTrans(sprite, 1);
    sprite->r0 = 0x80;
    sprite->g0 = 0x80;
    sprite->b0 = 0x80;
    sprite->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0xC8);
    sprite->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] + ((u32) (0xFF - D_8015EDB4->unk84[1]) >> 2) + 0x18);
    sprite->u0 = 0xA0;
    sprite->v0 = 0x60;
    sprite->clut = func_8009ED74(0x20, 0x1EF);
    AddPrim((D_8011ECA0 << 0xA) + 4 + D_800E8CA0, sprite);
    dtd_on_tpage(0x340, 0x100, 1, 0, 0);
    sprite = GetWorkBase(0x14, D_8011ECA0);
    func_8009F0A4(sprite);
    sprite->h = 0x18;
    sprite->w = 0x28;
    SetSemiTrans(sprite, 1);
    sprite->r0 = 0x80;
    sprite->g0 = 0x80;
    sprite->b0 = 0x80;
    sprite->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0xCE);
    sprite->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] + 0x68);
    sprite->u0 = 0xA0;
    sprite->v0 = 0x40;
    sprite->clut = func_8009ED74(0x20, 0x1EF);
    AddPrim((D_8011ECA0 << 0xA) + 4 + D_800E8CA0, sprite);
    dtd_on_tpage(0x340, 0x100, 1, 0, 0);
}
