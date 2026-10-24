#include "common.h"
#include "ovl/DATE2.h"

void func_80136D50(void) {
    D_8013A6B0 = 0x801B00DC;
    D_8013A6B4 = 0x801B02AC;
    D_8013A6B8 = 0x801B03C8;
    D_8013A6BC = 0x801B04C4;
    D_8013A6C0 = 0x801B0668;
    D_8013A6C4 = 0x801B0808;
    D_8013A6C8 = 0x801B09DC;
    D_8013A6CC = 0x801B0BE8;
    D_8013A6D0 = 0x801B0D8C;
    D_8013A6D4 = 0x801B0F5C;
    D_8013A6D8 = 0x801B1164;
    D_8013A6DC = 0x801B12D8;
    D_8013A6E0 = 0x801B1454;
    D_8013A6E4 = 0x801B00F4;
    D_8013A6E8 = 0x801B02C8;
    D_8013A6EC = 0x801B03D0;
    D_8013A6F0 = 0x801B04DC;
    D_8013A6F4 = 0x801B0680;
    D_8013A6F8 = 0x801B0820;
    D_8013A6FC = 0x801B09F8;
    D_8013A700 = 0x801B0C00;
    D_8013A704 = 0x801B0DA4;
    D_8013A708 = 0x801B0F78;
    D_8013A70C = 0x801B1180;
    D_8013A710 = 0x801B12E0;
    D_8013A714 = 0x801B1470;
    D_8013A718 = 0x801B01A8;
    D_8013A71C = 0x801B0398;
    D_8013A720 = 0x801B03F0;
    D_8013A724 = 0x801B0590;
    D_8013A728 = 0x801B0734;
    D_8013A72C = 0x801B08D4;
    D_8013A730 = 0x801B0AE4;
    D_8013A734 = 0x801B0CB4;
    D_8013A738 = 0x801B0E58;
    D_8013A73C = 0x801B1064;
    D_8013A740 = 0x801B126C;
    D_8013A744 = 0x801B1338;
    D_8013A748 = 0x801B155C;
}

typedef struct {
    void (*f[8])();
} FnTbl8; /* size 0x20 */
extern FnTbl8 D_8013A74C;

void func_80136FC4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl8 tbl;

    tbl = D_8013A74C;
    idx = game_state.step;
    tbl.f[idx](0x80);
}

void func_80137064(void) {
    cd_read_sectors(3, 0x801B0000, 0xAF43);
    func_80136D50();
    main_step_next();
}

void func_8013709C(void) {
    dialogue_group = 1;
    dialogue_line = 0;
    dialogue_names_set(game_state.current_girl);
    func_801370E4();
    main_step_next();
}

void func_801370E4(void) {
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        dialogue_text_table = D_8013A6B4;
        dialogue_command_table = D_8013A6E8;
        dialogue_voice_table = D_8013A71C;
        return;
    case GIRL_MIO:
        dialogue_text_table = D_8013A6C8;
        dialogue_command_table = D_8013A6FC;
        dialogue_voice_table = D_8013A730;
        return;
    case GIRL_YUINA:
        dialogue_text_table = D_8013A6BC;
        dialogue_command_table = D_8013A6F0;
        dialogue_voice_table = D_8013A724;
        return;
    case GIRL_AYAKO:
        dialogue_text_table = D_8013A6C0;
        dialogue_command_table = D_8013A6F4;
        dialogue_voice_table = D_8013A728;
        return;
    case GIRL_SAKI:
        dialogue_text_table = D_8013A6D8;
        dialogue_command_table = D_8013A70C;
        dialogue_voice_table = D_8013A740;
        return;
    case GIRL_YUKARI:
        dialogue_text_table = D_8013A6CC;
        dialogue_command_table = D_8013A700;
        dialogue_voice_table = D_8013A734;
        return;
    case GIRL_NOZOMI:
        dialogue_text_table = D_8013A6D0;
        dialogue_command_table = D_8013A704;
        dialogue_voice_table = D_8013A738;
        return;
    case GIRL_MIRA:
        dialogue_text_table = D_8013A6C4;
        dialogue_command_table = D_8013A6F8;
        dialogue_voice_table = D_8013A72C;
        return;
    case GIRL_YUKO:
        dialogue_text_table = D_8013A6B0;
        dialogue_command_table = D_8013A6E4;
        dialogue_voice_table = D_8013A718;
        return;
    case GIRL_MEGUMI:
        dialogue_text_table = D_8013A6D4;
        dialogue_command_table = D_8013A708;
        dialogue_voice_table = D_8013A73C;
        return;
    case GIRL_YUMI:
        dialogue_text_table = D_8013A6E0;
        dialogue_command_table = D_8013A714;
        dialogue_voice_table = D_8013A748;
        /* fallthrough */
    default:
        return;
    }
}
