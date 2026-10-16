void x_taku_menu_set(s32 menu, s32 count)
{
    s16 x[4];
    s16 width[4];
    s16 height[4];
    s32 i;

    for (i = 0; i < count; i++) {
        x[i] = -120;
        width[i] = 240;
        height[i] = 16;
    }
    if (count == 3) {
        menu_set(menu, 3, x, D_800B40F0, width, height);
        return;
    }
    menu_set(menu, 2, x, D_800B40F0, width, height);
}
