/* annual_scene_setup.c: ; src/ovl/GYOZI/801388E0.c:18-43 initializes sound, sector data and scene state. Original object 0x801388E0-0x80138B50 (config/objects/GYOZI.txt). */
#include "common.h"
#include "ovl/annual_events.h"

typedef struct {
    void (*f[30])();
} FnTbl30; /* size 0x78 */
extern FnTbl30 D_801471A0;

void annual_scene_setup_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl30 tbl;

    tbl = D_801471A0;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

void annual_scene_setup_resource_pointer_init(void) {
    func_800504CC(1, 0xB290, 0xB271, 0xBB4E, 0xBB06, 0xBAF5);
    func_8004DE1C();
}

void annual_scene_setup_conditional_sound(void) {
    if (func_80050AB8() == 1) {
        func_80086AB0(0x201);
        func_8004DE1C();
    }
}

void annual_scene_setup_resource_command(void) {
    func_80051DD8(7, 0x80197000, 0xA464);
    annual_scene_resource_addresses_init();
    func_8004DE1C();
}

INCLUDE_RODATA("asm/ovl/GYOZI/data/annual_events/annual_scene_setup.rodata", D_801459E0);

void annual_scene_setup_state_init(void) {
    D_800F5AAE |= 0x10;
    func_8004EDE0(1, 0);
    func_80054864(1);
    func_8004C670();
    func_8005493C(0);
    func_8004EDF4(1);
    func_80053EFC();
    func_8005ABC0();
    func_800BCE10(&D_800D92A0, &D_801459E0);
    D_800F6412 = 0;
    D_800F6458 = 1;
    func_8009068C();
    D_800F53DA = 0x80;
    D_801317EB = 0;
    D_800C51C4 = 0;
    func_80089200();
    func_800744A0();
    func_80074950();
    func_8008FC10();
    annual_scene_resource_addresses_init();
    annual_scene_group_talk_text_table = D_80147100;
    annual_scene_group_talk_command_table = D_80147134;
    annual_scene_group_talk_voice_table = D_80147168;
    annual_scene_group_talk_group = 0;
    annual_scene_group_talk_line = 0;
    func_8008F618(D_800F62CF = 0xC);
    D_800F5AAE |= 8;
    func_8004DE1C();
}
