/* player_and_asset_uploads.c: src/ovl/TT/80142D40.c:339: VRAM/CD load helpers plus player spawn/control/movement, hit, pickup effects and draw; body 350: p = tt_player; 351: pad = tt_sys + 0x20;. Original object 0x80142D40-0x80144D70 (config/objects/TT.txt). */
#include "common.h"
#include "ovl/TT.h"

void tt_vram_upload_palettes(u8 *arg0, s16 arg1, s16 arg2) {
    RECT r;
    s32 i;

    /* FAKE: w and h on one source line; IDO then stores h before w (line-based scheduling). T-6070 */
    r.w = 0x100; r.h = 0x10;
    r.x = arg1;
    r.y = arg2;
    for (i = 0; i < 0x2000; i += 0x20) {
        *(s16 *)(arg0 + i) = 0;
    }
    func_8009C884(&r, arg0);
}

void tt_vram_upload_tiles(void *arg0, s16 arg1, s16 arg2) {
    RECT r;

    /* FAKE: w and h on one source line; IDO then stores h before w (line-based scheduling). T-6070 */
    r.w = 0x20; r.h = 0x80;
    r.x = arg1;
    r.y = arg2;
    func_8009C884(&r, arg0);
}

void tt_cd_read_blocking(s32 arg0, s32 arg1, s32 arg2) {
    u8 pos[4];
    s32 sec;
    s32 min;
    s32 frame;

    frame = arg1 % 75;
    sec = arg1 / 75;
    pos[2] = (frame / 10) * 16 + frame % 10;
    min = sec / 60;
    sec = sec % 60;
    pos[0] = (min / 10) * 16 + min % 10;
    pos[1] = (sec / 10) * 16 + sec % 10;
    while (func_800879D0(2, pos, 0) == 0) {
    }
    while (func_80087954(1, 0) != 2) {
    }
    func_80087E6C((arg2 + 0x7FF) / 0x800, arg0, 0x80);
    while (func_80087E4C(1, 0) > 0) {
    }
}

void tt_cd_read_start(s32 arg0, s32 arg1, s32 arg2) {
    tt_cd_read_count_previous = tt_cd_read_count_now;
    tt_cd_read_count_now = cd_done_count();
    cd_audio_command(9, 0, 0);
    cd_read_sectors_nowait((arg2 + 0x7FF) / 2048, arg0, arg1);
}

u8 tt_cd_read_counter_before(void) {
    return tt_cd_read_count_previous;
}

void tt_assets_load(void) {
    RECT r;
    s32 pad[2]; /* FAKE: unused home below r puts the RECT at sp+0x30 in the 56-byte frame. T-11520 */

    tt_cd_read_blocking(0x80162000, 0xBECA, 0x54900);
    tt_vram_upload_palettes((u8 *)0x80162000, 0, 0xF0);
    tt_vram_upload_palettes((u8 *)0x80164000, 0x100, 0xF0);
    tt_vram_upload_tiles((void *)0x80166000, 0x280, 0);
    tt_vram_upload_tiles((void *)0x80168000, 0x280, 0x80);
    tt_vram_upload_tiles((void *)0x8016A000, 0x2A0, 0);
    tt_vram_upload_tiles((void *)0x8016C000, 0x2A0, 0x80);
    tt_vram_upload_tiles((void *)0x8016E000, 0x2C0, 0);
    tt_vram_upload_tiles((void *)0x80170000, 0x2C0, 0x80);
    tt_vram_upload_tiles((void *)0x80172000, 0x2E0, 0);
    tt_vram_upload_tiles((void *)0x80174000, 0x380, 0);
    tt_vram_upload_tiles((void *)0x80176000, 0x380, 0x80);
    tt_vram_upload_tiles((void *)0x80178000, 0, 0x100);
    tt_vram_upload_tiles((void *)0x8017A000, 0, 0x180);
    tt_vram_upload_tiles((void *)0x8017C000, 0x20, 0x100);
    tt_vram_upload_tiles((void *)0x8017E000, 0x20, 0x180);
    tt_vram_upload_tiles((void *)0x80180000, 0x40, 0x100);
    tt_vram_upload_tiles((void *)0x80182000, 0x40, 0x180);
    tt_vram_upload_tiles((void *)0x80184000, 0x60, 0x100);
    tt_vram_upload_tiles((void *)0x80186000, 0x60, 0x180);
    tt_vram_upload_tiles((void *)0x80188000, 0x80, 0x100);
    tt_vram_upload_tiles((void *)0x8018A000, 0x80, 0x180);
    tt_vram_upload_tiles((void *)0x8018C000, 0xA0, 0x100);
    tt_vram_upload_tiles((void *)0x8018E000, 0xA0, 0x180);
    tt_vram_upload_tiles((void *)0x80190000, 0xC0, 0x100);
    tt_vram_upload_tiles((void *)0x80192000, 0xC0, 0x180);
    tt_vram_upload_tiles((void *)0x80194000, 0xE0, 0x100);
    tt_vram_upload_tiles((void *)0x80196000, 0xE0, 0x180);
    tt_vram_upload_tiles((void *)0x80198000, 0x100, 0x100);
    tt_vram_upload_tiles((void *)0x8019A000, 0x100, 0x180);
    tt_vram_upload_tiles((void *)0x8019C000, 0x120, 0x100);
    tt_vram_upload_tiles((void *)0x8019E000, 0x120, 0x180);
    tt_vram_upload_tiles((void *)0x801A0000, 0x140, 0x100);
    tt_vram_upload_tiles((void *)0x801A2000, 0x140, 0x180);
    tt_vram_upload_tiles((void *)0x801A4000, 0x200, 0x100);
    tt_vram_upload_tiles((void *)0x801A6000, 0x200, 0x180);
    tt_vram_upload_tiles((void *)0x801A8000, 0x260, 0x100);
    tt_vram_upload_tiles((void *)0x801AA000, 0x260, 0x180);
    tt_vram_upload_tiles((void *)0x801AC000, 0x280, 0x100);
    tt_vram_upload_tiles((void *)0x801AE000, 0x280, 0x180);
    tt_vram_upload_tiles((void *)0x801B0000, 0x2A0, 0x100);
    tt_vram_upload_tiles((void *)0x801B2000, 0x240, 0x100);
    tt_vram_upload_tiles((void *)0x801B4000, 0x240, 0x180);
    /* The four RECT fields on one line: IDO then stores h, w, y, x (line-based scheduling). T-11520 */
    r.x = 0x3C0; r.y = 0x180; r.w = 0x18; r.h = 0x30;
    func_8009C884(&r, (void *)0x801B6000);
    r.x = 0x3C0; r.y = 0x1FF; r.w = 0x10; r.h = 1;
    func_8009C884(&r, tt_vram_palette_row);
    func_8009C674(0);
    tt_cd_read_blocking(0x80010000, 0xC08B, 0xE20);
    tt_cd_read_blocking(0x80162000, 0xC06B, 0xFED0);
    tt_cd_read_blocking(0x80012000, 0xD6D4, 0x2420);
    tt_cd_read_blocking(0x8001C000, 0xD678, 0x69E4);
    tt_cd_read_blocking(0x80172000, 0xD685, 0x27D60);
    tt_cd_read_blocking(0x80016000, 0xC8A8, 0x2420);
    tt_cd_read_blocking(0x8002E800, 0xC857, 0x69E4);
    tt_cd_read_blocking(0x8019A000, 0xC85B, 0x27D60);
    tt_cd_read_blocking(0x8001A000, 0xC067, 0xE20);
    tt_cd_read_blocking(0x801C2000, 0xC032, 0x1A680);
}

void tt_player_clear(void) {
    *tt_player = 0;
}

void tt_player_power_preset(void) {
    s32 i;
    u8 *follower;
    u8 *player;
    s8 v;

    player = tt_player;
    follower = tt_options;
    v = 8;
    if (tt_core[0x22] != 0) {
        switch (tt_sys[0x52]) {
        default:
            v = 8;
            player[0x53] = 0;
            player[0x54] = 4;
            player[0x55] = v;
            follower[0x92] = v;
            break;
        case 1:
            v = 8;
            player[0x53] = 0;
            player[0x54] = 4;
            player[0x55] = v;
            follower[0x92] = v;
            break;
        case 2:
            v = 9;
            player[0x53] = 0;
            player[0x54] = 4;
            player[0x55] = v;
            follower[0x92] = v;
            break;
        case 3:
            v = 9;
            player[0x53] = 0;
            player[0x54] = 4;
            player[0x55] = v;
            follower[0x92] = v;
            break;
        case 4:
            player[0x53] = 1;
            player[0x54] = 5;
            player[0x55] = 0xA;
            follower[0x92] = 9;
            for (i = 0; i != 3; i++) {
                tt_option_activate(follower);
                follower += 0xA4;
            }
            break;
        }
    } else {
        player[0x53] = 0;
        player[0x54] = 4;
        player[0x55] = 8;
        follower[0x92] = v;
    }
}

void tt_player_init(void) {
    u8 *p;
    u8 *arm;
    s32 n;

    p = tt_player;
    *p = 0;
    *(s16 *)(p + 2) = 0;
    p[12] = 0;
    *(s16 *)(p + 4) = 0;
    /* FAKE: chained zero stores; their reloads keep dead ranges (no code) that push the hoisted 2 and tt_sprite_set_table into t0/t1 as in the original. T-11850 */
    p[13] = p[14] = p[15] = 0;
    *(u16 *)(p + 6) = *(u16 *)(p + 8) = *(u16 *)(p + 10) = 0; /* FAKE: u16 view, see above. T-11850 */
    *(s16 *)(p + 16) = 2;
    *(u8 **)(p + 56) = tt_sprite_set_table;
    p[60] = 0;
    p[61] = 0;
    p[62] = 0;
    p[64] = 0x60;
    p[72] = 0;
    tt_player_power_preset();
    p[87] = 0;
    p[88] = 0;
    p[89] = 0;
    p[90] = 0;
    p[91] = 0;
    p[92] = 0;
    p[93] = 0;
    p[94] = 0;
    p[95] = 0;
    p[96] = 0;
    *(s16 *)(p + 4) = 0x20;
    *(u16 *)(p + 6) = *(u16 *)(p + 8) = *(u16 *)(p + 10) = 0; /* FAKE: chained, see above. T-11850 */
    p[13] = p[14] = p[15] = 0;
    arm = tt_arms;
    for (n = 5; n < 15; n += 5) {
        arm[62] = n;
        arm += 0x60;
        arm[-96] = 0;
        *(s16 *)(arm - 94) = 0;
        *(s16 *)(arm - 92) = 0;
        arm[-84] = 0;
        *(u16 *)(arm - 90) = *(u16 *)(arm - 88) = *(u16 *)(arm - 86) = 0; /* FAKE: chained, see above. T-11850 */
        arm[-83] = arm[-82] = arm[-81] = 0;
        *(s16 *)(arm - 80) = 2;
        *(u8 **)(arm - 40) = tt_sprite_set_table;
        arm[-36] = 0;
        arm[-35] = 0;
        arm[-32] = 0x60;
    }
}

/* No prototype: tt_player_update calls it with two arguments the body never reads (declared parameters would be homed to the stack). */
void tt_player_spawn_step() {
    u8 *player;
    u8 *arm;
    u8 *follower;
    s32 i;
    s32 x;

    player = tt_player;
    switch (*(u16 *)(player + 6)) {
    case 0:
        tt_se_request(TT_SE_PLAYER_SPAWN);
        player[0] = 1;
        *(s32 *)(player + 0x20) = 0;
        *(s32 *)(player + 0x24) = 0xF80000;
        *(s32 *)(player + 0x28) = 0;
        *(s32 *)(player + 0x2C) = 0xFFFE0000;
        *(s16 *)(player + 0x14) = 0;
        *(s16 *)(player + 0x16) = 0xF8;
        *(u8 **)(player + 0x38) = tt_sprite_set_table;
        player[0x3C] = 0;
        player[0x3D] = 0;
        player[0x3E] = 0;
        player[0x40] = 0x60;
        player[0x48] = 0;
        *(u16 *)(player + 0x50) |= 2;
        player[0x52] = 1;
        player[0x56] = 1;
        *(u16 *)(player + 6) = 0x40;
        arm = tt_arms;
        /* the counter i (reused by the next loop) is LFTR'd into x, giving the original slti 15 test */
        for (i = 0; i < 2; i++) {
            x = i * 5 + 5;
            arm[0x3E] = x;
            arm += 0x60;
            arm[-0x60] = 1;
            *(s16 *)(arm - 0x5E) = 0;
            *(s16 *)(arm - 0x5C) = 0;
            arm[-0x54] = 0;
            /* FAKE: chained zero stores (u16 view); their reloads keep dead ranges in v1/a0 (no code), so x, 1, tt_sprite_set_table, 0x60 and 2 land in a1, a2, a3, t0, t1 as in the original. T-11850 */
            *(u16 *)(arm - 0x5A) = *(u16 *)(arm - 0x58) = *(u16 *)(arm - 0x56) = 0;
            arm[-0x53] = arm[-0x52] = arm[-0x51] = 0;
            *(s16 *)(arm - 0x50) = 2;
            *(u8 **)(arm - 0x28) = tt_sprite_set_table;
            arm[-0x24] = 0;
            arm[-0x23] = 0;
            arm[-0x20] = 0x60;
        }
        follower = tt_options;
        for (i = 0; i != 3; i++, follower += 0xA4) {
            tt_option_activate(follower);
        }
        break;
    case 0x40:
        if (*(s32 *)(tt_player + 0x24) < 0xC00000) {
            *(s32 *)(player + 0x24) = 0xC00000;
            player[0x56] = 0;
            *(u16 *)(player + 6) = 0;
            player[0xD] = 1;
        }
        break;
    }
}

void tt_nop(void) {
}

void tt_player_exit_step(void) {
    u8 *player = tt_player;
    u8 *run = tt_run;

    switch (*(u16 *)(player + 6)) {
    case 0:
        player[0x52] = 1;
        *(u16 *)(player + 0x50) |= 2;
        player[0x56] = 1;
        player[0x5A] = 0;
        player[0x5B] = 0;
        *(u16 *)(player + 6) = 0x40;
        break;
    case 0x40:
        *(s32 *)(player + 0x28) = 0;
        *(s32 *)(player + 0x2C) = 0xFFFC0000;
        if (*(s32 *)(player + 0x24) < (s32)0xFFF00000) {
            *(s32 *)(player + 0x2C) = 0;
            *(u16 *)(player + 6) = 0x50;
            *(u16 *)(run + 0x10) = 0;
        }
        break;
    case 0x50:
        if ((u32)*(u16 *)(tt_run + 0x10) >= 0x5B) {
            *(u16 *)(player + 6) = 0;
            player[0xD] = 1;
        }
        *(u16 *)(run + 0x10) += 1;
        break;
    }
}

void tt_player_control(void) {
    s32 i;
    s32 j;
    u8 *pad;
    u8 *player;
    s32 found;
    u8 *r;
    u8 *bomb_target;
    u8 *a;
    u8 *q;

    player = tt_player;
    pad = tt_sys + 0x20;
    if (*(u16 *)pad & 0x8000) {
        *(s32 *)(player + 0x28) = -1;
        player[0x59] = 0;
        if (player[0x58] < 8) {
            player[0x58]++;
        }
    } else if (*(u16 *)pad & 0x2000) {
        *(s32 *)(player + 0x28) = 1;
        player[0x58] = 0;
        if (player[0x59] < 8) {
            player[0x59]++;
        }
    } else {
        *(s32 *)(player + 0x28) = 0;
        if (player[0x58] != 0) {
            player[0x58]--;
        }
        if (player[0x59] != 0) {
            player[0x59]--;
        }
    }
    if (*(u16 *)pad & 0x1000) {
        *(s32 *)(player + 0x2C) = -1;
    } else if (*(u16 *)pad & 0x4000) {
        *(s32 *)(player + 0x2C) = 1;
    } else {
        *(s32 *)(player + 0x2C) = 0;
    }
    if (*(s32 *)(player + 0x28) != 0 && *(s32 *)(player + 0x2C) != 0) {
        *(s32 *)(player + 0x28) *= player[0x54] * 0x6000;
        *(s32 *)(player + 0x2C) *= player[0x54] * 0x6000;
    } else {
        *(s32 *)(player + 0x28) *= player[0x54] << 15;
        *(s32 *)(player + 0x2C) *= player[0x54] << 15;
    }
    if (*(u16 *)(pad + 4) & 1) {
        r = tt_shots;
        for (i = 0; i < 2; i++) {
            if (player[0x55] == 0xA) {
                if (r[0] == 0 && r[0xA8] == 0 && r[0x150] == 0) {
                    tt_se_request(TT_SE_SHOT_FIRE);
                    r[0] = 1;
                    *(u16 *)(r + 4) = 0x20;
                    r[0xD] = 0;
                    r[0xE] = 0;
                    r[0xF] = 0;
                    *(s16 *)(r + 6) = 0;
                    *(s16 *)(r + 8) = 0;
                    *(s16 *)(r + 0xA) = 0;
                    r[0xA8] = 1;
                    *(u16 *)(r + 0xAC) = 0x20;
                    r[0x150] = 1;
                    *(u16 *)(r + 0x154) = 0x20;
                    break;
                }
            } else if (r[0] == 0) {
                tt_se_request(TT_SE_SHOT_FIRE);
                r[0] = 1;
                *(u16 *)(r + 4) = 0x20;
                r[0xD] = 0;
                r[0xE] = 0;
                r[0xF] = 0;
                *(s16 *)(r + 6) = 0;
                *(s16 *)(r + 8) = 0;
                *(s16 *)(r + 0xA) = 0;
                break;
            }
            r += 0x54;
        }
        if (player[0x53] != 0) {
            r = tt_shots + 6 * 0x54;
            for (i = 6; i < 8; i++) {
                if (r[0] == 0) {
                    r[0] = 1;
                    *(u16 *)(r + 4) = 0x20;
                    *(s16 *)(r + 0xA) = 0;
                    *(s16 *)(r + 8) = 0;
                    *(s16 *)(r + 6) = 0;
                    r[0xF] = 0;
                    r[0xE] = 0;
                    r[0xD] = 0;
                    break;
                }
                r += 0x54;
            }
            r = tt_shots + 8 * 0x54;
            for (i = 8; i < 10; i++) {
                if (r[0] == 0) {
                    r[0] = 1;
                    *(u16 *)(r + 4) = 0x20;
                    *(s16 *)(r + 0xA) = 0;
                    *(s16 *)(r + 8) = 0;
                    *(s16 *)(r + 6) = 0;
                    r[0xF] = 0;
                    r[0xE] = 0;
                    r[0xD] = 0;
                    break;
                }
                r += 0x54;
            }
            r = tt_shots + 10 * 0x54;
            for (i = 10; i < 12; i++) {
                if (r[0] == 0) {
                    r[0] = 1;
                    *(u16 *)(r + 4) = 0x20;
                    *(s16 *)(r + 0xA) = 0;
                    *(s16 *)(r + 8) = 0;
                    *(s16 *)(r + 6) = 0;
                    r[0xF] = 0;
                    r[0xE] = 0;
                    r[0xD] = 0;
                    break;
                }
                r += 0x54;
            }
        }
    }
    if (*(u16 *)(pad + 4) & 2) {
        /* FAKE: both loads on one line; IDO then schedules the lui of tt_bomb_targets first and the lw of tt_bombs first. T-11840 */
        bomb_target = tt_bomb_targets; a = tt_bombs;
        found = 0;
        for (i = 0; i < 2; i++) {
            if (bomb_target[0] == 0 && a[0] == 0 && (player[0x5C] == 0 || player[0x5D] == 0)) {
                tt_se_request(TT_SE_BOMB_THROW);
                bomb_target[0] = 1;
                *(u16 *)(bomb_target + 4) = 0x20;
                bomb_target[0xD] = 0;
                bomb_target[0xE] = 0;
                bomb_target[0xF] = 0;
                *(s16 *)(bomb_target + 6) = 0;
                *(s16 *)(bomb_target + 8) = 0;
                *(s16 *)(bomb_target + 0xA) = 0;
                a[0] = 1;
                a[1] = 1;
                *(u16 *)(a + 0x3E) = 1;
                a[0x3D] = player[0x60];
                *(u16 *)(a + 4) = 0x20;
                a[0xD] = 0;
                a[0xE] = 0;
                a[0xF] = 0;
                *(s16 *)(a + 6) = 0;
                *(s16 *)(a + 8) = 0;
                *(s16 *)(a + 0xA) = 0;
                found = a[0];
                break;
            }
            bomb_target += 0x58;
            a += 0x58;
        }
        if (player[0x53] != 0) {
            for (j = 1; j < 4; j++) {
                /* FAKE: one line, as above (temp numbering t5 = tt_bomb_targets, loads in the other order). T-11840 */
                q = tt_bomb_targets + j * 0xB0; a = tt_bombs + j * 0xB0;
                for (i = 0; i < 2; i++) {
                    if (q[0] == 0 && a[0] == 0 && (player[0x5C] == 0 || player[0x5D] == 0)) {
                        q[0] = 1;
                        *(u16 *)(q + 4) = 0x20;
                        /* FAKE: the u16 and byte chains keep a dead range in $a1 (no code), so 2, j * 0xB0 and player land in a2/a3/t0. T-11840 */
                        *(u16 *)(q + 6) = *(u16 *)(q + 8) = *(u16 *)(q + 0xA) = 0;
                        q[0xD] = q[0xE] = q[0xF] = 0;
                        a[0] = 1;
                        a[1] = 1;
                        *(u16 *)(a + 0x3E) = 1;
                        *(u16 *)(a + 4) = 0x20;
                        a[0xD] = 0;
                        a[0xE] = 0;
                        a[0xF] = 0;
                        *(s16 *)(a + 6) = 0;
                        *(s16 *)(a + 8) = 0;
                        *(s16 *)(a + 0xA) = 0;
                        found = 1;
                        a[0x3D] = player[0x60];
                        break;
                    }
                    q += 0x58;
                    a += 0x58;
                }
            }
        }
        if (found) {
            if (player[0x60] != 0) {
                player[0x5D] = 1;
            } else {
                player[0x5C] = 1;
            }
            player[0x60]++;
            player[0x60] &= 1;
        }
    }
}

void tt_player_move(void) {
    u8 *player = tt_player;
    s32 v;

    *(s32 *)(player + 0x20) += *(s32 *)(player + 0x28);
    *(s32 *)(player + 0x24) += *(s32 *)(player + 0x2C);
    if (player[0x56] == 0) {
        if (*(s32 *)(player + 0x20) < (s32)0xFF880000) {
            *(s32 *)(player + 0x20) = -0x780000;
        } else if (*(s32 *)(player + 0x20) >= 0x780001) {
            *(s32 *)(player + 0x20) = 0x780000;
        }
        v = *(s32 *)(player + 0x24);
        if (v < 0x380000) {
            *(s32 *)(player + 0x24) = 0x380000;
        } else if (v >= 0xE00001) {
            *(s32 *)(player + 0x24) = 0xE00000;
        }
    }
    *(s16 *)(player + 0x14) = *(s16 *)(player + 0x22);
    *(s16 *)(player + 0x16) = *(s16 *)(player + 0x26);
}

typedef struct {
    s32 w[9];
} TtCopyBlock; /* a 0x24-byte struct copy: IDO copies it in a 3-trip loop of 3 words */

void tt_player_hit(void) {
    s32 pad; /* FAKE: unused local declared first puts sp38/sp34 at the original slots; real source unknown. T-9340 */
    u8 *sp38;
    s32 *sp34;
    u8 *player;
    u8 *arm;

    sp38 = tt_run;
    sp34 = tt_stage_script;
    player = tt_player;
    arm = tt_arms;
    tt_se_request(TT_SE_PLAYER_DEATH);
    tt_se_request(TT_SE_CODE_OR_HIT);
    if (*(u16 *)((u8 *)sp34 + 2) < 0x401) {
        *(s32 *)(sp38 + 4) = 0x8000;
        *(s32 *)sp38 = 0x8000;
    }
    player[0x52] = 1;
    player[0x56] = 1;
    player[0x53] = 0;
    *(s32 *)(player + 0x28) = 0;
    *(s32 *)(player + 0x2C) = 0x10000;
    *(u8 **)(player + 0x38) = tt_sprite_set_table;
    player[0x3C] = 0;
    player[0x3D] = 1;
    player[0x3E] = 0;
    player[0x40] = 0x80;
    tt_sprite_animation_step(player + 0x38);
    *(TtCopyBlock *)(arm + 0x14) = *(TtCopyBlock *)(player + 0x14);
    *(s32 *)(arm + 0x28) = 0;
    *(s32 *)(arm + 0x2C) = 0;
    *(u8 **)(arm + 0x38) = tt_sprite_set_table;
    arm[0x3C] = 0;
    arm[0x3D] = 2;
    arm[0x3E] = 0;
    arm[0x40] = 0x80;
    tt_sprite_animation_step(arm + 0x38);
    *(s32 *)(arm + 0x98) = 0;
}

void tt_player_update(void) {
    s32 i;
    u8 *g;
    u8 *run;
    u8 *r;
    u8 *player;
    u8 *arm;
    u16 v;

    player = tt_player;
    arm = tt_arms;
    g = tt_core;
    run = tt_run;
    switch (*(u16 *)(player + 4)) {
    case 0x20:
        tt_player_spawn_step(run, g);
        if (player[0xD] != 0) {
            player[0xD] = 0;
            *(s16 *)(player + 0x62) = 0;
            tt_bombs_clear();
            tt_bomb_targets_clear();
            *(u16 *)(player + 4) = 0x30;
            *(s16 *)(player + 0xA) = 0;
            *(s16 *)(player + 8) = 0;
            *(s16 *)(player + 6) = 0;
            player[0xF] = 0;
            player[0xE] = 0;
            player[0xD] = 0;
        }
        break;
    case 0x30:
        tt_player_control();
        v = *(u16 *)(player + 0x62);
        if (v >= 0x5A) {
            player[0x52] = 0;
            *(u16 *)(player + 0x50) &= ~2;
            *(u16 *)(player + 4) = 0x40;
            *(s16 *)(player + 0xA) = 0;
            *(s16 *)(player + 8) = 0;
            *(s16 *)(player + 6) = 0;
            player[0xF] = 0;
            player[0xE] = 0;
            player[0xD] = 0;
        } else {
            *(u16 *)(player + 0x62) = v + 1;
        }
        break;
    case 0x40:
        tt_player_control();
        break;
    case 0x60:
        *(s32 *)(player + 0x28) = 0;
        *(s32 *)(player + 0x2C) = 0x10000;
        tt_sprite_animation_step(player + 0x38);
        tt_sprite_animation_step(arm + 0x38);
        if (player[0x40] & 1) {
            *(s32 *)(player + 0x28) = 0;
            *(s32 *)(player + 0x2C) = 0;
            *(u16 *)(player + 4) = 0x61;
        }
        break;
    case 0x61:
        if (tt_run[0xC] == 0) {
            g[0x12] = 0;
            *(u16 *)g = 5;
            *(s16 *)(g + 4) = 0;
        } else {
            run[0xC] -= 1;
            tt_lives_icons_update();
        }
        tt_player_init();
        player[0] = 0;
        *(s32 *)(player + 0x20) = 0;
        *(s32 *)(player + 0x24) = 0xF80000;
        *(s32 *)(player + 0x28) = 0;
        *(s32 *)(player + 0x2C) = 0;
        *(s16 *)(player + 0x14) = 0;
        *(s16 *)(player + 0x16) = 0xF8;
        *(u16 *)(player + 4) = 0x20;
        *(s16 *)(player + 0xA) = 0;
        *(s16 *)(player + 8) = 0;
        *(s16 *)(player + 6) = 0;
        player[0xF] = 0;
        player[0xE] = 0;
        player[0xD] = 0;
        break;
    case 0xA0:
        tt_player_exit_step();
        player[0x3E] = 0;
        arm[0x3E] = 5;
        arm[0x9E] = 0xA;
        if (player[0xD] != 0) {
            /* u16, not s16: the 4 then joins the a2 range of the later == 4 tests (li before the beqz here, and
             * the empty b/nop join blocks after the inner ifs of cases 0x20, 0x30 and 0x60). T-11840 */
            *(u16 *)g = 4;
            *(s16 *)(g + 6) = 0;
            *(s16 *)(g + 4) = 0;
            *(s16 *)(g + 2) = 0;
            g[0xB] = 0;
            g[0xA] = 0;
            g[9] = 0;
        }
        break;
    }
    tt_player_move();
    if (*(u16 *)(player + 4) < 0x60) {
        if (player[0x58] == 0 && player[0x59] == 0) {
            player[0x57] = 0;
        } else if (player[0x58] >= 7) {
            player[0x57] = 2;
        } else if (player[0x58] >= 3) {
            player[0x57] = 1;
        } else if (player[0x59] >= 7) {
            player[0x57] = 4;
        } else if (player[0x59] >= 3) {
            player[0x57] = 3;
        }
        player[0x3E] = player[0x57];
        /* Byte before the product: sets the operand order of the addu and the t-register numbering. T-11840 */
        arm[0x3E] = player[0x5A] + player[0x57] * 10 + 5;
        arm[0x9E] = player[0x5B] + player[0x57] * 10 + 10;
        if (player[0x5C] != 0) {
            if (tt_arm_swing_frames[player[0x5A]] < ++player[0x5E]) {
                player[0x5E] = 0;
                if (++player[0x5A] >= 5) {
                    player[0x5A] = 0;
                    player[0x5C] = 0;
                }
            }
            r = tt_bombs;
            for (i = 0; i < 8; i++) {
                if (r[0] != 0 && r[0x3D] == 0 && player[0x5A] == 4 && player[0x5E] == 1) {
                    r[1] = 0;
                    *(s16 *)(r + 0x3E) = 0;
                }
                r += 0x58;
            }
        }
        if (player[0x5D] != 0) {
            if (tt_arm_swing_frames[player[0x5B]] < ++player[0x5F]) {
                player[0x5F] = 0;
                if (++player[0x5B] >= 5) {
                    player[0x5B] = 0;
                    player[0x5D] = 0;
                }
            }
            r = tt_bombs;
            for (i = 0; i < 8; i++) {
                if (r[0] != 0 && r[0x3D] == 1 && player[0x5B] == 4 && player[0x5F] == 1) {
                    r[1] = 0;
                    *(s16 *)(r + 0x3E) = 0;
                }
                r += 0x58;
            }
        }
    }
}

/* FAKE: the first test reads tt_player through an array view of the pointer block at tt_sys, so uopt does not
 * merge it with p; the original loads p straight into s2 and reloads the global for the test (T-0018 promo). T-11620 */
void tt_player_draw(void) {
    u8 *player;
    u8 *arm;
    u16 k;
    u8 n;

    player = tt_player;
    arm = tt_arms;
    if (player[0] != 0) {
        if (!(*(u16 *)(((u8 **)&tt_sys)[5] + 0x50) & 2) || !(*(u16 *)(tt_sys + 8) & 1)) {
            k = *(u16 *)(tt_player + 4);
            if (k >= 0x60 && k < 0x64) {
                if (!(tt_player[0x40] & 1)) {
                    tt_part_sprite_draw(player + 0x14, player + 0x38, *(u16 *)(player + 0x10));
                }
                if (!(arm[0x40] & 1)) {
                    tt_part_sprite_draw(arm + 0x14, arm + 0x38, *(u16 *)(player + 0x10));
                }
            } else {
                n = tt_player[0x57];
                if (n == 0) {
                    tt_part_sprite_draw(player + 0x14, arm + 0x38, *(u16 *)(arm + 0x10));
                    arm += 0x60;
                    tt_part_sprite_draw(player + 0x14, arm + 0x38, *(u16 *)(arm + 0x10));
                    tt_part_sprite_draw(player + 0x14, player + 0x38, *(u16 *)(player + 0x10));
                } else if (n < 3) {
                    tt_part_sprite_draw(player + 0x14, arm + 0x98, *(u16 *)(arm + 0x10));
                    tt_part_sprite_draw(player + 0x14, player + 0x38, *(u16 *)(player + 0x10));
                    tt_part_sprite_draw(player + 0x14, arm + 0x38, *(u16 *)(arm + 0x10));
                } else {
                    tt_part_sprite_draw(player + 0x14, arm + 0x38, *(u16 *)(arm + 0x10));
                    tt_part_sprite_draw(player + 0x14, player + 0x38, *(u16 *)(player + 0x10));
                    tt_part_sprite_draw(player + 0x14, arm + 0x98, *(u16 *)(arm + 0x10));
                }
            }
        }
    }
}

void tt_bell_collect(u8 *arg0) {
    u8 *player;
    u8 *w;
    s32 i;
    u8 *follower;

    player = tt_player;
    switch (*(u16 *)(arg0 + 4)) {
    case 0x40:
    case 0x60:
        switch (arg0[0x58]) {
        case 0:
            tt_se_request(TT_SE_PICKUP_COLLECT);
            arg0[0] = 0;
            break;
        case 1:
            tt_se_request(TT_SE_SHOT_LEVEL_PICKUP);
            tt_bell_set_popup_label(arg0, 0x28);
            player[0x55] = 9;
            tt_options[0x92] = 9;
            *(s32 *)(arg0 + 0x34) = 0;
            *(s32 *)(arg0 + 0x2C) = 0;
            *(s32 *)(arg0 + 0x28) = 0;
            *(s16 *)(arg0 + 0x62) = 0;
            *(u16 *)(arg0 + 4) = 0x90;
            /* FAKE: chained stores; the reloads keep dead ranges in v0/v1 (no code, as the function is void), so the shared 9 lands in a0 as in the original. T-11850 */
            arg0[0xD] = arg0[0xE] = arg0[0xF] = 0;
            *(u16 *)(arg0 + 6) = *(u16 *)(arg0 + 8) = *(u16 *)(arg0 + 0xA) = 0; /* FAKE: u16 view, see above. T-11850 */
            break;
        case 2:
            tt_se_request(TT_SE_SPEED_UP);
            tt_bell_set_popup_label(arg0, 0x29);
            player[0x54] += 1;
            *(s32 *)(arg0 + 0x34) = 0;
            *(s32 *)(arg0 + 0x2C) = 0;
            *(s32 *)(arg0 + 0x28) = 0;
            *(s16 *)(arg0 + 0x62) = 0;
            *(u16 *)(arg0 + 4) = 0x90;
            arg0[0xD] = 0;
            arg0[0xE] = 0;
            arg0[0xF] = 0;
            *(s16 *)(arg0 + 6) = 0;
            *(s16 *)(arg0 + 8) = 0;
            *(s16 *)(arg0 + 0xA) = 0;
            break;
        case 3:
            tt_se_request(TT_SE_FOLLOWER_PICKUP);
            if (player[0x53] == 0) {
                tt_bell_set_popup_label(arg0, 0x2A);
                player[0x53] = 1;
                follower = tt_options;
                for (i = 0; i != 3; i++, follower += 0xA4) {
                    tt_option_activate(follower);
                }
            }
            *(s32 *)(arg0 + 0x34) = 0;
            *(s32 *)(arg0 + 0x2C) = 0;
            *(s32 *)(arg0 + 0x28) = 0;
            *(s16 *)(arg0 + 0x62) = 0;
            *(u16 *)(arg0 + 4) = 0x90;
            *(s16 *)(arg0 + 0xA) = 0;
            *(s16 *)(arg0 + 8) = 0;
            *(s16 *)(arg0 + 6) = 0;
            arg0[0xF] = 0;
            arg0[0xE] = 0;
            arg0[0xD] = 0;
            break;
        case 6:
            tt_se_request(TT_SE_SPEED_DOWN);
            tt_bell_set_popup_label(arg0, 0x2B);
            player[0x54] -= 1;
            if (player[0x54] == 0) {
                player[0x54] = 1;
            }
            *(s32 *)(arg0 + 0x34) = 0;
            *(s32 *)(arg0 + 0x2C) = 0;
            *(s32 *)(arg0 + 0x28) = 0;
            *(s16 *)(arg0 + 0x62) = 0;
            *(u16 *)(arg0 + 4) = 0x90;
            *(s16 *)(arg0 + 0xA) = 0;
            *(s16 *)(arg0 + 8) = 0;
            *(s16 *)(arg0 + 6) = 0;
            arg0[0xF] = 0;
            arg0[0xE] = 0;
            arg0[0xD] = 0;
            break;
        }
        break;
    case 0x20:
        break;
    case 0x70:
        if (tt_player[0x52] == 0) {
            w = tt_run;
            if (*(u16 *)((u8 *)tt_stage_script + 2) < 0x401) {
                *(s32 *)(w + 4) = 0x8000;
                *(s32 *)w = 0x8000;
            }
            tt_player_hit();
            *(u16 *)(player + 4) = 0x60;
            *(s16 *)(player + 0xA) = 0;
            *(s16 *)(player + 8) = 0;
            *(s16 *)(player + 6) = 0;
            player[0xF] = 0;
            player[0xE] = 0;
            player[0xD] = 0;
            arg0[0] = 0;
        }
        break;
    }
}

void tt_lives_icons_update(void) {
    u8 *run;
    u8 *p;
    u32 i;

    run = tt_run;
    for (i = 0, p = tt_sprites + 0x64; i < (u32)(2 - run[0xC]); i++) {
        *p = 0;
        p -= 0x64;
    }
}

void tt_cheat_grant_all(void) {
    u8 *player;
    u8 *follower;
    s32 i;

    player = tt_player;
    follower = tt_options;
    *(u16 *)(tt_run + 8) = 0x8CA0;
    player[0x53] = 1;
    player[0x55] = 9;
    follower[0x92] = 9;
    for (i = 0; i != 3; i++) {
        tt_option_activate(follower);
        follower += 0xA4;
    }
}
