#include "common.h"
#include "ovl/DATE.h"

void func_80145AA0(void) {
    D_8015CE80 = 0x801B00DC;
    D_8015CE84 = 0x801B02AC;
    D_8015CE88 = 0x801B03C8;
    D_8015CE8C = 0x801B04C4;
    D_8015CE90 = 0x801B0668;
    D_8015CE94 = 0x801B0808;
    D_8015CE98 = 0x801B09DC;
    D_8015CE9C = 0x801B0BE8;
    D_8015CEA0 = 0x801B0D8C;
    D_8015CEA4 = 0x801B0F5C;
    D_8015CEA8 = 0x801B1164;
    D_8015CEAC = 0x801B12D8;
    D_8015CEB0 = 0x801B1454;
    D_8015CEB4 = 0x801B00F4;
    D_8015CEB8 = 0x801B02C8;
    D_8015CEBC = 0x801B03D0;
    D_8015CEC0 = 0x801B04DC;
    D_8015CEC4 = 0x801B0680;
    D_8015CEC8 = 0x801B0820;
    D_8015CECC = 0x801B09F8;
    D_8015CED0 = 0x801B0C00;
    D_8015CED4 = 0x801B0DA4;
    D_8015CED8 = 0x801B0F78;
    D_8015CEDC = 0x801B1180;
    D_8015CEE0 = 0x801B12E0;
    D_8015CEE4 = 0x801B1470;
    D_8015CEE8 = 0x801B01A8;
    D_8015CEEC = 0x801B0398;
    D_8015CEF0 = 0x801B03F0;
    D_8015CEF4 = 0x801B0590;
    D_8015CEF8 = 0x801B0734;
    D_8015CEFC = 0x801B08D4;
    D_8015CF00 = 0x801B0AE4;
    D_8015CF04 = 0x801B0CB4;
    D_8015CF08 = 0x801B0E58;
    D_8015CF0C = 0x801B1064;
    D_8015CF10 = 0x801B126C;
    D_8015CF14 = 0x801B1338;
    D_8015CF18 = 0x801B155C;
}

void func_80145D14(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl15 tbl;

    tbl = D_8015CF3C;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
    if ((D_80122D44 & 1) && (dec_bg_brightness == 0x80)) {
        sprite_light_effect_draw();
    }
}

void func_80145DB8(void) {
    cd_read_sectors(3, 0x801B0000, 0xAF43);
    func_80145AA0();
    mode_step_next();
}

void func_80145DF0(void) {
    u8 x;

    game_state.day_event_done_bits |= 8;
    x = get_g_zyotai_s(game_state.current_girl);
    if ((x & 0x7F) >= 2U) {
        mode_step_set(9);
        return;
    }
    game_state.day_event_done_bits |= 4;
    event_log_add(0xD, D_80122D08);
    mode_step_next();
}

void func_80145E74(void) {
    D_8015CF34 = 1;
    D_8015CF38 = 0;
    talk_names_set(game_state.current_girl);
    func_80145AA0();
    func_80145EC4();
    mode_step_next();
}

void func_80145EC4(void) {
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        D_8015CF1C = D_8015CE84;
        D_8015CF20 = D_8015CEB8;
        D_8015CF24 = D_8015CEEC;
        return;
    case GIRL_MIO:
        D_8015CF1C = D_8015CE98;
        D_8015CF20 = D_8015CECC;
        D_8015CF24 = D_8015CF00;
        return;
    case GIRL_YUINA:
        D_8015CF1C = D_8015CE8C;
        D_8015CF20 = D_8015CEC0;
        D_8015CF24 = D_8015CEF4;
        return;
    case GIRL_AYAKO:
        D_8015CF1C = D_8015CE90;
        D_8015CF20 = D_8015CEC4;
        D_8015CF24 = D_8015CEF8;
        return;
    case GIRL_SAKI:
        D_8015CF1C = D_8015CEA8;
        D_8015CF20 = D_8015CEDC;
        D_8015CF24 = D_8015CF10;
        return;
    case GIRL_YUKARI:
        D_8015CF1C = D_8015CE9C;
        D_8015CF20 = D_8015CED0;
        D_8015CF24 = D_8015CF04;
        return;
    case GIRL_NOZOMI:
        D_8015CF1C = D_8015CEA0;
        D_8015CF20 = D_8015CED4;
        D_8015CF24 = D_8015CF08;
        return;
    case GIRL_MIRA:
        D_8015CF1C = D_8015CE94;
        D_8015CF20 = D_8015CEC8;
        D_8015CF24 = D_8015CEFC;
        return;
    case GIRL_YUKO:
        D_8015CF1C = D_8015CE80;
        D_8015CF20 = D_8015CEB4;
        D_8015CF24 = D_8015CEE8;
        return;
    case GIRL_MEGUMI:
        D_8015CF1C = D_8015CEA4;
        D_8015CF20 = D_8015CED8;
        D_8015CF24 = D_8015CF0C;
        return;
    case GIRL_YUMI:
        D_8015CF1C = D_8015CEB0;
        D_8015CF20 = D_8015CEE4;
        D_8015CF24 = D_8015CF18;
        /* fallthrough */
    default:
        return;
    }
}

void func_80146134(void) {
    talk_tables = &D_8015CF34;
    D_800CA138 = &D_8015CF38;
    D_800CA13C = D_8015CF1C;
    D_800CA140 = D_8015CF20;
    D_800CA144 = D_8015CF24;
    talk_run(D_80122CDC, 1, 0);
}
