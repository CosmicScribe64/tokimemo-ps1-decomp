#define MAIN_API_OVERRIDE_get_h_tokimeki /* u8 return narrows the metric byte on return (T-24122). */
#define MAIN_API_OVERRIDE_get_h_yuukou   /* u8 return narrows the metric byte on return (T-24122). */
#include "common.h"
#include "ovl/SHUGAKU.h"

u8 get_h_tokimeki(s32 arg0);
u8 get_h_yuukou(s32 arg0);

void func_80133430(void) {
    D_8013BE70 = 0x801B00DC;
    D_8013BE74 = 0x801B02AC;
    D_8013BE78 = 0x801B03C8;
    D_8013BE7C = 0x801B04C4;
    D_8013BE80 = 0x801B0668;
    D_8013BE84 = 0x801B0808;
    D_8013BE88 = 0x801B09DC;
    D_8013BE8C = 0x801B0BE8;
    D_8013BE90 = 0x801B0D8C;
    D_8013BE94 = 0x801B0F5C;
    D_8013BE98 = 0x801B1164;
    D_8013BE9C = 0x801B12D8;
    D_8013BEA0 = 0x801B1454;
    D_8013BEA4 = 0x801B00F4;
    D_8013BEA8 = 0x801B02C8;
    D_8013BEAC = 0x801B03D0;
    D_8013BEB0 = 0x801B04DC;
    D_8013BEB4 = 0x801B0680;
    D_8013BEB8 = 0x801B0820;
    D_8013BEBC = 0x801B09F8;
    D_8013BEC0 = 0x801B0C00;
    D_8013BEC4 = 0x801B0DA4;
    D_8013BEC8 = 0x801B0F78;
    D_8013BECC = 0x801B1180;
    D_8013BED0 = 0x801B12E0;
    D_8013BED4 = 0x801B1470;
    D_8013BED8 = 0x801B01A8;
    D_8013BEDC = 0x801B0398;
    D_8013BEE0 = 0x801B03F0;
    D_8013BEE4 = 0x801B0590;
    D_8013BEE8 = 0x801B0734;
    D_8013BEEC = 0x801B08D4;
    D_8013BEF0 = 0x801B0AE4;
    D_8013BEF4 = 0x801B0CB4;
    D_8013BEF8 = 0x801B0E58;
    D_8013BEFC = 0x801B1064;
    D_8013BF00 = 0x801B126C;
    D_8013BF04 = 0x801B1338;
    D_8013BF08 = 0x801B155C;
}

void func_801336A4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    ShFnTbl43 tbl;

    tbl = D_8013BF20;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void func_80133720(void) {
    cd_read_sectors(3, 0x801B0000, 0xAF43);
    func_80133430();
    mode_step_next();
}

void func_80133758(void) {
    talk_names_set(game_state.current_girl);
    func_80133430();
    func_80133A68();
    if (*(u8 *)&D_800CA2F0 != 2) {
        if (dec_bg_brightness != 0) {
            D_800CA2F0 = 1;
        } else {
            D_800CA2F0 = 0;
        }
    }
    D_8013BF18 = 0;
    D_8013BF1C = 0;
    mode_step_next();
    if (*(u8 *)&D_800CA2F0 == 2) {
        game_state.mode_step += 2;
    }
}

s32 func_801337FC(void) {
    s16 i;
    u8 best_i;
    u8 best_s;
    u8 s;

    best_i = 0xFF;
    best_s = 4;
    for (i = 0; i < 10; i++) {
        /* FAKE: dead branch adjusts the loop basic-block count so uopt assigns best_s to $s5 (T-24122). */
        if (1) {}
        if (D_800CA2B0[i] != 0) {
            s = get_g_zyotai_s(i) & 0x7F;
            /* FAKE: duplicate `s == best_s` shifts uopt's live-range priority between s and best_s (T-24122). */
            if (s < best_s ||
                (s == best_s && s == best_s &&
                 get_h_tokimeki(i) + get_h_yuukou(i) >
                     get_h_tokimeki(best_i) + get_h_yuukou(best_i))) {
                best_i = i;
                best_s = s;
            }
        }
    }
    s = get_g_zyotai_s(best_i) & 0x7F;
    if (best_i == 4 && s < 2U && game_state.saved_mode_stage == 5) {
        if (game_state.scene_flags[0x20] != 0) {
            game_state.day_event_done_bits |= 8;
        }
    }
    if (best_i != 0xFF && s < 2U && !(game_state.day_event_done_bits & 8)) {
        /* FAKE: `(best_i & 0) +` keeps cfe's CVT J L narrowing so uopt emits `andi $a0, $s7, 0xff` (T-24122). */
        game_state.current_girl = (best_i & 0) + best_i;
        talk_names_set(game_state.current_girl);
        func_80133A68();
        if (game_state.saved_game_mode != 6) {
            event_log_add(0xD, 0x26);
        } else {
            switch (D_800CA2CC) {
            case 0:
                event_log_add(0xD, 0x27);
                break;
            case 1:
                event_log_add(0xD, 0x28);
                break;
            default:
                event_log_add(0xD, 0x29);
                break;
            }
        }
    } else {
        D_8013BF0C = D_8013BE9C;
        D_8013BF10 = D_8013BED0;
        D_8013BF14 = D_8013BF04;
        D_8013BF18 = 0;
        D_8013BF1C = 0;
        game_state.mode_step += 0x20;
        return 0;
    }
    mode_step_next();
}

void func_80133A68(void) {
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        D_8013BF0C = D_8013BE74;
        D_8013BF10 = D_8013BEA8;
        D_8013BF14 = D_8013BEDC;
        return;
    case GIRL_MIO:
        D_8013BF0C = D_8013BE88;
        D_8013BF10 = D_8013BEBC;
        D_8013BF14 = D_8013BEF0;
        return;
    case GIRL_YUINA:
        D_8013BF0C = D_8013BE7C;
        D_8013BF10 = D_8013BEB0;
        D_8013BF14 = D_8013BEE4;
        return;
    case GIRL_AYAKO:
        D_8013BF0C = D_8013BE80;
        D_8013BF10 = D_8013BEB4;
        D_8013BF14 = D_8013BEE8;
        return;
    case GIRL_SAKI:
        D_8013BF0C = D_8013BE98;
        D_8013BF10 = D_8013BECC;
        D_8013BF14 = D_8013BF00;
        return;
    case GIRL_YUKARI:
        D_8013BF0C = D_8013BE8C;
        D_8013BF10 = D_8013BEC0;
        D_8013BF14 = D_8013BEF4;
        return;
    case GIRL_NOZOMI:
        D_8013BF0C = D_8013BE90;
        D_8013BF10 = D_8013BEC4;
        D_8013BF14 = D_8013BEF8;
        return;
    case GIRL_MIRA:
        D_8013BF0C = D_8013BE84;
        D_8013BF10 = D_8013BEB8;
        D_8013BF14 = D_8013BEEC;
        return;
    case GIRL_YUKO:
        D_8013BF0C = D_8013BE70;
        D_8013BF10 = D_8013BEA4;
        D_8013BF14 = D_8013BED8;
        return;
    case GIRL_MEGUMI:
        D_8013BF0C = D_8013BE94;
        D_8013BF10 = D_8013BEC8;
        D_8013BF14 = D_8013BEFC;
        return;
    case GIRL_YUMI:
        D_8013BF0C = D_8013BEA0;
        D_8013BF10 = D_8013BED4;
        D_8013BF14 = D_8013BF08;
        /* fallthrough */
    default:
        return;
    }
}

void func_80133CD8(void) {
    talk_tables = (u8 *) &D_8013BF18;
    D_800CA138 = (u8 *) &D_8013BF1C;
    D_800CA13C = D_8013BF0C;
    D_800CA140 = D_8013BF10;
    D_800CA144 = D_8013BF14;
    talk_run(D_80122CDC, 1, 0);
}
