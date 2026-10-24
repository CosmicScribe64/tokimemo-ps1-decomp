/* actor_animation_damage_voice.c: src/ovl/RPG_BAT/8013E4D0.c:4: actor animation and position accessors, damage application, latched sounds and voice waits; body 8: p = (Actor *)actor_pool; 9: for (i = 40; i < 64; i++) {. Original object 0x8013E4D0-0x8013F4F0 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/RPG_BAT.h"

void func_8013E4D0(void) {
    s32 i;
    Actor *p;

    p = (Actor *)actor_pool;
    for (i = 40; i < 64; i++) {
        p[96 + i].unk_01 = 5;
        p[96 + i].quad_flags = 0x41000000;
        p[96 + i].flags = 0x1;
        p[96 + i].blink_period = 0x1;
        p[96 + i].scale_x = 0x1000;
        p[96 + i].scale_y = 0x1000;
    }
    for (i = 29; i < 31; i++) {
        p[96 + i].unk_01 = 5;
        p[96 + i].quad_flags = 0x41000000;
        p[96 + i].flags = 0x1;
        p[96 + i].blink_period = 0x1;
        p[96 + i].scale_x = 0x1000;
        p[96 + i].scale_y = 0x1000;
    }
}

/* Implicit int with no return value (the callers have no prototype): with $v0 live at the exit, uopt
 * emits the hoisted loop limit before the strength-reduced start in each case, as the original (T-20600). */
s32 func_8013E54C(s32 arg0) {
    s32 i;

    switch (arg0) {
    case 3:
        for (i = 4; i < 16; i++) {
            *(s32 *)&actor_pool[0x198C + i * 0x44] = D_8015EEB4;
            *(s32 *)&actor_pool[0x1990 + i * 0x44] = D_8015EECC;
            *(s32 *)&actor_pool[0x19B4 + i * 0x44] = D_8015EE9C;
            ((Actor *)actor_pool)[i + 96].unk_01 = 5;
            ((Actor *)actor_pool)[i + 96].unk_43 = 0x1D;
        }
        break;
    case 4:
        for (i = 4; i < 16; i++) {
            *(s32 *)&actor_pool[0x198C + i * 0x44] = D_8015EEA8;
            *(s32 *)&actor_pool[0x1990 + i * 0x44] = D_8015EEC0;
            *(s32 *)&actor_pool[0x19B4 + i * 0x44] = D_8015EE90;
            ((Actor *)actor_pool)[i + 96].unk_01 = 5;
            ((Actor *)actor_pool)[i + 96].unk_43 = 0x12;
        }
        break;
    case 5:
        for (i = 4; i < 16; i++) {
            *(s32 *)&actor_pool[0x198C + i * 0x44] = D_8015EEB8;
            *(s32 *)&actor_pool[0x1990 + i * 0x44] = D_8015EED0;
            *(s32 *)&actor_pool[0x19B4 + i * 0x44] = D_8015EEA0;
            ((Actor *)actor_pool)[i + 96].unk_01 = 5;
            ((Actor *)actor_pool)[i + 96].unk_43 = 0x1C;
        }
        break;
    case 0xFF:
        for (i = 4; i < 16; i++) {
            *(s32 *)&actor_pool[0x198C + i * 0x44] = D_8015EEA4;
            *(s32 *)&actor_pool[0x1990 + i * 0x44] = D_8015EEBC;
            *(s32 *)&actor_pool[0x19B4 + i * 0x44] = D_8015EE8C;
            ((Actor *)actor_pool)[i + 96].unk_43 = 0;
            ((Actor *)actor_pool)[i + 96].unk_01 = 3;
            ((Actor *)actor_pool)[i + 96].draw_flags = 0;
        }
        break;
    }
}

void func_8013E7C0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    u8 *temp_v0;

    temp_v0 = actor_pool + arg0 * 0x44;
    *(s16 *)(temp_v0 + 0x1988) = 0;
    *(u8 *)(temp_v0 + 0x1982) = arg2;
    if (arg3 == 1) {
        *(u8 *)(temp_v0 + 0x1983) = 0x80;
    } else {
        *(u8 *)(temp_v0 + 0x1983) = 0;
    }
    *(s16 *)(temp_v0 + 0x1998) = 0;
    if (arg1 != 0xFF) {
        *(s16 *)(temp_v0 + 0x1996) = arg1;
    }
}

void func_8013E810(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    u8 *temp_v0;

    temp_v0 = actor_pool + arg0 * 0x44;
    *(s16 *)(temp_v0 + 0x1988) = 0;
    *(u8 *)(temp_v0 + 0x1982) = arg2;
    if (arg3 == 1) {
        *(u8 *)(temp_v0 + 0x1983) = 0x84;
    } else {
        *(u8 *)(temp_v0 + 0x1983) = 4;
    }
    *(s16 *)(temp_v0 + 0x1998) = 0;
    if (arg1 != 0xFF) {
        *(s16 *)(temp_v0 + 0x1996) = arg1;
    }
}

void func_8013E864(s32 arg0, s32 arg1, s32 arg2) {
    u8 *temp_v0;

    temp_v0 = actor_pool + arg0 * 0x44;
    *(s16 *)(temp_v0 + 0x8) = 0;
    *(u8 *)(temp_v0 + 0x2) = 1;
    if (arg2 == 1) {
        *(u8 *)(temp_v0 + 0x3) = 0x80;
    } else {
        *(u8 *)(temp_v0 + 0x3) = 0;
    }
    *(s16 *)(temp_v0 + 0x18) = 0;
    if (arg1 != 0xFF) {
        *(s16 *)(temp_v0 + 0x16) = arg1;
    }
}

void func_8013E8B8(s32 arg0, s32 arg1, s32 arg2) {
    u8 *temp_v0;

    temp_v0 = actor_pool + arg0 * 0x44;
    *(s16 *)(temp_v0 + 0x1108) = 0;
    *(u8 *)(temp_v0 + 0x1102) = 1;
    if (arg2 == 1) {
        *(u8 *)(temp_v0 + 0x1103) = 0x80;
    } else {
        *(u8 *)(temp_v0 + 0x1103) = 0;
    }
    *(s16 *)(temp_v0 + 0x1118) = 0;
    if (arg1 != 0xFF) {
        *(s16 *)(temp_v0 + 0x1116) = arg1;
    }
}

s32 func_8013E90C(s32 i) {
    return (*(&D_80120652 + i * 0x44) & 1) == 0;
}

void func_8013E934(s32 arg0, s32 arg1, s32 arg2) {
    set_kanji_string(arg0 - 0xA0, arg1 - 0x78, 0, arg2, 0);
}

void func_8013E97C(s32 i, s32 x, s32 y) {
    u8 *p = actor_pool + i * 0x44;
    *(s16 *)(p + 0x19A6) = x - 0xA0;
    *(s16 *)(p + 0x19AA) = y - 0x78;
}

s32 func_8013E9A8(s32 arg0) {
    if (arg0 == 0) {
        arg0 = 1;
    }
    return rand() % arg0;
}

s32 func_8013EA00(s32 arg0, s32 arg1) {
    s32 a = func_8013E9A8(arg0);
    s32 b = func_8013E9A8(arg1);
    return a + b;
}

void func_8013EA30(s32 arg0) {
    s32 i;

    for (i = 0; i < 96; i++) {
        ((Actor *)actor_pool)[i + 64].brightness = arg0;
    }
}

void func_8013EA60(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    switch (arg1) {
    case 0:
        ((Actor *)actor_pool)[155].draw_flags &= 0xFFFE;
        RPG_GAUGE->flag[0] = 0;
        RPG_GAUGE->cur[0] -= arg2;
        if (RPG_GAUGE->cur[0] < 0) {
            RPG_GAUGE->cur[0] = 0;
        }
        if (arg2 == 0) {
            func_8014C360(0, 0x6C, 0x77);
            return;
        }
        func_8014BFE4(arg0, 0x74, 0x77, arg2, arg3);
        return;
    case 1:
        ((Actor *)actor_pool)[156].draw_flags &= 0xFFFE;
        RPG_GAUGE->flag[1] = 0;
        RPG_GAUGE->cur[1] -= arg2;
        if (RPG_GAUGE->cur[1] < 0) {
            RPG_GAUGE->cur[1] = 0;
        }
        if (arg2 == 0) {
            func_8014C360(0, 0x33, 0x67);
            return;
        }
        func_8014BFE4(arg0, 0x3B, 0x67, arg2, arg3);
        return;
    case 2:
        ((Actor *)actor_pool)[157].draw_flags &= 0xFFFE;
        RPG_GAUGE->flag[2] = 0;
        RPG_GAUGE->cur[2] -= arg2;
        if (RPG_GAUGE->cur[2] < 0) {
            RPG_GAUGE->cur[2] = 0;
        }
        if (arg2 == 0) {
            func_8014C360(0, 0x30, 0x97);
            return;
        }
        func_8014BFE4(arg0, 0x34, 0x97, arg2, arg3);
        return;
    case 3:
        D_8015EDC0 -= arg2;
        if (D_8015EDC0 < 0) {
            D_8015EDC0 = 0;
        }
        if (arg2 == 0) {
            func_8014C360(0, 0x38, 0x98);
            return;
        }
        func_8014BFE4(arg0, 0x40, 0x98, arg2, arg3);
        return;
    case 4:
        D_8015EDC0 -= arg2;
        if (D_8015EDC0 < 0) {
            D_8015EDC0 = 0;
        }
        if (arg2 == 0) {
            func_8014C360(0, 0x38, 0x98);
            return;
        }
        func_8014BFE4(arg0, 0x40, 0x98, arg2, arg3);
        return;
    case 5:
        D_8015EDC0 -= arg2;
        if (D_8015EDC0 < 0) {
            D_8015EDC0 = 0;
        }
        if (arg2 == 0) {
            func_8014C360(0, 0x38, 0x95);
            return;
        }
        func_8014BFE4(arg0, 0x40, 0x95, arg2, arg3);
        return;
    case 16:
        if (arg2 == 0) {
            func_8014C360(0, 0x40, 0x97);
            return;
        }
        func_8014BFE4(arg0, 0x48, 0x97, arg2, arg3);
        return;
    case 17:
        if (arg2 == 0) {
            func_8014C360(0, 0x40, 0x85);
            return;
        }
        func_8014BFE4(arg0, 0x48, 0x85, arg2, arg3);
        return;
    case 18:
        if (arg2 == 0) {
            func_8014C360(0, 0x48, 0x8D);
            return;
        }
        func_8014BFE4(arg0, 0x50, 0x8D, arg2, arg3);
        return;
    case 32:
        D_8015ED94 -= arg2;
        if (D_8015ED94 < 0) {
            D_8015ED94 = 0;
        }
        func_8014BFE4(arg0, 0xF4, 0x96, arg2, arg3);
        return;
    case 33:
        if (arg3 == 1) {
            D_8015ED94 -= arg2;
            if (D_8015ED94 < 0) {
                D_8015ED94 = 0;
            }
        } else {
            D_8015ED94 += arg2;
            if (D_8015EDA0 < D_8015ED94) {
                D_8015ED94 = D_8015EDA0;
            }
        }
        func_8014BFE4(arg0, 0x10C, 0x66, arg2, arg3);
        return;
    case 35:
        func_8014BFE4(arg0, 0x114, 0x96, arg2, arg3);
        /* fallthrough */
    default:
        return;
    }
}

void func_8013EEAC(u32 arg0) {
    switch (arg0) {
    case 0:
        D_8015EC28 = 0x78;
        D_8015EC2C = 0x68;
        return;
    case 1:
        D_8015EC28 = 0x40;
        D_8015EC2C = 0x58;
        return;
    case 2:
        D_8015EC28 = 0x38;
        D_8015EC2C = 0x88;
        return;
    case 3:
        D_8015EC28 = 0x48;
        D_8015EC2C = 0x60;
        return;
    case 4:
        D_8015EC28 = 0x48;
        D_8015EC2C = 0x60;
        return;
    case 5:
        D_8015EC28 = 0x48;
        D_8015EC2C = 0x60;
        return;
    case 16:
        D_8015EC28 = 0x48;
        D_8015EC2C = 0x60;
        return;
    case 17:
        D_8015EC28 = 0x48;
        D_8015EC2C = 0x60;
        return;
    case 18:
        D_8015EC28 = 0x48;
        D_8015EC2C = 0x60;
        /* fallthrough */
    default:
        return;
    }
}

void func_8013EFD0(u32 arg0) {
    switch (arg0) {
    case 0:
        D_8015EC28 = 0x78;
        D_8015EC2C = 0x68;
        return;
    case 1:
        D_8015EC28 = 0x40;
        D_8015EC2C = 0x58;
        return;
    case 2:
        D_8015EC28 = 0x38;
        D_8015EC2C = 0x88;
        return;
    case 3:
        D_8015EC28 = 0x48;
        D_8015EC2C = 0x40;
        return;
    case 4:
        D_8015EC28 = 0x48;
        D_8015EC2C = 0x50;
        return;
    case 5:
        D_8015EC28 = 0x48;
        D_8015EC2C = 0x60;
        return;
    case 16:
        D_8015EC28 = 0x48;
        D_8015EC2C = 0x60;
        return;
    case 17:
        D_8015EC28 = 0x48;
        D_8015EC2C = 0x60;
        return;
    case 18:
        D_8015EC28 = 0x48;
        D_8015EC2C = 0x60;
        /* fallthrough */
    default:
        return;
    }
}

void func_8013F0F4(s32 a, s32 b, s32 c) {
    if (b == 0) {
        b = -1;
    }
    if (D_8015ED3C[c] == 0) {
        audio_command((u16)((a & 0xFF) | 0x600));
        D_8015ED3C[c] = b;
    }
    D_8015ED3C[c] -= 1;
}

void func_8013F15C(s32 a, s32 b, s32 c) {
    if (b == 0) {
        b = -1;
    }
    if (D_8015ED3C[c] == 0) {
        audio_command((u16)((a & 0xFF) | 0x500));
        D_8015ED3C[c] = b;
    }
    D_8015ED3C[c] -= 1;
}

void func_8013F1C4(s32 a, s32 b, s32 c) {
    s32 *p;

    p = &D_8015ED64[0][c];
    if (*p == 0) {
        game_state.current_girl = GIRL_SOTOI;
        cd_audio_request_set(a, b, 0xE);
        audio_command(0x300);
        *p = 1;
    }
}

void func_8013F220(void) {
    s32 i;

    for (i = 0; i < 3; i++) {
        D_8015ED64[i][0] = 0;
        D_8015ED64[i][1] = 0;
        D_8015ED64[i][2] = 0;
        D_8015ED64[i][3] = 0;
    }
}

void func_8013F250(s32 arg0) {
    if (D_8015ED8C[1] == 0) {
        D_8015EBE4 += 1;
        if (cd_audio_state() == 4 || D_8015EBE4 >= 0x3D) {
            if (D_8015EBE4 >= 0x3D) {
                audio_command(0x74);
            }
            D_8015EE44.unk_04 = 4;
            D_8015ED8C[1] = 1;
            D_8015EBE4 = 0;
        }
    }
    if (D_8015EE44.unk_04 == 0) {
        D_8015ED8C[1] = 0;
        D_8015ED8C[0] += 1;
    }
    if (arg0 != 0 && D_8015EE44.unk_00 - 3 >= arg0) {
        D_8015ED8C[1] = 0;
        D_8015ED8C[0] += 1;
    }
}

/* FAKE: XOR zero retains the original repeated store; the final dead test keeps
 * value in v0 on the implicit-return paths. T-9720. */
s32 func_8013F350(s32 arg0, s32 arg1) {
    s32 value;

    value = D_8015ED8C[1];
    if (value == 0) {
        value = D_8015E744;
        D_8015E744 = arg1;
    }
    if (D_8015EE44.unk_04 == 0) {
        D_8015E744 = value;
        value = 0;
        D_8015ED8C[0] += 1;
    }
    D_8015ED8C[1] = value ^ 0;
    if (arg0 != 0) {
        D_8015ED8C[1] = value;
        if ((D_8015EE44.unk_00 - 3) >= arg0) {
            D_8015E744 = value;
            D_8015ED8C[0] += 1;
            value = 0;
            D_8015ED8C[1] = 0;
        }
    }
    if (value) {
    }
}

void func_8013F3F0(s32 arg0, s32 arg1) {
    s32 i;
    Actor *p;

    p = (Actor *)actor_pool;
    for (i = 40; i < 64; i++) {
        p[96 + i].x += arg0;
        p[96 + i].y += arg1;
    }
    ((Actor *)actor_pool)[125].x += arg0;
    ((Actor *)actor_pool)[126].x += arg0;
    ((Actor *)actor_pool)[127].x += arg0;
    ((Actor *)actor_pool)[125].y += arg1;
    ((Actor *)actor_pool)[126].y += arg1;
    ((Actor *)actor_pool)[127].y += arg1;
}
