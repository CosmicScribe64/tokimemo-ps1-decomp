#define MAIN_API_OVERRIDE_D_80122CD0 /* matched as u32 (main_api.h: s32), switch selector in $v1 (T-6010) */
#include "common.h"
#include "ovl/GEKO.h"

extern u32 D_80122CD0;

typedef struct {
    void (*f[41])();
} FnTbl41; /* size 0xA4 */
extern FnTbl41 D_801466D8;

void func_8013AF80(void) {
    D_80146600 = 0x801D212C;
    D_80146604 = 0x801D2130;
    D_80146608 = 0x801D2150;
    D_8014660C = (*(s16 *)0x801D216C);
    D_80146610 = 0x801B0000;
    D_80146614 = 0x801B2000;
    D_80146618 = 0x801B6000;
    D_8014661C = 0x801BA000;
    D_80146620 = 0x801BE000;
    D_80146624 = 0x801C2000;
    D_80146628 = 0x801C6000;
}

void func_8013B030(void) {
    switch (D_80122CD0) {
    case 1:
        func_8013B094();
        return;
    case 7:
        func_8013B0D0();
        return;
    default:
        func_80046500();
        return;
    }
}

void func_8013B094(void) {
    if (D_800E6280.stage == 0) {
        func_8013B10C();
        return;
    }
    func_80046500();
}

void func_8013B0D0(void) {
    if (D_800E6280.stage == 0) {
        func_8013B2C8();
        return;
    }
    func_80046500();
}

s32 func_8013B10C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    s32 level;
    GekoFnTbl42 tbl;

    tbl = D_80146630;
    idx = D_800E6280.step;
    tbl.f[idx](0x80);
    if (D_80122D04 != 0) {
        /* FAKE: DisplayActor44 view on the first two stores keeps as1 from hoisting the unk_03C load above sb D_80120723. T-24095 */
        ((DisplayActor44 *)D_80120650)[3].brightness = D_800B593C;
        ((DisplayActor44 *)D_80120650)[3].draw_flags |= 0x80;
        if ((u8)D_800E6280.voice_mode != 0) {
            if (!D_8014662C) {
                D_8012071E = 3;
            }
            if (D_80120734 == 6 && !D_8014662C) {
                D_8012071E = 0;
            }
            if (func_800460EC() & 4) {
                level = getCDlevel();
            } else {
                level = 0;
            }
            if (level != 0) {
                if (!D_8014662C) {
                    D_8012071E = 3;
                    D_8014662C = 1;
                }
            }
        } else if (!D_8014662C) {
            D_8012071E = 3;
            D_80120734 = 1;
            D_8014662C = 1;
        }
    } else {
        D_8012071F &= 0xFF7F;
    }
}

void func_8013B290(void) {
    cd_read_sectors(0x45, 0x801B0000, 0x80FF);
    func_8013AF80();
    func_8004284C();
}

void func_8013B2C8(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl41 tbl;

    tbl = D_801466D8;
    idx = D_800E6280.step;
    tbl.f[idx](0x80);
}

void func_8013B350(void) {
    D_80122CE0 = 6;
    func_80048EB8(0);
    func_8006BD6C(0);
    D_800E6280.saved_mode = D_800E6280.mode;
    D_800E6280.saved_stage = D_800E6280.stage;
    D_800E6280.saved_step = D_800E6280.step + 1;
    main_state_set(0x81);
}

void func_8013B3C0(void) {
    s32 pad; /* FAKE: unused scalar reproduces RECT home at +0x2C; original local unknown. T-9770 */
    RECT r;

    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    func_8006BD6C(0);
    back_clear_switch(1);
    tpage_buf_clear();
    func_8004E58C();
    func_8008585C();
    k_reset(1);
    r.x = 0x140;
    r.y = 0x100;
    r.w = 0x140;
    r.h = 0xF8;
    func_8009C7F8(&r, 8, 0, 0);
    r.x = 0x2C0;
    r.y = 0;
    func_8009C7F8(&r, 8, 0, 0);
    D_800E6280.tile_layer_enabled = 0;
    D_800E6280.quads_enabled = 1;
    /* FAKE: scalar byte store then member read preserves argument scheduling; sibling func_8013CC20 uses the same idiom. T-9770 */
    *(u8 *)&D_800E6280.current_girl = 2;
    func_800847B8(D_800E6280.current_girl);
    addr_init_bustup();
    D_800E6280.screen_brightness = 0x80;
    D_800B593C = 0;
    hizuke_init();
    message_window_init();
    func_80084E4C();
    func_80137990();
    D_80146274 = D_80145F10;
    D_80146278 = D_8014604C;
    D_8014627C = D_80146188;
    func_8004284C();
}

void func_8013B510(void) {
    D_800E6280.current_girl = 2;
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    back_clear_switch(1);
    tpage_buf_clear();
    func_8004E58C();
    D_8011ED9F = 0;
    D_8011EDE3 = 0;
    D_800CA150 = 0;
    D_800CA154 = 0;
    addr_init_bustup();
    func_80137990();
    hizuke_init();
    message_window_init();
    func_80084E4C();
    func_800847B8(D_800E6280.current_girl);
    func_8008585C();
    func_80137990();
    D_80146274 = D_80145EF8;
    D_80146278 = D_80146034;
    D_8014627C = D_80146170;
    strcpy(D_800CA19C, "屋上");
    D_8014662C = 0;
    D_800E6280.girls[2].tokimeki += 1;
    D_800E6280.girls[2].affection += 2;
    check_para_limit();
    D_800CA368 = 0xE;
    func_8004284C();
}

void func_8013B65C(void) {
    func_8013AF80();
    load_palette(D_80146610, 0x11, 1, 2, 0);
    func_80084E90(D_80146614, D_80146618, D_8014661C, D_80146620, D_80146624, D_80146628);
    func_800850D4(0, 0, 0, 0);
    func_80048F64(0x63);
    D_8012071D = 0xA;
    D_8012071E = 0;
    D_80120754 = 0x41000000;
    D_8012071F = 4;
    D_80120728 = D_80146604;
    D_8012072C = D_80146608;
    D_80120750 = D_80146600;
    D_80120730 = D_8014660C;
    D_80120732 = 4;
    D_80120734 = 0;
    D_80120724 = 0;
    D_80120722 = 1;
    D_8012075F = 0x11;
    D_80120742 = -0xA0;
    D_80120746 = -0x78;
    D_80120723 = 0;
    D_80120721 = 0;
    D_80120720 = 0;
    D_800E6280.tpage_buf[26] = -1;
    func_8004284C();
}

void func_8013B7D0(void) {
    D_800E6280.year = 0x62;
    D_800E6280.month = 2;
    D_800E6280.day = 0x1C;
    D_800E6280.weekday = 5;
    D_800E6280.current_girl = 2;
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    back_clear_switch(1);
    tpage_buf_clear();
    func_8004E58C();
    D_800CA150 = 0;
    D_800CA154 = 0;
    addr_init_bustup();
    func_80137990();
    hizuke_init();
    message_window_init();
    func_80084E4C();
    func_800847B8(D_800E6280.current_girl);
    func_8008585C();
    func_80137990();
    D_80146274 = D_80145F10;
    D_80146278 = D_8014604C;
    D_8014627C = D_80146188;
    strcpy(D_800CA19C, "近所の公園");
    D_800CA368 = 0x14;
    func_8004284C();
}

void func_8013B908(void) {
    bg_read_sub2(0x412C);
    func_8004284C();
}
