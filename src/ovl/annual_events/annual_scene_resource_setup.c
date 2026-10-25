/* annual_scene_resource_setup.c: ; src/ovl/GYOZI/8013DB50.c:38-58 initializes display/audio scene resources. Original object 0x8013DB50-0x8013DE30 (config/objects/GYOZI.txt). */
#include "common.h"
#include "ovl/annual_events.h"

void annual_scene_mode_step_update(void) {
    annual_scene_mode_step_dispatch();
}

typedef struct {
    void (*f[42])();
} FnTbl42; /* size 0xA8 */
extern FnTbl42 D_80147FB0;

void annual_scene_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl42 tbl;

    tbl = D_80147FB0;
    idx = D_800F647A;
    tbl.f[idx]();
}

void annual_scene_result_flag_update(void) {
    annual_scene_talk_run_current();
    if (annual_scene_talk_line == 2) {
        D_8012E6B0 = 1;
        return;
    }
    D_8012E6B0 = 0;
}

/* Bit 3 of a girl record's flag byte, set as a bit-field store (lbu into another register, ori, sb). */
typedef struct {
    u8 pad0 : 3;
    u8 f : 1;
    u8 pad1 : 4;
} GyoziBit3;

void annual_scene_resource_state_init(void) {
    func_8004EDE0(1, 0);
    func_80054864(1);
    func_8004C670();
    func_8005493C(0);
    func_8004EDF4(1);
    func_80053EFC();
    func_8005ABC0();
    D_800F6412 = 0;
    /* FAKE: D_800F6458 written as the GyoziWork member so IDO keeps the girl[10] bit-field load below this store; the
     * original most likely uses the scalar as annual_scene_group_state_init does. T-17600 */
    D_800F53A0.unk_10B8 = 1;
    /* FAKE: bit-field view of the flag byte; the original record probably declares the field itself. T-17600 */
    ((GyoziBit3 *)&D_800F53A0.girl[10].unk_10[0])->f = 1;
    func_8009068C();
    D_800F53DA = 0x80;
    D_801317EB = 0;
    D_800C51C4 = 0;
    func_80089200();
    func_800744A0();
    func_80074950();
    func_8008FC10();
    /* FAKE: byte-typed chain store and reload, the T-9020 idiom as in annual_scene_group_state_init. T-17600 */
    *(u8 *)&D_800F5ACD = *(u8 *)&D_800F62CF = 0xA;
    func_8008F618(*(u8 *)&D_800F62CF);
    annual_scene_mode_callback_addresses_init();
    annual_scene_talk_text_table = D_80147450;
    annual_scene_talk_command_table = D_80147474;
    annual_scene_talk_voice_table = D_80147498;
    func_8004DE1C();
}

INCLUDE_RODATA("asm/ovl/GYOZI/data/annual_events/annual_scene_resource_setup.rodata", D_80145C40);

void annual_scene_table_0_callback(void) {
    annual_scene_talk_group = 0;
    annual_scene_talk_line = 0;
    func_800BCE10(&D_800D92A0, &D_80145C40);
    func_8004DE1C();
}

void annual_callback_audio_3fc0(void) {
    func_8008A0D4(0x3FC0);
    func_8004DE1C();
}

void annual_scene_selector_increment_below_threshold(void) {
    if ((u8) D_800F5833 < 2U) {
        annual_scene_talk_group += 1;
    }
    func_8004DE1C();
}

void annual_scene_selector_increment_at_threshold(void) {
    if ((u8) D_800F5833 >= 2U) {
        annual_scene_talk_group += 1;
    }
    func_8004DE1C();
}
