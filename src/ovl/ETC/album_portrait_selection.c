/* album_portrait_selection.c: ; src/ovl/ETC/8013B1F0.c:4-18 selects portrait resources for album event records. Original object 0x8013B1F0-0x8013BEF0 (config/objects/ETC.txt). */
#include "common.h"
#include "ovl/ETC.h"

void album_event_picture_select(u32 category, s32 scene, s32 girl, s32 season) {
    album_event_is_portrait = 0;
    switch (category) {
    case 0:
        switch (scene) {
        case 29:
        case 30:
        case 40:
            album_portrait_start_a(girl);
            return;
        case 32:
            album_portrait_start_b(girl);
            return;
        case 34:
            album_portrait_start_c(girl);
            return;
        default:
            album_portrait_start_seasonal(girl, season);
            return;
        }
        break;
    case 1:
        switch (scene) {
        case 32:
            album_portrait_start_b(girl);
            return;
        case 34:
            album_portrait_start_c(girl);
            return;
        default:
            album_portrait_start_seasonal(girl, season);
            return;
        }
        break;
    case 2:
        switch (scene) {
        case 0x14:
            album_portrait_start_seasonal(girl, season);
            return;
        case 0x1:
        case 0x22:
        case 0x35:
        case 0x3D:
        case 0x44:
        case 0x4A:
        case 0x4F:
        case 0x51:
            album_portrait_start_school_wear(girl, season);
            return;
        case 0x4C:
            album_portrait_start_seasonal(girl, season);
            return;
        case 0x4E:
            album_portrait_start_a(girl);
            return;
        default:
            return;
        }
        break;
    case 3:
        album_portrait_start_school_wear(girl, season);
        return;
    case 4:
        if ((scene == 1) && (girl == 9)) {
            album_event_is_portrait = 1;
            portrait_read_start(0x6638);
            return;
        }
        album_portrait_start_l(girl);
        return;
    case 5:
        if (scene < 3) {
            album_portrait_start_j(girl);
            return;
        }
        album_portrait_start_k(girl);
        return;
    case 6:
        album_portrait_start_school_wear(girl, season);
        return;
    case 7:
        album_portrait_start_school_wear(girl, season);
        return;
    case 8:
        switch (scene) {
        case 0x16:
        case 0x17:
        case 0x18:
            album_portrait_start_e(2, 2);
            return;
        case 0x1F:
            album_portrait_start_e(3, 4);
            return;
        case 0x29:
        case 0x2A:
            album_portrait_start_e(4, 7);
            return;
        case 0x32:
        case 0x33:
            album_portrait_start_e(5, 8);
            return;
        case 0x3C:
            album_event_is_portrait = 1;
            portrait_read_start(0x6449);
            return;
        case 0x3D:
            album_portrait_start_e(6, 9);
            return;
        case 0x5A:
            album_event_is_portrait = 1;
            portrait_read_start(0x6638);
            return;
        case 0xE:
        case 0x46:
            /* empty cases: they pull the jump table down to 0xE; without them IDO emits a compare chain (T-22026) */
            break;
        default:
            album_portrait_start_school_wear(girl, season);
            return;
        }
        break;
    case 9:
        album_portrait_start_school_wear(girl, season);
        return;
    case 10:
        switch (scene) {
        case 0x20:
            album_portrait_start_b(girl);
            return;
        case 0x22:
            album_portrait_start_c(girl);
            return;
        case 0x25:
        case 0x29:
        case 0x2A:
        case 0x2B:
        case 0x2C:
            album_portrait_start_school_wear(girl, season);
            return;
        case 0x2E:
            album_portrait_start_j(girl);
            return;
        case 0x32:
            album_portrait_start_k(girl);
            return;
        case 0x30:
        case 0x31:
            album_portrait_start_i(girl);
            return;
        case 0x33:
        case 0x34:
        case 0x35:
        case 0x36:
        case 0x37:
        case 0x38:
        case 0x39:
        case 0x3A:
        case 0x3B:
        case 0x3C:
        case 0x3D:
            album_portrait_start_e(girl, scene - 0x33);
            return;
        default:
            album_portrait_start_seasonal(girl, season);
            return;
        }
        break;
    case 11:
        if ((scene == 0x1E) || (scene == 0x1D) || (scene == 0x28)) {
            album_event_is_portrait = 1;
            portrait_read_start(0x6BD8);
            return;
        }
        album_portrait_start_seasonal(girl, season);
        return;
    case 12:
        album_portrait_start_seasonal(girl, season);
        return;
    case 13:
        switch (scene) {
        case 0x20:
            album_portrait_start_b(girl);
            return;
        case 0x22:
            album_portrait_start_c(girl);
            return;
        case 0x26:
        case 0x27:
        case 0x28:
        case 0x29:
        case 0x2B:
            album_portrait_start_school_wear(girl, season);
            return;
        case 0x2A:
            if ((girl == 7) || (girl == 8) || (girl == 9)) {
                album_portrait_start_seasonal(girl, season);
                return;
            }
            album_portrait_start_school_wear(girl, season);
            return;
        case 0x2C:
            album_portrait_start_j(girl);
            return;
        case 0x2D:
            album_portrait_start_k(girl);
            return;
        case 0x2E:
        case 0x2F:
        case 0x30:
        case 0x31:
        case 0x32:
        case 0x33:
        case 0x34:
        case 0x35:
        case 0x36:
        case 0x37:
        case 0x38:
            album_portrait_start_e(girl, scene - 0x2E);
            return;
        default:
            album_portrait_start_seasonal(girl, season);
            return;
        }
        break;
    case 14:
        if (scene == 0xB) {
            album_portrait_start_f(girl);
            return;
        }
        if (scene == 0xC) {
            album_portrait_start_g(girl);
            return;
        }
        if (scene == 0xD) {
            album_portrait_start_h(girl);
            return;
        }
        album_portrait_start_e(girl, scene);
        return;
    case 15:
        album_portrait_start_school_wear(girl, season);
        return;
    case 16:
        album_portrait_start_e(girl, scene);
        return;
    case 18:
        switch (scene) {
        case 5:
            album_portrait_start_e(girl, 6);
            return;
        case 6:
            album_portrait_start_e(girl, 7);
            return;
        case 7:
            album_portrait_start_e(girl, 8);
            return;
        case 8:
            album_portrait_start_e(girl, 9);
            return;
        case 9:
            album_portrait_start_e(girl, 0xA);
            return;
        default:
            album_portrait_start_school_wear(girl, season);
            return;
        }
        break;
    case 19:
        album_portrait_start_d(girl);
        return;
    case 20:
        switch (scene) {
        case 6:
        case 7:
        case 29:
            album_portrait_start_e(girl, 1);
            return;
        case 10:
        case 11:
        case 30:
            album_portrait_start_e(girl, 2);
            return;
        case 18:
        case 19:
        case 32:
        case 33:
            album_portrait_start_e(girl, 4);
            return;
        default:
            album_portrait_start_school_wear(girl, season);
            return;
        }
        break;
    case 21:
        if ((girl == 0xC) || (girl == 0xB)) {
            album_portrait_start_school_wear(girl, season);
            return;
        }
        switch (scene) {
        case 0:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            album_portrait_start_school_wear(girl, season);
            return;
        case 12:
            album_event_is_portrait = 1;
            portrait_read_start(0x5D6E);
            return;
        default:
            album_portrait_start_seasonal(girl, season);
            break;
        }
        break;
    }
}

void album_portrait_start_a(s32 arg0) {
    s32 *p;
    s32 v;

    p = &album_portrait_lba_a[arg0];
    v = *p;
    if (v != -1) {
        album_event_is_portrait = 1;
        if (p == &album_portrait_lba_a_girl8) {
            switch (album_event_year) {
            case 0x5F:
                portrait_read_start(0x50F3);
                break;
            case 0x60:
                portrait_read_start(0x5120);
                break;
            default:
            case 0x61:
                portrait_read_start(0x514D);
                break;
            }
        } else {
            portrait_read_start(v);
        }
    }
}

void album_portrait_start_b(s32 arg0) {
    s32 t = album_portrait_lba_b[arg0];

    if (t != -1) {
        album_event_is_portrait = 1;
        portrait_read_start(t);
    }
}

void album_portrait_start_c(s32 arg0) {
    s32 t = album_portrait_lba_c[arg0];

    if (t != -1) {
        album_event_is_portrait = 1;
        portrait_read_start(t);
    }
}

void album_portrait_start_d(s32 arg0) {
    s32 t = album_portrait_lba_d[arg0];

    if (t != -1) {
        album_event_is_portrait = 1;
        portrait_read_start(t);
    }
}

void album_portrait_start_e(s32 arg0, s32 arg1) {
    s32 t = album_portrait_lba_e[arg0][arg1];

    if (t != -1) {
        album_event_is_portrait = 1;
        portrait_read_start(t);
    }
}

void album_portrait_start_f(s32 arg0) {
    s32 t = album_portrait_lba_f[arg0];

    if (t != -1) {
        album_event_is_portrait = 1;
        portrait_read_start(t);
    }
}

void album_portrait_start_g(s32 arg0) {
    s32 t = album_portrait_lba_g[arg0];

    if (t != -1) {
        album_event_is_portrait = 1;
        portrait_read_start(t);
    }
}

void album_portrait_start_h(s32 arg0) {
    s32 t = album_portrait_lba_h[arg0];

    if (t != -1) {
        album_event_is_portrait = 1;
        portrait_read_start(t);
    }
}

void album_portrait_start_i(s32 arg0) {
    s32 t = album_portrait_lba_i[arg0];

    if (t != -1) {
        album_event_is_portrait = 1;
        portrait_read_start(t);
    }
}

void album_portrait_start_j(s32 arg0) {
    s32 t = album_portrait_lba_j[arg0];

    if (t != -1) {
        album_event_is_portrait = 1;
        portrait_read_start(t);
    }
}

void album_portrait_start_k(s32 arg0) {
    s32 t = album_portrait_lba_k[arg0];

    if (t != -1) {
        album_event_is_portrait = 1;
        portrait_read_start(t);
    }
}

void album_portrait_start_l(s32 arg0) {
    s32 t = album_portrait_lba_l[arg0];

    if (t != -1) {
        album_event_is_portrait = 1;
        portrait_read_start(t);
    }
}

void album_portrait_start_school_wear(s32 arg0, s32 arg1) {
    s32 v;

    switch (album_event_month) {
    case 6:
    case 7:
    case 8:
    case 9:
        v = album_portrait_lba_summer_wear[arg0];
        if (v != -1) {
            album_event_is_portrait = 1;
            portrait_read_start(v, arg0);
        }
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 10:
    case 11:
    case 12:
        v = album_portrait_lba_winter_wear[arg0];
        if (v != -1) {
            album_event_is_portrait = 1;
            portrait_read_start(v, arg0);
        }
        break;
    }
}

void album_portrait_start_seasonal(s32 arg0, s32 arg1) {
    s32 t;

    switch (arg1) {
    case 0:
        t = album_portrait_lba_spring[arg0];
        if (t != -1) {
            album_event_is_portrait = 1;
            portrait_read_start(t);
        }
        break;
    case 1:
        t = album_portrait_lba_summer[arg0];
        if (t != -1) {
            album_event_is_portrait = 1;
            portrait_read_start(t);
        }
        break;
    case 2:
        t = album_portrait_lba_autumn[arg0];
        if (t != -1) {
            album_event_is_portrait = 1;
            portrait_read_start(t);
        }
        break;
    case 3:
        t = album_portrait_lba_winter[arg0];
        if (t != -1) {
            album_event_is_portrait = 1;
            portrait_read_start(t);
        }
        break;
    }
}
