/* animation_and_angle.c: src/ovl/TT/80132000.c:4: frame-list control bytes advance sprite animation; second function computes relative angle; body 9: if (arg0[8] & 0x80) {; 10: arg0[8] &= 0xFF7F;. Original object 0x80132000-0x80132130 (config/objects/TT.txt). */
#include "common.h"
#include "ovl/TT.h"

void tt_animation_list_step(u8 *animation, s8 *tile_u, s8 *tile_v) {
    u8 *e;
    u8 b;
    u8 c;

    if (animation[8] & 0x80) {
        animation[8] &= 0xFF7F;
        animation[6] = 0;
        goto step;
    }
    if (animation[8] & 0x20) {
        animation[8] &= 0xFFDF;
        goto step;
    }
    if (!(animation[8] & 0x40)) {
        if (--animation[9] == 0) {
            animation[6] += 1;
step: /* the three flag paths join here, as in the original control flow */
            e = animation[6] * 3 + *(u8 **)animation;
            b = *e;
            animation[9] = b;
            if (b == 0xFE) {
                animation[6] = 0;
                e = *(u8 **)animation;
                animation[9] = *e;
                animation[8] |= 1;
            }
            *tile_u = (e[1] & 7) * 0x10 + 0x80;
            *tile_v = (e[1] & 0xFFF8) * 2;
            e += 2;
            c = *e;
            if (c != 0xFF) {
                animation[4] = c;
            }
        }
    }
}

void tt_angle_between(u8 *origin, u8 *target) {
    s32 dx = *(s32 *)(target + 0xC) - *(s32 *)(origin + 0xC);
    s32 dy = *(s32 *)(target + 0x10) - *(s32 *)(origin + 0x10);

    func_8014D260(dy >> 16, dx >> 16);
}
