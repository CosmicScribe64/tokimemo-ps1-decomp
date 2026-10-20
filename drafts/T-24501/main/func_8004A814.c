void func_8004A814(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    GameState *state;
    TILE *tile;
    s32 x;
    s32 y;
    s32 abs_x;
    u32 red;
    u32 green;
    s32 blue;
    s32 r;
    s32 g;
    s32 b;

    red = (u32)(arg5 & 0xFF0000) >> 16;
    green = (u32)(arg5 & 0xFF00) >> 8;
    blue = arg5 & 0xFF;
    arg3 &= 0x7F;
    arg4 &= 0x7F;
    if (arg3 < 2) {
        arg3 = 2;
    }
    if (arg4 < 2) {
        arg4 = 2;
    }
    for (x = arg0 - arg2; x < arg0 + arg2; x += arg3) {
        y = arg1 - arg2;
        if (y < arg1 + arg2) {
            do {
                if ((arg0 - x) * (arg0 - x) + (arg1 - y) * (arg1 - y) < arg2 * arg2) {
                    tile = (TILE *)GetWorkBase(sizeof(TILE), D_8011ECA0);
                    func_8009F0F4(tile);
                    SetSemiTrans(tile, 1);
                    state = &D_800E6280;
                    tile->x0 = state->unk_014[D_8011ECA0] + (s16)x - arg3 / 2;
                    tile->y0 = state->unk_018[D_8011ECA0] + y - arg4 / 2;
                    /* The target uses the x step for both dimensions. */
                    setWH(tile, arg3, arg3);
                    abs_x = abs(arg0 - x);
                    r = (s32)((arg2 * 2 - abs(arg1 - y) - abs_x) * red) / arg2;
                    abs_x = abs(arg0 - x);
                    g = (s32)((arg2 * 2 - abs(arg1 - y) - abs_x) * green) / arg2;
                    abs_x = abs(arg0 - x);
                    b = ((arg2 * 2 - abs(arg1 - y) - abs_x) * blue) / arg2;
                    if (r >= 256) {
                        r = 255;
                    }
                    if (g >= 256) {
                        g = 255;
                    }
                    if (b >= 256) {
                        b = 255;
                    }
                    tile->r0 = r;
                    tile->g0 = g;
                    tile->b0 = b;
                    AddPrim(&D_800E8CA0[(D_8011ECA0 << 10) + arg7 * 4], tile);
                }
                y += arg4;
            } while (y < arg1 + arg2);
        }
    }
    dtd_on_tpage(0, 0, arg7, arg6 & 3, 0);
}
