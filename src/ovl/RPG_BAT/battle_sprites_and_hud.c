/* battle_sprites_and_hud.c: src/ovl/RPG_BAT/80141060.c:6 clears actor records then fills sprite frames/animation pointers; :244 sets HUD actors. Original object 0x80141060-0x80141C30 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/RPG_BAT.h"

#define ACT(i) (((Actor *) actor_pool)[i])

void rpg_stage_sprites_setup(void) {
    s32 sel; /* a named copy of the switch value: its home word is the extra frame word (T-17119) */
    s32 i;
    s32 j;

    for (i = 0; i < 0x60; i++) {
        actor_clear(i + 0x40);
        if (i < 0x3F) {
            ACT(64 + i).unk_01 = 3;
        } else if (i < 0x40) {
            ACT(64 + i).unk_01 = 9;
        } else if (i < 0x48) {
            ACT(64 + i).unk_01 = 4;
        } else {
            ACT(64 + i).unk_01 = 5;
        }
        ACT(64 + i).quad_flags = 0x41000000;
        ACT(64 + i).draw_flags = 0;
        ACT(64 + i).flags = 1;
        ACT(64 + i).blink_period = 1;
        ACT(64 + i).brightness = 0;
        ACT(64 + i).x = -0xA0;
        ACT(64 + i).y = -0x78;
        ACT(64 + i).unk_05 = 0x80;
        ACT(64 + i).unk_43 = 0;
        ACT(64 + i).unk_04 = 8;
        ACT(64 + i).frame_table = (u8 *) rpg_asset_frames_0;
        ACT(64 + i).sequence_table = (u8 *) rpg_asset_animation_0;
        ACT(64 + i).tpage_table = (u8 *) rpg_asset_sheet_0;
    }
    for (i = 0; i < 0x48; i++) {
        ACT(64 + i).frame_table = (u8 *) rpg_asset_frames_0;
        ACT(64 + i).sequence_table = (u8 *) rpg_asset_animation_0;
        ACT(64 + i).tpage_table = (u8 *) rpg_asset_sheet_0;
    }
    load_tpage_buf_lock(0x15, ((s32 *) rpg_asset_sheet_0)[0], 1);
    load_tpage_buf_lock(0x16, ((s32 *) rpg_asset_sheet_0)[1], 1);
    load_tpage_buf_lock(0x17, ((s32 *) rpg_asset_sheet_0)[2], 1);
    load_tpage_buf_lock(0x18, ((s32 *) rpg_asset_sheet_0)[3], 1);
    load_tpage_buf_lock(0x19, ((s32 *) rpg_asset_sheet_0)[4], 1);
    load_tpage_buf_lock(0x35, ((s32 *) rpg_asset_sheet_3)[0], 1);
    load_tpage_buf_lock(0x39, ((s32 *) rpg_asset_sheet_3)[1], 1);
    sel = rpg_encounter;
    switch (sel) {
    case 0x10000:
        load_tpage_buf_lock(0xB, ((s32 *) rpg_asset_sheet_2)[1], 1);
        load_tpage_buf_lock(0xC, ((s32 *) rpg_asset_sheet_2)[2], 1);
        /* fall through */
    case 0x20000:
    case 0x40000:
        load_tpage_buf_lock(0xD, ((s32 *) rpg_asset_sheet_2)[0], 1);
        load_tpage_buf_lock(0x2B, ((s32 *) rpg_asset_sheet_1)[0], 1);
        load_tpage_buf_lock(0x2C, ((s32 *) rpg_asset_sheet_1)[1], 1);
        load_tpage_buf_lock(0x2D, ((s32 *) rpg_asset_sheet_1)[2], 1);
        if (rpg_girl_id == 0xD) {
            load_tpage_buf_lock(5, ((s32 *) rpg_asset_sheet_5)[0], 1);
            load_tpage_buf_lock(6, ((s32 *) rpg_asset_sheet_5)[1], 1);
            load_tpage_buf_lock(7, ((s32 *) rpg_asset_sheet_5)[2], 1);
        }
        break;
    case 0x80000000:
        load_tpage_buf_lock(0xB, ((s32 *) rpg_asset_sheet_5)[0], 1);
        load_tpage_buf_lock(0xC, ((s32 *) rpg_asset_sheet_5)[1], 1);
        load_tpage_buf_lock(0xD, ((s32 *) rpg_asset_sheet_5)[2], 1);
        load_tpage_buf_lock(0xE, ((s32 *) rpg_asset_sheet_5)[3], 1);
        load_tpage_buf_lock(0xF, ((s32 *) rpg_asset_sheet_5)[4], 1);
        load_tpage_buf_lock(0x2B, ((s32 *) rpg_asset_sheet_5)[5], 1);
        load_tpage_buf_lock(0x2D, ((s32 *) rpg_asset_sheet_1)[0], 1);
        load_tpage_buf_lock(0x2E, ((s32 *) rpg_asset_sheet_1)[1], 1);
        load_tpage_buf_lock(0x2F, ((s32 *) rpg_asset_sheet_1)[2], 1);
        break;
    case 0x100000:
        load_tpage_buf_lock(0xB, ((s32 *) rpg_asset_sheet_1)[6], 1);
        if (rpg_girl_id != 0xE) {
            switch (rpg_girl_id) {
            case 0:
                i = 1;
                break;
            case 1:
                i = 2;
                break;
            case 2:
                i = 4;
                break;
            case 3:
                i = 3;
                break;
            case 4:
                i = 2;
                break;
            case 5:
                i = 2;
                break;
            case 6:
                i = 3;
                break;
            case 7:
                i = 2;
                break;
            case 8:
                i = 2;
                break;
            case 9:
                i = 1;
                break;
            case 10:
                i = 2;
                break;
            case 13:
                i = 3;
                break;
            }
            for (j = 0; j < i; j++) {
                load_tpage_buf_lock(j + 0x25, ((s32 *) rpg_asset_sheet_5)[j], 1);
            }
        }
        /* fall through */
    case 0x8000000:
        if (rpg_club_move_available == 1) {
            switch (rpg_player_club) {
            case 0:
                i = 4;
                break;
            case 1:
                i = 4;
                break;
            case 2:
                i = 4;
                break;
            case 3:
                i = 5;
                break;
            case 4:
                i = 5;
                break;
            case 5:
                i = 4;
                break;
            case 6:
                i = 3;
                break;
            case 7:
                i = 3;
                break;
            case 8:
                i = 5;
                break;
            case 9:
                i = 5;
                break;
            case 10:
                i = 3;
                break;
            case 11:
                i = 6;
                break;
            }
            for (j = 0; j < 6; j++) {
                if (j < i) {
                    load_tpage_buf_lock(j + 5, ((s32 *) rpg_asset_sheet_4)[j], 1);
                } else {
                    func_80041168(j + 5);
                }
            }
        }
        load_tpage_buf_lock(0x2C, ((s32 *) rpg_asset_sheet_1)[0], 1);
        load_tpage_buf_lock(0xC, ((s32 *) rpg_asset_sheet_1)[1], 1);
        load_tpage_buf_lock(0xD, ((s32 *) rpg_asset_sheet_1)[2], 1);
        load_tpage_buf_lock(0xE, ((s32 *) rpg_asset_sheet_1)[3], 1);
        load_tpage_buf_lock(0xF, ((s32 *) rpg_asset_sheet_1)[4], 1);
        load_tpage_buf_lock(0x2B, ((s32 *) rpg_asset_sheet_1)[5], 1);
        break;
    }
    for (i = 0; i < 0x30; i++) {
        rpg_popup_digit_set_animation(i, 0x27, 0);
    }
    /* The record loops below index actor_pool as bytes (fields 0x0C, 0x10, 0x34, 0x43, 0x01 of record 96 + i), like
     * the other RPG_BAT record loops: the typed Actor index folds 0x1980 into the start pointer and builds
     * a different loop preheader (T-17119, wiki/register-cheatsheet.md). */
    if (rpg_girl_id != 0xE) {
        for (i = 29; i < 31; i++) {
            *(s32 *) &actor_pool[0x198C + i * 0x44] = rpg_asset_frames_5;
            *(s32 *) &actor_pool[0x1990 + i * 0x44] = rpg_asset_animation_5;
            *(s32 *) &actor_pool[0x19B4 + i * 0x44] = rpg_asset_sheet_5;
            ((Actor *)actor_pool)[i + 96].unk_43 = 0x1C;
            ((Actor *)actor_pool)[i + 96].unk_01 = 5;
        }
    }
    if (rpg_club_move_available == 1) {
        for (i = 40; i < 48; i++) {
            *(s32 *) &actor_pool[0x198C + i * 0x44] = rpg_asset_frames_4;
            *(s32 *) &actor_pool[0x1990 + i * 0x44] = rpg_asset_animation_4;
            *(s32 *) &actor_pool[0x19B4 + i * 0x44] = rpg_asset_sheet_4;
            ((Actor *)actor_pool)[i + 96].unk_43 = 0x1D;
        }
    }
    if (rpg_encounter & 0xF0000) {
        for (i = 40; i < 48; i++) {
            *(s32 *) &actor_pool[0x198C + i * 0x44] = rpg_asset_frames_2;
            *(s32 *) &actor_pool[0x1990 + i * 0x44] = rpg_asset_animation_2;
            *(s32 *) &actor_pool[0x19B4 + i * 0x44] = rpg_asset_sheet_2;
            ((Actor *)actor_pool)[i + 96].unk_43 = 0x18;
        }
    }
    if (rpg_encounter & 0x80000000) {
        for (i = 40; i < 48; i++) {
            *(s32 *) &actor_pool[0x198C + i * 0x44] = rpg_asset_frames_1;
            *(s32 *) &actor_pool[0x1990 + i * 0x44] = rpg_asset_animation_1;
            *(s32 *) &actor_pool[0x19B4 + i * 0x44] = rpg_asset_sheet_1;
            ((Actor *)actor_pool)[i + 96].unk_43 = 0x12;
        }
    }
    for (i = 48; i < 50; i++) {
        *(s32 *) &actor_pool[0x198C + i * 0x44] = rpg_asset_frames_3;
        *(s32 *) &actor_pool[0x1990 + i * 0x44] = rpg_asset_animation_3;
        *(s32 *) &actor_pool[0x19B4 + i * 0x44] = rpg_asset_sheet_3;
        ((Actor *)actor_pool)[i + 96].unk_43 = 0x1E;
    }
    if (rpg_club_move_available == 1) {
        for (i = 50; i < 56; i++) {
            *(s32 *) &actor_pool[0x198C + i * 0x44] = rpg_asset_frames_4;
            *(s32 *) &actor_pool[0x1990 + i * 0x44] = rpg_asset_animation_4;
            *(s32 *) &actor_pool[0x19B4 + i * 0x44] = rpg_asset_sheet_4;
            ((Actor *)actor_pool)[i + 96].unk_43 = 0x1D;
        }
    }
    for (i = 56; i < 64; i++) {
        *(s32 *) &actor_pool[0x198C + i * 0x44] = rpg_asset_frames_1;
        *(s32 *) &actor_pool[0x1990 + i * 0x44] = rpg_asset_animation_1;
        *(s32 *) &actor_pool[0x19B4 + i * 0x44] = rpg_asset_sheet_1;
        if (rpg_encounter == 0x20000) {
            ((Actor *)actor_pool)[i + 96].unk_43 = 0x13;
        } else {
            ((Actor *)actor_pool)[i + 96].unk_43 = 0x12;
        }
    }
}

void rpg_stage_hud_setup(void) {
    D_80120DC1 = 1;
    rpg_actor_set_animation(0x1F, 0, 1, 1);
    rpg_actor_set_animation(0x26, 1, 1, 1);
    rpg_actor_set_animation(0x23, 4, 1, 0);
    D_80120AB6 = 0x6B;
    D_80120ABA = 0x3B;
    D_80120BC6 = 0x6B;
    D_80120BCA = 0x4B;
    D_80120CD6 = 0x6B;
    D_80120CDA = 0x5B;
    D_80120AFA = 0x74;
    D_80120AFE = 0x3B;
    D_80120C0A = 0x74;
    D_80120C0E = 0x4B;
    D_80120D1A = 0x74;
    D_80120D1E = 0x5B;
    D_80120B3E = 0x7D;
    D_80120B42 = 0x3B;
    D_80120C4E = 0x7D;
    D_80120C52 = 0x4B;
    D_80120D5E = 0x7D;
    D_80120D62 = 0x5B;
    D_80120B82 = 0x86;
    D_80120B86 = 0x3B;
    D_80120C92 = 0x86;
    D_80120C96 = 0x4B;
    D_80120DA2 = 0x86;
    D_80120DA6 = 0x5B;
    D_80120F3A = 0x58;
    D_80120F3E = 0x50;
    D_80120F2A = 8;
    D_80120EF6 = 0x58;
    D_80120EFA = 0x60;
    D_80120EE6 = 8;
}
