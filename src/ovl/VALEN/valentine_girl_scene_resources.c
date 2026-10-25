/* valentine_girl_scene_resources.c: ; src/ovl/VALEN/80133C70.c:3-38 initializes girl-specific scene resource addresses. Original object 0x80133C70-0x80134320 (config/objects/VALEN.txt). */
#include "ovl/VALEN.h"

typedef struct {
    void (*f[12])();
} FnTbl12; /* size 0x30 */
extern FnTbl12 D_801346FC;

void valentine_birthday_pointer_setup(void) {
    D_80134660 = 0x801B00DC;
    D_80134664 = 0x801B02AC;
    D_80134668 = 0x801B03C8;
    D_8013466C = 0x801B04C4;
    D_80134670 = 0x801B0668;
    D_80134674 = 0x801B0808;
    D_80134678 = 0x801B09DC;
    D_8013467C = 0x801B0BE8;
    D_80134680 = 0x801B0D8C;
    D_80134684 = 0x801B0F5C;
    D_80134688 = 0x801B1164;
    D_8013468C = 0x801B12D8;
    D_80134690 = 0x801B1454;
    D_80134694 = 0x801B00F4;
    D_80134698 = 0x801B02C8;
    D_8013469C = 0x801B03D0;
    D_801346A0 = 0x801B04DC;
    D_801346A4 = 0x801B0680;
    D_801346A8 = 0x801B0820;
    D_801346AC = 0x801B09F8;
    D_801346B0 = 0x801B0C00;
    D_801346B4 = 0x801B0DA4;
    D_801346B8 = 0x801B0F78;
    D_801346BC = 0x801B1180;
    D_801346C0 = 0x801B12E0;
    D_801346C4 = 0x801B1470;
    D_801346C8 = 0x801B01A8;
    D_801346CC = 0x801B0398;
    D_801346D0 = 0x801B03F0;
    D_801346D4 = 0x801B0590;
    D_801346D8 = 0x801B0734;
    D_801346DC = 0x801B08D4;
    D_801346E0 = 0x801B0AE4;
    D_801346E4 = 0x801B0CB4;
    D_801346E8 = 0x801B0E58;
    D_801346EC = 0x801B1064;
    D_801346F0 = 0x801B126C;
    D_801346F4 = 0x801B1338;
    D_801346F8 = 0x801B155C;
}

void valentine_birthday_callback_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl12 tbl;

    tbl = D_801346FC;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void valentine_birthday_girl_entry(void) {
    if (game_state.current_girl == GIRL_MEGUMI) {
        mode_step_next();
        return;
    }
    normal_date_girl_in();
}

void valentine_birthday_sector_load(void) {
    cd_read_sectors(3, 0x801B0000, 0xAF43);
    valentine_birthday_pointer_setup();
    mode_step_next();
}

void valentine_birthday_scene_setup(void) {
    D_8013448C = D_80134400;
    D_80134490 = D_80134434;
    D_80134494 = D_80134468;
    D_80134498 = 0xC;
    D_8013449C = 0;
    mode_step_next();
}

/* T-24211, T-24900: matches with the default arm's three globals declared volatile in VALEN.h
 * (FAKE there); otherwise as1 hoists the default block's first two lui. */
void valentine_birthday_event_mark(void) {
    valentine_birthday_pointer_setup();
    /* two statements: the single expression (x & 0xFFF7) | 4 takes $t6/$t7/$t8 where the original has $v0, $t7, $t9 */
    game_state.day_event_done_bits &= 0xFFF7;
    game_state.day_event_done_bits |= 4;
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        D_8013448C = D_80134664;
        D_80134490 = D_80134698;
        D_80134494 = D_801346CC;
        break;
    case GIRL_MIO:
        D_8013448C = D_80134678;
        D_80134490 = D_801346AC;
        D_80134494 = D_801346E0;
        break;
    case GIRL_YUINA:
        D_8013448C = D_8013466C;
        D_80134490 = D_801346A0;
        D_80134494 = D_801346D4;
        break;
    case GIRL_AYAKO:
        D_8013448C = D_80134670;
        D_80134490 = D_801346A4;
        D_80134494 = D_801346D8;
        break;
    case GIRL_SAKI:
        D_8013448C = D_80134688;
        D_80134490 = D_801346BC;
        D_80134494 = D_801346F0;
        break;
    case GIRL_YUKARI:
        D_8013448C = D_8013467C;
        D_80134490 = D_801346B0;
        D_80134494 = D_801346E4;
        break;
    case GIRL_NOZOMI:
        D_8013448C = D_80134680;
        D_80134490 = D_801346B4;
        D_80134494 = D_801346E8;
        break;
    case GIRL_MIRA:
        D_8013448C = D_80134674;
        D_80134490 = D_801346A8;
        D_80134494 = D_801346DC;
        break;
    case GIRL_YUKO:
        D_8013448C = D_80134660;
        D_80134490 = D_80134694;
        D_80134494 = D_801346C8;
        break;
    case GIRL_MEGUMI:
        D_8013448C = D_80134684;
        D_80134490 = D_801346B8;
        D_80134494 = D_801346EC;
        break;
    default:
        D_8013448C = D_80134690;
        D_80134490 = D_801346C4;
        D_80134494 = D_801346F8;
        break;
    }
    D_80134498 = 1;
    D_8013449C = 0;
    mode_step_next();
}

void valentine_saved_state_restore(void) {
    mode_stage_set(game_state.saved_mode_stage);
    mode_step_set(game_state.saved_mode_step);
}
