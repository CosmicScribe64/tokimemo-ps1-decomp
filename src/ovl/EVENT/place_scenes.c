/* place_scenes.c: src/ovl/EVENT/8010FE10.c:5: 21 place arrival steps, the shared date steps (special event choice, outcome factors, talk and close steps), effects. Original object 0x8010FE10-0x8011C650 (config/objects/EVENT.txt). */
#define MAIN_API_OVERRIDE_func_8004A8EC /* matched with s32(s32) (main_api.h: void(u8)); the s32 return fixes spot_result_talk_step registers, T-22024 */
#include "common.h"
#include "ovl/EVENT.h"

s32 func_8004A8EC(s32 arg0);

void spot_place_result_pointers_init(void) {
    spot_place_result_tables = 0x801A427C;
    D_801251D4 = 0x801A4D2C;
    D_801251D8 = 0x801A43EC;
    D_801251DC = 0x801A4E9C;
    D_801251E0 = 0x801A47F0;
    D_801251E4 = 0x801A52A0;
    D_801251E8 = 0x801A4190;
    D_801251EC = 0x801A4C40;
    D_801251F0 = 0x801A4A04;
    D_801251F4 = 0x801A54B4;
    D_801251F8 = 0x801A4368;
    D_801251FC = 0x801A4E18;
    D_80125200 = 0x801A4430;
    D_80125204 = 0x801A4EE0;
    D_80125208 = 0x801A4684;
    D_8012520C = 0x801A5134;
    D_80125210 = 0x801A497C;
    D_80125214 = 0x801A542C;
    D_80125218 = 0x801A4238;
    D_8012521C = 0x801A4CE8;
    D_80125220 = 0x801A49C0;
    D_80125224 = 0x801A5470;
    D_80125228 = 0x801A41F4;
    D_8012522C = 0x801A4CA4;
    D_80125230 = 0x801A4064;
    D_80125234 = 0x801A4B14;
    D_80125238 = 0x801A4A28;
    D_8012523C = 0x801A54D8;
    D_80125240 = 0x801A4A6C;
    D_80125244 = 0x801A551C;
    D_80125248 = 0x801A4600;
    D_8012524C = 0x801A50B0;
    D_80125250 = 0x801A412C;
    D_80125254 = 0x801A4BDC;
    D_80125258 = 0x801A4324;
    D_8012525C = 0x801A4DD4;
    D_80125260 = 0x801A4474;
    D_80125264 = 0x801A4F24;
    D_80125268 = 0x801A4000;
    D_8012526C = 0x801A4AB0;
}

void spot_place_asset_pointers_init(void) {
    spot_place_asset_pointers = 0x801E2820;
    D_80125274 = 0x801E2830;
    D_80125278 = 0x801E28B4;
    D_80125280 = 0x801A0000;
    D_80125284 = 0x801BA000;
    D_80125288 = 0x801D2200;
    D_8012528C = 0x801A2000;
    D_80125290 = 0x801A6000;
    D_80125294 = 0x801AA000;
    D_80125298 = 0x801AE000;
    D_8012529C = 0x801B2000;
    D_801252A0 = 0x801B6000;
    D_801252A4 = 0x801BA200;
    D_801252A8 = 0x801BE200;
    D_801252AC = 0x801C2200;
    D_801252B0 = 0x801C6200;
    D_801252B4 = 0x801CA200;
    D_801252B8 = 0x801CE200;
}

void spot_place_frame_pointers_init(void) {
    spot_place_frame_pointers = 0x801F0638;
    D_801252C0 = 0x801F0C34;
    D_801252C4 = 0x801F063C;
    D_801252C8 = 0x801F0C3C;
    D_801252CC = 0x801F071C;
    D_801252D0 = 0x801F0CEC;
    D_801252DC = 0x801E0000;
    D_801252E0 = 0x801E2000;
}

void spot_place_intro_step(void) {
    u8 v;

    func_80017E50();
    D_80094730 += spot_place_variant * (u8)D_801252E8;
    D_80094734 = 0;
    v = func_8002328C(D_800B1746);
    spot_dialogue_group_offset = (((v & 0x7F) >= 2U) + ((v & 0x7F) >= 3U)) * (((*(s8 *)&spot_arrival_mode) == 0) + 1) + spot_arrival_mode * 6 - ((*(s8 *)&spot_arrival_mode) == 2) * 3;
    D_80124348 = 0;
    *(s16 *)&D_80124358 = 0;
    *(s16 *)&D_80124360 = 0;
    D_8012434C = 0;
    D_80124350 = 0;
    D_8012437C = 0;
    D_80124380 = 0;
    D_80124370 = 0;
    D_80124378 = 0;
    *(s16 *)&D_80124364 = 0;
    *(s16 *)&D_80124368 = 0;
    if (((s16)D_800B0898 == 6) && (D_800B0897 < 8)) {
        D_80125328 = 1;
    } else {
        D_80125328 = 0;
    }
    func_80017E50();
    func_80045B14();
    D_80094868 = 0;
    func_80011DFC();
}

/* Matched: T-24023.
 * FAKE: spot_reaction_low_cells + 0xC is spot_place. The containing aggregate is not
 * recovered; this byte view keeps the conversion pass from reversing selector
 * comparisons. The local copy in each dispatch arm also preserves their order.
 * Linked bytes and the complete EVENT binary verify the address and load width. */
s32 spot_date_event_choose(void) {
    SpotDevelopmentState *g;
    u8 sp2B;
    /* FAKE: keep the observed byte gap between saved masks. Capturing four
     * comparison constants here preserves branch operand order. T-24023. */
    u8 date_ok;
    u8 sp29;
    u32 temp_v0;
    s32 temp_v0_2;

    g = (SpotDevelopmentState *)&D_800B0860;
    temp_v0 = func_8002328C((s32)g->current_girl);
    sp29 = temp_v0 & 0x80;
    sp2B = temp_v0 & 0x7F;
    temp_v0_2 = func_8002336C((s32)g->current_girl);
    if (g->current_girl == 0) {
        temp_v0 = (((u8 *)&spot_reaction_low_cells)[0xC]);
        if ((temp_v0 == 1) && ((u8) g->month >= 3U) && ((u8) g->month < 6U) && (g->scene_flags[0x0] == 0) && (g->step == 0xD)) {
            D_800EECB0 = 4;
            func_80011E28(0x60);
            g->scene_flags[0x0] = 1;
            return 0;
        }
        if ((temp_v0 == 0x10) && ((u8) g->month >= 6U) && ((u8) g->month < 9U) && (sp2B < 3U) && ((u32) temp_v0_2 < 2U) && (g->scene_flags[0x1] == 0) && (g->girls[g->current_girl].pad2E[2] != 0) && (g->stage == 1) && (g->step == 0xA)) {
            D_800EECB0 = 6;
            func_80011E28(0x60);
            g->scene_flags[0x1] = 1;
            return 0;
        }
        if ((temp_v0 == 0) && ((u8) g->month >= 9U) && ((u8) g->month < 0xCU)) {
            if (sp2B < 2U) {
                if (temp_v0_2 == 0) {
                    if ((g->scene_flags[0x2] == 0) && (g->stage == 1)) {
                        if (g->step == 1) {
                            D_800EECB0 = 3;
                            func_80011E28(0x60);
                            g->scene_flags[0x2] = 1;
                            return 0;
                        }

                    }

                }

            }

        }
        if ((temp_v0 == 0xC) && ((g->month == 0xC) || (g->month == 1) || (g->month == 2)) && (sp2B == 0) && (temp_v0_2 == 0) && (sp29 != 0) && (g->scene_flags[0x3] == 0) && (D_800EECBC == 2) && (g->stage == 1) && (g->step == 0x23)) {
            D_800EECB0 = 5;
            func_80011E28(0x60);
            g->scene_flags[0x3] = 1;
            return 0;
        }
    } else if (g->current_girl == 1) {
        temp_v0 = (((u8 *)&spot_reaction_low_cells)[0xC]);
        if ((temp_v0 == 0xC) && ((u8) g->month >= 3U) && ((u8) g->month < 6U) && ((u8) g->scene_flags[0x7] == 0) && (D_800EECBC == 0) && (D_80125304 != 0) && (g->stage == 1) && (g->step == 0x23)) {
            D_800EECB0 = 2;
            func_80011E28(0x60);
            return 0;
        }
        if ((temp_v0 == 0x11) && ((u8) g->month >= 6U) && ((u8) g->month < 9U) && (g->scene_flags[0x8] == 0)) {
            if (sp2B < 2U) {
                if ((g->stage == 1) && (g->step == 0xA)) {
                    D_800EECB0 = 3;
                    func_80011E28(0x60);
                    g->scene_flags[0x8] = 1;
                    return 0;
                }

            }

        }
        if ((temp_v0 == 8) && ((u8) g->month >= 9U) && ((u8) g->month < 0xCU)) {
            if (g->scene_flags[0x9] != 0) {
                if (g->scene_flags[0x9] == 1) {
                    if (!((GirlFlagBits *)&D_800B1AE4)->b0) {
                        goto block_64;
                    }
                    goto block_68;
                }
                goto block_67;
            }
block_64:
            if ((g->stage == 1) && (g->step == 0xA)) {
                D_800EECB0 = 4;
                func_80011E28(0x60);
                g->scene_flags[0x9] = 1;
                return 0;
            }

        }
block_67:
block_68:
        if ((temp_v0 == 0xE) && ((g->month == 0xC) || (g->month == 1) || (g->month == 2)) && (g->scene_flags[0xA] == 0) && (D_80125314 == 6) && (g->stage == 1) && (g->step == 0x11)) {
            D_800EECB0 = 5;
            func_80011E28(0x60);
            g->scene_flags[0xA] = 1;
            return 0;
        }
    } else if (g->current_girl == 2) {
        temp_v0 = (((u8 *)&spot_reaction_low_cells)[0xC]);
        if ((temp_v0 == 4) && ((u8) g->month >= 3U) && ((u8) g->month < 6U) && (g->scene_flags[0xD] == 0) && ((u8) g->year >= 0x60U) && (g->stage == 1) && (g->step == 9)) {
            D_800EECB0 = 2;
            func_80011E28(0x60);
            g->scene_flags[0xD] = 1;
            return 0;
        }
        if ((temp_v0 == 0xA) && ((u8) g->month >= 6U) && ((u8) g->month < 9U) && (g->scene_flags[0xE] == 0) && (g->year == 0x61) && (g->stage == 1) && (g->step == 9)) {
            D_800EECB0 = 3;
            func_80011E28(0x60);
            g->scene_flags[0xE] = 1;
            return 0;
        }
        if ((temp_v0 == 9) && ((u8) g->month >= 9U) && ((u8) g->month < 0xCU) && (g->scene_flags[0xF] == 0) && ((u8) g->year >= 0x60U) && (D_80125300 != 0) && (D_800EECBC == 1) && (g->stage == 1) && (g->step == 0xD)) {
            D_800EECB0 = 4;
            func_80011E28(0x60);
            g->scene_flags[0xF] = 1;
            return 0;
        }
        if ((temp_v0 == 2) && ((g->month == 0xC) || (g->month == 1) || (g->month == 2)) && (D_80125324 == (date_ok = 2)) && (g->stage == 1) && (g->step == 0xE)) {
            D_800EECB0 = 5;
            func_80011E28(0x60);
            g->scene_flags[0x10] = 1;
            return 0;
        }
    } else if (g->current_girl == 3) {
        temp_v0 = (((u8 *)&spot_reaction_low_cells)[0xC]);
        if ((temp_v0 == 0xB) && ((u8) g->month >= 3U) && ((u8) g->month < 6U) && (g->scene_flags[0x13] == 0) && (g->year == 0x61) && (g->stage == 1) && (g->step == 9)) {
            D_800EECB0 = 2;
            func_80011E28(0x60);
            g->scene_flags[0x13] = 1;
            return 0;
        }
        if ((temp_v0 == 0x11) && ((u8) g->month >= 6U) && ((u8) g->month < 9U)) {
            if (g->scene_flags[0x14] == 0) {
                if ((g->stage == 1) && (g->step == 0xA)) {
                    D_800EECB0 = 3;
                    func_80011E28(0x60);
                    g->scene_flags[0x14] = 1;
                    return 0;
                }

            }

        }
        if ((temp_v0 == 7) && ((u8) g->month >= 9U) && ((u8) g->month < 0xCU) && (g->scene_flags[0x15] == 0)) {
            if ((D_801252F4 == (date_ok = 1)) && (g->stage == 1)) {
                if (g->step == 7) {
                    D_800EECB0 = 4;
                    func_80011E28(0x60);
                    g->scene_flags[0x15] = 1;
                    return 0;
                }

            }

        }
        if ((temp_v0 == 6) && ((g->month == 0xC) || (g->month == 1) || (g->month == 2)) && (sp2B < 2U) && (temp_v0_2 == 0) && (g->scene_flags[0x16] == 0) && ((u8) g->year >= 0x61U) && (g->stage == 1) && (g->step == 0xB)) {
            D_800EECB0 = 5;
            func_80011E28(0x60);
            g->scene_flags[0x16] = 1;
            return 0;
        }
    } else if (g->current_girl == 4) {
        temp_v0 = (((u8 *)&spot_reaction_low_cells)[0xC]);
        if ((temp_v0 == 1) && ((u8) g->month >= 3U) && ((u8) g->month < 6U) && (sp2B == 0) && (sp29 != 0) && (temp_v0_2 == 0) && (g->scene_flags[0x19] == 0) && (g->stage == 1) && (g->step == 0xD)) {
            D_800EECB0 = 2;
            func_80011E28(0x60);
            g->scene_flags[0x19] = 1;
            return 0;
        }
        if ((temp_v0 == 0x10) && ((u8) g->month >= 6U)) {
            if (((u8) g->month < 9U) && (sp2B == 0) && (sp29 != 0)) {
                if (temp_v0_2 == 0) {
                    if (g->scene_flags[0x1A] == 0) {
                        if ((g->stage == 1) && (g->step == 0xA)) {
                            D_800EECB0 = 3;
                            func_80011E28(0x60);
                            g->scene_flags[0x1A] = 1;
                            return 0;
                        }

                    }

                }

            }

        }
        if ((temp_v0 == 0xD) && ((u8) D_8012532C == 0) && ((u8) g->month >= 9U) && ((u8) g->month < 0xCU) && (g->scene_flags[0x1B] == 0) && ((u8) spot_place_variant < 2U) && (g->stage == 1) && (g->step == 8)) {
            D_800EECB0 = 4;
            func_80011E28(0x60);
            g->scene_flags[0x1B] = 1;
            return 0;
        }
        if ((temp_v0 == 2) && ((g->month == 0xC) || (g->month == 1) || (g->month == 2)) && (g->scene_flags[0x1C] == 0) && (D_800EECBC == 1) && (g->stage == 1) && (g->step == 0xE)) {
            D_800EECB0 = 5;
            func_80011E28(0x60);
            g->scene_flags[0x1C] = 1;
            return 0;
        }
    } else if (g->current_girl == 5) {
        temp_v0 = (((u8 *)&spot_reaction_low_cells)[0xC]);
        if ((temp_v0 == 5) && ((u8) g->month >= 3U) && ((u8) g->month < 6U) && (g->scene_flags[0x20] == 0) && (g->year == 0x61) && (g->stage == 1) && (g->step == 9)) {
            D_800EECB0 = 3;
            func_80011E28(0x60);
            g->scene_flags[0x20] = 1;
            return 0;
        }
        if ((temp_v0 == 3) && ((u8) g->month >= 6U) && ((u8) g->month < 9U) && (sp2B < 3U) && ((u32) temp_v0_2 < 2U) && (g->scene_flags[0x21] == 0) && ((u8) g->year >= 0x61U) && (g->stage == 1) && (g->step == 9)) {
            D_800EECB0 = 4;
            func_80011E28(0x60);
            g->scene_flags[0x21] = 1;
            return 0;
        }
        if ((temp_v0 == 1) && ((u8) g->month >= 9U) && ((u8) g->month < 0xCU)) {
            if ((g->scene_flags[0x22] == 0) && (sp2B == 0) && (sp29 != 0) && (temp_v0_2 == 0)) {
                if ((g->stage == 1) && (g->step == 0xD)) {
                    D_800EECB0 = 5;
                    func_80011E28(0x60);
                    g->scene_flags[0x22] = 1;
                    return 0;
                }

            }

        }
        if ((temp_v0 == 0x14) && ((g->month == 0xC) || (g->month == 1) || (g->month == 2)) && (g->scene_flags[0x23] == 0) && (g->stage == 1) && (g->step == 0)) {
            D_800EECB0 = 6;
            func_80011E28(0x60);
            g->scene_flags[0x23] = 1;
            return 0;
        }
    } else if (g->current_girl == 6) {
        temp_v0 = (((u8 *)&spot_reaction_low_cells)[0xC]);
        if ((temp_v0 == 7) && ((u8) g->month >= 3U) && ((u8) g->month < 6U) && (g->scene_flags[0x26] == 0) && (D_801252F8 != 0) && (g->stage == 1) && (g->step == 7)) {
            D_800EECB0 = 2;
            func_80011E28(0x60);
            g->scene_flags[0x26] = 1;
            return 0;
        }
        if ((temp_v0 == 0x11) && ((u8) g->month >= 6U) && ((u8) g->month < 9U) && (sp2B == 0) && (sp29 != 0) && (temp_v0_2 == 0) && (g->scene_flags[0x27] == 0) && (g->athletics >= 0x64) && (g->stage == 1) && (g->step == 0xA)) {
            D_800EECB0 = 3;
            func_80011E28(0x60);
            g->scene_flags[0x27] = 1;
            return 0;
        }
        if ((temp_v0 == 5) && ((u8) g->month >= 9U) && ((u8) g->month < 0xCU)) {
            if (g->scene_flags[0x28] == 0) {
                if ((g->stage == 1) && (g->step == 9)) {
                    D_800EECB0 = 4;
                    func_80011E28(0x60);
                    g->scene_flags[0x28] = 1;
                    return 0;
                }

            }

        }
        if ((temp_v0 == 0xA) && ((g->month == 0xC) || (g->month == 1) || (g->month == 2)) && (g->scene_flags[0x29] == 0) && (g->stage == 1) && (g->step == 9)) {
            D_800EECB0 = 5;
            func_80011E28(0x60);
            g->scene_flags[0x29] = 1;
            return 0;
        }
    } else if (g->current_girl == 7) {
        temp_v0 = (((u8 *)&spot_reaction_low_cells)[0xC]);
        if ((temp_v0 == 2) && (D_800EECBC == 0) && ((u8) g->month >= 3U) && ((u8) g->month < 6U) && (g->scene_flags[0x2C] == 0) && (g->stage == 1) && (g->step == 0xF)) {
            D_800EECB0 = 2;
            func_80011E28(0x60);
            g->scene_flags[0x2C] = 1;
            return 0;
        }
        if ((temp_v0 == 0x11) && ((u8) g->month >= 6U)) {
            if ((u8) g->month < 9U) {
                if ((g->stage == 1) && (g->step == 0xA)) {
                    D_800EECB0 = 3;
                    func_80011E28(0x60);
                    g->scene_flags[0x2D] = 1;
                    return 0;
                }

            }

        }
        if ((temp_v0 == 0xC) && (spot_ride == (date_ok = 1)) && ((u8) g->month >= 9U) && ((u8) g->month < 0xCU)) {
            if ((g->scene_flags[0x2E] == 0) && (sp2B == 0) && (sp29 != 0) && (temp_v0_2 == 0)) {
                if ((g->stage == 1) && (g->step == 0x38)) {
                    D_800EECB0 = 4;
                    func_80011E28(0x60);
                    g->scene_flags[0x2E] = 1;
                    return 0;
                }

            }

        }
        if ((temp_v0 == 0x14) && ((g->month == 0xC) || (g->month == 1) || (g->month == 2)) && (g->scene_flags[0x2F] == 0) && (sp2B == 0) && (sp29 != 0) && (temp_v0_2 == 0) && (g->stage == 1) && (g->step == 8)) {
            D_800EECB0 = 5;
            func_80011E28(0x60);
            g->scene_flags[0x2F] = 1;
            return 0;
        }
        if ((g->scene_flags[0x30] == 0) && (sp2B == 0) && (sp29 != 0) && (temp_v0_2 == 0) && (g->stage == 0) && (g->step == 0x29)) {
            D_800EECB0 = 6;
            func_80011E28(0x60);
            g->scene_flags[0x30] = 1;
            return 0;
        }
    } else if (g->current_girl == 8) {
        temp_v0 = (((u8 *)&spot_reaction_low_cells)[0xC]);
        if ((temp_v0 == 9) && ((u8) g->month >= 3U) && ((u8) g->month < 6U) && (g->scene_flags[0x33] == 0) && ((D_80125300 == 0) || (D_800EECBC == 0)) && (g->stage == 1) && (g->step == 0xD)) {
            D_800EECB0 = 2;
            func_80011E28(0x60);
            g->scene_flags[0x33] = 1;
            return 0;
        }
        if ((temp_v0 == 0xC) && ((u8) D_80125310 == (date_ok = 1)) && ((u8) g->month >= 6U)) {
            if (((u8) g->month < 9U) && (sp2B == 0) && (sp29 != 0)) {
                if (temp_v0_2 == 0) {
                    if (g->scene_flags[0x34] == 0) {
                        if ((g->stage == 1) && (g->step == 0x3C)) {
                            D_800EECB0 = 3;
                            func_80011E28(0x60);
                            g->scene_flags[0x34] = 1;
                            return 0;
                        }

                    }

                }

            }

        }
        if ((temp_v0 == 0xF) && (D_80125318 == 0xA) && ((u8) g->month >= 9U) && ((u8) g->month < 0xCU)) {
            if (g->scene_flags[0x35] == 0) {
                if ((g->stage == 1) && (g->step == 0x11)) {
                    D_800EECB0 = 4;
                    func_80011E28(0x60);
                    g->scene_flags[0x35] = 1;
                    return 0;
                }

            }

        }
        if ((temp_v0 == 0x13) && ((g->month == 0xC) || (g->month == 1) || (g->month == 2)) && (g->scene_flags[0x36] == 0) && (g->girls[g->current_girl].pad2E[4] != 0) && (g->stage == 1) && (g->step == 8)) {
            D_800EECB0 = 5;
            func_80011E28(0x60);
            g->scene_flags[0x36] = 1;
            return 0;
        }
        if (temp_v0 == 8) {
            /* FAKE: distinct identity operands prevent month-equality CSE;
             * all three materializations are required by the original. T-24023. */
            if ((g->year = date_ok = ((g->month * 1) == 12 && g->day == 10)) ||
                  (g->year = date_ok = ((g->month ^ 0) == 12 && g->day == 8)) ||
                  (g->year = date_ok = ((g->month | 0) == 12 && g->day == 7))) {
                if ((sp2B < 3U) && g->player_flags.f.met && (g->stage == 1) && (g->step == 0)) {
                    D_800EECB0 = 6;
                    func_80011E28(0x60);
                    g->scene_flags[0x37] = 1;
                    return 0;
                }
            }
        }
    } else if (g->current_girl == 9) {
        temp_v0 = (((u8 *)&spot_reaction_low_cells)[0xC]);
        if ((temp_v0 == 0xC) && (D_80125328 != 0) && ((u8) g->month >= 3U) && ((u8) g->month < 6U) && (g->scene_flags[0x3A] == 0) && (g->stage == 1) && (g->step == 0x1B)) {
            D_800EECB0 = 3;
            func_80011E28(0x60);
            g->scene_flags[0x3A] = 1;
            return 0;
        }
        if ((temp_v0 == 1) && ((u8) g->month >= 6U)) {
            if (((u8) g->month < 9U) && (sp2B == 0) && (sp29 != 0)) {
                if (temp_v0_2 == 0) {
                    if (g->scene_flags[0x3B] == 0) {
                        if ((g->stage == 0) && (g->step == 0x29)) {
                            D_800EECB0 = 4;
                            func_80011E28(0x60);
                            g->scene_flags[0x3B] = 1;
                            return 0;
                        }

                    }

                }

            }

        }
        if ((temp_v0 == 4) && ((u8) g->month >= 9U) && ((u8) g->month < 0xCU)) {
            if ((g->scene_flags[0x3C] == 0) && (g->stage == 1)) {
                if (g->step == 9) {
                    D_800EECB0 = 5;
                    func_80011E28(0x60);
                    g->scene_flags[0x3C] = 1;
                    return 0;
                }

            }

        }
        if ((temp_v0 == 0x13) && ((g->month == 0xC) || (g->month == 1) || (g->month == 2)) && (g->scene_flags[0x3D] == 0) && (sp2B == 0) && (sp29 != 0) && (temp_v0_2 == 0) && (g->athletics >= 0x50) && (g->stage == 1) && (g->step == 8)) {
            D_800EECB0 = 6;
            func_80011E28(0x60);
            g->scene_flags[0x3D] = 1;
            return 0;
        }
    } else if (g->current_girl == 0xA) {
        temp_v0 = (((u8 *)&spot_reaction_low_cells)[0xC]);
        if ((temp_v0 == 0xC) && ((u8) g->month >= 3U) && ((u8) g->month < 6U) && (g->scene_flags[0x41] == 0) && (g->stage == 1) && (g->step == 8)) {
            D_800EECB0 = 3;
            func_80011E28(0x60);
            g->scene_flags[0x41] = 1;
            return 0;
        }
        if ((temp_v0 == 1) && ((u8) g->month >= 6U)) {
            if (((u8) g->month < 9U) && (sp2B == 0) && (sp29 != 0)) {
                if (temp_v0_2 == 0) {
                    if ((g->scene_flags[0x42] == 0) && (g->stage == 1)) {
                        if (g->step == 1) {
                            D_800EECB0 = 4;
                            func_80011E28(0x60);
                            g->scene_flags[0x42] = 1;
                            return 0;
                        }

                    }

                }

            }

        }
        if ((temp_v0 == 0xD) && ((u8) g->month >= 9U) && ((u8) g->month < 0xCU) && (g->scene_flags[0x43] == 0)) {
            if (((u8) spot_place_variant >= 2U) && (g->stage == 1)) {
                if (g->step == 8) {
                    D_800EECB0 = 5;
                    func_80011E28(0x60);
                    g->scene_flags[0x43] = 1;
                    return 0;
                }

            }

        }
        if ((temp_v0 == 0x14) && ((g->month == 0xC) || (g->month == 1) || (g->month == 2)) && (sp2B == 0) && (sp29 != 0) && (temp_v0_2 == 0) && (g->scene_flags[0x44] == 0) && (g->stage == 1) && (g->step == 8)) {
            D_800EECB0 = 6;
            func_80011E28(0x60);
            g->scene_flags[0x44] = 1;
            return 0;
        }
    } else {

        temp_v0 = (((u8 *)&spot_reaction_low_cells)[0xC]);
        if ((temp_v0 == 0xB) && (g->girls[1].pad23[3] == 2) && (g->stage == 1) && (g->step == 9)) {
            func_80011E28(0xD1);
            return 0;
        }
    }
    if (((u8) g->month >= 3U) && ((u8) g->month < 6U) && ((u8) g->scene_flags[0x46] >= 2U)) {
        if (g->scene_flags[0x48] == 0) {
            if ((g->stage == 0) && (g->step == 0x29)) {
                D_800EECB0 = 3;
                g->saved_game_mode = 0x50;
                g->saved_mode_stage = 0;
                g->saved_mode_step = 0xC;
                D_800EECB4 = 0xB;
                func_80011E28(0x60);
                g->scene_flags[0x48] = 1;
                return 0;
            }

        }

    }
    if (((((u8 *)&spot_reaction_low_cells)[0xC]) == 0x11) && ((u8) g->month >= 3U) && ((u8) g->month < 6U) && ((u8) g->scene_flags[0x46] >= 2U) && (g->scene_flags[0x4A] == 0) && (g->stage == 0) && (g->step == 0x29)) {
        D_800EECB0 = 5;
        g->saved_game_mode = 0x50;
        g->saved_mode_stage = 0;
        g->saved_mode_step = 0xC;
        D_800EECB4 = 0xB;
        func_80011E28(0x60);
        g->scene_flags[0x4A] = 1;
        return 0;
    }
    func_80011DFC();
}

void spot_place_neighborhood_park_arrival(void) {
    D_8009473C = D_8012421C;
    D_80094740 = D_80124288;
    D_80094744 = D_801242F4;
    D_800EECD0 = 0;
    spot_place_variant = 0;
    func_80078970(D_80094784, "近所の公園");
    D_801252E8 = 2;
    spot_reaction_low_cells = 0;
    D_801243A6 = 0;
    D_800EECE4 = -1;
    func_800469F4(0x4264);
    func_80011DFC();
}

void spot_place_kirameki_central_park_arrival(void) {
    SpotDevelopmentState *g;
    u8 sp2B;
    s32 temp_v0;
    s32 temp_t5;

    g = (SpotDevelopmentState *)&D_800B0860;

    D_8009473C = D_80124254;
    D_80094740 = D_801242C0;
    D_80094744 = D_8012432C;
    D_800EECD0 = 0;
    func_80078970(D_80094784, "きらめき公園");
    D_801252E8 = 3;
    sp2B = func_8002328C(g->current_girl);
    temp_v0 = func_8002336C(g->current_girl);
    if ((((g->month == 3) && (g->day >= 0x1AU)) || ((g->month == 4) && (g->day < 0xBU))) &&
        ((g->scene_flags[0] != 0) || (g->current_girl != 0)) &&
        ((D_800B0E43 != 0) || (g->current_girl != 4) || (sp2B != 0x80) || (temp_v0 != 0))) {
        spot_place_variant = 2;
        D_801252E8 = 3;
        func_80078970(D_800947C4, "桜");
        D_800EECE4 = -1;
    } else {
        if (!(g->girls[g->current_girl].pad10[4] & 1) &&
            ((g->month < 6U) || (g->month >= 9U) || (g->scene_flags[0x42] != 0) || (sp2B != 0x80) || (temp_v0 != 0))) {
            spot_place_variant = 0;
            func_80078970(D_800947C4, "並木道");
            D_800EECE4 = 0;
        } else {
            spot_place_variant = 1;
            func_80078970(D_800947C4, "池");
            D_800EECE4 = 0;
        }
    }
    temp_t5 = spot_place_variant * 3;
    spot_reaction_cells = (s16) *((g->current_girl * 9) + ((u8 *)D_80125268) + temp_t5);
    D_8012439E = (s16) *(u8 *)((unsigned char *)((g->current_girl * 9) + ((u8 *)D_80125268) + temp_t5) + (1));
    D_801243A0 = (s16) *(u8 *)((unsigned char *)((g->current_girl * 9) + ((u8 *)D_80125268) + temp_t5) + (2));
    spot_face_cells = (s16) *((g->current_girl * 9) + ((u8 *)D_8012526C) + temp_t5);
    D_8012438E = (s16) *(u8 *)((unsigned char *)((g->current_girl * 9) + ((u8 *)D_8012526C) + temp_t5) + (1));
    D_80124390 = (s16) *(u8 *)((unsigned char *)((g->current_girl * 9) + ((u8 *)D_8012526C) + temp_t5) + (2));
    func_800469F4(0x4277);
    func_80011DFC();
}

void spot_place_aquarium_arrival(void) {
    SpotDevelopmentState *g;
    u8 *r;
    s32 temp_t2;

    g = (SpotDevelopmentState *)&D_800B0860;

    D_8009473C = D_80124248;
    D_80094740 = D_801242B4;
    D_80094744 = D_80124320;
    func_80078970(D_80094784, "水族館");
    D_801252E8 = 3;
    r = D_800B0860 + g->current_girl * 0x34;
    if ((r[0x1BD] == 0) && ((g->year >= 0x61U) || ((g->year == 0x60) && (g->month >= 3U)))) {
        spot_place_variant = 2;
        func_80078970(D_800947C4, "イルカショー");
        D_800EECE4 = 5;
    } else if (!(r[0x1BC] & 1)) {
        spot_place_variant = 0;
        func_80078970(D_800947C4, "館内");
        D_800EECE4 = 4;
    } else {
        spot_place_variant = 1;
        func_80078970(D_800947C4, "館内");
        D_800EECE4 = 4;
    }
    temp_t2 = spot_place_variant * 3;
    spot_reaction_cells = (s16) *((g->current_girl * 9) + ((u8 *)D_80125250) + temp_t2);
    D_8012439E = (s16) *(u8 *)((unsigned char *)((g->current_girl * 9) + ((u8 *)D_80125250) + temp_t2) + (1));
    D_801243A0 = (s16) *(u8 *)((unsigned char *)((g->current_girl * 9) + ((u8 *)D_80125250) + temp_t2) + (2));
    spot_face_cells = (s16) *((g->current_girl * 9) + ((u8 *)D_80125254) + temp_t2);
    D_8012438E = (s16) *(u8 *)((unsigned char *)((g->current_girl * 9) + ((u8 *)D_80125254) + temp_t2) + (1));
    D_80124390 = (s16) *(u8 *)((unsigned char *)((g->current_girl * 9) + ((u8 *)D_80125254) + temp_t2) + (2));
    func_800469F4(0x44FD);
    func_80011DFC();
}

void spot_place_zoo_arrival(void) {
    SpotDevelopmentState *g;
    u8 *r;
    s32 temp_t9;
    u32 temp_hi;

    g = (SpotDevelopmentState *)&D_800B0860;

    D_8009473C = D_8012420C;
    D_80094740 = D_80124278;
    D_80094744 = D_801242E4;
    func_80078970(D_80094784, "動物園");
    D_801252E8 = 3;
    r = D_800B0860 + g->current_girl * 0x34;
    if ((r[0x1BF] == 0) && ((g->year >= 0x60U) || ((g->year == 0x5F) && (g->month >= 0xCU)))) {
        spot_place_variant = 2;
        func_80078970(D_800947C4, "コアラ");
        D_800EECE4 = 7;
    } else if (!(r[0x1BE] & 1)) {
        spot_place_variant = 0;
        func_80078970(D_800947C4, "園内");
        D_800EECE4 = 6;
    } else {
        spot_place_variant = 1;
        func_80078970(D_800947C4, "園内");
        D_800EECE4 = 6;
    }
    if (g->current_girl == 7) {
        D_800EECE4 = 6;
        func_80078970(D_800947C4, "園内");
        temp_hi = D_800B0860[g->current_girl * 0x34 + 0x1BE] % 3U;
        if (temp_hi == 0) {
            spot_place_variant = 0;
        } else if (temp_hi == 1) {
            spot_place_variant = 1;
        } else {
            spot_place_variant = 2;
        }
    }
    temp_t9 = spot_place_variant * 3;
    spot_reaction_cells = (s16) *((g->current_girl * 9) + ((u8 *)D_801251E8) + temp_t9);
    D_8012439E = (s16) *(u8 *)((unsigned char *)((g->current_girl * 9) + ((u8 *)D_801251E8) + temp_t9) + (1));
    D_801243A0 = (s16) *(u8 *)((unsigned char *)((g->current_girl * 9) + ((u8 *)D_801251E8) + temp_t9) + (2));
    spot_face_cells = (s16) *((g->current_girl * 9) + ((u8 *)D_801251EC) + temp_t9);
    D_8012438E = (s16) *(u8 *)((unsigned char *)((g->current_girl * 9) + ((u8 *)D_801251EC) + temp_t9) + (1));
    D_80124390 = (s16) *(u8 *)((unsigned char *)((g->current_girl * 9) + ((u8 *)D_801251EC) + temp_t9) + (2));
    func_800469F4(0x44E9);
    func_80011DFC();
}

void spot_place_botanical_garden_arrival(void) {
    SpotDevelopmentState *g;
    s32 temp_t8;

    g = (SpotDevelopmentState *)D_800B0860;

    D_8009473C = D_80124234;
    D_80094740 = D_801242A0;
    D_80094744 = D_8012430C;
    func_80078970(D_80094784, "植物園");
    func_80078970(D_800947C4, "園内");
    D_801252E8 = 3;
    if (g->current_girl == 7) {
        D_801252E8 = 5;
    }
    if (!(g->girls[g->current_girl].pad18[4] & 1)) {
        spot_place_variant = 0;
    } else {
        spot_place_variant = 1;
    }
    temp_t8 = spot_place_variant * 3;
    D_800EECE4 = 8;
    spot_reaction_cells = (s16) *((g->current_girl * 6) + ((u8 *)D_80125228) + temp_t8);
    D_8012439E = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_80125228) + temp_t8) + (1));
    D_801243A0 = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_80125228) + temp_t8) + (2));
    spot_face_cells = (s16) *((g->current_girl * 6) + ((u8 *)D_8012522C) + temp_t8);
    D_8012438E = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_8012522C) + temp_t8) + (1));
    D_80124390 = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_8012522C) + temp_t8) + (2));
    func_800469F4(0x453F);
    func_80011DFC();
}

void spot_place_planetarium_arrival(void) {
    SpotDevelopmentState *g;
    s32 temp_v0;

    g = (SpotDevelopmentState *)&D_800B0860;

    D_8009473C = D_8012422C;
    D_80094740 = D_80124298;
    D_80094744 = D_80124304;
    func_80078970(D_80094784, "プラネタリウム");
    D_801252E8 = 7;
    if (!(D_800B0860[g->current_girl * 0x34 + 0x1C1] & 1)) {
        (*((u8 *)&D_80125318 + 4)) = 0;
        func_80078970(D_800947C4, "館内");
    } else {
        (*((u8 *)&D_80125318 + 4)) = 1;
        func_80078970(D_800947C4, "館内");
        if (g->current_girl == 0xA) {
            D_80094730 = 1;
        }
    }
    D_800EECE4 = 9;
    temp_v0 = (*((u8 *)&D_80125318 + 4)) * 3;
    spot_reaction_cells = (s16) *((g->current_girl * 6) + ((u8 *)D_80125218) + temp_v0);
    D_8012439E = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_80125218) + temp_v0) + (1));
    D_801243A0 = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_80125218) + temp_v0) + (2));
    spot_face_cells = (s16) *((g->current_girl * 6) + ((u8 *)D_8012521C) + temp_v0);
    D_8012438E = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_8012521C) + temp_v0) + (1));
    D_80124390 = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_8012521C) + temp_v0) + (2));
    if ((D_800B0956 < 0x78) && ((*((u8 *)&D_80125318 + 4)) != 0)) {
        switch (g->current_girl) {
        case 0:
        case 3:
            spot_reaction_cells = 4;
            spot_face_cells = 4;
            break;
        case 2:
        case 6:
        case 8:
            spot_reaction_cells = 4;
            spot_face_cells = 3;
            break;
        case 1:
        case 4:
        case 5:
        case 9:
            spot_face_cells = 3;
            spot_reaction_cells = 3;
            break;
        case 7:
            D_80124390 = 3;
            D_801243A0 = 3;
            break;
        case 10:
            D_80124390 = 1;
            D_801243A0 = 1;
            break;
        }
    }
    func_800469F4(0x4552);
    func_80011DFC();
}

/* FAKE: a signed narrow constant preserves the shared multiply/compare
 * range. SpotResultCells models the two signed three-halfword outputs.
 * T-10870: the final message helper takes only its message id. */
void spot_place_art_museum_arrival(void) {
    s16 three = 3;
    s32 choice_offset;
    SpotDevelopmentState *g;

    g = (SpotDevelopmentState *)&D_800B0860;
    D_8009473C = spot_date_sections;
    D_80094740 = D_8012426C;
    D_80094744 = D_801242D8;
    func_80078970(D_80094784, "美術館");
    D_801252E8 = three;
    if ((g->year == 0x61) && ((u8) g->month >= 9U) && ((u8) g->month < 0xCU)) {
        (*(u8 *)&spot_place_variant) = 2;
        D_801252F4 = 1;
        func_80078970(D_800947C4, "ガーキー展");
        if (g->girls[g->current_girl].pad18[7] != 0) {
            (*(u8 *)&spot_place_variant) = 5;
        }
        D_800EECE4 = 0xB;
        if (g->current_girl == 7) {
            D_80094730 = 2;
        }
    } else if ((g->year == 0x61) && ((u8) g->month >= 3U) && ((u8) g->month < 6U)) {
        (*(u8 *)&spot_place_variant) = three;
        D_801252F8 = 1;
        func_80078970(D_800947C4, "彫塑展");
        if (g->girls[g->current_girl].pad18[8] != 0) {
            (*(u8 *)&spot_place_variant) = 5;
        }
        D_800EECE4 = 0xC;
        if (g->current_girl == 7) {
            D_80094730 = 4;
        }
    } else if ((g->year == 0x5F) && ((u8) g->month >= 9U) && ((u8) g->month < 0xCU)) {
        (*(u8 *)&spot_place_variant) = 4;
        D_801252FC = 1;
        func_80078970(D_800947C4, "有名画家展");
        if (g->girls[g->current_girl].pad18[9] != 0) {
            (*(u8 *)&spot_place_variant) = 5;
        }
        D_800EECE4 = 0xD;
        if (g->current_girl == 7) {
            D_80094730 = 4;
        }
    } else if (!(g->girls[g->current_girl].pad18[6] & 1)) {
        (*(u8 *)&spot_place_variant) = 0;
        func_80078970(D_800947C4, "館内");
        D_800EECE4 = 0xA;
    } else {
        (*(u8 *)&spot_place_variant) = 1;
        func_80078970(D_800947C4, "館内");
        D_800EECE4 = 0xA;
        if (g->current_girl == 7) {
            D_80094730 = 2;
        }
    }
    choice_offset = (*(u8 *)&spot_place_variant) * three;
    ((SpotResultCells *)&spot_reaction_cells)->value[0] = (s16) *(u8 *)((g->current_girl * 0xF) + spot_place_result_tables + choice_offset);
    ((SpotResultCells *)&spot_reaction_cells)->value[1] = (s16) *(u8 *)((u8 *)((g->current_girl * 0xF) + spot_place_result_tables + choice_offset) + (1));
    ((SpotResultCells *)&spot_reaction_cells)->value[2] = (s16) *(u8 *)((u8 *)((g->current_girl * 0xF) + spot_place_result_tables + choice_offset) + (2));
    ((SpotResultCells *)&spot_face_cells)->value[0] = (s16) *(u8 *)((g->current_girl * 0xF) + D_801251D4 + choice_offset);
    ((SpotResultCells *)&spot_face_cells)->value[1] = (s16) *(u8 *)((u8 *)((g->current_girl * 0xF) + D_801251D4 + choice_offset) + (1));
    ((SpotResultCells *)&spot_face_cells)->value[2] = (s16) *(u8 *)((u8 *)((g->current_girl * 0xF) + D_801251D4 + choice_offset) + (2));
    if ((*(u8 *)&spot_place_variant) == 5) {
        switch (g->current_girl) {
        default:
            ((SpotResultCells *)&spot_face_cells)->value[0] = ((SpotResultCells *)&spot_face_cells)->value[1] = ((SpotResultCells *)&spot_face_cells)->value[2] = three;
            ((SpotResultCells *)&spot_reaction_cells)->value[0] = ((SpotResultCells *)&spot_reaction_cells)->value[1] = ((SpotResultCells *)&spot_reaction_cells)->value[2] = three;
            break;
        case 1: case 3: case 7: case 8:
            ((SpotResultCells *)&spot_face_cells)->value[0] = ((SpotResultCells *)&spot_face_cells)->value[1] = ((SpotResultCells *)&spot_face_cells)->value[2] = 4;
            ((SpotResultCells *)&spot_reaction_cells)->value[0] = ((SpotResultCells *)&spot_reaction_cells)->value[1] = ((SpotResultCells *)&spot_reaction_cells)->value[2] = 4;
            break;
        }
        if ((g->current_girl == three) && (D_801252F4 != 0)) {
            ((SpotResultCells *)&spot_reaction_cells)->value[0] = ((SpotResultCells *)&spot_reaction_cells)->value[1] = ((SpotResultCells *)&spot_reaction_cells)->value[2] = three;
        }
        if (g->current_girl == 0xA) {
            D_80094730 += 1;
        }
    }
    func_800469F4(0x445D);
    func_80011DFC();
}

void spot_place_library_arrival(void) {
    SpotDevelopmentState *g;
    s32 temp_t8;

    g = (SpotDevelopmentState *)D_800B0860;

    D_8009473C = D_8012424C;
    D_80094740 = D_801242B8;
    D_80094744 = D_80124324;
    func_80078970(D_80094784, "図書館");
    D_801252E8 = 3;
    if (!(g->girls[g->current_girl].unk_22 & 1)) {
        spot_place_variant = 0;
        func_80078970(D_800947C4, "図書館内");
    } else {
        spot_place_variant = 1;
        func_80078970(D_800947C4, "図書館内");
        if (g->current_girl == 7) {
            D_80094730 = 3;
        } else if (g->current_girl == 0xA) {
            D_80094730 = 1;
        }
    }
    temp_t8 = spot_place_variant * 3;
    D_800EECE4 = 0xE;
    spot_reaction_cells = (s16) *((g->current_girl * 6) + ((u8 *)D_80125258) + temp_t8);
    D_8012439E = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_80125258) + temp_t8) + (1));
    D_801243A0 = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_80125258) + temp_t8) + (2));
    spot_face_cells = (s16) *((g->current_girl * 6) + ((u8 *)D_8012525C) + temp_t8);
    D_8012438E = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_8012525C) + temp_t8) + (1));
    D_80124390 = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_8012525C) + temp_t8) + (2));
    func_800469F4(0x44B0);
    func_80011DFC();
}

void spot_place_game_center_arrival(void) {
    SpotDevelopmentState *g;
    s32 temp_t7;

    g = (SpotDevelopmentState *)D_800B0860;

    D_8009473C = D_80124214;
    D_80094740 = D_80124280;
    D_80094744 = D_801242EC;
    func_80078970(D_80094784, "ゲームセンター");
    D_801252E8 = 0;
    if (!(g->girls[g->current_girl].pad23[0] & 1)) {
        spot_place_variant = 0;
        func_80078970(D_800947C4, "ビデオゲームコーナー");
    } else {
        spot_place_variant = 1;
        func_80078970(D_800947C4, "ビデオゲームコーナー");
    }
    D_800EECE4 = 0xF;
    if (((u8) g->year >= 0x61U) || (((u8) g->year >= 0x60U) && ((u8) g->month >= 9U))) {
        D_80125300 = 1;
    } else {
        D_80125300 = 0;
    }
    temp_t7 = spot_place_variant * 3;
    spot_reaction_cells = (s16) *((g->current_girl * 0xC) + ((u8 *)D_801251F8) + temp_t7);
    D_8012439E = (s16) *(u8 *)((unsigned char *)((g->current_girl * 0xC) + ((u8 *)D_801251F8) + temp_t7) + (1));
    D_801243A0 = (s16) *(u8 *)((unsigned char *)((g->current_girl * 0xC) + ((u8 *)D_801251F8) + temp_t7) + (2));
    spot_face_cells = (s16) *((g->current_girl * 0xC) + ((u8 *)D_801251FC) + temp_t7);
    D_8012438E = (s16) *(u8 *)((unsigned char *)((g->current_girl * 0xC) + ((u8 *)D_801251FC) + temp_t7) + (1));
    D_80124390 = (s16) *(u8 *)((unsigned char *)((g->current_girl * 0xC) + ((u8 *)D_801251FC) + temp_t7) + (2));
    func_800469F4(0x4442);
    func_80011DFC();
}

void spot_place_bowling_alley_arrival(void) {
    SpotDevelopmentState *g;
    s32 temp;

    g = (SpotDevelopmentState *)&D_800B0860;

    D_8009473C = D_80124204;
    D_80094740 = D_80124270;
    D_80094744 = D_801242DC;
    func_80078970(D_80094784, "ボウリング場");
    D_801252E8 = 3;
    if (!(D_800B0860[g->current_girl * 0x34 + 0x1C9] & 1)) {
        spot_place_variant = 0;
        func_80078970(D_800947C4, "ボウリング場");
    } else {
        spot_place_variant = 1;
        func_80078970(D_800947C4, "ボウリング場");
    }
    D_800EECE4 = 0x11;
    temp = spot_place_variant * 3;
    spot_reaction_cells = (s16) *((g->current_girl * 6) + ((u8 *)D_801251D8) + temp);
    D_8012439E = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_801251D8) + temp) + (1));
    D_801243A0 = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_801251D8) + temp) + (2));
    spot_face_cells = (s16) *((g->current_girl * 6) + ((u8 *)D_801251DC) + temp);
    D_8012438E = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_801251DC) + temp) + (1));
    D_80124390 = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_801251DC) + temp) + (2));
    func_800469F4(0x459D);
    func_80011DFC();
}

void spot_place_karaoke_arrival(void) {
    SpotDevelopmentState *g;
    s32 temp_t6;

    g = (SpotDevelopmentState *)D_800B0860;

    D_8009473C = D_80124218;
    D_80094740 = D_80124284;
    D_80094744 = D_801242F0;
    func_80078970(D_80094784, "カラオケ");
    D_801252E8 = 3;
    if (!(g->girls[g->current_girl].pad23[3] & 1)) {
        spot_place_variant = 0;
        func_80078970(D_800947C4, "ＢＯＸ内");
    } else {
        spot_place_variant = 1;
        func_80078970(D_800947C4, "ＢＯＸ内");
    }
    temp_t6 = spot_place_variant * 3;
    D_800EECE4 = 0x12;
    spot_reaction_cells = (s16) *((g->current_girl * 6) + ((u8 *)D_80125200) + temp_t6);
    D_8012439E = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_80125200) + temp_t6) + (1));
    D_801243A0 = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_80125200) + temp_t6) + (2));
    spot_face_cells = (s16) *((g->current_girl * 6) + ((u8 *)D_80125204) + temp_t6);
    D_8012438E = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_80125204) + temp_t6) + (1));
    D_80124390 = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_80125204) + temp_t6) + (2));
    func_800469F4(0x457F);
    func_80011DFC();
}

void spot_place_amusement_park_arrival(void) {
    SpotDevelopmentState *g;

    g = (SpotDevelopmentState *)&D_800B0860;
    D_8009473C = D_80124250;
    D_80094740 = D_801242BC;
    D_80094744 = D_80124328;
    if ((g->year >= 0x62U) || ((g->year == 0x61) && (g->month >= 3U))) {
        D_80125304 = 1;
    } else {
        D_80125304 = 0;
    }
    if ((g->year == 0x62) || ((g->year == 0x61) && (g->month >= 0xCU))) {
        D_80125308 = 1;
    } else {
        D_80125308 = 0;
    }
    D_801252E8 = 0;
    func_80078970(D_80094784, "遊園地");
    func_800469F4(0x429B);
    func_80011DFC();
}

void spot_place_shopping_street_arrival(void) {
    D_8009473C = D_80124238;
    D_80094740 = D_801242A4;
    D_80094744 = D_80124310;
    D_801252E8 = 0;
    func_80078970(D_80094784, "ショッピング街");
    func_800469F4(0x448C);
    func_80011DFC();
}

void spot_place_stadium_arrival(void) {
    SpotDevelopmentState *g;
    s32 temp_t3;
    s8 v;

    g = (SpotDevelopmentState *)&D_800B0860;

    D_8009473C = D_80124244;
    D_80094740 = D_801242B0;
    D_80094744 = D_8012431C;
    func_80078970(D_80094784, "スタジアム");
    func_80078970(D_800947C4, "スタジアム内");
    D_801252E8 = 3;
    if ((g->month == 2) || (g->month == 3) || (g->month == 0xC)) {
        v = 1;
    } else {
        v = 0;
    }
    D_8012532C = v;
    if (v != 0) {
        if (!(g->girls[g->current_girl].pad2E[1] & 1)) {
            spot_place_variant = 2;
        } else {
            spot_place_variant = 3;
        }
        D_800EECE4 = 0x1B;
    } else {
        if (!(g->girls[g->current_girl].pad2E[0] & 1)) {
            spot_place_variant = 0;
        } else {
            spot_place_variant = 1;
        }
        D_800EECE4 = 0x1A;
    }
    temp_t3 = spot_place_variant * 3;
    spot_reaction_cells = (s16) *((g->current_girl * 0xC) + ((u8 *)D_80125248) + temp_t3);
    D_8012439E = (s16) *(u8 *)((unsigned char *)((g->current_girl * 0xC) + ((u8 *)D_80125248) + temp_t3) + (1));
    D_801243A0 = (s16) *(u8 *)((unsigned char *)((g->current_girl * 0xC) + ((u8 *)D_80125248) + temp_t3) + (2));
    spot_face_cells = (s16) *((g->current_girl * 0xC) + ((u8 *)D_8012524C) + temp_t3);
    D_8012438E = (s16) *(u8 *)((unsigned char *)((g->current_girl * 0xC) + ((u8 *)D_8012524C) + temp_t3) + (1));
    D_80124390 = (s16) *(u8 *)((unsigned char *)((g->current_girl * 0xC) + ((u8 *)D_8012524C) + temp_t3) + (2));
    func_800469F4(0x4564);
    func_80011DFC();
}

void spot_place_movie_theater_arrival(void) {
    SpotDevelopmentState *g;
    s32 temp_t3;
    u8 var_t1;

    g = (SpotDevelopmentState *)&D_800B0860;

    D_8009473C = D_80124220;
    D_80094740 = D_8012428C;
    D_80094744 = D_801242F8;
    D_801252E8 = 0;
    if (g->year == 0x5F) {
        if (((u8) g->month >= 6U) && ((u8) g->month < 9U)) {
            D_80125314 = 0;
        } else if (((u8) g->month >= 9U) && ((u8) g->month < 0xCU)) {
            D_80125314 = 1;
        } else {
            D_80125314 = 2;
        }
    } else if (g->year == 0x60) {
        if ((g->month != 0) && ((u8)g->month < 3U)) {
            D_80125314 = 2;
        } else if (((u8) g->month >= 3U) && ((u8) g->month < 6U)) {
            D_80125314 = 3;
        } else if (((u8) g->month >= 6U) && ((u8) g->month < 9U)) {
            D_80125314 = 4;
        } else if (((u8) g->month >= 9U) && ((u8) g->month < 0xCU)) {
            D_80125314 = 5;
        } else {
            D_80125314 = 6;
        }
    } else {
        var_t1 = 0xA;
        if (g->year == 0x61) {
            if ((g->month != 0) && ((u8) g->month < 3U)) {
                D_80125314 = 6;
            } else if (((u8) g->month >= 3U) && ((u8) g->month < 6U)) {
                D_80125314 = 7;
            } else if (((u8) g->month >= 6U) && ((u8) g->month < 9U)) {
                D_80125314 = 8;
            } else if (((u8) g->month >= 9U) && ((u8) g->month < 0xCU)) {
                D_80125314 = 9;
            } else {
                D_80125314 = 0xA;
            }
        } else {
            D_80125314 = var_t1;
        }
    }
    D_800EECE4 = -1;
    func_80078970(D_80094784, "映画館");
    func_80078970(D_800947C4, "館内");
    temp_t3 = D_80125314 * 3;
    spot_reaction_cells = (s16) *((g->current_girl * 0x21) + ((u8 *)D_80125208) + temp_t3);
    D_8012439E = (s16) *(u8 *)((unsigned char *)((g->current_girl * 0x21) + ((u8 *)D_80125208) + temp_t3) + (1));
    D_801243A0 = (s16) *(u8 *)((unsigned char *)((g->current_girl * 0x21) + ((u8 *)D_80125208) + temp_t3) + (2));
    spot_face_cells = (s16) *((g->current_girl * 0x21) + ((u8 *)D_8012520C) + temp_t3);
    D_8012438E = (s16) *(u8 *)((unsigned char *)((g->current_girl * 0x21) + ((u8 *)D_8012520C) + temp_t3) + (1));
    D_80124390 = (s16) *(u8 *)((unsigned char *)((g->current_girl * 0x21) + ((u8 *)D_8012520C) + temp_t3) + (2));
    func_800469F4(0x4360);
    func_80011DFC();
}

void spot_place_concert_hall_arrival(void) {
    SpotDevelopmentState *g;
    s32 temp_t3;
    u8 var_t1;

    g = (SpotDevelopmentState *)&D_800B0860;

    D_8009473C = D_80124208;
    D_80094740 = D_80124274;
    D_80094744 = D_801242E0;
    D_801252E8 = 0;
    if (g->year == 0x5F) {
        if (((u8) g->month >= 9U) && ((u8) g->month < 0xCU)) {
            D_80125318 = 2;
        } else {
            D_80125318 = 3;
        }
    } else if (g->year == 0x60) {
        if ((g->month != 0) && ((u8)g->month < 3U)) {
            D_80125318 = 3;
        } else if (((u8) g->month >= 3U) && ((u8) g->month < 6U)) {
            D_80125318 = 4;
        } else if (((u8) g->month >= 6U) && ((u8) g->month < 9U)) {
            D_80125318 = 5;
        } else if (((u8) g->month >= 9U) && ((u8) g->month < 0xCU)) {
            D_80125318 = 6;
        } else {
            D_80125318 = 7;
        }
    } else {
        var_t1 = 0xB;
        if (g->year == 0x61) {
            if ((g->month != 0) && ((u8) g->month < 3U)) {
                D_80125318 = 7;
            } else if (((u8) g->month >= 3U) && ((u8) g->month < 6U)) {
                D_80125318 = 8;
            } else if (((u8) g->month >= 6U) && ((u8) g->month < 9U)) {
                D_80125318 = 9;
            } else if (((u8) g->month >= 9U) && ((u8) g->month < 0xCU)) {
                D_80125318 = 0xA;
            } else {
                D_80125318 = 0xB;
            }
        } else {
            D_80125318 = var_t1;
        }
    }
    D_800EECE4 = -1;
    func_80078970(D_80094784, "コンサート会場");
    func_80078970(D_800947C4, "会場内");
    temp_t3 = D_80125318 * 3;
    spot_reaction_cells = (s16) *((g->current_girl * 0x24) + ((u8 *)D_801251E0) + temp_t3);
    D_8012439E = (s16) *(u8 *)((unsigned char *)((g->current_girl * 0x24) + ((u8 *)D_801251E0) + temp_t3) + (1));
    D_801243A0 = (s16) *(u8 *)((unsigned char *)((g->current_girl * 0x24) + ((u8 *)D_801251E0) + temp_t3) + (2));
    spot_face_cells = (s16) *((g->current_girl * 0x24) + ((u8 *)D_801251E4) + temp_t3);
    D_8012438E = (s16) *(u8 *)((unsigned char *)((g->current_girl * 0x24) + ((u8 *)D_801251E4) + temp_t3) + (1));
    D_80124390 = (s16) *(u8 *)((unsigned char *)((g->current_girl * 0x24) + ((u8 *)D_801251E4) + temp_t3) + (2));
    func_800469F4(0x43D6);
    func_80011DFC();
}

void spot_place_pool_arrival(void) {
    SpotDevelopmentState *g;
    s32 temp;

    g = (SpotDevelopmentState *)&D_800B0860;

    D_8009473C = D_80124228;
    D_80094740 = D_80124294;
    D_80094744 = D_80124300;
    D_801252E8 = 3;
    func_80078970(D_80094784, "プール");
    func_80078970(D_800947C4, "プール内");
    if (!(D_800B0860[g->current_girl * 0x34 + 0x1D4] & 1)) {
        spot_place_variant = 0;
    } else {
        spot_place_variant = 1;
    }
    D_800EECE4 = 0x1C;
    temp = spot_place_variant * 3;
    spot_reaction_cells = (s16) *((g->current_girl * 6) + ((u8 *)D_80125210) + temp);
    D_8012439E = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_80125210) + temp) + (1));
    D_801243A0 = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_80125210) + temp) + (2));
    spot_face_cells = (s16) *((g->current_girl * 6) + ((u8 *)D_80125214) + temp);
    D_8012438E = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_80125214) + temp) + (1));
    D_80124390 = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_80125214) + temp) + (2));
    func_800469F4(0x452B);
    func_80011DFC();
}

void spot_place_sea_arrival(void) {
    SpotDevelopmentState *g;
    s32 temp_t9;

    g = (SpotDevelopmentState *)D_800B0860;

    D_8009473C = D_80124230;
    D_80094740 = D_8012429C;
    D_80094744 = D_80124308;
    D_801252E8 = 3;
    func_80078970(D_80094784, "海水浴");
    func_80078970(D_800947C4, "浜辺");
    if (g->current_girl == 6) {
        func_80078970(D_800947C4, "無人島");
    }
    if (!(g->girls[g->current_girl].pad2E[3] & 1)) {
        spot_place_variant = 0;
    } else {
        spot_place_variant = 1;
        if (g->current_girl == 5) {
            D_80094730 = 2;
        }
    }
    temp_t9 = spot_place_variant * 3;
    D_800EECE4 = 0x1D;
    spot_reaction_cells = (s16) *((g->current_girl * 6) + ((u8 *)D_80125220) + temp_t9);
    D_8012439E = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_80125220) + temp_t9) + (1));
    D_801243A0 = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_80125220) + temp_t9) + (2));
    spot_face_cells = (s16) *((g->current_girl * 6) + ((u8 *)D_80125224) + temp_t9);
    D_8012438E = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_80125224) + temp_t9) + (1));
    D_80124390 = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_80125224) + temp_t9) + (2));
    func_800469F4(0x450F);
    func_80011DFC();
}

void spot_place_shrine_arrival(void) {
    D_8009473C = D_80124210;
    D_80094740 = D_8012427C;
    D_80094744 = D_801242E8;
    func_80078970(D_80094784, "縁日");
    func_80078970(D_800947C4, "花火大会");
    D_801252E8 = 5;
    spot_face_cells = 0;
    /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
    D_8012438E = (u8)func_8002328C(D_800B1746) & 0x7F;
    spot_reaction_low_cells = 0;
    D_801243A6 = 0;
    spot_place_variant = 0;
    D_800EECE4 = -1;
    func_800469F4(0x425B);
    func_80011DFC();
}

void spot_place_skating_rink_arrival(void) {
    SpotDevelopmentState *g;
    s32 temp;

    g = (SpotDevelopmentState *)&D_800B0860;

    D_8009473C = D_8012423C;
    D_80094740 = D_801242A8;
    D_80094744 = D_80124314;
    D_801252E8 = 3;
    func_80078970(D_80094784, "スケート場");
    func_80078970(D_800947C4, "スケートリンク");
    if (!(D_800B0860[g->current_girl * 0x34 + 0x1D6] & 1)) {
        spot_place_variant = 0;
    } else {
        spot_place_variant = 1;
    }
    D_800EECE4 = 0x1E;
    temp = spot_place_variant * 3;
    spot_reaction_cells = (s16) *((g->current_girl * 6) + ((u8 *)D_80125238) + temp);
    D_8012439E = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_80125238) + temp) + (1));
    D_801243A0 = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_80125238) + temp) + (2));
    spot_face_cells = (s16) *((g->current_girl * 6) + ((u8 *)D_8012523C) + temp);
    D_8012438E = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_8012523C) + temp) + (1));
    D_80124390 = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_8012523C) + temp) + (2));
    func_800469F4(0x44D8);
    func_80011DFC();
}

void spot_place_ski_resort_arrival(void) {
    SpotDevelopmentState *g;
    s32 temp;

    g = (SpotDevelopmentState *)&D_800B0860;

    D_8009473C = D_80124240;
    D_80094740 = D_801242AC;
    D_80094744 = D_80124318;
    D_801252E8 = 3;
    func_80078970(D_80094784, "スキー場");
    func_80078970(D_800947C4, "ゲレンデ");
    if (!(D_800B0860[g->current_girl * 0x34 + 0x1D7] & 1)) {
        spot_place_variant = 0;
    } else {
        spot_place_variant = 1;
    }
    D_800EECE4 = 0x1F;
    temp = spot_place_variant * 3;
    spot_reaction_cells = (s16) *((g->current_girl * 6) + ((u8 *)D_80125240) + temp);
    D_8012439E = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_80125240) + temp) + (1));
    D_801243A0 = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_80125240) + temp) + (2));
    spot_face_cells = (s16) *((g->current_girl * 6) + ((u8 *)D_80125244) + temp);
    D_8012438E = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_80125244) + temp) + (1));
    D_80124390 = (s16) *(u8 *)((unsigned char *)((g->current_girl * 6) + ((u8 *)D_80125244) + temp) + (2));
    func_800469F4(0x44C4);
    func_80011DFC();
}

void spot_reaction_rows_build_step(void) {
    u8 sel;
    s16 i;
    s16 *p;
    s16 *r;
    s16 *q;
    s16 v;
    u8 temp;

    sel = D_800B0F35[D_800B1746];
    for (i = 0; i < 3; i++) {
        p = &spot_reaction_tier_cells + i;
        r = &spot_reaction_cells + i;
        q = &spot_reaction_low_cells + i;
        /* FAKE: orders spot_reaction_tier_cells before spot_reaction_cells in find_ix_loadstores after copy propagation; T-24105 */
        if (p) {}
        *p = v = *r;
        if (*p == 0) {
            *p = 1;
        }
        if (*p == 4) {
            *p = 3;
        }
        temp = D_800B0F2C;
        if (temp == sel) {
            *p = 3;
        }
        *q = v - 1;
        /* FAKE: balances i ($a1) and v ($a2) live-range priorities after the empty p guard; T-24105 */
        if (i) {}
        if (*q == -1) {
            *q = 0;
        }
        if (*q == 3) {
            *q = 2;
        }
    }
    func_80011DFC();
}

void spot_place_kirameki_central_park_bg_load_05(void) {
    if (spot_place_variant == 0) {
        func_800469F4(0x4289);
    } else if (spot_place_variant == 1) {
        func_800469F4(0x4280);
    } else {
        func_800469F4(0x4292);
    }
    func_80011DFC();
}

void spot_shopping_street_shop_choose_step(void) {
    SpotDevelopmentState *g = (SpotDevelopmentState *)&D_800B0860;
    s32 choice_offset;
    u8 dummy;

    /* FAKE: dead loads before the first branch let cvt_pass strip the CVTs of spot_place_variant and D_800B1746; T-24103 */
    dummy = spot_place_variant;
    dummy = D_800B1746;
    D_80125324 = D_800EECBC;
    if (D_800EECBC == 0) {
        func_80078970(D_800947C4, "ブティック");
        if (!(D_800B0860[g->current_girl * 0x34 + 0x1B9] & 1)) {
            spot_place_variant = 0;
        } else {
            spot_place_variant = 1;
        }
        if ((D_800B1746 == 0) || (D_800B1746 == 7)) {
            D_80094730 += spot_place_variant * 6;
        } else {
            D_80094730 += spot_place_variant * 2;
        }
        if (D_800B1746 == 8) {
            D_80094730 += 1;
        }
        D_800EECE4 = 1;
        /* FAKE: indirect load reloads spot_place_variant after the D_80094730 store instead of keeping it across the branch; T-24103 */
        choice_offset = *(u8 *)&spot_place_variant * 3;
        spot_reaction_cells = (s16) *((g->current_girl * 0x12) + ((u8 *)D_80125230) + choice_offset);
        D_8012439E = (s16) *(u8 *)((u8 *)((g->current_girl * 0x12) + ((u8 *)D_80125230) + choice_offset) + (1));
        D_801243A0 = (s16) *(u8 *)((u8 *)((g->current_girl * 0x12) + ((u8 *)D_80125230) + choice_offset) + (2));
        spot_face_cells = (s16) *((g->current_girl * 0x12) + ((u8 *)D_80125234) + choice_offset);
        D_8012438E = (s16) *(u8 *)((u8 *)((g->current_girl * 0x12) + ((u8 *)D_80125234) + choice_offset) + (1));
        D_80124390 = (s16) *(u8 *)((u8 *)((g->current_girl * 0x12) + ((u8 *)D_80125234) + choice_offset) + (2));
        func_800469F4(0x4495);
    } else if (D_800EECBC == 1) {
        func_80078970(D_800947C4, "ファンシーショップ");
        if (!(D_800B0860[g->current_girl * 0x34 + 0x1BA] & 1)) {
            spot_place_variant = 0;
        } else {
            spot_place_variant = 1;
        }
        if (D_800B1746 == 0) {
            D_80094730 += spot_place_variant * 2 + 0xA;
        } else if (D_800B1746 == 7) {
            D_80094730 += spot_place_variant * 2 + 8;
        } else if ((D_800B1746 == 1) || (D_800B1746 == 4) || (D_800B1746 == 9)) {
            D_80094730 += spot_place_variant * 2 + 6;
        } else if ((D_800B1746 == 5) || (D_800B1746 == 0xA)) {
            D_80094730 += spot_place_variant * 2 + 5;
        } else {
            D_80094730 += spot_place_variant * 2 + 4;
        }
        if (D_800B1746 == 8) {
            D_80094730 += 1;
        }
        D_800EECE4 = 2;
        choice_offset = *(u8 *)&spot_place_variant * 3;
        spot_reaction_cells = (s16) *(u8 *)((u8 *)((g->current_girl * 0x12) + ((u8 *)D_80125230) + choice_offset) + (6));
        D_8012439E = (s16) *(u8 *)((u8 *)((g->current_girl * 0x12) + ((u8 *)D_80125230) + choice_offset) + (7));
        D_801243A0 = (s16) *(u8 *)((u8 *)((g->current_girl * 0x12) + ((u8 *)D_80125230) + choice_offset) + (8));
        spot_face_cells = (s16) *(u8 *)((u8 *)((g->current_girl * 0x12) + ((u8 *)D_80125234) + choice_offset) + (6));
        D_8012438E = (s16) *(u8 *)((u8 *)((g->current_girl * 0x12) + ((u8 *)D_80125234) + choice_offset) + (7));
        D_80124390 = (s16) *(u8 *)((u8 *)((g->current_girl * 0x12) + ((u8 *)D_80125234) + choice_offset) + (8));
        func_800469F4(0x449E);
    } else {
        func_80078970(D_800947C4, "ジャンク屋");
        if (!(D_800B0860[g->current_girl * 0x34 + 0x1BB] & 1)) {
            spot_place_variant = 0;
        } else {
            spot_place_variant = 1;
        }
        if (D_800B1746 == 0) {
            D_80094730 += spot_place_variant * 2 + 0xE;
        } else if (D_800B1746 == 7) {
            D_80094730 += spot_place_variant * 2 + 0xC;
        } else if ((D_800B1746 == 1) || (D_800B1746 == 4) || (D_800B1746 == 9)) {
            D_80094730 += spot_place_variant * 2 + 0xA;
        } else if ((D_800B1746 == 5) || (D_800B1746 == 0xA)) {
            D_80094730 += spot_place_variant * 2 + 9;
        } else {
            D_80094730 += spot_place_variant * 2 + 8;
        }
        D_800EECE4 = 3;
        choice_offset = *(u8 *)&spot_place_variant * 3;
        spot_reaction_cells = (s16) *(u8 *)((u8 *)((g->current_girl * 0x12) + ((u8 *)D_80125230) + choice_offset) + (12));
        D_8012439E = (s16) *(u8 *)((u8 *)((g->current_girl * 0x12) + ((u8 *)D_80125230) + choice_offset) + (13));
        D_801243A0 = (s16) *(u8 *)((u8 *)((g->current_girl * 0x12) + ((u8 *)D_80125230) + choice_offset) + (14));
        spot_face_cells = (s16) *(u8 *)((u8 *)((g->current_girl * 0x12) + ((u8 *)D_8012522C) + choice_offset) + (12));
        D_8012438E = (s16) *(u8 *)((u8 *)((g->current_girl * 0x12) + ((u8 *)D_8012522C) + choice_offset) + (13));
        D_80124390 = (s16) *(u8 *)((u8 *)((g->current_girl * 0x12) + ((u8 *)D_8012522C) + choice_offset) + (14));
        func_800469F4(0x44A7);
    }
    spot_reaction_rows_build_step();
}

void spot_place_aquarium_bg_load_04(void) {
    func_800469F4(0x4506);
    func_80011DFC();
}

void spot_place_zoo_bg_load_04(void) {
    func_800469F4(0x44F3);
    func_80011DFC();
}

void spot_place_botanical_garden_bg_load_04(void) {
    func_800469F4(0x4548);
    func_80011DFC();
}

void spot_place_planetarium_bg_load_02(void) {
    func_800469F4(0x455C);
    func_80011DFC();
}

void spot_place_art_museum_bg_load_02(void) {
    if (D_801252F4 != 0) {
        func_800469F4(0x446F);
    } else if (D_801252F8 != 0) {
        func_800469F4(0x4479);
    } else if (D_801252FC != 0) {
        func_800469F4(0x4482);
    } else {
        func_800469F4(0x4466);
    }
    func_80011DFC();
}

void spot_place_library_bg_load_06(void) {
    func_800469F4(0x44BA);
    func_80011DFC();
}

void spot_place_game_center_bg_load_07(void) {
    if (D_800EECBC == 0) {
        func_800469F4(0x444B);
    } else {
        func_800469F4(0x4454);
    }
    func_80011DFC();
}

void spot_place_bowling_alley_bg_load_04(void) {
    func_800469F4(0x4593);
    func_80011DFC();
}

void spot_place_karaoke_bg_load_04(void) {
    func_800469F4(0x4589);
    func_80011DFC();
}

void spot_place_amusement_park_bg_load_03(void) {
    func_800469F4(0x42A5);
    func_80011DFC();
}

void spot_place_amusement_park_bg_load_labeled_21(void) {
    u8 v;

    func_800433D0(0x200);
    func_80078970(D_80094784, "ヒーローショー");
    func_800469F4(0x42E4);
    if (!(D_800B0A04[D_800B1746].unk_27 & 1)) {
        v = 0;
    } else {
        v = 1;
    }
    D_80094730 += v * 2;
    D_800EECE4 = 0x13;
    spot_place_variant = v;
    func_80011DFC();
}

/* s32 with no return (K&R): the stored D_800EECBC value stays in $v0 as the return register. */
s32 spot_amusement_ride_choose_step(void) {
    /* FAKE: unused words reserve top frame slots so s32 msg sits at sp+0x28 in a 0x50 frame; T-11110 */
    s32 pad[9];
    s32 msg;

    if (D_800EECBC == 0) {
        if ((D_80125308 != 0) || (D_80125304 != 0)) {
            msg = 0x42C9;
            spot_ride = 3;
        } else {
            msg = 0x42AE;
            spot_ride = 0;
        }
    }
    if (D_800EECBC == 1) {
        D_80094730 += 5;
        if (D_80125308 != 0) {
            msg = 0x42D2;
            spot_ride = 4;
        } else {
            msg = 0x42B7;
            spot_ride = 1;
        }
    }
    if (D_800EECBC == 2) {
        D_80094730 += 0xA;
        msg = 0x42C0;
        spot_ride = 2;
    }
    func_800469F4(msg);
    spot_ride_setup();
    spot_reaction_rows_build_step();
    if ((D_800B1746 == 8) && ((u8)spot_ride == 3) && (D_800B0BCF != 0)) {
        D_800EECBC = 1;
    } else {
        D_800EECBC = 0;
    }
}

void spot_place_amusement_park_bg_load_labeled_65(void) {
    func_80078970(D_800947C4, "ナイトパレード");
    func_800433D0(0x204);
    func_800469F4(0x42DB);
    (&spot_reaction_tier_cells)[0] = (&spot_reaction_tier_cells)[1] = (&spot_reaction_tier_cells)[2] = 1;
    (&spot_reaction_low_cells)[0] = (&spot_reaction_low_cells)[1] = (&spot_reaction_low_cells)[2] = 0;
    if (D_800B1746 == 2) {
        (&spot_reaction_tier_cells)[0] = (&spot_reaction_tier_cells)[1] = (&spot_reaction_tier_cells)[2] = 3;
        (&spot_reaction_low_cells)[0] = (&spot_reaction_low_cells)[1] = (&spot_reaction_low_cells)[2] = 2;
    }
    func_80011DFC();
}

void spot_place_amusement_park_bg_load_labeled_77(void) {
    func_800469F4(0x429B);
    func_80078970(D_800947C4, "遊園地");
    func_80011DFC();
}

void spot_place_stadium_step_02(void) {
    if ((u8) spot_place_variant < 2U) {
        func_800433D0(0x200);
        func_800469F4(0x456D);
    } else {
        func_800433D0(0x201);
        func_800469F4(0x4576);
    }
    func_80011DFC();
}

void spot_place_movie_theater_bg_load_02(void) {
    func_800469F4(0x4369);
    func_80011DFC();
}

void spot_place_movie_theater_bg_load_11(void) {
    switch (D_80125314) {
    case 0:
        func_800469F4(0x4372);
        break;
    case 1:
        func_800469F4(0x437B);
        break;
    case 2:
        func_800469F4(0x4384);
        break;
    case 3:
        func_800469F4(0x438C);
        break;
    case 4:
        func_800469F4(0x4396);
        break;
    case 5:
        func_800469F4(0x439F);
        break;
    case 6:
        func_800469F4(0x43A7);
        break;
    case 7:
        func_800469F4(0x43B0);
        break;
    case 8:
        func_800469F4(0x43B9);
        break;
    case 9:
        func_800469F4(0x43C3);
        break;
    case 10:
        func_800469F4(0x43CD);
        break;
    }
    func_80011DFC();
}

void spot_place_movie_theater_bg_load_22(void) {
    func_800469F4(0x4360);
    func_80011DFC();
}

void spot_place_concert_hall_bg_load_02(void) {
    func_800469F4(0x43E0);
    func_80011DFC();
}

void spot_place_concert_hall_bg_load_11(void) {
    switch (D_80125318) {
    case 0:
        func_800469F4(0x4430);
        break;
    case 1:
        func_800469F4(0x43E9);
        break;
    case 2:
        func_800469F4(0x440E);
        break;
    case 3:
        func_800469F4(0x4420);
        break;
    case 4:
        func_800469F4(0x43F3);
        break;
    case 5:
        func_800469F4(0x4405);
        break;
    case 6:
        func_800469F4(0x4430);
        break;
    case 7:
        func_800469F4(0x4417);
        break;
    case 8:
        func_800469F4(0x43FC);
        break;
    case 9:
        func_800469F4(0x4430);
        break;
    case 10:
        func_800469F4(0x4428);
        break;
    case 11:
        func_800469F4(0x4439);
        break;
    }
    func_80011DFC();
}

void spot_place_concert_hall_bg_load_18(void) {
    func_800469F4(0x43D6);
    func_80011DFC();
}

void spot_place_pool_bg_load_05(void) {
    func_800469F4(0x4535);
    func_80011DFC();
}

void spot_place_sea_bg_load_05(void) {
    if ((D_800B1746 != 6) || (D_800B095E < 0x96)) {
        func_800469F4(0x4518);
    } else {
        func_800469F4(0x4522);
    }
    func_80011DFC();
}

void spot_place_skating_rink_bg_load_03(void) {
    func_800469F4(0x44E0);
    func_80011DFC();
}

void spot_place_ski_resort_bg_load_03(void) {
    func_800469F4(0x44CE);
    func_80011DFC();
}

void spot_place_shrine_step_33(void) {
    func_80015D28(0x86, 0x801B0000, 0xAC0C);
    func_800433D0(0x200);
    spot_place_asset_pointers_init();
    func_80011DFC();
}

void spot_ride_setup(void) {
    SpotDevelopmentState *g;
    s32 temp_t0;
    s32 temp_t1;
    s32 temp_t1_2;
    s32 temp_t8;
    s32 temp_t9;

    g = (SpotDevelopmentState *)D_800B0860;
    func_80078970(D_800947C4, "遊園地");
    if ((u8) spot_ride == 0) {
        func_80078970(D_80094784, "ジェットコースター");
        if (!(D_800B0A04[g->current_girl].pad28[0] & 1)) {
            spot_place_variant = 0;
        } else {
            spot_place_variant = 1;
        }
        temp_t1 = spot_place_variant * 3;
        ((SpotResultCells *)&spot_reaction_cells)->value[0] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125260 + temp_t1);
        ((SpotResultCells *)&spot_reaction_cells)->value[1] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125260 + temp_t1 + 1);
        ((SpotResultCells *)&spot_reaction_cells)->value[2] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125260 + temp_t1 + 2);
        ((SpotResultCells *)&spot_face_cells)->value[0] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125264 + temp_t1);
        ((SpotResultCells *)&spot_face_cells)->value[1] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125264 + temp_t1 + 1);
        ((SpotResultCells *)&spot_face_cells)->value[2] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125264 + temp_t1 + 2);
        D_800EECE4 = 0x14;
        return;
    }
    if ((u8) spot_ride == 3) {
        func_80078970(D_80094784, "絶叫マシーンビビール");
        if (!(D_800B0A04[g->current_girl].pad28[3] & 1)) {
            spot_place_variant = 0;
        } else {
            spot_place_variant = 1;
        }
        temp_t0 = spot_place_variant * 3;
        D_800EECE4 = 0x17;
        ((SpotResultCells *)&spot_reaction_cells)->value[0] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125260 + temp_t0 + 18);
        ((SpotResultCells *)&spot_reaction_cells)->value[1] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125260 + temp_t0 + 19);
        ((SpotResultCells *)&spot_reaction_cells)->value[2] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125260 + temp_t0 + 20);
        ((SpotResultCells *)&spot_face_cells)->value[0] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125264 + temp_t0 + 18);
        ((SpotResultCells *)&spot_face_cells)->value[1] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125264 + temp_t0 + 19);
        ((SpotResultCells *)&spot_face_cells)->value[2] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125264 + temp_t0 + 20);
        return;
    }
    if ((u8) spot_ride == 1) {
        func_80078970(D_80094784, "ゴーストハウス");
        if (!(D_800B0A04[g->current_girl].pad28[1] & 1)) {
            spot_place_variant = 0;
        } else {
            spot_place_variant = 1;
        }
        temp_t8 = spot_place_variant * 3;
        D_800EECE4 = 0x15;
        ((SpotResultCells *)&spot_reaction_cells)->value[0] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125260 + temp_t8 + 6);
        ((SpotResultCells *)&spot_reaction_cells)->value[1] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125260 + temp_t8 + 7);
        ((SpotResultCells *)&spot_reaction_cells)->value[2] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125260 + temp_t8 + 8);
        ((SpotResultCells *)&spot_face_cells)->value[0] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125264 + temp_t8 + 6);
        ((SpotResultCells *)&spot_face_cells)->value[1] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125264 + temp_t8 + 7);
        ((SpotResultCells *)&spot_face_cells)->value[2] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125264 + temp_t8 + 8);
        return;
    }
    if ((u8) spot_ride == 4) {
        func_80078970(D_80094784, "バーチャルシップ");
        if (!(D_800B0A04[g->current_girl].pad28[4] & 1)) {
            spot_place_variant = 0;
        } else {
            spot_place_variant = 1;
        }
        temp_t1_2 = spot_place_variant * 3;
        D_800EECE4 = 0x18;
        ((SpotResultCells *)&spot_reaction_cells)->value[0] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125260 + temp_t1_2 + 24);
        ((SpotResultCells *)&spot_reaction_cells)->value[1] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125260 + temp_t1_2 + 25);
        ((SpotResultCells *)&spot_reaction_cells)->value[2] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125260 + temp_t1_2 + 26);
        ((SpotResultCells *)&spot_face_cells)->value[0] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125264 + temp_t1_2 + 24);
        ((SpotResultCells *)&spot_face_cells)->value[1] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125264 + temp_t1_2 + 25);
        ((SpotResultCells *)&spot_face_cells)->value[2] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125264 + temp_t1_2 + 26);
        return;
    }
    func_80078970(D_80094784, "観覧車");
    if (!(D_800B0A04[g->current_girl].pad28[2] & 1)) {
        spot_place_variant = 0;
    } else {
        spot_place_variant = 1;
    }
    temp_t9 = spot_place_variant * 3;
    D_800EECE4 = 0x16;
    ((SpotResultCells *)&spot_reaction_cells)->value[0] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125260 + temp_t9 + 12);
    ((SpotResultCells *)&spot_reaction_cells)->value[1] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125260 + temp_t9 + 13);
    ((SpotResultCells *)&spot_reaction_cells)->value[2] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125260 + temp_t9 + 14);
    ((SpotResultCells *)&spot_face_cells)->value[0] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125264 + temp_t9 + 12);
    ((SpotResultCells *)&spot_face_cells)->value[1] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125264 + temp_t9 + 13);
    ((SpotResultCells *)&spot_face_cells)->value[2] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125264 + temp_t9 + 14);
}

void spot_place_game_center_branch_01(void) {
    if (D_80125300 == 0) {
        D_80094730 += 1;
        func_80011DFC();
        func_80011DFC();
        D_800EECBC = 0;
    }
    func_80011DFC();
}

s32 spot_place_shopping_street_branch_27(void) {
    if ((D_800B1746 == 0) && (D_80094730 == 0xA) && (D_800EECBC == 0)) {
        func_80011DFC();
        return 0;
    }
    if ((D_800B1746 == 1) || (D_800B1746 == 4) || (D_800B1746 == 9)) {
        if ((D_80094730 == 6) && (D_800EECBC == 0)) {
            func_80011DFC();
            return 0;
        }
    }
    if ((D_800B1746 == 0xA) && (D_80094730 == 6) && (D_800EECBC == 0)) {
        func_80011DFC();
        func_80011DFC();
        func_80011DFC();
        return 0;
    }
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
}

void spot_place_shopping_street_branch_on_mood_16(void) {
    u8 temp_v0;

    temp_v0 = func_8002328C(D_800B1746);
    if ((D_800EECBC == 0) && ((D_800B1746 == 0) || (D_800B1746 == 7)) && (spot_place_variant == 0)) {
        D_80094730 = D_80094730 + ((temp_v0 & 0x7F) >= 2U) + ((temp_v0 & 0x7F) >= 3U);
    } else {
        func_80011DFC();
        func_80011DFC();
        func_80011DFC();
        func_80011DFC();
        func_80011DFC();
    }
    func_80011DFC();
}

void spot_place_shopping_street_branch_on_mood_18(void) {
    u32 t;

    t = (u8) func_8002328C(D_800B1746) & 0x7F;
    D_80094730 = ((D_80094730 - (t >= 2U)) - (t >= 3U)) + 2;
    func_80011DFC();
}

void spot_place_shopping_street_set_state_22(void) {
    func_80011DFC();
}

void spot_place_planetarium_branch_14(void) {
    if (D_800B0956 < 0x78) {
        D_80094730 += 2;
    }
    func_80011DFC();
}

void spot_place_art_museum_step_09(void) {
    if (spot_place_variant == 5) {
        spot_outcome_apply_step();
        D_800EECBC = 0;
        return;
    }
    func_80011DFC();
}

void spot_place_art_museum_branch_on_mood_11(void) {
    u8 v;

    v = func_8002328C(D_800B1746) & 0x7F;
    if (spot_place_variant == 5) {
        D_80094730 = D_80094730 + (v >= 2U) + (v >= 3U);
        func_80011DFC();
        D_800B1AF6 += 8;
        return;
    }
    func_80011DFC();
}

void spot_place_game_center_label_set_04(void) {
    SpotDevelopmentState *g;
    s32 temp_t4;

    /* FAKE: discarded entry read suppresses selector CVT hoisting. T-14300 */
    temp_t4 = spot_place_variant;
    g = (SpotDevelopmentState *)D_800B0860;

    if (D_800EECBC != 0) {
        if (!(g->girls[g->current_girl].pad23[1] & 1)) {
            spot_place_variant = 2;
            func_80078970(D_800947C4, "メダルゲームコーナー");
        } else {
            spot_place_variant = 3;
            func_80078970(D_800947C4, "メダルゲームコーナー");
        }
        temp_t4 = spot_place_variant * 3;
        D_800EECE4 = 0x10;
        spot_reaction_cells = (s16) *((g->current_girl * 0xC) + ((u8 *)D_801251F8) + temp_t4);
        D_8012439E = (s16) *(u8 *)((unsigned char *)((g->current_girl * 0xC) + ((u8 *)D_801251F8) + temp_t4) + (1));
        D_801243A0 = (s16) *(u8 *)((unsigned char *)((g->current_girl * 0xC) + ((u8 *)D_801251F8) + temp_t4) + (2));
        spot_face_cells = (s16) *((g->current_girl * 0xC) + ((u8 *)D_801251FC) + temp_t4);
        D_8012438E = (s16) *(u8 *)((unsigned char *)((g->current_girl * 0xC) + ((u8 *)D_801251FC) + temp_t4) + (1));
        D_80124390 = (s16) *(u8 *)((unsigned char *)((g->current_girl * 0xC) + ((u8 *)D_801251FC) + temp_t4) + (2));
    }
    D_80094730 += spot_place_variant * 2;
    if ((g->current_girl == 7) && (spot_place_variant >= 2U)) {
        D_80094730 += 3;
    }
    func_80011DFC();
}

void spot_place_amusement_park_branch_10(void) {
    u32 v;

    if (D_80125328 == 0) {
        D_80094730 = 9;
        v = D_800B1746;
        if (v == 7) {
            D_80094730 = 0xA;
        } else if (v == 2 || v == 9) {
            D_80094730 = 8;
        } else if (v == 4 || v == 8 || v == 5 || v == 0xA) {
            D_80094730 = 7;
        }
        D_800B1AF6 += 0x14;
    }
    func_80011DFC();
}

void spot_place_amusement_park_branch_13(void) {
    SpotDevelopmentState *g;

    g = (SpotDevelopmentState *)D_800B0860;
    if (D_800EECBC == 1) {
        /* FAKE: evaluate cvt(D_800B1746) into $v0 before D_80094730 = 9; T-11110 */
        if (D_800B1746 + 0) {
        }
        D_80094730 = 9;
        if (D_800B1746 == 7) {
            D_80094730 = 0xA;
        } else if (D_800B1746 == 2 || D_800B1746 == 9) {
            D_80094730 = 8;
        } else if (D_800B1746 == 4 || D_800B1746 == 8 || D_800B1746 == 5 || D_800B1746 == 0xA) {
            D_80094730 = 7;
        }
        D_800B1AF6 += 0x11;
    } else if (D_800B1746 == 4 || D_800B1746 == 8 || D_800B1746 == 5 || D_800B1746 == 0xA) {
        D_800B1AF6 += 5;
    } else if (D_800B1746 == 9 && g->scene_flags[0x3A] == 0) {
        D_80094730 += 1;
        D_800B1AF6 += 5;
    } else if (D_800B1746 == 9 && g->scene_flags[0x3A] != 0) {
        D_800B1AF6 += 0x11;
    }
    func_80011DFC();
}

s32 spot_place_amusement_park_speak_branch_18(void) {
    /* FAKE: unused word reserves top frame slot so s16 before sits at sp+0x2A; T-11110 */
    s32 pad;
    s16 before;

    if (D_800EECBC == 1) {
        if (D_800B1746 == 7) {
            before = D_800B1AF6;
            func_8004CF30();
            if (before != D_800B1AF6) {
                D_80094730 = 0xA;
                D_800B1AF6 += 0xB;
            }
            return 0;
        }
        D_80094730 = 9;
        if (D_800B1746 == 2) {
            D_80094730 = 8;
        } else if ((D_800B1746 == 4) || (D_800B1746 == 8) || (D_800B1746 == 5)) {
            D_80094730 = 7;
        }
        /* FAKE: dead test gives cvt(D_800B1AF6) a second use in this block so uopt allocates $v0; T-11110 */
        if ((s32)D_800B1AF6) {
        }
        D_800B1AF6 += 0xC;
    } else {
        D_8012530C = 1;
    }
    func_80011DFC();
}

void spot_place_amusement_park_special_case_29(void) {
    SpotDevelopmentState *g;

    g = (SpotDevelopmentState *)D_800B0860;
    func_80011DFC();
    g->step += 0x16;
    ((SpotResultCells *)&spot_reaction_cells)->value[0] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125260 + spot_place_variant * 3 + 30);
    ((SpotResultCells *)&spot_reaction_cells)->value[1] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125260 + spot_place_variant * 3 + 31);
    ((SpotResultCells *)&spot_reaction_cells)->value[2] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125260 + spot_place_variant * 3 + 32);
    ((SpotResultCells *)&spot_face_cells)->value[0] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125264 + spot_place_variant * 3 + 30);
    ((SpotResultCells *)&spot_face_cells)->value[1] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125264 + spot_place_variant * 3 + 31);
    ((SpotResultCells *)&spot_face_cells)->value[2] = (s16) *(g->current_girl * 0x24 + (u8 *)D_80125264 + spot_place_variant * 3 + 32);
    spot_reaction_rows_build_step();
}

void spot_place_amusement_park_branch_32(void) {
    if (D_80125304 != 0) {
        D_80094730 += 0x10;
    }
    if (D_80125308 != 0) {
        D_80094730 += 0x10;
    }
    func_80011DFC();
}

void spot_place_amusement_park_branch_63(void) {
    if (D_800EECBC == 1) {
        D_800EECBC = D_800EECC8;
        D_80125310 = 0;
        func_80011DFC();
        D_800B1AF6 += 0xC;
        return;
    }
    if (D_800B0A04[D_800B1746].unk_2D != 0) {
        D_8012437C += 1;
    }
    func_80011DFC();
}

void spot_place_amusement_park_branch_73(void) {
    if (D_800B0A04[D_800B1746].unk_2D == 0) {
        D_8012437C += 1;
    }
    func_80011DFC();
}

void spot_place_amusement_park_set_state_75(void) {
    D_800B0860[D_800B1746 * 0x34 + 0x1D1] += 1;
    func_80011DFC();
    D_800B1AF6 += 7;
}

void spot_place_amusement_park_set_state_51(void) {
    D_80094730 += spot_place_variant * 2;
    func_80011DFC();
}

void spot_place_amusement_park_label_set_01(void) {
    func_80078970(D_800947C4, "遊園地内");
    func_80011DFC();
}

void spot_place_movie_theater_branch_30(void) {
    switch (D_800B1746) {
    case 5:
    case 9:
        if (spot_reaction_cells == 3) {
            D_80094730 += 1;
        } else if (spot_reaction_cells == 2) {
            D_80094730 += 2;
        }
        break;
    case 7:
        if (spot_reaction_cells == 4) {
            D_80094730 += 1;
        } else if (spot_reaction_cells == 3) {
            D_80094730 += 2;
        }
        break;
    default:
        if (spot_reaction_cells == 4) {
            D_80094730 += 1;
        } else if (spot_reaction_cells == 2) {
            D_80094730 += 2;
        }
        break;
    }
    func_80011DFC();
}

void spot_place_concert_hall_branch_26(void) {
    /* FAKE: the read through a pointer makes uopt compare constant-first as the original does (its spot_reaction_cells is
     * probably a member of an aggregate we do not know); real source unknown. T-16930 */
    s16 *res = &spot_reaction_cells;

    switch (D_800B1746) {
    case 0:
    case 1:
    case 7:
        if (*res == 4) {
            D_80094730 += 1;
        } else if (*res == 2) {
            D_80094730 += 2;
        }
        break;
    case 2:
    case 3:
    case 8:
    case 10:
        if (*res == 4) {
            D_80094730 += 1;
        } else if (*res == 3) {
            D_80094730 += 2;
        }
        break;
    case 4:
    case 6:
    case 9:
        if ((*res == 3) && (*res == 2)) {
            D_80094730 += 1;
        } else if ((*res == 3) && (*res == 3)) {
            D_80094730 += 2;
        }
        break;
    case 5:
        if (*res == 3) {
            D_80094730 += 1;
        } else if (*res == 2) {
            D_80094730 += 2;
        }
        break;
    }
    func_80011DFC();
}

/* The record index is read through the SpotDevelopmentState field (D_800B1746 is its unk_EE6): an indirect load, so
 * IDO reloads it after the compare chain instead of keeping D_800B1746 in a register, as the original does. */
void spot_place_shrine_special_case_08(void) {
    if (D_800B0895 == 0x5F) {
        if ((D_800B1746 == 2) || (D_800B1746 == 8) || (D_800B1746 == 0xA)) {
            D_80094730 = 1;
        }
        *(s16 *)(D_800B0860 + ((SpotDevelopmentState *)D_800B0860)->current_girl * 0x34 + 0x1A6) += 1;
        func_80011DFC();
    } else if (D_800B0895 == 0x60) {
        if ((D_800B1746 == 2) || (D_800B1746 == 8) || (D_800B1746 == 0xA)) {
            D_80094730 = 4;
        } else {
            D_80094730 = 6;
        }
        func_80011EC4(0x14);
    } else {
        if ((D_800B1746 == 2) || (D_800B1746 == 8) || (D_800B1746 == 0xA)) {
            D_80094730 = 9;
        } else {
            D_80094730 = 0xB;
        }
        *(s16 *)(D_800B0860 + ((SpotDevelopmentState *)D_800B0860)->current_girl * 0x34 + 0x1A6) += 1;
        func_80011EC4(0x1F);
    }
    func_8004C250();
}

void spot_place_shrine_jump_19(void) {
    func_80011EC4(0x32);
}

void spot_place_shrine_special_case_40(void) {
    s32 offset;
    u8 *first;
    u8 *second;

    offset = D_800B1746 * 3;
    /* The canonical word globals contain byte-table addresses. */
    first = (u8 *)(offset + D_801251F0);
    spot_reaction_cells = (s16) first[0];
    D_8012439E = (s16) first[1];
    D_801243A0 = (s16) first[2];
    second = (u8 *)(offset + D_801251F4);
    spot_face_cells = (s16) second[0];
    D_8012438E = (s16) second[1];
    D_80124390 = (s16) second[2];
    spot_reaction_rows_build_step();
}

void spot_place_shrine_stat_update_22(void) {
    D_80094730 += D_801243AC;
    switch (D_801243AC) {
    case 0:
        D_800B096E += 5;
        break;
    case 1:
        D_800B094E += 5;
        break;
    case 2:
        *(s16 *)(D_800B0860 + D_800B1746 * 0x34 + 0x1A6) -= 1;
        break;
    case 3:
        *(s16 *)(D_800B0860 + D_800B1746 * 0x34 + 0x1A6) += 1;
        break;
    }
    func_8004C250();
    func_80011DFC();
}

void spot_place_amusement_park_face_step_59(void) {
    D_800EECC8 = D_800EECBC;
    if (D_800B0896 == 8) {
        D_80125310 = 1;
        D_8012437C = 0;
        D_80124380 = 0;
        func_8004A8EC(func_8002336C(D_800B1746));
        func_80033E88();
        func_80011DFC();
        return;
    }
    func_80011DFC();
    D_800B1AF6 += 0x10;
}

/* s32 with no return (K&R): the decremented byte stays in $v0 as the return register. */
s32 spot_place_shrine_special_case_10(void) {
    /* FAKE: unused word reserves top frame slot so before sits at sp+0x28; T-11360 */
    s32 pad;
    s32 before;

    /* FAKE: dead load at entry lets the cvt pass keep D_800B1AF6 in $v0; T-11360 */
    before = D_800B1AF6;
    switch (D_800EECC4) {
    case 0:
        before = D_800B1AF6;
        func_80046600();
        /* FAKE: (s32) cast gives the original operand order of the beq; T-11360 */
        if (before != (s32)D_800B1AF6) {
            D_800B1AF6 -= 1;
            D_800EECC4 += 1;
        }
        break;
    case 1:
        before = D_800B1AF6;
        func_800464E8();
        if (before != (s32)D_800B1AF6) {
            D_800B1AF6 -= 1;
            D_800EECC4 += 1;
        }
        break;
    default:
        func_80011DFC();
        break;
    }
}

void spot_place_neighborhood_park_branch_on_mood_00(void) {
    u8 v;

    v = func_8002328C(D_800B1746) & 0x7F;
    D_80094730 = ((v >= 2U) + (v >= 3U)) * 2;
    D_80094734 = 0;
    D_8012439E = 0;
    spot_reaction_cells = 0;
    D_801243A0 = 0;
    func_80011DFC();
}

#define ES (*(SpotDevelopmentStateGirls *)&D_800B0860)

void spot_place_neighborhood_park_special_case_04(void) {
    u32 month_diff;
    u8 status;
    EvShorts11 counts_a;
    EvShorts11 counts_b;
    u8 first_clear;
    s32 heart;

    counts_a = D_80125330;
    counts_b = D_80125348;
    /* FAKE: pointer-indexed (&ES.unk_036)[0] and ((s16 *)&counts_b)[3] match DATE twin func_8013A94C (T-24083); T-24103 */
    if ((ES.year == 0x61) && ((&ES.month)[0] >= 6U)) {
        ((s16 *)&counts_b)[3] += 1;
    } else if (ES.year == 0x62) { ((s16 *)&counts_b)[3] += 1; }
    *(s16 *)&D_80124364 = D_800EECBC;
    *(s16 *)&D_80124368 = (u32)ES.frame_count % (u32)(((D_800EECBC == 0) * counts_a.v[ES.current_girl]) + (counts_b.v[ES.current_girl] * (D_800EECBC == 1)));
    D_801252EC = 0;
    first_clear = 0;
    if ((&ES.month)[0] == ES.girls[ES.current_girl].birthday.date.month) {
        /* FAKE: `unk_10.u * 0 +` and `((&ES.unk_036)[0] == 2U) * 0 +` preserve register allocation across the birthday check, as in DATE func_8013A94C (T-24083); T-24103 */
        if (ES.girls[ES.current_girl].birthday.u * 0 + ES.day < ES.girls[ES.current_girl].birthday.date.day) {
            D_801252EC = 1;
        }
    } else {
        month_diff = ((&ES.month)[0] == 2U) * 0 + (ES.girls[ES.current_girl].birthday.date.month - (&ES.month)[0]) + 12;
        if ((month_diff % 12) == 1) {
            D_801252EC = 1;
        }
    }
    D_801252F0 = 0;
    if ((((ES.year == 0x61) && ((&ES.month)[0] >= 3U)) || (ES.year == 0x62))) {
        D_801252F0 = 1;
    }
    status = func_8002328C(ES.current_girl) & 0x7F;
    heart = func_8002336C(ES.current_girl);
    if ((status == 0) && (heart == 0)) {
        first_clear = 1;
    }
    if ((first_clear != 0) && (D_800EECBC == 0)) {
        *(s16 *)&D_80124364 = 4;
        *(s16 *)&D_80124368 = 0;
    } else if ((D_801252EC != 0) && (D_800EECBC == 0)) {
        *(s16 *)&D_80124364 = 2;
        *(s16 *)&D_80124368 = ES.year - 0x5F;
        if (ES.current_girl == 6) {
            if ((status < 2U) && (heart < 2U)) {
                *(s16 *)&D_80124368 = 1;
            } else {
                *(s16 *)&D_80124368 = 0;
            }
        }
    } else if ((D_801252F0 != 0) && (D_800EECBC == 0)) {
        *(s16 *)&D_80124364 = 3;
        *(s16 *)&D_80124368 = 0;
    }
    if ((ES.current_girl == 5) && (*(s16 *)&D_80124364 == 0) && (*(s16 *)&D_80124368 == 3)) {
        ES.girls[5].flags.f.b14 = 1;
    }
    func_80011DFC();
}
#undef ES

s32 spot_arrival_talk_gate_step(void) {
    if ((spot_arrival_mode != 0) || (D_801243B4 == 1)) {
        func_80011DFC();
        func_80011DFC();
        return 0;
    }
    return spot_arrival_talk_step();
}

s32 spot_arrival_talk_step(void) {
    u32 r;

    D_8009471C = (u8 *)&spot_dialogue_group_offset;
    D_80094720 = (u8 *)&D_80124348;
    D_80094724 = D_8012425C;
    D_80094728 = D_801242C8;
    D_8009472C = D_80124334;
    /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
    r = (u8)func_8002328C(D_800B1746) & 0x7F;
    if (spot_arrival_mode == 2 && D_80124348 == 2 && r >= 2U) {
        func_8004A8EC(3);
        func_80033E88();
    }
    return func_80049B20(D_800EECBC, 1, 0);
}

s32 spot_special_day_talk_step(void) {
    if (D_800B1AF0++ == 0) {
        if (D_800B0896 == 3 && D_800B0897 == 0x15) {
            D_8012434C = 0;
            goto block_26;
        }
        if (D_800B0895 == 0x5F && D_800B0896 == 7 && D_800B0897 == 0x18) {
            D_8012434C = 1;
            goto block_26;
        }
        if (D_800B0895 == 0x60 && D_800B0896 == 7 && D_800B0897 == 0x1C) {
            D_8012434C = 1;
            goto block_26;
        }
        if (D_800B0895 == 0x61 && D_800B0896 == 7 && D_800B0897 == 0x1B) {
            D_8012434C = 1;
            goto block_26;
        }
        if (D_800B0896 == 9 && D_800B0897 == 0x17) {
            if (D_800B1AE4 & 1) {
                D_8012434C = 2;
            } else {
                D_8012434C = 3;
            }
            goto block_26;
        }
        if (D_800B0896 == 0xC && D_800B0897 == 0x17) {
            D_8012434C = 4;
            goto block_26;
        }
        func_80011DFC(D_800B0895);
        return 0;
    }
block_26:
    D_8009471C = (u8 *)&D_8012434C;
    D_80094720 = (u8 *)&D_80124350;
    D_80094724 = D_80124260;
    D_80094728 = D_801242CC;
    D_8009472C = D_80124338;
    return func_80049B20(D_800EECBC, 1, 0);
}

void spot_talk_until_done_step(void) {
    s16 sp2E;

    D_8009471C = (u8 *) &sp2E;
    D_80094720 = (u8 *) &(&spot_reaction_cells)[D_800EECBC];
    D_80094724 = D_801241D8;
    D_80094728 = D_801241E8;
    sp2E = 0;
    D_8009472C = D_801241F8;
    if (func_80049B20(D_800EECBC, 0, 0) != 0) {
        func_80011DFC();
    }
}

void spot_athletics_check_talk_step(void) {
    SpotDevelopmentState *g;
    s16 sp2A;
    s16 sp28;

    g = (SpotDevelopmentState *)D_800B0860;

    sp2A = spot_place_variant;
    D_8009471C = (u8 *)&sp2A;
    D_80094724 = D_801241D8;
    D_80094728 = D_801241E8;
    sp2A = 0;
    D_8009472C = D_801241F8;
    if ((D_800EECBC == 0) && (g->athletics < 0x28) && (spot_place_variant == 1) && (spot_place == 0x10) && ((g->current_girl == 0) || (g->current_girl == 1) || (g->current_girl == 3) || (g->current_girl == 4) || (g->current_girl == 5) || (g->current_girl == 8) || (g->current_girl == 9))) {
        spot_reaction_cells = 3;
        spot_face_cells = 3;
        spot_reaction_tier_cells = 3;
        spot_reaction_low_cells = 2;
        func_8004A8EC(3);
        func_80033E88();
        sp28 = 5;
        D_80094720 = (u8 *) &sp28;
        if (func_80049B20(D_800EECBC, 0, 0) != 0) {
            func_80011DFC();
        }
    } else if (((D_800EECBC == 0) && (g->athletics < 0x28) && (spot_place_variant == 0) && (spot_place == 0x13) &&
                ((g->current_girl == 0) || (g->current_girl == 1) || (g->current_girl == 3) || (g->current_girl == 6) || (g->current_girl == 7))) ||
               ((D_800EECBC == 0) && (g->athletics < 0x64) && (spot_place_variant == 0) && (spot_place == 0x14) &&
                ((g->current_girl == 0) || (g->current_girl == 4) || (g->current_girl == 6) || (g->current_girl == 7) || (g->current_girl == 8) || (g->current_girl == 0xA)))) {
        spot_reaction_cells = 3;
        spot_face_cells = 3;
        spot_reaction_tier_cells = 3;
        spot_reaction_low_cells = 2;
        func_8004A8EC(3);
        func_80033E88();
        sp28 = 6;
        D_80094720 = (u8 *) &sp28;
        if (func_80049B20(D_800EECBC, 0, 0) != 0) {
            func_80011DFC();
        }
    } else {
        func_80011DFC();
    }
}

void spot_result_talk_step(void) {
    SpotDevelopmentState *g;
    s16 sp32;
    u8 temp_v0;

    g = (SpotDevelopmentState *)D_800B0860;
    temp_v0 = func_8002328C(g->current_girl);
    if (((SpotDevelopmentStepView *)&D_800B1AF0)->substep++ == 0) {
        D_80124354 = (((temp_v0 & 0x7F) >= 2U) + ((temp_v0 & 0x7F) >= 3U) + ((temp_v0 & 0x7F) >= 4U)) * 3;
    }
    sp32 = (&spot_reaction_low_cells)[D_800EECBC] + D_80124354;
    if (g->appointment_flag != 0) {
        sp32 = 0xA;
    }
    if ((g->appointment_flag != 0) && ((u32) (temp_v0 & 0x7F) < 2U) && (g->current_girl == 6)) {
        sp32 = 0x12;
    } else if ((D_800EECE4 >= 0) && (D_800EECE4 == g->girl_last_date_spot[g->current_girl]) && (((SpotDevelopmentStepView *)&D_800B1AF0)->mode == 0x50) && ((u8) D_80125310 == 0)) {
        sp32 = 0xB;
        if ((g->appointment_flag != 0) && ((u32) (temp_v0 & 0x7F) < 2U) && (g->current_girl == 6)) {
            sp32 = 0x13;
        }
        if (((SpotDevelopmentStepView *)&D_800B1AF0)->substep == 1) {
            func_8004A8EC(3);
            g->girls[g->current_girl].unk_02 -= 1;
            g->girls[g->current_girl].unk_06 -= 2;
            g->girls[g->current_girl].unk_0A += 5;
            func_8004C250();
            func_80033E88();
        }
    }
    D_8009471C = (u8 *) &sp32;
    D_80094720 = &D_80124358;
    temp_v0 = func_8002328C(g->current_girl);
    if ((g->current_girl == 1) && ((temp_v0 & 0x7F) < 2U) && (*(s16 *)&D_80124358 == 3)) {
        func_8004A8EC(1);
        func_80033E88();
    }
    D_80094724 = D_80124258;
    D_80094728 = D_801242C4;
    D_8009472C = D_80124330;
    func_80049B20(D_800EECBC, 1, 0);
}

s32 spot_mood_talk_step(void) {
    u8 x;
    u32 t;

    x = func_8002328C(D_800B1746);
    if (D_800B1AF9++ == 0) {
        t = x & 0x7F;
        spot_mood_tier = (t >= 2U) + (t >= 3U) + (t >= 4U);
    }
    D_8009471C = (u8 *) &spot_mood_tier;
    D_80094720 = (u8 *) &D_80124360;
    D_80094724 = D_80124268;
    D_80094728 = D_801242D4;
    D_8009472C = D_80124340;
    return func_80049B20(D_800EECBC, 1, 0);
}

void spot_place_neighborhood_park_talk_variant_06(void) {
    D_8009471C = (u8 *) &D_80124364;
    D_80094720 = (u8 *) &D_80124368;
    D_80094724 = D_80124264;
    D_80094728 = D_801242D0;
    D_8009472C = D_8012433C;
    if (func_80049B20(D_800EECBC, 0, 0) != 0) {
        func_80011DFC();
    }
}

void spot_place_movie_theater_talk_variant_16(void) {
    if (D_800B1AF0++ == 0) {
        D_80124370 += D_80125314;
    }
    D_8009471C = &D_8012436C;
    D_80094720 = (u8 *)&D_80124370;
    D_80094724 = D_801241DC;
    D_80094728 = D_801241EC;
    D_8009472C = D_801241FC;
    if (func_80049B20(D_800EECBC, 0, 0) != 0) {
        func_80011DFC();
    }
}

void spot_place_concert_hall_talk_variant_16(void) {
    if (D_800B1AF0++ == 0) {
        D_80124378 += D_80125318;
    }
    D_8009471C = &D_80124374;
    D_80094720 = (u8 *)&D_80124378;
    D_80094724 = spot_date_common_pointers;
    D_80094728 = D_801241E0;
    D_8009472C = D_801241F0;
    if (func_80049B20(D_800EECBC, 0, 0) != 0) {
        func_80011DFC();
    }
}

void spot_ride_talk_step(void) {
    D_8009471C = &D_8012437C;
    D_80094720 = &D_80124380;
    D_80094724 = D_80124224;
    D_80094728 = D_80124290;
    D_8009472C = D_801242FC;
    func_80049B20(0xFF, 1, 0);
}

void spot_arrival_talk_step_common(void) {
    D_8009471C = &D_80124384;
    D_80094720 = &D_80124388;
    D_80094724 = D_801241D4;
    D_80094728 = D_801241E4;
    D_8009472C = D_801241F4;
    func_80049B20(0xFF, 1, 0);
}

void spot_place_stadium_talk_variant_12(void) {
    D_8009471C = (u8 *)&D_80094730;
    if (D_800B1746 == 0 && (D_800B0940 >> 0x1C) == 6 && spot_place_variant == 0 && D_80094734 == 0) {
        D_80094734 += 1;
    }
    D_80094720 = (u8 *)&D_80094734;
    D_80094724 = D_8009473C;
    D_80094728 = D_80094740;
    D_8009472C = D_80094744;
    if (func_80049B20(D_800EECBC, 0, 0) != 0) {
        func_80011DFC();
    }
}

void spot_place_shopping_street_special_case_03(void) {
    if (D_800EECBC == 2) {
        if ((D_800B1746 == 2) || (D_800B1746 == 3)) {
            func_80011DFC();
            return;
        }
        if (D_800B1746 == 8) {
            func_8004A7F4();
            return;
        }
        if ((D_800B1746 == 5) && (D_800B0A04[5].unk_17 == 0)) {
            func_80011DFC();
            return;
        }
        if ((D_800B1746 == 5) && (D_800B0A04[5].unk_17 != 0)) {
            D_80094730 += 1;
            func_8004A7C4();
            return;
        }
        func_8004A7C4();
        return;
    }
    func_80011DFC();
}

void spot_place_amusement_park_set_state_15(void) {
    func_80011DFC();
}

void spot_place_sea_branch_12(void) {
    s32 x;

    if (D_800B1746 != 6) {
        func_80011DFC();
        D_800B1AF6 += 8;
        return;
    }
    if (D_800B095E >= 0x96) {
        D_80094730 += 3;
        func_80011DFC();
        D_800B1AF6 += 8;
        return;
    }
    x = (&spot_reaction_cells)[0] = (&spot_reaction_cells)[3] = 2;
    (&spot_face_cells)[0] = (&spot_face_cells)[1] = (&spot_face_cells)[2] = 3;
    (&spot_reaction_cells)[2] = x;
    (&spot_reaction_low_cells)[0] = x;
    /* FAKE: the u16 view of the row gives its constant 3 the original's $v0 (autotweak, T-17202). */
    ((u16 *)&spot_reaction_tier_cells)[0] = ((u16 *)&spot_reaction_tier_cells)[1] = ((u16 *)&spot_reaction_tier_cells)[2] = 3;
    func_80011DFC();
}

void spot_portrait_read_pair_step(void) {
    if (spot_place == 0x12) {
        spot_portrait_read_b();
        func_80011DFC();
    } else if (spot_place == 0x14) {
        spot_portrait_read_a();
        func_80011DFC();
    }
    func_80011DFC();
}

void spot_portrait_read_a(void) {
    switch (D_800B1746) {
    case 0:
        func_800335E0(0x55BA);
        return;
    case 4:
        func_800335E0(0x64D8);
        return;
    case 5:
        func_800335E0(0x5E84);
        return;
    case 6:
        func_800335E0(0x60CD);
        return;
    case 7:
        func_800335E0(0x5B5A);
        return;
    case 8:
        func_800335E0(0x50CE);
        return;
    default:
        func_800335E0(0x6721);
        return;
    }
}

void spot_portrait_read_b(void) {
    switch (D_800B1746) {
    case 0:
        func_800335E0(0x5614);
        break;
    case 1:
        func_800335E0(0x5C68);
        break;
    case 2:
        func_800335E0(0x51DC);
        break;
    case 3:
        func_800335E0(0x593E);
        break;
    case 4:
        func_800335E0(0x6532);
        break;
    case 5:
        func_800335E0(0x5DD0);
        break;
    case 6:
        func_800335E0(0x6127);
        break;
    case 7:
        func_800335E0(0x5BE1);
        break;
    case 8:
        func_800335E0(0x4F93);
        break;
    case 9:
        func_800335E0(0x6316);
        break;
    case 10:
        func_800335E0(0x666D);
        break;
    }
}

void spot_outcome_factors();

void spot_outcome_apply_step(void) {
    /* FAKE: unused words reserve frame slots so sp38 sits at sp+0x38 and sp48/sp4A sit at sp+0x48/0x4A in a 0x50 frame; T-24103 */
    s32 padTop;
    s16 sp4A;
    s16 sp48;
    s32 padMid;
    EvShorts5 sp38;

    sp38 = spot_outcome_terms;
    func_8004A8EC((&spot_face_cells)[D_800EECBC]);
    func_80033E88();
    spot_outcome_factors(3 - (&spot_reaction_cells)[D_800EECBC], &sp4A, &sp48);
    ((SpotDevelopmentStateFixed *)&D_800B0860)->girls[D_800B1746].tokimeki_fixed += (((3 - (&spot_reaction_cells)[D_800EECBC]) * sp4A * sp48) << 16) / 16;
    /* FAKE: byte-offset load prevents uopt from CSEing the two spot_reaction_cells[D_800EECBC] reads in the same block while still hoisting &spot_reaction_cells into $s0; T-24103 */
    spot_outcome_factors(2 - *(s16 *)((u8 *)&spot_reaction_cells + (D_800EECBC << 1)), &sp4A, &sp48);
    ((SpotDevelopmentStateFixed *)&D_800B0860)->girls[D_800B1746].affection_fixed += (((2 - (&spot_reaction_cells)[D_800EECBC]) * sp4A * sp48) << 16) / 16;
    spot_outcome_factors(2 - *(s16 *)((u8 *)&spot_reaction_cells + (D_800EECBC << 1)), &sp4A, &sp48);
    ((SpotDevelopmentStateFixed *)&D_800B0860)->girls[D_800B1746].affection_fixed += ((sp38.v[(&spot_reaction_cells)[D_800EECBC]] * sp4A * sp48) << 16) / 16;
    func_8004C250();
    func_80011DFC();
}

void spot_outcome_factors(arg0, arg1, arg2)
    s16 arg0;
    s16 *arg1;
    s16 *arg2;
{
    s16 i;
    s16 j;
    s32 row;
    SpotPlaceAffinityTable table;

    table = spot_place_affinity;
    if (arg0 > 0) {
        i = 0;
positive_loop:
        if (D_800B0962 >= (0x64 - (i * 0x19))) {
            *arg1 = 5 - i;
        } else {
            i += 1;
            if (i < 5) {
                goto positive_loop;
            }
        }
        if (D_800B0962 == 0) {
            *arg1 = 0;
        }
        row = D_800B1746;
        *arg2 = 5 - table.unk_00[row][spot_place];
        return;
    }
    j = 0;
negative_loop:
    if (D_800B0962 >= (0x64 - (j * 0x19))) {
        *arg1 = j + 3;
    } else {
        j += 1;
        if (j < 5) {
            goto negative_loop;
        }
    }
    if (D_800B0962 == 0) {
        *arg1 = 8;
    }
    row = D_800B1746;
    *arg2 = table.unk_00[row][spot_place] + 3;
}

void spot_place_pool_face_step_24(void) {
    func_8004A8EC((&spot_face_cells)[D_800EECBC]);
    func_80011DFC();
}

void spot_face_from_row_step(void) {
    func_8004A8EC((&spot_reaction_tier_cells)[D_800EECBC]);
    func_80033E88();
    func_80011DFC();
}

void spot_place_sea_face_step_35(void) {
    func_8004A8EC((&spot_reaction_tier_cells)[D_800EECBC]);
    func_80011DFC();
}

void spot_place_amusement_park_expression_71(void) {
    if (D_800B1746 == 2) {
        func_8004A7C4();
        return;
    }
    if ((D_800B1746 == 1) && (D_800B0A04[1].unk_2D == 0)) {
        func_8004A734();
        return;
    }
    func_8004A764();
}

void spot_all_places_equal_step(void) {
    if ((D_800B0F32 == D_800B0F31) && (D_800B0F33 == D_800B0F31) && (D_800B0F34 == D_800B0F31) && ((u8) D_800B0895 >= 0x60U)) {
        spot_place_empty_step();
        return;
    }
    func_80011DFC();
}

void spot_place_empty_step(void) {
    func_80011DFC();
}

void spot_scene_close_step(void) {
    SpotDevelopmentStateGirls *g = (SpotDevelopmentStateGirls *)&D_800B0860;

    if ((g->step_timer == 0) && (D_800EECE4 >= 0) && (g->mode != 0x60)) {
        g->girls[g->current_girl].scene_counters[D_800EECE4] += 1;
    }
    if (g->step_timer == 0) {
        if (D_800EECE4 >= 0) {
            g->girl_last_date_spot[g->current_girl] = D_800EECE4;
        }
        if (D_800EECE4 == 7) {
            g->girl_last_date_spot[g->current_girl] = 6;
        }
        if (D_800EECE4 == 5) {
            g->girl_last_date_spot[g->current_girl] = 4;
        }
        if ((D_800EECE4 == 0xB) || (D_800EECE4 == 0xC) || (D_800EECE4 == 0xD)) {
            g->girl_last_date_spot[g->current_girl] = 0xA;
        }
    }
    if ((g->looks >= 0x23) || ((g->looks >= 0x1E) && (g->current_girl != 7))) {
        g->girls[g->current_girl].syoushin -= 0x28;
    } else {
        if (spot_place == 0x12) {
            g->girls[g->current_girl].tokimeki += 1;
        }
        if ((g->month == 5) && ((g->day == 4) || (g->day == 5)) && (g->girls[g->current_girl].flags.f.b4)) {
            g->girls[g->current_girl].tokimeki += 2;
        }
    }
    func_8004C250();
    g->appointment_place_0 = spot_place;
    func_80014CA8(0, 0);
    func_8003E638(1);
}

void spot_birthday_check_step(void) {
    if ((((SpotDevelopmentState *)&D_800B0860)->month == ((SpotBirthdayBits *)&D_800B0A04[D_800B1746].pad10[0])->month) &&
        (D_800B0897 == ((SpotBirthdayBits *)&D_800B0A04[D_800B1746].pad10[0])->day)) {
        func_80011E28(0x50);
        func_80011E8C(2);
        return;
    }
    if ((D_800B0896 == ((SpotBirthdayBits *)&D_800B0948)->month) &&
        (D_800B0897 == ((SpotBirthdayBits *)&D_800B0948)->day)) {
        func_80011E28(0x50);
        func_80011E8C(3);
        return;
    }
    func_80011DFC();
}

void spot_place_kirameki_central_park_branch_02(void) {
    s32 one; /* FAKE: keeps the constant 1 in a register shared by the compare and the store (permuter) */

    if (1 == D_800B1746) {
        if ((one = 1) == spot_place_variant) {
            if (D_800B0896 >= 6 && D_800B0896 < 9) {
                D_80094730 += 9;
                spot_reaction_cells = one;
                D_8012439E = 3;
                D_801243A0 = 2;
                spot_face_cells = 2;
                D_8012438E = 3;
                D_80124390 = 3;
            }
        } else if (spot_place_variant == 2) {
            D_80094730 += 9;
        }
    }
    func_80011DFC();
}

void spot_place_zoo_special_case_01(void) {
    u8 sel; /* local copy of the selector: the original compares the register as the first operand (T-10400) */

    if (D_800B1746 == 1) {
        sel = spot_place_variant;
        if (sel == 0) {
            D_80094730 = 4;
        }
        if (sel == 1) {
            D_80094730 = 7;
        }
        if (sel == 2) {
            D_80094730 = 0xA;
        }
        if ((D_800B0896 >= 6) && (D_800B0896 < 9)) {
            D_80094730 = 0;
            spot_reaction_cells = 1;
            D_8012439E = 4;
            D_801243A0 = 2;
            spot_face_cells = 2;
            D_8012438E = 3;
            D_80124390 = 3;
        }
        spot_reaction_rows_build_step();
        return;
    }
    func_80011DFC();
}

s32 spot_place_zoo_branch_15(void) {
    if ((D_800B1746 == 1) && (D_800B0896 >= 6) && (D_800B0896 < 9) && (D_800EECBC == 1)) {
        func_80011DFC();
        return 0;
    }
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
}

void spot_place_zoo_set_state_18(void) {
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
}

s32 spot_place_amusement_park_special_case_43(void) {
    /* FAKE: label stops cvt_pass entry strip so uopt puts 1 ($a0) first in bne; T-11110 */
entry:;
    if ((D_800B1746 == 1) && (*(u8 *)&spot_ride == 3)) {
        if (((u8)D_800B0E31 == 0) && ((D_800B0896 < 3) || (D_800B0896 >= 6))) {
            D_80094730 += 1;
            (&spot_reaction_cells)[0] = (&spot_reaction_cells)[1] = (&spot_reaction_cells)[2] = 3;
            (&spot_face_cells)[0] = (&spot_face_cells)[1] = (&spot_face_cells)[2] = 3;
            spot_reaction_rows_build_step();
            func_8004A8EC(3);
            func_80033E88();
            return 0;
        }
        if ((u8)D_800B0E31 == 1) {
            (&spot_reaction_cells)[0] = (&spot_reaction_cells)[1] = (&spot_reaction_cells)[2] = 4;
            (&spot_face_cells)[0] = (&spot_face_cells)[1] = (&spot_face_cells)[2] = 4;
            spot_reaction_rows_build_step();
            func_8004A8EC(4);
            func_80033E88();
            return 0;
        }
    } else if ((D_800B1746 == 8) && (*(u8 *)&spot_ride == 2) && (D_800B0BCE != 0)) {
        D_80094730 += 3;
    }
    func_80011DFC();
}

void spot_place_amusement_park_special_case_44(void) {
    if (D_800B1746 == 8 && (u32)(u8)spot_ride == 3 && D_800B0BCF != 0) {
        D_80094734 = 2;
    }
    func_8004CF30();
    if (D_800B1746 == 8 && (u32)(u8)spot_ride == 3) {
        if (D_800B0BCF == 0 && D_80094734 == 3) {
            func_800201FC(1);
            D_80094730 += 1;
            D_80094734 = 0;
            func_80011DFC();
        }
    }
}

void spot_place_amusement_park_special_case_45(void) {
    s32 v2;
    u8 v;

    /* FAKE: dead load before the first label lets cvt_pass strip the CVT of spot_ride, so it goes first in bne; T-11600 */
    v = spot_ride;
    /* FAKE: label stops cvt_pass entry strip so uopt puts 1 ($a0) first in bne; T-11110 */
entry:;
    if ((D_800B1746 == 1) && (spot_ride == 3)) {
        if (((u8)D_800B0E31 == 0) && ((D_800B0896 < 3) || (D_800B0896 >= 6))) {
            D_800B1AF6 += 0xD;
        }
        if ((u8)D_800B0E31 == 1) {
            D_800B1AF6 += 0xD;
        }
    } else if ((D_800B1746 == 4) && (spot_ride == 3) && (D_800B0AFF != 0)) {
        D_800B1AF6 += 6;
    } else if ((D_800B1746 == 7) && (spot_ride == 1)) {
        v = func_8002328C(7);
        v2 = func_8002336C(7);
        if ((v == 0x80) && (v2 == 0)) {
            D_800B1AF6 += 0xA;
        } else {
            D_800B1AF6 += 0xB;
        }
    } else if ((D_800B1746 == 8) && (spot_ride == 2) && (D_800B0BCE != 0)) {
        D_800B1AF6 += 6;
    }
    func_80011DFC();
}

void spot_place_amusement_park_face_step_27(void) {
    if (D_800B1746 == 2) {
        func_8004A8EC(4);
        func_80033E88();
        D_80094730 = 5;
        D_800B0A04[0].unk_02 += 1;
        D_800B0A04[0].unk_06 += 1;
        D_800B0A04[0].unk_0A -= 0x14;
        func_8004C250();
        func_80011EC4(0x5C);
        return;
    }
    spot_date_event_choose();
}

void spot_place_shrine_branch_00(void) {
    if ((D_800B1746 == 2) || (D_800B1746 == 8) || (D_800B1746 == 5) || (D_800B1746 == 0xA)) {
        D_800B1AF6 += 5;
    }
    func_80011DFC();
}

void spot_place_amusement_park_jump_101(void) {
    func_80011EC4(0x3A);
}

/* s32 with no return (K&R): the decremented copy `cur` stays in $v0 as the return register. */
s32 spot_place_shopping_street_speak_branch_06(void) {
    /* FAKE: unused word reserves top frame slot so before sits at sp+0x28; T-11600 */
    s32 pad;
    s32 before;
    u8 cur;

    if ((D_800B1746 == 6) && (D_800B0A04[6].unk_17 != 0) && (D_800EECBC == 2)) {
        D_80094730 = 0;
        D_800B1AF6 -= 5;
        func_80011DFC();
        return;
    }
    if ((D_800B1746 == 8) && (D_800EECBC == 2)) {
        before = D_800B1AF6;
        func_8004CF30();
        if (before != D_800B1AF6) {
            cur = D_800B1AF6;
            D_80094730 = 0;
            D_800B1AF6 = cur -= 5;
        }
    }
}

void spot_place_shopping_street_branch_04(void) {
    if ((D_800B1746 == 6) && (D_800B0A04[6].unk_17 != 0) && (D_800EECBC == 2)) {
        D_800EECBC = 3;
    }
    func_80011DFC();
}

void spot_place_sea_set_state_20(void) {
    (&spot_reaction_tier_cells)[0] = (&spot_reaction_tier_cells)[1] = (&spot_reaction_tier_cells)[2] = 3;
    (&spot_reaction_cells)[0] = (&spot_reaction_cells)[1] = (&spot_reaction_cells)[2] = 4;
    (&spot_reaction_low_cells)[0] = (&spot_reaction_low_cells)[1] = (&spot_reaction_low_cells)[2] = 2;
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
}

void spot_place_botanical_garden_branch_13(void) {
    if ((D_800B1746 == 7) && (D_800EECBC == 0) && (spot_place_variant == 0)) {
        func_80011DFC();
        return;
    }
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
}

s32 spot_place_art_museum_expression_13(void) {
    if ((D_800B1746 == 7) && (D_800EECBC == 1) && ((spot_place_variant == 2) || (spot_place_variant == 0))) {
        func_8004A7F4();
        return 0;
    }
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
}

void spot_place_art_museum_expression_14(void) {
    if (spot_place_variant == 0) {
        func_8004A7F4();
        return;
    }
    spot_outcome_apply_step();
}

void spot_place_library_branch_17(void) {
    if ((D_800B1746 == 7) && (spot_place_variant == 0)) {
        D_80094730 += D_800EECBC;
        func_80011DFC();
        return;
    }
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
}

void spot_place_library_branch_19(void) {
    if ((D_800B1746 == 7) && (spot_place_variant == 0)) {
        func_80011DFC();
        func_80011DFC();
        func_80011DFC();
        return;
    }
    func_80011DFC();
}

void spot_place_game_center_speak_branch_17(void) {
    if ((D_800B1746 == 7) && (D_800EECBC == 0) && (spot_place_variant == 1)) {
        func_8004CF30();
        return;
    }
    func_80011DFC();
}

void spot_place_game_center_expression_19(void) {
    if ((D_800B1746 == 7) && (D_800EECBC == 0) && (spot_place_variant == 1)) {
        func_8004A7F4();
        return;
    }
    func_80011DFC();
}

void spot_place_game_center_expression_21(void) {
    if ((D_800B1746 == 7) && (D_800EECBC == 0) && (spot_place_variant == 1)) {
        func_8004A764();
        return;
    }
    func_80011DFC();
}

void spot_place_karaoke_speak_branch_14(void) {
    if ((D_800B1746 == 7) && (D_800EECBC == 2) && (spot_place_variant == 1)) {
        func_8004CF30();
        return;
    }
    func_80011DFC();
}

void spot_place_amusement_park_speak_branch_57(void) {
    func_8004CF30();
}

void spot_place_library_branch_04(void) {
    if ((D_800B1746 == 8) && (D_800B0A04[8].unk_22 != 0)) {
        D_800B1AF6 += 0x19;
    }
    func_80011DFC();
}

void spot_place_sea_speak_branch_25(void) {
    if (D_800B1746 == 5 && spot_place_variant == 0 && D_800EECBC == 2) {
        func_8004CF30();
    }
    if (D_800B1746 == 5 && spot_place_variant == 1 && D_800EECBC == 0) {
        func_8004CF30();
        return;
    }
    func_80011DFC();
}

void spot_place_sea_face_step_26(void) {
    if (D_800B1746 == 5 && spot_place_variant == 0 && D_800EECBC == 2) {
        if (D_800B1AF0++ == 0) {
            func_8004A8EC(3);
            func_80033E88();
        }
        func_8004CF30();
        return;
    }
    func_80011DFC();
}

void spot_place_shrine_speak_branch_44(void) {
    if ((D_800B1746 == 9) && (D_800EECBC == 2)) {
        func_8004CF30();
        return;
    }
    func_80011DFC();
}

void spot_shared_speak_branch_1(void) {
    if ((D_800B1746 == 0xA) && (D_800EECBC == 1) && (spot_place_variant == 1)) {
        func_8004CF30();
        return;
    }
    func_80011DFC();
}

void spot_place_unused_speak_a(void) {
    if (D_800B1746 == 0xA && D_800EECBC == 0 && spot_place_variant == 0) {
        func_8004CF30();
    }
    if (D_800B1746 == 0xA && D_800EECBC == 0 && spot_place_variant == 1 && D_800B0956 < 0x78) {
        func_8004CF30();
        return;
    }
    func_80011DFC();
}

void spot_place_art_museum_speak_branch_20(void) {
    if ((D_800B1746 == 0xA) && (D_800EECBC == 2) && (spot_place_variant == 4)) {
        func_8004CF30();
        return;
    }
    func_80011DFC();
}

void spot_place_unused_speak_b(void) {
    if ((D_800B1746 == 0xA) && (D_800EECBC == 0) && (spot_place_variant == 0)) {
        func_8004CF30();
        return;
    }
    func_80011DFC();
}

void spot_place_bowling_alley_speak_branch_15(void) {
    if ((D_800B1746 == 0xA) && (D_800EECBC == 1) && (spot_place_variant == 1)) {
        func_8004CF30();
        return;
    }
    func_80011DFC();
}

void spot_place_amusement_park_speak_branch_55(void) {
    if (D_800B1746 == 0xA) {
        if (D_800EECBC == 1) {
            if ((u8)spot_ride == 2) {
                if (spot_place_variant == 1) {
                    if (D_800B1AF0++ == 0) {
                        D_80094730 = 0x37;
                    }
                    func_8004CF30();
                    return;
                }
            }
        }
    }
    func_80011DFC();
}

void spot_place_shrine_expression_03(void) {
    if ((D_800B1746 == 5) || (D_800B1746 == 7)) {
        func_8004A764();
        return;
    }
    func_8004A734();
}

void spot_place_shopping_street_expression_29(void) {
    if (D_800B1746 == 9) {
        func_8004A734();
        return;
    }
    func_8004A764();
}

void spot_date_script_load_step(void) {
    switch (D_800B0F30) {
    case 0:
        spot_fill_sections_shiori();
        func_80015D28(0x17, 0x80198000, 0x9ED8);
        break;
    case 1:
        spot_fill_sections_mio();
        func_80015D28(0x18, 0x80198000, 0x9F31);
        break;
    case 2:
        spot_fill_sections_yuina();
        func_80015D28(0x15, 0x80198000, 0x9EEF);
        break;
    case 3:
        spot_fill_sections_ayako();
        func_80015D28(0x16, 0x80198000, 0x9F04);
        break;
    case 4:
        spot_fill_sections_saki();
        func_80015D28(0x16, 0x80198000, 0x9F8A);
        break;
    case 5:
        spot_fill_sections_yukari();
        func_80015D28(0x17, 0x80198000, 0x9F49);
        break;
    case 6:
        spot_fill_sections_nozomi();
        func_80015D28(0x15, 0x80198000, 0x9F60);
        break;
    case 7:
        spot_fill_sections_mira();
        func_80015D28(0x17, 0x80198000, 0x9F1A);
        break;
    case 8:
        spot_fill_sections_yuko();
        func_80015D28(0x13, 0x80198000, 0x9EC5);
        break;
    case 9:
        spot_fill_sections_megumi();
        func_80015D28(0x15, 0x80198000, 0x9F75);
        break;
    default:
        spot_fill_sections_yumi();
        func_80015D28(0x16, 0x80198000, 0x9FA0);
        break;
    }
    func_80011DFC();
}

void spot_common_script_load_step(void) {
    func_80015D28(2, 0x80197000, 0xA0E0);
    spot_fill_sections_common();
    func_80011DFC();
}

void spot_place_tables_load_step(void) {
    func_80015D28(3, 0x801A4000, 0xA143);
    spot_place_result_pointers_init();
    func_80011DFC();
}

s32 spot_place_sea_step_11(void) {
    if (spot_place_variant == 1) {
        switch (D_800B1746) {
        case 0:
        case 3:
        case 4:
        case 5:
        case 8:
        case 9:
            func_80011DFC();
            return 0;
        }
    }
    return func_80045FF4();
}

void spot_place_bank_load_step(void) {
    s32 var_a1; /* declared above the tables: their stack slots sit 4 bytes lower (T-22024) */
    SpotPlaceHalfwords sp90;
    SpotPlaceHalfwords sp64;
    SpotPlaceHalfwords sp38;
    s32 var_a2;

    sp90 = spot_place_bank_argument_f;
    sp64 = spot_place_bank_argument_d;
    sp38 = spot_place_bank_argument_e;
    var_a1 = 0xAF42;
    var_a2 = 0xAF23;
    if (spot_place == 2) {
        var_a1 = 0xB384;
        var_a2 = 0xB381;
    } else if (spot_place == 0x11) {
        var_a1 = 0xB3DA;
        var_a2 = 0xB3CB;
    } else if ((spot_place == 1) || (spot_place == 3)) {
        var_a1 = 0xB163;
        var_a2 = 0xB14D;
    } else if ((spot_place == 0xA) || (spot_place == 9)) {
        var_a1 = 0xB172;
        var_a2 = 0xB165;
    } else if (spot_place == 0xC) {
        var_a1 = 0xB444;
        var_a2 = 0xB42F;
    } else if ((spot_place == 0x11) || (spot_place == 0x10)) { /* the 0x11 half is dead; the original tests it twice */
        var_a1 = 0xB3DA;
        var_a2 = 0xB3CB;
    } else if ((spot_place == 0x14) || (spot_place == 0x13)) {
        var_a1 = 0xB41D;
        var_a2 = 0xB410;
    } else if (spot_place == 8) {
        var_a1 = 0xB054;
        var_a2 = 0xB04A;
    } else if (spot_place == 7) {
        var_a1 = 0xB304;
        var_a2 = 0xB2EB;
    }
    func_800143DC(1, var_a1, var_a2, sp64.v[spot_place], sp38.v[spot_place], sp90.v[spot_place]);
    func_80011DFC();
}

void spot_place_bgm_start_step(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    SpotPlaceHalfwords tbl;

    tbl = spot_place_bgm_ids;
    if (func_8001496C() == 1) {
        idx = spot_place;
        func_800433D0(tbl.v[idx]);
        func_80011DFC();
    }
}

void spot_place_shopping_street_bgm_start_11(void) {
    if ((u8) D_800B1AF6 >= 0x14U) {
        func_800433D0(0x203);
    } else if (D_80125324 == 0) {
        func_800433D0(0x200);
    } else if (D_80125324 == 1) {
        func_800433D0(0x201);
    } else {
        func_800433D0(0x202);
    }
    func_80011DFC();
}

s32 spot_meeting_names_step(void) {
    /* FAKE: unused word reserves top frame slot so SpotPlaceNameTable tbl sits at sp+0x2C in a 0x308 frame; T-11110 */
    s32 pad;
    SpotPlaceNameTable tbl;

    tbl = spot_place_names;
    if (((D_800B0F2D == D_800B0F2C) && (spot_arrival_mode == 1)) || (D_800B0F2E == 0xFF)) {
        D_800B1AF6 += 0x11;
        return;
    }
    if (D_800B0F2E == D_800B0F2C) {
        func_80078970(D_80094818, tbl.unk_00[D_800B0F2D]);
    } else {
        func_80078970(D_80094818, tbl.unk_00[(s32)D_800B0F2E]);
    }
    if (D_800B0F2F != 0xFF) {
        D_80124384 = 3;
        if (D_800B0F2F == D_800B0F2C) {
            func_80078970(D_8009483C, tbl.unk_00[D_800B0F2D]);
        } else {
            func_80078970(D_8009483C, tbl.unk_00[(s32)D_800B0F2F]);
        }
    }
    func_80011DFC();
}

void spot_meeting_decide_step(void) {
    u8 v;
    u8 sel;

    /* FAKE: dead load before the first branch lets cvt_pass strip the CVTs of spot_arrival_mode, so uopt keeps it in $a1; T-11600 */
    sel = spot_arrival_mode;
    if ((D_800B0F2D == D_800B0F2C) && (D_800EECBC == 0)) {
        D_801243B4 = 1;
        spot_dialogue_group_offset += 1;
        D_800B1AF6 += 0xD;
        return;
    }
    if ((D_800B0F2D != D_800B0F2C) && (D_800EECBC == 0)) {
        D_800B1AF6 += 0x1E;
        return;
    }
    D_80124384 += D_800EECBC;
    spot_arrival_mode = 2;
    v = func_8002328C(D_800B1746) & 0x7F;
    spot_dialogue_group_offset = ((v >= 2) + (v >= 3)) * ((spot_arrival_mode == 0) + 1) + spot_arrival_mode * 6 - (spot_arrival_mode == 2) * 3;
    if (D_800EECBC == 1) {
        if (D_800B0F2E == D_800B0F2C) {
            spot_place = D_800B0F2D;
        } else {
            spot_place = D_800B0F2E;
        }
    }
    if (D_800EECBC == 2) {
        if (D_800B0F2F == D_800B0F2C) {
            spot_place = D_800B0F2D;
        } else {
            spot_place = D_800B0F2F;
        }
    }
    func_80011DFC();
}

void spot_meeting_place_check_step(void) {
    if (D_800B0F2D != spot_place) {
        D_80124384 = 7;
        D_800B1AF6 += 0x14;
        return;
    }
    func_80011DFC();
    func_80011DFC();
    *(s16 *)(D_800B0860 + D_800B1746 * 0x34 + 0x1AE) += 8;
    func_8004C250();
}

void spot_meeting_choice_menu_step(void) {
    if (D_800B0F2F != 0xFF) {
        func_8004A2C4();
        return;
    }
    func_8004A4D8();
}

void spot_arrival_wait_step(void) {
    if ((spot_arrival_mode == 0) && (D_801243B4 == 0)) {
        func_80046B88(0x80);
        return;
    }
    func_80011DFC();
}

void spot_bustup_addr_init_step(void) {
    func_80045B14();
    func_80011DFC();
}

void spot_cell_seven_step(void) {
    D_80124384 = 7;
    func_80011DFC();
}

void spot_low_looks_penalty_step(void) {
    u8 *q;

    if (D_800B0966 < 0x1E || (D_800B0966 < 0x23 && D_800B1746 == 7)) {
        spot_dialogue_group_offset = 0xC;
        /* FAKE: D_800B1AF6 (= D_800B1746 + 0x3B0) is updated through D_800B1746 so that as1 keeps the next load of D_800B1746 after the store; real source unknown. T-4050 */
        (&D_800B1746)[0x3B0] += 0xF;
        /* FAKE: the record address goes through (u32) so IDO does not keep D_800B1746 in a register across the halfword stores (the original reloads it each time); real source unknown. T-4050 */
        q = (u8 *)((u32)D_800B0860 + D_800B1746 * 0x34);
        *(s16 *)(q + 0x1A6) -= 1;
        q = (u8 *)((u32)D_800B0860 + D_800B1746 * 0x34);
        *(s16 *)(q + 0x1AA) -= 1;
        q = (u8 *)((u32)D_800B0860 + D_800B1746 * 0x34);
        *(s16 *)(q + 0x1AE) += 5;
        func_8004C250(0x34);
        return;
    }
    func_80011DFC();
}

void spot_place_shrine_still_show_36(void) {
    s16 i;
    ActorFixed *p;

    if ((u8)((SpotRideMovieWork *)D_800F1990)->unk_19 == 1) {
        for (i = 11; i < 16; i++) {
            ((SpotDevelopmentEffectView *)D_800B0860)->unk_13A8[i] = -1;
            ((SpotDevelopmentEffectView *)D_800B0860)->unk_13A8[i + 32] = -1;
        }
        ((SpotRideMovieWork *)D_800F1990)->unk_06 = 0;
    } else {
        for (i = 21; i < 26; i++) {
            ((SpotDevelopmentEffectView *)D_800B0860)->unk_13A8[i] = -1;
            ((SpotDevelopmentEffectView *)D_800B0860)->unk_13A8[i + 16] = -1;
        }
        ((SpotRideMovieWork *)D_800F1990)->unk_04 = 0;
    }
    D_80125864 = 0x80;
    D_800EECF0 = 1;
    D_8012585C = 0;
    D_80125860 = 1;
    spot_place_asset_pointers_init();
    func_80012D64(D_80094710, 15, 1, 2, 0);
    ((ActorFixed *)D_800E9620)[31].unk_43 = 15;
    ((SpriteQuad *)D_800EC190)[0].unk_12 = 0x1EF;
    ((SpriteQuad *)D_800EC190)[1].unk_12 = 0x1EF;
    for (i = 16; i < 32; i++) {
        func_80012D64(D_80125280, i & 0xFF, 1, 0, i - 16);
    }
    func_80012D64(D_80125284, 14, 1, 0, 0);
    func_80012D64(D_80125288, 13, 1, 0, 0);
    for (i = 0; i < 10; i++) {
        func_800189C4(i + 96);
        p = (ActorFixed *)D_800E9620 + i;
        p[96].unk_01 = 13;
        p[96].flags = 1;
        p[96].quad_flags = 0x1000000;
        p[96].draw_flags = 4;
        p[96].frame_table = D_80125274;
        p[96].sequence_table = D_80125278;
        p[96].tpage_table = spot_place_asset_pointers;
        p[96].unk_14 = D_8012527C;
        p[96].sequence_slot = 3;
        p[96].frame = 0;
        p[96].unk_06 = 1;
        p[96].unk_43 = 0;
        p[96].x.h[1] = (func_800789A0() % 256) - 0x80;
        p[96].y.h[1] = 0xA0;
        p[96].unk_07 = 0;
        p[96].unk_05 = 0x80;
        p[96].unk_04 = 8;
        p[96].dx = (func_800789A0() % 0x8000) - 0x4000;
        D_801257DC[i] = ((u32)-(func_800789A0() % 9) << 16) - 0x280000;
    }
    ((ActorFixed *)D_800E9620)[96].draw_flags |= 0x80;
    func_8004C46C(D_8012528C, D_80125290, D_80125294, D_80125298, D_8012529C, D_801252A0);
    for (i = 0; i < 6; i++) {
        ((SpriteQuad *)D_800EC190)[i + 4].flags &= 0x7FFFFFFF;
        ((SpriteQuad *)D_800EC190)[i + 4].unk_12 = 0x1EE;
        ((SpotDevelopmentEffectView *)D_800B0860)->unk_1031[i] = 12;
    }
    for (i = 0; i < 6; i++) {
        ((SpriteQuad *)D_800EC190)[i + 4].tint_r = ((SpriteQuad *)D_800EC190)[i + 4].tint_g = ((SpriteQuad *)D_800EC190)[i + 4].tint_b = 0x80;
    }
    func_80011DFC();
}

void spot_place_shrine_step_37(void) {
    func_80046B88(0x3E8);
}

/* FAKE: the first chain is dead (uopt drops its stores as overwritten); it moves the shared `li v0,0x80` above the second `lui $at` as in the original. Twin of DATE func_8013FA6C, T-9380 */
void spot_place_shrine_set_state_38(void) {
    (&D_800EC1C8)[0] = (&D_800EC1C9)[0] = (&D_800EC1CA)[0] = (&D_800EC1A4)[0] = (&D_800EC1A5)[0] = (&D_800EC1A6)[0] = (&D_800E9E63)[0] = 0x80;
    D_800EECD0 = 1;
    (&D_800EC1C8)[0] = (&D_800EC1C9)[0] = (&D_800EC1CA)[0] = (&D_800EC1A4)[0] = (&D_800EC1A5)[0] = (&D_800EC1A6)[0] = (&D_800E9E63)[0] = 0x80;
    func_80011DFC();
}

void spot_place_shrine_palette_load_46(void) {
    s16 i;

    D_80125864 -= 4;
    ((ActorFixed *)D_800E9620)[31].unk_07 = D_80125864;
    ((SpriteQuad *)D_800EC190)[0].tint_r = ((SpriteQuad *)D_800EC190)[0].tint_g = ((SpriteQuad *)D_800EC190)[0].tint_b = ((ActorFixed *)D_800E9620)[31].unk_07;
    ((SpriteQuad *)D_800EC190)[1].tint_r = ((SpriteQuad *)D_800EC190)[1].tint_g = ((SpriteQuad *)D_800EC190)[1].tint_b = ((ActorFixed *)D_800E9620)[31].unk_07;
    for (i = 0; i < 4; i++) {
        ((ActorFixed *)D_800E9620)[i + 96].unk_07 = ((ActorFixed *)D_800E9620)[31].unk_07;
    }
    for (i = 0; i < 6; i++) {
        ((SpriteQuad *)D_800EC190)[i + 4].tint_r = ((SpriteQuad *)D_800EC190)[i + 4].tint_g = ((SpriteQuad *)D_800EC190)[i + 4].tint_b = ((ActorFixed *)D_800E9620)[31].unk_07;
    }
    if (((ActorFixed *)D_800E9620)[31].unk_07 == 0) {
        D_800EECF0 = 0;
        D_800EECD0 = 0;
        ((ActorFixed *)D_800E9620)[31].draw_flags &= ~0x80;
        ((SpriteQuad *)D_800EC190)[0].tint_r = ((SpriteQuad *)D_800EC190)[0].tint_g = ((SpriteQuad *)D_800EC190)[0].tint_b = 0x80;
        ((SpriteQuad *)D_800EC190)[1].tint_r = ((SpriteQuad *)D_800EC190)[1].tint_g = ((SpriteQuad *)D_800EC190)[1].tint_b = 0x80;
        ((SpriteQuad *)D_800EC190)[0].flags |= 0x80000000;
        ((SpriteQuad *)D_800EC190)[1].flags |= 0x80000000;
        for (i = 0; i < 6; i++) {
            ((SpriteQuad *)D_800EC190)[i + 4].flags |= 0x80000000;
        }
        func_80012D64(D_80094710, 0x10, 1, 2, 0);
        for (i = 11; i < 16; i++) {
            ((SpotDevelopmentEffectView *)D_800B0860)->unk_13A8[i] = 0x77;
            ((SpotDevelopmentEffectView *)D_800B0860)->unk_13A8[i + 32] = 0x77;
        }
        for (i = 21; i < 26; i++) {
            ((SpotDevelopmentEffectView *)D_800B0860)->unk_13A8[i] = 0x77;
            ((SpotDevelopmentEffectView *)D_800B0860)->unk_13A8[i + 32] = 0x77;
        }
        func_80011DFC();
    }
}

void spot_particles_update(void) {
    s16 i;
    s32 y_diff;
    u8 unused;

    /* FAKE: dead load before the first call lets cvt_pass strip the CVTs of D_800B1AF6, so uopt keeps it in $a1; T-24103 */
    unused = D_800B1AF6;
    D_8012585C += 1;
    if ((D_8012585C % ((func_800789A0() % 60) + 60)) == 0) {
        if (D_80125860 < 4) {
            D_80125860 += 1;
            ((ActorFixed *)D_800E9620)[D_80125860 + 95].sequence_slot = 3;
            ((ActorFixed *)D_800E9620)[D_80125860 + 95].draw_flags |= 0x80;
            ((ActorFixed *)D_800E9620)[D_80125860 + 95].frame = 0;
            ((ActorFixed *)D_800E9620)[D_80125860 + 95].unk_08 = 0;
            ((ActorFixed *)D_800E9620)[D_80125860 + 95].flags = 1;
            ((ActorFixed *)D_800E9620)[D_80125860 + 95].y.h[1] = 0xA0;
            ((ActorFixed *)D_800E9620)[D_80125860 + 95].dx = (func_800789A0() % 0x8000) - 0x4000;
            D_801257DC[D_80125860 - 1] = ((u32)-(func_800789A0() % 9) << 16) - 0x480000;
            ((ActorFixed *)D_800E9620)[D_80125860 + 95].x.h[1] = (func_800789A0() % 256) - 0x80;
        }
    }
    if (D_800B1AF6 >= 0x26U) {
        D_80125860 = 1;
        for (i = 1; i < 4; i++) {
            ((ActorFixed *)D_800E9620)[i + 96].draw_flags &= 0x7F;
        }
    }
    for (i = 0; i < D_80125860; i++) {
        if (((ActorFixed *)D_800E9620)[i + 96].sequence_slot == 3) {
            ((ActorFixed *)D_800E9620)[i + 96].y.w += (((Fix32 *)D_801257DC)[i].w - ((ActorFixed *)D_800E9620)[i + 96].y.w) >> 5;
            ((ActorFixed *)D_800E9620)[i + 96].x.w += ((ActorFixed *)D_800E9620)[i + 96].dx;
            y_diff = ((ActorFixed *)D_800E9620)[i + 96].y.h[1] - ((Fix32 *)D_801257DC)[i].h[1];
            ((ActorFixed *)D_800E9620)[i + 96].unk_07 = (y_diff / 2) + 0x40;
            if (y_diff < 10) {
                ((ActorFixed *)D_800E9620)[i + 96].sequence_slot = (((ActorFixed *)D_800E9620)[i + 96].sequence_slot - (func_800789A0() % 2)) - 1;
                if ((D_800B1AF6 == 0x25) && ((u32)D_800B1AF0 >= 0x1F5U)) {
                    if (i < 2) {
                        ((ActorFixed *)D_800E9620)[i + 96].sequence_slot = 0;
                    }
                }
                ((ActorFixed *)D_800E9620)[i + 96].frame = 0;
                ((ActorFixed *)D_800E9620)[i + 96].unk_08 = 0;
                ((ActorFixed *)D_800E9620)[i + 96].flags = 3;
                ((ActorFixed *)D_800E9620)[i + 96].unk_07 = 0x80;
            }
        } else if ((((ActorFixed *)D_800E9620)[i + 96].frame == 0) && (((ActorFixed *)D_800E9620)[i + 96].unk_08 == 0)) {
            if ((D_800B1AF6 == 0x25) && ((u32)D_800B1AF0 >= 0xC9U) && ((u32)D_800B1AF0 < 0x1F5U)) {
                ((ActorFixed *)D_800E9620)[i + 96].draw_flags &= ~0x80;
            } else if ((D_800B1AF6 == 0x25) && ((u32)D_800B1AF0 >= 0x2BDU)) {
                ((ActorFixed *)D_800E9620)[i + 96].draw_flags &= ~0x80;
            } else if ((D_800B1AF6 == 0x25) && ((u32)D_800B1AF0 >= 0x1F5U) && (i >= 2)) {
                ((ActorFixed *)D_800E9620)[i + 96].draw_flags &= ~0x80;
            } else {
                func_800433D0(0x500);
                ((ActorFixed *)D_800E9620)[i + 96].draw_flags |= 0x80;
                ((ActorFixed *)D_800E9620)[i + 96].sequence_slot = 3;
                ((ActorFixed *)D_800E9620)[i + 96].frame = 0;
                ((ActorFixed *)D_800E9620)[i + 96].unk_08 = 0;
                ((ActorFixed *)D_800E9620)[i + 96].flags = 1;
                ((ActorFixed *)D_800E9620)[i + 96].y.h[1] = 0xA0;
                ((ActorFixed *)D_800E9620)[i + 96].dx = (func_800789A0() % 0x8000) - 0x4000;
                D_801257DC[i] = ((u32)-(func_800789A0() % 9) << 16) - 0x280000;
                ((ActorFixed *)D_800E9620)[i + 96].x.h[1] = (func_800789A0() % 256) - 0x80;
                if ((D_800B1AF6 == 0x25) && ((u32)D_800B1AF0 >= 0x1F5U)) {
                    if (i == 0) {
                        ((ActorFixed *)D_800E9620)[i + 96].x.h[1] = -0x50;
                    } else if (i == 1) {
                        ((ActorFixed *)D_800E9620)[i + 96].x.h[1] = 0x50;
                    } else {
                        ((ActorFixed *)D_800E9620)[i + 96].draw_flags &= 0x7F;
                    }
                }
            }
        }
    }
    if ((u8)D_80125864 < 0x80U) {
        func_8001A650(-0xA0, -0x78, 0x140, 0xA0, 0xE, D_80125864, 4);
    } else {
        func_8001A650(-0xA0, -0x78, 0x140, 0xA0, 0xE, 0xC0, 4);
    }
    func_8001A16C(0xE);
}

void spot_place_kirameki_central_park_assets_load_08(void) {
    func_80015D28(0x22, 0x801E0000, 0xABEA);
    spot_place_frame_pointers_init();
    func_80011DFC();
}

void spot_place_kirameki_central_park_step_10(void) {
    s32 drift;
    s16 size_variant;
    s16 i;
    s32 scale;
    s32 divisor;
    s32 random_span;
    s32 choice;
    WordView *position;

    if (spot_place_variant == 2) {
        func_80012D64(D_801252DC, 0x12, 1, 2, 0);
    } else {
        func_80012D64(D_801252E0, 0x12, 1, 2, 0);
    }
    for (i = 0; i < 0x14; i++) {
        func_800189C4(i + 0x62);
        ((ActorFixed *) D_800E9620)[i + 98].quad_flags = 0x01000000;
        ((ActorFixed *) D_800E9620)[i + 98].unk_01 = 0xD;
        ((ActorFixed *) D_800E9620)[i + 98].flags = 1;
        ((ActorFixed *) D_800E9620)[i + 98].draw_flags = 4;
        if (spot_place_variant == 2) {
            ((ActorFixed *) D_800E9620)[i + 98].frame_table = (u8 *)D_801252C4;
            ((ActorFixed *) D_800E9620)[i + 98].sequence_table = (u8 *)D_801252CC;
            ((ActorFixed *) D_800E9620)[i + 98].tpage_table = (u8 *)spot_place_frame_pointers;
        } else {
            ((ActorFixed *) D_800E9620)[i + 98].frame_table = (u8 *)D_801252C8;
            ((ActorFixed *) D_800E9620)[i + 98].sequence_table = (u8 *)D_801252D0;
            ((ActorFixed *) D_800E9620)[i + 98].tpage_table = (u8 *)D_801252C0;
        }
        ((ActorFixed *) D_800E9620)[i + 98].sequence_slot = (s16) (func_800789A0() % 8);
        ((ActorFixed *) D_800E9620)[i + 98].unk_43 = 0x12;
        choice = func_800789A0() % 2;
        scale = 0x1000 / ((s32) ((((s16) choice) * 2) + 2));
        size_variant = (s16) choice;
        ((ActorFixed *) D_800E9620)[i + 98].unk_1A = (s16) scale;
        ((ActorFixed *) D_800E9620)[i + 98].unk_1C = (s16) scale;
        ((ActorFixed *) D_800E9620)[i + 98].unk_07 = 0x80;
        position = &D_80125868[i];
        ((ActorFixed *) D_800E9620)[i + 98].x.h[1] = (s16) ((func_800789A0() % 300) - 0x96);
        position->w = (s32) ((ActorFixed *) D_800E9620)[i + 98].x.w;
        random_span = func_800789A0();
        divisor = size_variant + 1;
        drift = ((s32) (((((func_800789A0() % 2) * 2) - 1) * (random_span % 8)) + 8)) / divisor;
        position->h[1] = (s16) (((s16) position->h[1]) + drift);
        ((ActorFixed *) D_800E9620)[i + 98].y.h[1] = (s16) ((func_800789A0() % 160) - 0x78);
        ((ActorFixed *) D_800E9620)[i + 98].dy = (s32) (0x8000 / divisor);
    }

    func_80011DFC();
}

void spot_place_falling_sprites_hide(void) {
    s16 i;

    for (i = 0; i < 20; i++) {
        ((Actor *)D_800E9620)[i + 98].draw_flags &= 0x7F;
    }
}

/* s32 with no return (K&R): the early returns set $v0 = 0; falling off the loop leaves $v0 untouched. */
s32 spot_place_falling_sprites_update(void) {
    s16 i;
    s16 size_variant;
    s32 choice;
    s32 scale;
    s32 random_span;
    s32 divisor;

    if (spot_place_variant == 1) {
        return 0;
    }
    if ((spot_place_variant != 2) && ((D_800B0896 < 9U) || (D_800B0896 >= 12U))) {
        return 0;
    }
    for (i = 0; i < 20; i++) {
        ((ActorFixed *)D_800E9620)[i + 98].draw_flags |= 0x80;
        ((ActorFixed *)D_800E9620)[i + 98].dx += (D_80125868[i].w - ((ActorFixed *)D_800E9620)[i + 98].x.w) >> 8;
        ((ActorFixed *)D_800E9620)[i + 98].x.w += ((ActorFixed *)D_800E9620)[i + 98].dx;
        ((ActorFixed *)D_800E9620)[i + 98].y.w += ((ActorFixed *)D_800E9620)[i + 98].dy;
        if (((ActorFixed *)D_800E9620)[i + 98].y.h[1] >= 0x29) {
            ((ActorFixed *)D_800E9620)[i + 98].y.h[1] = -0x78;
            choice = func_800789A0() % 2;
            scale = 0x1000 / (((s16)choice * 2) + 2);
            size_variant = (s16)choice;
            ((ActorFixed *)D_800E9620)[i + 98].unk_1A = scale;
            ((ActorFixed *)D_800E9620)[i + 98].unk_1C = scale;
            ((ActorFixed *)D_800E9620)[i + 98].unk_07 = 0x80;
            ((ActorFixed *)D_800E9620)[i + 98].x.h[1] = (func_800789A0() % 300) - 0x96;
            D_80125868[i].w = ((ActorFixed *)D_800E9620)[i + 98].x.w;
            random_span = func_800789A0();
            divisor = size_variant + 1;
            /* FAKE: s16 lvalue cast on WordView.h[1] emits lh with += operand order in ugen. T-24083 */
            *(s16 *)&D_80125868[i].h[1] += ((((func_800789A0() % 2) * 2) - 1) * (random_span % 8) + 8) / divisor;
            ((ActorFixed *)D_800E9620)[i + 98].dy = 0x8000 / divisor;
        }
    }
}

void spot_mira_mood_jump_step(void) {
    u32 v;

    v = func_8002328C(D_800B1746);
    if (D_800B1746 == 7 && v >= 3U && spot_arrival_mode == 2) {
        func_80011EC4(0x39);
        spot_place += 1;
        return;
    }
    func_80011DFC();
}

/* s32 with no return (K&R): the incremented byte stays in $v0 as the return register. */
s32 spot_place_amusement_park_special_case_52(void) {
    /* FAKE: unused word reserves top frame slot so before sits at sp+0x28; T-11600 */
    s32 pad;
    s32 before;
    u8 sel;

    before = D_800B1AF6;
    /* FAKE: dead load before the first call lets cvt_pass strip the CVT of spot_ride, so it goes first in bne; T-11600 */
    sel = spot_ride;
    func_8004CF30();
    if ((D_800B1746 == 9) && (spot_ride == 9)) {
        if (D_800B0BCF == 0) {
            func_8004A8EC(3);
        } else {
            func_8004A8EC(4);
        }
        func_80033E88();
        /* FAKE: (s32) cast gives the original operand order of the beq; T-11600 */
        if (before != (s32)D_800B1AF6) {
            D_800B1AF6 += 5;
        }
    }
}

void spot_skip_step_unless_flag_step(void) {
    if (D_800B0A04[D_800B1746].unk_0C.b12 || D_800B0A04[D_800B1746].unk_0C.b13) {
        func_80011DFC();
        return;
    }
    func_80011DFC();
    func_80011DFC();
}

s32 spot_year_talk_step(void) {
    SpotDevelopmentState *g;
    u8 v;

    g = (SpotDevelopmentState *)&D_800B0860;
    v = func_8002328C(D_800B1746);
    D_80124354 = g->year * 3 + ((v & 0x7F) >= 2U) + ((v & 0x7F) >= 3U) - 0x111;
    D_8009471C = (u8 *)&D_80124354;
    D_80094720 = &D_80124358;
    D_80094724 = D_80124258;
    D_80094728 = D_801242C4;
    D_8009472C = D_80124330;
    return func_80049B20(D_800EECBC, 1, 0);
}
