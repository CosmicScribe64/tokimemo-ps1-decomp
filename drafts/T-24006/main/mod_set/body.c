typedef struct ModelSprite24 {
    /* 0x00 */ u32 attribute;
    /* 0x04 */ s16 x;
    /* 0x06 */ s16 y;
    /* 0x08 */ s16 w;
    /* 0x0A */ s16 h;
    /* 0x0C */ s16 tpage;
    /* 0x0E */ u8 u;
    /* 0x0F */ u8 v;
    /* 0x10 */ s16 cx;
    /* 0x12 */ s16 cy;
    /* 0x14 */ u8 r;
    /* 0x15 */ u8 g;
    /* 0x16 */ u8 b;
    /* 0x17 */ u8 pad_17;
    /* 0x18 */ s16 mx;
    /* 0x1A */ s16 my;
    /* 0x1C */ s16 scalex;
    /* 0x1E */ s16 scaley;
    /* 0x20 */ s32 rotate;
} ModelSprite24; /* size 0x24; GsSPRITE-like request, T-24006 hypothesis. */
void mod_set(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u16 arg6) {
    u32 spD4;
    ModelSprite24 sprite;
    s32 sp70;
    u32 *var_s5;
    s32 temp_v0;
    u32 temp_v0_2;
    s32 var_a1;
    s32 var_s1;
    u32 temp_t9;
    u32 var_a2;
    u32 var_s2;
    u8 temp_a0;
    u8 temp_s4;
    u8 temp_t8;
    u8 *temp_fp;

    temp_v0 = func_8004901C();
    temp_fp = (arg0 * 0x14) + D_801217A0;
    temp_t8 = *(u8 *)((unsigned char *)temp_fp + (8));
    var_a1 = 1;
    switch (temp_t8) {
    case 0:
        sprite.attribute = 0x01000000;
        var_a1 = 1;
        break;
    case 1:
        sp70 = 2;
        sprite.attribute = 0;
        var_a1 = 2;
        break;
    case 2:
        sp70 = 2;
        sprite.attribute = 0;
        var_a1 = 2;
        break;
    case 3:
        sp70 = 1;
        sprite.attribute = 0x01000000;
        var_a1 = 1;
        break;
    case 4:
        sp70 = 1;
        sprite.attribute = 0x01000000;
        var_a1 = 1;
        break;
    case 5:
        sp70 = 1;
        sprite.attribute = 0x01000000;
        var_a1 = 1;
        break;
    default:
        sprite.attribute = 0x01000000;
        sp70 = 1;
        break;
    }
    temp_a0 = *(u8 *)((unsigned char *)temp_fp + (13));
    temp_s4 = *(u8 *)((unsigned char *)temp_fp + (14));
    sprite.mx = 0;
    sprite.my = 0;
    sprite.r = (s8) temp_v0;
    sprite.g = (s8) temp_v0;
    sprite.b = (s8) temp_v0;
    sprite.rotate = 0;
    sprite.scalex = 0x1000;
    sprite.scaley = 0x1000;
    var_a2 = 0;
    sprite.w = (s16) temp_s4;
    sprite.h = (s16) *(u8 *)((unsigned char *)temp_fp + (15));
    var_s5 = (u32 *)(temp_a0 * arg1 * *(u8 *)((unsigned char *)temp_fp + (12)) * 4 + *(u32 *)((unsigned char *)temp_fp + 4));
    if (temp_a0 != 0) {
        do {
            var_s2 = 0;
            if (*(u8 *)((unsigned char *)temp_fp + (12)) != 0) {
                var_s1 = arg2;
                sp70 = var_a1;
                spD4 = var_a2;
                do {
                    sprite.x = (s16) var_s1;
                    sprite.y = (var_a2 * temp_s4) + arg3;
                    temp_v0_2 = *var_s5;
                    temp_t9 = (u32) (temp_v0_2 & 0xFF0000) >> 0x10;
                    sprite.tpage = search_tpage_multi(*(s32 *)((*(s32 *)temp_fp) + (((u32) (temp_v0_2 & 0xFF000000) >> 0x18) * 4)), sp70);
                    if (temp_s4 != 8) {
                        if (temp_s4 != 0x10) {
                            sprite.v = ((u32) (temp_t9 & 0xF0) >> 4) * 8;
                            sprite.u = (temp_t9 & 0xF) * 8;
                        } else {
                            sprite.u = (temp_t9 & 7) * 0x10;
                            sprite.v = ((u32) (temp_t9 & 0xF8) >> 4) * 0x10;
                        }
                    } else {
                        sprite.u = (temp_t9 & 0xF) * 8;
                        sprite.v = ((u32) (temp_t9 & 0xF0) >> 4) * 8;
                    }
                    sprite.cx = arg4 + (((u32) (temp_v0_2 & 0xFF00) >> 8) * 0x10);
                    sprite.cy = (s16) arg5;
                    func_80098B80((s32) &sprite, (s32) ((D_8011ECA0 * 0x14) + &D_800E8C70), arg6 & 0xFFFF);
                    var_s2 += 1;
                    var_s1 += temp_s4;
                    var_s5++;
                } while (var_s2 < (u8) *(u8 *)((unsigned char *)temp_fp + (12)));
                var_a2 = spD4;
                var_a1 = sp70;
            }
            var_a2 += 1;
        } while (var_a2 < *(u8 *)((unsigned char *)temp_fp + (13)));
    }
}
