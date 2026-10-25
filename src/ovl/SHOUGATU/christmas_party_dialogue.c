/* christmas_party_dialogue.c: ; src/ovl/SHOUGATU/8013DD90.c:51-56 increments Rei story stage and sets the party-room location. Original object 0x8013DD90-0x8013E330 (config/objects/SHOUGATU.txt). */
#include "common.h"
#include "ovl/SHOUGATU.h"

typedef struct {
    u32 pad : 1;
    u32 f : 1;
    u32 rest : 30;
} Bit1Flag;

void christmas_party_resource_addresses_init(void) {
    D_80145FD0 = 0x801A6558;
    D_80145FD4 = 0x801A655C;
    D_80145FD8 = 0x801A6580;
    D_80145FE0 = 0x801A0000;
}

s32 christmas_party_actor_update(void) {
    s32 index;
    ShoCallbacks23 table;

    table = D_80145FE8;
    index = game_state.mode_step;
    table.f[index](0x80);
    ((Actor *)actor_pool_96)->brightness = bg_brightness;
    if (((Actor *)actor_pool_96)->unk_18 == 0 && ((Actor *)actor_pool_96)->unk_08 == 0) {
        ((Actor *)actor_pool_96)->sequence_slot++;
        if (((Actor *)actor_pool_96)->sequence_slot >= 3) {
            ((Actor *)actor_pool_96)->sequence_slot = 0;
        }
    }
}

void christmas_party_audio_start(void) {
    audio_command(0x204);
    mode_step_next();
}

void christmas_party_display_flag_set(void) {
    D_80120653 |= 0x80;
    mode_step_next();
}

void christmas_party_story_progress_init(void) {
    /* FAKE: pointer alias for increment/compare paired with direct global store in the if-body so uopt shares $v0 for lw/lbu/sb at +0x468/+0x469 without forwarding the byte into the clamp store. T-24088 */
    GameState *state;

    state = &game_state;
    event_log_add(4, 0);
    state->girls[12].flags.f.story_stage += 1;
    if (state->girls[12].flags.f.story_stage >= 4) {
        game_state.girls[12].flags.f.story_stage = 3;
    }
    strcpy(name_code_girl_short, "伊集院");
    strcpy(girl_name_buf, "伊集院");
    strcpy(scene_place_title_2, "パーティー会場");
    D_80145F2C = 9;
    mode_step_next();
    christmas_party_resource_addresses_init();
    load_palette(D_80145FE0, 0x11, 1, 3, 0);
    actor_clear(0x60);
    D_80120651 = 0xC;
    D_80120652 = 1;
    D_80120688 = 0x41000000;
    D_80120653 = 4;
    D_8012065C = (u8 *)D_80145FD4;
    D_80120660 = (u8 *)D_80145FD8;
    D_80120684 = (u8 *)D_80145FD0;
    D_80120664 = D_80145FDC;
    D_80120666 = 0;
    D_80120668 = 0;
    D_80120693 = 0x11;
    D_80120676 = -0xA0;
    D_8012067A = -0x78;
    D_80120657 = 0;
    D_80120655 = 0;
    D_80120654 = 8;
}

void christmas_party_background_read(void) {
    bg_read_sub2(0x41D4);
    mode_step_next();
}

void christmas_party_story_progress_selector_update(void) {
    D_80145F2C += game_state.girls[12].flags.f.story_stage * 3 - 3;
    mode_step_next();
}

void christmas_party_portrait_read(void) {
    portrait_read_start(0x5855);
    mode_step_next();
}

void christmas_party_scene_resource_read(void) {
    cd_read_sectors(0xD, 0x801A0000, 0xBD9F);
    christmas_party_resource_addresses_init();
    mode_step_next();
}

void christmas_party_candidate_select(void) {
    s32 r;
    u8 a;
    u8 pad; /* FAKE: unused byte between a and b, real source unknown (frame layout) */
    u8 b;

    a = get_g_zyotai_s(GIRL_MIRA);
    r = get_g_zyotai_h(GIRL_MIRA);
    b = 0xFF;
    if (((Bit1Flag *)&game_state.girls[7].flags.w)->f && game_state.scene_flags[52] == 0 && a == 0x80 && r == 0) {
        b = 7;
    }
    a = get_g_zyotai_s(GIRL_SHIORI);
    r = get_g_zyotai_h(GIRL_SHIORI);
    if (game_state.scene_flags[6] == 0 && a == 0x80 && r == 0 && (b == 0xFF || (b == 7 && game_state.girls[0].syoushin < game_state.girls[7].syoushin))) {
        b = 0;
    }
    a = get_g_zyotai_s(GIRL_MEGUMI);
    r = get_g_zyotai_h(GIRL_MEGUMI);
    if (((Bit1Flag *)&game_state.girls[9].flags.w)->f && game_state.scene_flags[66] == 0 && a == 0x80 && r == 0 &&
        (b == 0xFF || (b == 7 && game_state.girls[9].syoushin < game_state.girls[7].syoushin) || (b == 0 && game_state.girls[9].syoushin < game_state.girls[0].syoushin))) {
        b = 9;
    }
    D_80145FE4 = b;
    mode_step_next();
}
