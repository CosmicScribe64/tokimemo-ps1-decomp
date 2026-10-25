/* yoshio_scenes.c: src/ovl/EVENT/8010B360.c:4: girl 13 (Yoshio) scene 1. Original object 0x8010B360-0x8010BB90 (config/objects/EVENT.txt). */
#include "common.h"
#include "ovl/EVENT.h"

void spot_yoshio_still_pointers_1_init(void) {
    spot_yoshio_still_pointers_1 = 0x801CE130;
    D_801240F4 = 0x801CE134;
    D_801240F8 = 0x801CE15C;
    D_801240FC = *(s16 *)0x801CE170;
    D_80124100 = 0x801B0000;
    D_80124104 = 0x801B2000;
    D_80124108 = 0x801B6000;
    D_8012410C = 0x801BA000;
    D_80124110 = 0x801BE000;
    D_80124114 = 0x801C2000;
    D_80124118 = 0x801C6000;
}

void spot_yoshio_scene_select(void) {
    spot_yoshio_scene1_run();
}

void spot_yoshio_scene1_run(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        spot_yoshio_scene1_steps_run();
    } else {
        func_80015FE0();
    }
}

void spot_yoshio_scene1_steps_run(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    SpotYukoYuinaAyakoMiraAndYoshioSceneStepTable tbl;

    tbl = spot_yoshio_scene1_steps;
    func_80078950("s%d %d\n", D_800B1AF6, D_80094714);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
    if (D_8012411C != 0) {
        func_80033E88();
    }
}

void spot_yoshio_scene1_assets_load_01(void) {
    func_80015D28(0x3D, 0x801B0000, 0x8C15);
    spot_yoshio_still_pointers_1_init();
    func_80011DFC();
}

void spot_yoshio_scene1_step_36(void) {
    spot_talk_step();
    D_800EECD0 = 0;
    D_800E9E5F |= 0x80;
    (&D_800E9E5F)[-0x44] |= 0x80;
    *(s32 *)D_800EC190 &= 0x7FFFFFFF;
    *(s32 *)(D_800EC190 + 0x24) &= 0x7FFFFFFF;
    if (D_800B1AE4 % 240U == 0) {
        func_8003424C();
    }
}

/* FAKE: byte lvalues keep the shared constant in a0 and prevent the later
 * index read from being promoted. The opaque record stride also preserves
 * index/base allocation; the canonical SpotDevelopmentState flag member differs
 * by 26 rows. T-9770/T-20620 recipe verified in T-24053. */
void spot_yoshio_scene1_names_set_23(void) {
    u8 *p;

    *(u8 *)&D_800B1746 = 4;
    func_8004BC20(D_800B1746);
    p = D_800B0860 + *(u8 *)&D_800B1746 * 0x34;
    p[0x1B0] &= 0xFFFD;
    func_80011DFC();
}

extern s16 D_800E9E3E;
extern s16 D_800EC194;

/* neighbouring words reached through one base symbol each: as1 keeps the loads after the stores */
void spot_yoshio_scene1_set_state_27(void) {
    (&D_800E9E3E)[0x22] += 0x40;
    D_800E9E3E += 0x40;
    D_800EC194 += 0x40;
    (&D_800EC194)[0x12] += 0x40;
    func_80011DFC();
}

void spot_yoshio_scene1_step_34(void) {
    spot_talk_step();
    if (D_80094718 == 2) {
        D_800EECD0 = 0;
        D_800E9E5F |= 0x80;
        (&D_800E9E5F)[-0x44] |= 0x80; /* FAKE: D_800E9E1B, 0x44 below D_800E9E5F (as in spot_yoshio_scene1_step_36); the record table is unknown. T-9320 */
        D_800E9E74 = 0;
        *(s32 *)D_800EC190 &= 0x7FFFFFFF;
        *(s32 *)(D_800EC190 + 0x24) &= 0x7FFFFFFF;
        if (D_800B1AE4 % 240U == 0) {
            func_8003424C();
        }
    } else {
        D_800EECD0 = 1;
        D_800E9E5F |= 0x80;
        (&D_800E9E5F)[-0x44] |= 0x80; /* FAKE: D_800E9E1B, 0x44 below D_800E9E5F (as in spot_yoshio_scene1_step_36); the record table is unknown. T-9320 */
        *(s32 *)D_800EC190 &= 0x7FFFFFFF;
        *(s32 *)(D_800EC190 + 0x24) &= 0x7FFFFFFF;
        D_800EAFA2 = 0;
        D_800EAFB8 = 0;
        if (D_800B1AE4 % 240U == 0x78) {
            func_80038388();
        }
    }
}

void spot_yoshio_scene1_set_state_33(void) {
    D_80094714 = (u16) D_80094714 + D_800B1746;
    func_80011DFC();
}

void spot_yoshio_scene1_set_state_35(void) {
    D_80094714 = 0xC;
    func_80011DFC();
}

/* T-9740: integer definition without an explicit return matches the original. */
s32 spot_yoshio_scene1_step_37(void) {
    s32 mask;
    u8 value;

    func_8004C984();
    D_800EECD0 = 1;
    value = D_800EAFA7;
    ((Actor *)D_800E9620)[31].brightness = value;
    ((Actor *)D_800E9620)[30].brightness = value;
    func_8004C060(0, value);
    func_8004C060(1, D_800EAFA7);
    if (D_800EAFA7 < 8U) {
        mask = ~0x80;
        D_800EECD0 = 0;
        ((Actor *)D_800E9620)[31].draw_flags &= mask;
        ((Actor *)D_800E9620)[30].draw_flags &= mask;
        ((SpriteQuad *)D_800EC190)[0].flags |= 0x80000000;
        ((SpriteQuad *)D_800EC190)[1].flags |= 0x80000000;
    }
}

void spot_yoshio_scene1_init_00(void) {
    D_800B1746 = 0xD;
    func_80012D2C(1, 0);
    func_8001886C(1);
    func_80010678();
    func_80018944(0);
    func_80012D40(1);
    func_80017E50();
    func_8001FD50();
    func_80036884("");
    D_800E96EF = 0;
    D_800E9733 = 0;
    D_80094714 = 0;
    D_80094718 = 0;
    func_80045B14();
    spot_script_sections_init();
    func_8003535C();
    func_8003580C();
    func_8004C360();
    D_800F19AB = 0;
    func_8004BC20(D_800B1746);
    func_8004CDDC();
    spot_script_sections_init();
    spot_talk_text_table = D_80120634;
    spot_talk_command_table = *(s32 *)&D_80120654;
    spot_talk_voice_table = D_80120674;
    func_80078970(D_80094784, "教室");
    D_8012411C = 0;
    func_80011DFC();
}

void spot_yoshio_scene1_still_show_28(void) {
    spot_yoshio_still_pointers_1_init();
    func_80012D64(D_80124100, 0x11, 1, 2, 0);
    func_8004C46C(D_80124104, D_80124108, D_8012410C, D_80124110, D_80124114, D_80124118);
    func_8004C6B0(D_801240F4, D_801240F8, spot_yoshio_still_pointers_1, (s32) D_801240FC);
    func_80011DFC();
}

void spot_yoshio_scene1_bg_load_04(void) {
    func_800469F4(0x3FF0);
    func_80011DFC();
}

void spot_yoshio_scene1_bg_load_18(void) {
    func_800469F4(0x4600);
    func_80011DFC();
}
