/* T-24501 scratch: clean full-TU resolved edit cost 109.
 * Requires s32 D_800B3D10[4]={0} replacing four data includes and the exact
 * existing RECT-by-value callee prototype. Scratch-only declarations.
 * Target frame0x78, colors0x64, RECT0x58. Clean colors0x68, RECT0x60.
 * Reordering and grouped locals do not resolve broad register/home misses. */
void func_8004A14C(s32, RECT, s32);
void func_8004443C(void) {
    
    s32 colors[4];
    
    RECT rect;
    s16 x;
    s32 *p;
    s32 *stamp;
    s32 i;
    s32 t;
    colors[0] = D_800B3D10[0] / 264;
    colors[1] = D_800B3D10[1] / 264;
    colors[2] = D_800B3D10[2] / 264;
    colors[3] = D_800B3D10[3] / 264;
    p = colors;
    do {
        if (*p >= 3) *p = 0x800000;
        else if (*p >= 2) *p = 0x808000;
        else if (*p > 0) *p = 0x8000;
        else *p = 0x80;
        p++;
    } while (p < colors + 4);
    rect.w = 4;
    p = colors;
    stamp = D_800B3D10;
    i = 0;
    x = 0x8C;
    do {
        rect.x = x;
        t = *stamp * 16 / 264;
        rect.h = t;
        rect.y = 0x4C - t;
        func_8004A14C(i + 6, rect, *p);
        i++;
        p++;
        x += 4;
        stamp++;
    } while (i != 4);
    rect.x = 0x88;
    rect.h = 16;
    rect.y = 0x3C;
    func_8004A14C(i + 6, rect, 0x808080);
}
