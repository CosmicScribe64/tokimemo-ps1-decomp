/* T-24501 scratch draft, resolved full-TU edit cost 374.
 * Existing POLY_G3 fields recover the packet. (red * 0) / 5 preserves
 * the original black-edge division, unlike literal 0 / 5. Frame homes,
 * color extraction scheduling and temporary allocation remain broad misses.
 * Not production C; include psx_types.h in the owning translation unit. */
void _sprite_set_light_effect1(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    POLY_G3 *sp38;
    s32 sp34;
    s32 sp30;
    s32 sp2C;
    s32 sp28;
    s32 temp_lo;

    sp34 = ((s32) (rsin(D_800E6280.unk_10F8 * 0xFF0) << 5) / 4096) + (((u32) arg7 >> 0x10) & 0xFF);
    sp30 = ((s32) (rsin(D_800E6280.unk_10F8 * 0xFF0) << 5) / 4096) + (((u32) arg7 >> 8) & 0xFF);
    sp2C = ((s32) (rsin(D_800E6280.unk_10F8 * 0xFF0) << 5) / 4096) + (arg7 & 0xFF);
    sp28 = abs(rsin((D_800E6280.unk_10F8 + arg8) * 0xFF0));
    sp38 = (POLY_G3 *)GetWorkBase(0x1C, D_8011ECA0);
    func_8009F004(sp38);
    SetSemiTrans(sp38, 1);
    sp38->r0 = (s8) ((s32) (sp34 * 3) / 5);
    sp38->g0 = (s8) ((s32) (sp30 * 3) / 5);
    sp38->b0 = (s8) ((s32) (sp2C * 3) / 5);
    temp_lo = (sp34 * 0) / 5;
    sp38->b1 = (s8) temp_lo;
    sp38->g1 = (s8) temp_lo;
    sp38->r1 = (s8) temp_lo;
    sp38->b2 = (s8) temp_lo;
    sp38->g2 = (s8) temp_lo;
    sp38->r2 = (s8) temp_lo;
    if (arg4 == 0) {
        sp38->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg0);
        sp38->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg1);
        temp_lo = (s32) (sp28 * 8) / 4096;
        sp38->x1 = (s16) ((D_800E6280.unk_014[D_8011ECA0] + arg2) - temp_lo);
        sp38->y1 = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg3);
        sp38->x2 = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg2 + temp_lo);
        sp38->y2 = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg3);
    } else {
        sp38->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg0);
        sp38->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg1);
        sp38->x1 = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg2);
        sp38->y1 = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg3);
        sp38->x2 = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg4);
        sp38->y2 = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg5);
    }
    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + (arg6 * 4)], sp38);
}
