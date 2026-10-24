#include "common.h"
#include "ovl/BUNKAKEN.h"

void func_80132000(void) {
    D_80155E90 = 0x801E8A20;
    D_80155E94 = 0x801E8BC8;
    D_80155E98 = 0x801E92F4;
}

void func_80132034(void) {
    D_80155E90 = 0x801EE270;
    D_80155E94 = 0x801EE93C;
    D_80155E98 = 0x801F2DA4;
}

void func_80132068(void) {
    D_80155E90 = 0x801E829C;
    D_80155E94 = 0x801E82E8;
    D_80155E98 = 0x801E8538;
}

void func_8013209C(void) {
    D_80155E90 = 0x801EBDEC;
    D_80155E94 = 0x801EC320;
    D_80155E98 = 0x801EF54C;
}

void func_801320D0(void) {
    D_80155E90 = 0x801E8630;
    D_80155E94 = 0x801E872C;
    D_80155E98 = 0x801E8BEC;
}

void func_80132104(void) {
    D_80155E90 = 0x801E9C78;
    D_80155E94 = 0x801E9E44;
    D_80155E98 = 0x801EB0AC;
}

void func_80132138(void) {
    D_80155E90 = 0x801E860C;
    D_80155E94 = 0x801E8704;
    D_80155E98 = 0x801E8BA8;
}

void func_8013216C(void) {
    D_80155E90 = 0x801EA384;
    D_80155E94 = 0x801EA614;
    D_80155E98 = 0x801EBFF8;
}

void func_801321A0(void) {
    D_80155E90 = 0x801E862C;
    D_80155E94 = 0x801E8724;
    D_80155E98 = 0x801E8BC8;
}

void func_801321D4(void) {
    D_80155E90 = 0x801EA570;
    D_80155E94 = 0x801EA834;
    D_80155E98 = 0x801EC1DC;
}

void func_80132208(void) {
    D_80155E90 = 0x801E83F4;
    D_80155E94 = 0x801E8458;
    D_80155E98 = 0x801E87C8;
}

void func_8013223C(void) {
    D_80155E90 = 0x801EB18C;
    D_80155E94 = 0x801EB518;
    D_80155E98 = 0x801ED920;
}

void func_80132270(void) {
    D_80155E90 = 0x801E8408;
    D_80155E94 = 0x801E845C;
    D_80155E98 = 0x801E8754;
}

void func_801322A4(void) {
    D_80155E90 = 0x801EC9D4;
    D_80155E94 = 0x801ECE08;
    D_80155E98 = 0x801EFD50;
}

void func_801322D8(void) {
    D_80155E90 = 0x801E8308;
    D_80155E94 = 0x801E8350;
    D_80155E98 = 0x801E85F4;
}

void func_8013230C(void) {
    D_80155E90 = 0x801EC134;
    D_80155E94 = 0x801EC63C;
    D_80155E98 = 0x801EF698;
}

void func_80132340(void) {
    D_80155E90 = 0x801E8494;
    D_80155E94 = 0x801E84E4;
    D_80155E98 = 0x801E8830;
}

void func_80132374(void) {
    D_80155E90 = 0x801ED8DC;
    D_80155E94 = 0x801EDD8C;
    D_80155E98 = 0x801F15D8;
}

void func_801323A8(void) {
    D_80155E90 = 0x801E8564;
    D_80155E94 = 0x801E85F4;
    D_80155E98 = 0x801E89D0;
}

void func_801323DC(void) {
    D_80155E90 = 0x801EC430;
    D_80155E94 = 0x801EC8BC;
    D_80155E98 = 0x801EF910;
}

void func_80132410(void) {
    D_80155E90 = 0x801E838C;
    D_80155E94 = 0x801E83E4;
    D_80155E98 = 0x801E86FC;
}

void func_80132444(void) {
    D_80155E90 = 0x801EA150;
    D_80155E94 = 0x801EA3E0;
    D_80155E98 = 0x801EC088;
}

void func_80132478(void) {
    D_80155E90 = 0x801E840C;
    D_80155E94 = 0x801E845C;
    D_80155E98 = 0x801E87A8;
}

void func_801324AC(void) {
    D_80155E90 = 0x801EB950;
    D_80155E94 = 0x801EBC94;
    D_80155E98 = 0x801EE3D0;
}

void func_801324E0(void) {
    D_80155E90 = 0x801E92FC;
    D_80155E94 = 0x801E9598;
    D_80155E98 = 0x801EA298;
}

void func_80132514(void) {
    D_80155E90 = 0x801E81BC;
    D_80155E94 = 0x801E81CC;
    D_80155E98 = 0x801E82CC;
}

void func_80132548(void) {
    D_80155E90 = 0x801E81CC;
    D_80155E94 = 0x801E8204;
    D_80155E98 = 0x801E82E4;
}

void func_8013257C(void) {
    D_80155E90 = 0x801E8A10;
    D_80155E94 = 0x801E8B7C;
    D_80155E98 = 0x801E93D4;
}

void func_801325B0(void) {
    switch (D_80122EB8) {
    case 0:
        switch (game_state.year) {
        case 0x5F:
            main_stage_set(0x13);
            return;
        case 0x60:
            main_stage_set(0x14);
            return;
        case 0x61:
            main_stage_set(0x15);
            return;
        default:
            game_reset_to_title();
            return;
        }
        break;
    case 1:
        main_stage_set(0x16);
        return;
    case 2:
        main_stage_set(0x17);
        return;
    case 3:
        switch (game_state.year) {
        case 0x5F:
            main_stage_set(0x18);
            return;
        case 0x60:
            main_stage_set(0x19);
            return;
        case 0x61:
            main_stage_set(0x1A);
            return;
        }
        break;
    case 4:
        switch (game_state.year) {
        case 0x5F:
            main_stage_set(0x1B);
            return;
        case 0x60:
            main_stage_set(0x1C);
            return;
        case 0x61:
            main_stage_set(0x1D);
            return;
        }
        break;
    case 5:
        if (game_state.year == 0x61) {
            main_stage_set(0x1F);
            return;
        }
        main_stage_set(0x1E);
        return;
    default:
        game_reset_to_title();
        return;
    }
}

void func_80132774(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x40EA, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(0);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 1;
                break;
            case 2:
                dialogue_group = 6;
                break;
            case 3:
                dialogue_group = 9;
                break;
            }
            func_80134FFC();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        dialogue_group = 0x89;
        normal_date_bggirl_fadeout();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        temp_v0_2 = girl_state_rank(0);
        switch (temp_v0_2) {
        case 0:
        case 1:
            dialogue_group = 2;
            break;
        case 2:
            dialogue_group = 7;
            break;
        case 3:
            dialogue_group = 0xA;
            break;
        }
        normal_date_bggirl_fadein();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (girl_state_rank(0) == 3) {
            if (game_state.param_liberal_arts.value >= 0x78) {
                dialogue_group = 0xB;
            } else {
                dialogue_group = 0xC;
            }
        } else if (game_state.param_liberal_arts.value >= 0x78) {
            dialogue_group = 3;
        } else {
            dialogue_group = 4;
        }
        main_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        temp_v0_3 = girl_state_rank(0);
        switch (temp_v0_3) {
        case 0:
        case 1:
            dialogue_group = 5;
            break;
        case 2:
            dialogue_group = 8;
            break;
        case 3:
            dialogue_group = 0xD;
            break;
        }
        main_step_next();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        normal_date_bggirl_fadeout();
        break;
    case 15:
        dialogue_group = 0;
        main_step_next();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}


void func_80132AC4(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x40EA, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(0);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0xE;
                break;
            case 2:
                dialogue_group = 0x11;
                break;
            case 3:
                dialogue_group = 0x14;
                break;
            }
            func_80134FFC();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        dialogue_group = 0x89;
        normal_date_bggirl_fadeout();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        temp_v0_2 = girl_state_rank(0);
        switch (temp_v0_2) {
        case 0:
        case 1:
            dialogue_group = 0xF;
            break;
        case 2:
            dialogue_group = 0x12;
            break;
        case 3:
            dialogue_group = 0x15;
            break;
        }
        normal_date_bggirl_fadein();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_bggirl_fadeout();
        break;
    case 11:
        normal_date_bggirl_fadein();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        dialogue_group = 0;
        main_step_next();
        break;
    case 14:
        normal_date_bggirl_fadeout();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_80132D40(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x467C, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(0);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x17;
                break;
            case 2:
                dialogue_group = 0x19;
                break;
            case 3:
                dialogue_group = 0x1B;
                break;
            }
            func_80134FFC();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bggirl_fadeout();
        break;
    case 7:
        D_80122EAC = 1;
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 8:
        bustup_read_step(0x544A);
        break;
    case 9:
        func_80145844();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_SHIORI;
        bustup_show_checked();
        cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(0);
            switch (temp_v0_2) {
            case 0:
            case 1:
                dialogue_group = 0x18;
                break;
            case 2:
                dialogue_group = 0x1A;
                break;
            case 3:
                dialogue_group = 0x1C;
                break;
            }
            func_80134FFC();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        normal_date_bggirl_fadeout();
        break;
    case 14:
        dialogue_group = 0;
        main_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}


void func_80133020(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x467C, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(0);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x1D;
                break;
            case 2:
                dialogue_group = 0x1F;
                break;
            case 3:
                dialogue_group = 0x21;
                break;
            }
            dialogue_group = (dialogue_group + (game_state.year * 6)) - 0x23A;
            func_80134FFC();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bggirl_fadeout();
        break;
    case 7:
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 8:
        dec_bg_reset();
        set_dec_bri(0);
        dec_bg_cd_read(0x467C, 1);
        main_step_next();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            main_step_next();
        }
        break;
    case 10:
        bustup_read_step(0x544A);
        break;
    case 11:
        func_80145844();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_SHIORI;
        bustup_show_checked();
        cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
        break;
    case 12:
        normal_date_bggirl_fadein();
        break;
    case 13:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(0);
            switch (temp_v0_2) {
            case 0:
            case 1:
                dialogue_group = 0x1E;
                break;
            case 2:
                dialogue_group = 0x20;
                break;
            case 3:
                dialogue_group = 0x22;
                break;
            }
            dialogue_group = (dialogue_group + (game_state.year * 6)) - 0x23A;
            func_80134FFC();
        }
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        normal_date_bggirl_fadeout();
        break;
    case 16:
        dialogue_group = 0;
        main_step_next();
        break;
    case 17:
        normal_date_speak();
        break;
    case 18:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}


void func_801333A8(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x40FC, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(0);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x2F;
                break;
            case 2:
                dialogue_group = 0x30;
                break;
            case 3:
                dialogue_group = 0x31;
                break;
            }
            func_80134FFC();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bggirl_fadeout();
        break;
    case 7:
        D_80122EAC = 1;
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 8:
        bustup_read_step(0x544A);
        break;
    case 9:
        func_80145844();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_SHIORI;
        bustup_show_checked();
        cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(0);
            switch (temp_v0_2) {
            case 0:
            case 1:
                dialogue_group = 0x32;
                break;
            case 2:
                dialogue_group = 0x33;
                break;
            case 3:
                dialogue_group = 0x34;
                break;
            }
            dialogue_group = (dialogue_group + (game_state.year * 3)) - 0x11D;
            func_80134FFC();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        normal_date_bggirl_fadeout();
        break;
    case 14:
        dialogue_group = 0;
        main_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}


void func_801336AC(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(0);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x3B;
                break;
            case 2:
                dialogue_group = 0x40;
                break;
            case 3:
                dialogue_group = 0x45;
                break;
            }
            func_80134FFC();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_girl_out();
        break;
    case 7:
        snd_command(0x202);
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_stage_set(0xB);
        break;
    case 8:
        cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
        main_step_next();
        break;
    case 9:
        normal_date_girl_in();
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(0);
            switch (temp_v0_2) {
            case 0:
            case 1:
                dialogue_group = 0x3C;
                break;
            case 2:
                dialogue_group = 0x41;
                break;
            case 3:
                dialogue_group = 0x46;
                break;
            }
            dialogue_group += D_80122EA8;
            func_80134FFC();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        temp_v0_3 = girl_state_rank(0);
        switch (temp_v0_3) {
        case 0:
        case 1:
            dialogue_group = 0x3F;
            break;
        case 2:
            dialogue_group = 0x44;
            break;
        case 3:
            dialogue_group = 0x49;
            break;
        }
        main_step_next();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        dialogue_group = 0;
        main_step_next();
        break;
    case 15:
        normal_date_bggirl_fadeout();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}


void func_801339F4(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(0);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x4A;
                break;
            case 2:
                dialogue_group = 0x4E;
                break;
            case 3:
                dialogue_group = 0x52;
                break;
            }
            func_80134FFC();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_girl_out();
        break;
    case 7:
        ((ExtraModeFlags *)&D_800E7D4F)->b1 = 1;
        ((ExtraModeFlags *)&D_800E7D4F)->unk_2_4 = 0;
        game_state.saved_mode = game_state.mode;
        game_state.saved_stage = game_state.stage;
        game_state.saved_step = game_state.step + 1;
        main_stage_set(0x8D);
        break;
    case 8:
        D_80122E98 = 1;
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_stage_set(0x8C);
        break;
    case 9:
        snd_command(0xBF);
        snd_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        scene_screen_reset(1, 0);
        func_80145844();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        text_marker_hide();
        dialogue_names_set(0U);
        D_80122E98 = 0;
        main_step_next();
        break;
    case 10:
        if (snd_bank_load_poll() == 1) {
            snd_command(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            main_step_next();
        }
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            main_step_next();
        }
        break;
    case 12:
        bustup_read_step(0x544A);
        break;
    case 13:
        addr_init_bustup();
        game_state.current_girl = GIRL_SHIORI;
        bustup_show_checked();
        cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
        break;
    case 14:
        normal_date_bggirl_fadein();
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(0);
            switch (temp_v0_2) {
            case 0:
            case 1:
                dialogue_group = 0x4B;
                break;
            case 2:
                dialogue_group = 0x4F;
                break;
            case 3:
                dialogue_group = 0x53;
                break;
            }
            if ((u8) D_800E7D4E < 2U) {
                dialogue_group += 1;
            }
            func_80134FFC();
        }
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        temp_v0_3 = girl_state_rank(0);
        switch (temp_v0_3) {
        case 0:
        case 1:
            dialogue_group = 0x4D;
            break;
        case 2:
            dialogue_group = 0x51;
            break;
        case 3:
            dialogue_group = 0x55;
            break;
        }
        main_step_next();
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        normal_date_bggirl_fadeout();
        break;
    case 20:
        dialogue_group = 0;
        main_step_next();
        break;
    case 21:
        normal_date_speak();
        break;
    case 22:
        ((ExtraModeFlags *)&D_800E7D4F)->b0 = 1;
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}


void func_80133EC4(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(0);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x56;
                break;
            case 2:
                dialogue_group = 0x58;
                break;
            case 3:
                dialogue_group = 0x5A;
                break;
            }
            func_80134FFC();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bggirl_fadeout();
        break;
    case 7:
        D_80122EAC = 1;
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 8:
        bustup_read_step(0x544A);
        break;
    case 9:
        func_80145844();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_SHIORI;
        bustup_show_checked();
        cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(0);
            switch (temp_v0_2) {
            case 0:
            case 1:
                dialogue_group = 0x57;
                break;
            case 2:
                dialogue_group = 0x59;
                break;
            case 3:
                dialogue_group = 0x5B;
                break;
            }
            func_80134FFC();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        normal_date_bggirl_fadeout();
        break;
    case 14:
        dialogue_group = 0;
        main_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}


void func_801341A4(void) {
    s32 temp_v0;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x410F, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(0);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x5C;
                break;
            case 2:
                dialogue_group = 0x5F;
                break;
            case 3:
                dialogue_group = 0x62;
                break;
            }
            func_80134FFC();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        if (girl_state_rank(0) == 3) {
            main_step_set(0xC);
        } else {
            main_step_next();
        }
        break;
    case 7:
        normal_date_girl_out();
        break;
    case 8:
        normal_date_speak();
        break;
    case 9:
        normal_date_girl_in();
        break;
    case 10:
        if (girl_state_rank(0) == 2) {
            dialogue_group = 0x61;
        } else {
            dialogue_group = 0x5E;
        }
        main_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        normal_date_bggirl_fadeout();
        break;
    case 13:
        dialogue_group = 0;
        main_step_next();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_80134400(void) {
    s32 temp_v0;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x4118, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(0);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x63;
                break;
            case 2:
                dialogue_group = 0x65;
                break;
            case 3:
                dialogue_group = 0x67;
                break;
            }
            func_80134FFC();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_girl_out();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        normal_date_bg_fadeout();
        break;
    case 9:
        dialogue_group = 0;
        main_step_next();
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_801345CC(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x410F, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(0);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x69;
                break;
            case 2:
                dialogue_group = 0x6E;
                break;
            case 3:
                dialogue_group = 0x71;
                break;
            }
            func_80134FFC();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        dialogue_group = 0x89;
        normal_date_bggirl_fadeout();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        temp_v0_2 = girl_state_rank(0);
        switch (temp_v0_2) {
        case 0:
        case 1:
            dialogue_group = 0x6A;
            break;
        case 2:
            dialogue_group = 0x6F;
            break;
        case 3:
            dialogue_group = 0x72;
            break;
        }
        normal_date_bggirl_fadein();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        if (girl_state_rank(0) == 3) {
            if (game_state.param_art.value >= 0x78) {
                dialogue_group = 0x73;
            } else {
                dialogue_group = 0x74;
            }
        } else if (game_state.param_art.value >= 0x78) {
            dialogue_group = 0x6B;
        } else {
            dialogue_group = 0x6C;
        }
        main_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        temp_v0_3 = girl_state_rank(0);
        switch (temp_v0_3) {
        case 0:
        case 1:
            dialogue_group = 0x6D;
            break;
        case 2:
            dialogue_group = 0x70;
            break;
        case 3:
            dialogue_group = 0x75;
            break;
        }
        main_step_next();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        normal_date_bggirl_fadeout();
        break;
    case 15:
        dialogue_group = 0;
        main_step_next();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}


void func_8013491C(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x467C, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(0);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x76;
                break;
            case 2:
                dialogue_group = 0x77;
                break;
            case 3:
                dialogue_group = 0x78;
                break;
            }
            func_80134FFC();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bggirl_fadeout();
        break;
    case 7:
        D_80122EAC = 0;
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 8:
        bustup_read_step(0x544A);
        break;
    case 9:
        func_80145844();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_SHIORI;
        bustup_show_checked();
        cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(0);
            switch (temp_v0_2) {
            case 0:
            case 1:
                dialogue_group = 0x79;
                break;
            case 2:
                dialogue_group = 0x7A;
                break;
            case 3:
                dialogue_group = 0x7B;
                break;
            }
            if (game_state.year == 0x60) {
                dialogue_group += 3;
            }
            func_80134FFC();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        normal_date_bggirl_fadeout();
        break;
    case 14:
        dialogue_group = 0;
        main_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}


void func_80134C1C(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x467C, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(0);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x76;
                break;
            case 2:
                dialogue_group = 0x77;
                break;
            case 3:
                dialogue_group = 0x78;
                break;
            }
            func_80134FFC();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bggirl_fadeout();
        break;
    case 7:
        D_80122EAC = 0;
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 8:
        bustup_read_step(0x544A);
        break;
    case 9:
        func_80145844();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_SHIORI;
        bustup_show_checked();
        cd_read_sectors(0xFU, 0x801E8000, 0x4A03);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(0);
            switch (temp_v0_2) {
            case 0:
            case 1:
                dialogue_group = 0x7F;
                break;
            case 2:
                dialogue_group = 0x83;
                break;
            case 3:
                dialogue_group = 0x87;
                break;
            }
            func_80134FFC();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        if (girl_state_rank(0) == 2) {
            if (game_state.param_art.value >= 0x78) {
                dialogue_group = 0x84;
            } else {
                dialogue_group = 0x85;
            }
        } else if (game_state.param_art.value >= 0x78) {
            dialogue_group = 0x80;
        } else {
            dialogue_group = 0x81;
        }
        main_step_next();
        break;
    case 14:
        normal_date_speak();
        break;
    case 15:
        temp_v0_3 = girl_state_rank(0);
        switch (temp_v0_3) {
        case 0:
        case 1:
            dialogue_group = 0x82;
            break;
        case 2:
            dialogue_group = 0x86;
            break;
        case 3:
            dialogue_group = 0x88;
            break;
        }
        main_step_next();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        normal_date_bggirl_fadeout();
        break;
    case 18:
        dialogue_group = 0;
        main_step_next();
        break;
    case 19:
        normal_date_speak();
        break;
    case 20:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}


void func_80134FFC(void) {
    dialogue_names_set(0);
    func_8013209C();
    dialogue_line = 0;
    dialogue_text_table = D_80155E90;
    dialogue_command_table = D_80155E94;
    dialogue_voice_table = D_80155E98;
    main_step_next();
}

void func_80135060(void) {
    switch (D_80122EB8) {
    case 0:
        switch (game_state.year) {
        case 0x5F:
            main_stage_set(0x49);
            return;
        case 0x60:
            main_stage_set(0x4A);
            return;
        case 0x61:
            main_stage_set(0x4B);
            return;
        default:
            game_reset_to_title();
            return;
        }
        break;
    case 1:
        main_stage_set(0x4C);
        return;
    case 2:
        main_stage_set(0x4D);
        return;
    case 3:
        switch (game_state.year) {
        case 0x5F:
            main_stage_set(0x4E);
            return;
        case 0x60:
            main_stage_set(0x4F);
            return;
        case 0x61:
            main_stage_set(0x50);
            return;
        }
        break;
    case 4:
        switch (game_state.year) {
        case 0x5F:
            main_stage_set(0x51);
            return;
        case 0x60:
            main_stage_set(0x52);
            return;
        case 0x61:
            main_stage_set(0x53);
            return;
        }
        break;
    case 5:
        main_stage_set(0x54);
        return;
    default:
        game_reset_to_title();
        return;
    }
}

void func_80135200(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x40EA, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(5);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 2;
                break;
            case 2:
                dialogue_group = 3;
                break;
            case 3:
                dialogue_group = 4;
                break;
            }
            func_801375D0();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        dialogue_group = 1;
        normal_date_bggirl_fadeout();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        temp_v0_2 = girl_state_rank(5);
        switch (temp_v0_2) {
        case 0:
        case 1:
            dialogue_group = 8;
            break;
        case 2:
            dialogue_group = 9;
            break;
        case 3:
            dialogue_group = 0xA;
            break;
        }
        normal_date_bggirl_fadein();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_bggirl_fadeout();
        break;
    case 11:
        dialogue_group = 0;
        main_step_next();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_8013544C(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x40EA, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(5);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 5;
                break;
            case 2:
                dialogue_group = 6;
                break;
            case 3:
                dialogue_group = 7;
                break;
            }
            func_801375D0();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        dialogue_group = 1;
        normal_date_bggirl_fadeout();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        temp_v0_2 = girl_state_rank(5);
        switch (temp_v0_2) {
        case 0:
        case 1:
            dialogue_group = 0xB;
            break;
        case 2:
            dialogue_group = 0xD;
            break;
        case 3:
            dialogue_group = 0xF;
            break;
        }
        normal_date_bggirl_fadein();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_bggirl_fadeout();
        break;
    case 11:
        normal_date_bggirl_fadein();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        normal_date_bggirl_fadeout();
        break;
    case 14:
        dialogue_group = 0;
        main_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_801356C8(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x467C, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(5);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x11;
                break;
            case 2:
                dialogue_group = 0x12;
                break;
            case 3:
                dialogue_group = 0x13;
                break;
            }
            func_801375D0();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bggirl_fadeout();
        break;
    case 7:
        D_80122EAC = 1;
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 8:
        bustup_read_step(0x5FB7);
        break;
    case 9:
        func_80145844();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKARI;
        bustup_show_checked();
        cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(5);
            switch (temp_v0_2) {
            case 0:
            case 1:
                dialogue_group = 0x14;
                break;
            case 2:
                dialogue_group = 0x15;
                break;
            case 3:
                dialogue_group = 0x16;
                break;
            }
            func_801375D0();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        normal_date_bggirl_fadeout();
        break;
    case 14:
        dialogue_group = 0;
        main_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}


void func_801359AC(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x467C, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(5);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x17;
                break;
            case 2:
                dialogue_group = 0x18;
                break;
            case 3:
                dialogue_group = 0x19;
                break;
            }
            func_801375D0();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bggirl_fadeout();
        break;
    case 7:
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 8:
        dec_bg_reset();
        set_dec_bri(0);
        dec_bg_cd_read(0x467C, 1);
        main_step_next();
        break;
    case 9:
        if (cd_read_sync_status() & 1) {
            bustup_read_step(0x5FB7);
        }
        break;
    case 10:
        func_80145844();
        D_80125CA0 = 0xC0;
        dec_bg_show_set(1, dec_bg_decode_to_vram(1));
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKARI;
        bustup_show_checked();
        cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
        break;
    case 11:
        normal_date_bggirl_fadein();
        break;
    case 12:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(5);
            switch (temp_v0_2) {
            case 0:
            case 1:
                dialogue_group = 0x1A;
                break;
            case 2:
                dialogue_group = 0x1B;
                break;
            case 3:
                dialogue_group = 0x1C;
                break;
            }
            dialogue_group = (dialogue_group + (game_state.year * 3)) - 0x11D;
            func_801375D0();
        }
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        normal_date_bggirl_fadeout();
        break;
    case 15:
        dialogue_group = 0;
        main_step_next();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}


void func_80135D00(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x40FC, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(5);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x23;
                break;
            case 2:
                dialogue_group = 0x24;
                break;
            case 3:
                dialogue_group = 0x25;
                break;
            }
            func_801375D0();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bggirl_fadeout();
        break;
    case 7:
        D_80122EAC = 1;
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 8:
        bustup_read_step(0x5FB7);
        break;
    case 9:
        func_80145844();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKARI;
        bustup_show_checked();
        cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(5);
            switch (temp_v0_2) {
            case 0:
            case 1:
                dialogue_group = 0x26;
                break;
            case 2:
                dialogue_group = 0x27;
                break;
            case 3:
                dialogue_group = 0x28;
                break;
            }
            dialogue_group = (dialogue_group + (game_state.year * 3)) - 0x11D;
            func_801375D0();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        normal_date_bggirl_fadeout();
        break;
    case 14:
        dialogue_group = 0;
        main_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}


void func_80136008(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(5);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x2F;
                break;
            case 2:
                dialogue_group = 0x30;
                break;
            case 3:
                dialogue_group = 0x31;
                break;
            }
            func_801375D0();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_girl_out();
        break;
    case 7:
        snd_command(0x202);
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_stage_set(0xB);
        break;
    case 8:
        cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
        main_step_next();
        break;
    case 9:
        normal_date_girl_in();
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            dialogue_group = D_80122EA8 + 0x32;
            func_801375D0();
        }
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        temp_v0_2 = girl_state_rank(5);
        switch (temp_v0_2) {
        case 0:
        case 1:
            dialogue_group = 0x35;
            break;
        case 2:
            dialogue_group = 0x36;
            break;
        case 3:
            dialogue_group = 0x37;
            break;
        }
        main_step_next();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        normal_date_bggirl_fadeout();
        break;
    case 15:
        dialogue_group = 0;
        main_step_next();
        break;
    case 16:
        normal_date_speak();
        break;
    case 17:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}


void func_801362EC(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(5);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x38;
                break;
            case 2:
                dialogue_group = 0x39;
                break;
            case 3:
                dialogue_group = 0x3A;
                break;
            }
            func_801375D0();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        dialogue_group += 2;
        main_step_next();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        normal_date_bggirl_fadeout();
        break;
    case 9:
        ((ExtraModeFlags *)&D_800E7D4F)->b1 = 1;
        ((ExtraModeFlags *)&D_800E7D4F)->unk_2_4 = 1;
        game_state.saved_mode = game_state.mode;
        game_state.saved_stage = game_state.stage;
        game_state.saved_step = game_state.step + 1;
        main_state_set(MODE_TT_GAME);
        break;
    case 10:
        D_80122E98 = 1;
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_stage_set(0x8C);
        break;
    case 11:
        snd_command(0xBF);
        snd_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        scene_screen_reset(1, 0);
        func_80145844();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        text_marker_hide();
        dialogue_names_set(5U);
        D_80122E98 = 0;
        main_step_next();
        break;
    case 12:
        if (snd_bank_load_poll() == 1) {
            snd_command(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            main_step_next();
        }
        break;
    case 13:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            main_step_next();
        }
        break;
    case 14:
        bustup_read_step(0x5FB7);
        break;
    case 15:
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKARI;
        bustup_show_checked();
        cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
        break;
    case 16:
        normal_date_bggirl_fadein();
        break;
    case 17:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(5);
            switch (temp_v0_2) {
            case 0:
            case 1:
                dialogue_group = 0x3E;
                break;
            case 2:
                dialogue_group = 0x3F;
                break;
            case 3:
                dialogue_group = 0x40;
                break;
            }
            func_801375D0();
        }
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        normal_date_girl_out();
        break;
    case 20:
        ((ExtraModeFlags *)&D_800E7D4F)->b1 = 1;
        ((ExtraModeFlags *)&D_800E7D4F)->unk_2_4 = 0;
        game_state.saved_mode = game_state.mode;
        game_state.saved_stage = game_state.stage;
        game_state.saved_step = game_state.step + 1;
        main_stage_set(0x8D);
        break;
    case 21:
        D_80122E98 = 1;
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_stage_set(0x8C);
        break;
    case 22:
        snd_command(0xBF);
        snd_bank_load_start(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        scene_screen_reset(1, 0);
        func_80145844();
        D_80125CA0 = 0xC0;
        message_window_init();
        hizuke_init();
        text_marker_hide();
        dialogue_names_set(5U);
        D_80122E98 = 0;
        main_step_next();
        break;
    case 23:
        if (snd_bank_load_poll() == 1) {
            snd_command(0x200);
            dec_bg_reset();
            set_dec_bri(0);
            dec_bg_cd_read(0x4105, 1);
            main_step_next();
        }
        break;
    case 24:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            main_step_next();
        }
        break;
    case 25:
        bustup_read_step(0x5FB7);
        break;
    case 26:
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKARI;
        bustup_show_checked();
        cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
        break;
    case 27:
        normal_date_bggirl_fadein();
        break;
    case 28:
        if (cd_read_sync_status() & 1) {
            temp_v0_3 = girl_state_rank(5);
            switch (temp_v0_3) {
            case 0:
            case 1:
                dialogue_group = 0x44;
                break;
            case 2:
                dialogue_group = 0x45;
                break;
            case 3:
                dialogue_group = 0x46;
                break;
            }
            func_801375D0();
        }
        break;
    case 29:
        normal_date_speak();
        break;
    case 30:
        normal_date_bggirl_fadeout();
        break;
    case 31:
        dialogue_group = 0;
        main_step_next();
        break;
    case 32:
        normal_date_speak();
        break;
    case 33:
        ((ExtraModeFlags *)&D_800E7D4F)->b0 = 1;
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}


void func_801369CC(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x4105, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(5);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x23;
                break;
            case 2:
                dialogue_group = 0x24;
                break;
            case 3:
                dialogue_group = 0x25;
                break;
            }
            func_801375D0();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bggirl_fadeout();
        break;
    case 7:
        D_80122EAC = 1;
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 8:
        bustup_read_step(0x5FB7);
        break;
    case 9:
        func_80145844();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKARI;
        bustup_show_checked();
        cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(5);
            switch (temp_v0_2) {
            case 0:
            case 1:
                dialogue_group = 0x47;
                break;
            case 2:
                dialogue_group = 0x48;
                break;
            case 3:
                dialogue_group = 0x49;
                break;
            }
            func_801375D0();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        normal_date_bggirl_fadeout();
        break;
    case 14:
        dialogue_group = 0;
        main_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}


void func_80136CB0(void) {
    s32 temp_v0;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x410F, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(5);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x4A;
                break;
            case 2:
                dialogue_group = 0x4D;
                break;
            case 3:
                dialogue_group = 0x50;
                break;
            }
            func_801375D0();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_girl_out();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        normal_date_girl_in();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_bggirl_fadeout();
        break;
    case 11:
        dialogue_group = 0;
        main_step_next();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_80136E9C(void) {
    s32 temp_v0;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x4118, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(5);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x53;
                break;
            case 2:
                dialogue_group = 0x55;
                break;
            case 3:
                dialogue_group = 0x57;
                break;
            }
            func_801375D0();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_girl_out();
        break;
    case 7:
        game_state.current_girl = GIRL_SOTOI;
        main_step_next();
        break;
    case 8:
        normal_date_speak();
        break;
    case 9:
        normal_date_bg_fadeout();
        break;
    case 10:
        dialogue_group = 0;
        main_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_80137080(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x410F, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(5);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x59;
                break;
            case 2:
                dialogue_group = 0x5A;
                break;
            case 3:
                dialogue_group = 0x5B;
                break;
            }
            func_801375D0();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        dialogue_group = 1;
        normal_date_bggirl_fadeout();
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        temp_v0_2 = girl_state_rank(5);
        switch (temp_v0_2) {
        case 0:
        case 1:
            dialogue_group = 0x5C;
            break;
        case 2:
            dialogue_group = 0x5D;
            break;
        case 3:
            dialogue_group = 0x5E;
            break;
        }
        normal_date_bggirl_fadein();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        normal_date_bggirl_fadeout();
        break;
    case 11:
        dialogue_group = 0;
        main_step_next();
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}

void func_801372CC(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (game_state.step) {
    case 0:
        dec_bg_cd_read(0x467C, 1);
        main_step_next();
        break;
    case 1:
        normal_date_bggirl_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
            main_step_next();
        }
        break;
    case 3:
        normal_date_bggirl_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            temp_v0 = girl_state_rank(5);
            switch (temp_v0) {
            case 0:
            case 1:
                dialogue_group = 0x5F;
                break;
            case 2:
                dialogue_group = 0x60;
                break;
            case 3:
                dialogue_group = 0x61;
                break;
            }
            func_801375D0();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bggirl_fadeout();
        break;
    case 7:
        D_80122EAC = 0;
        D_80122EB0 = (s32) game_state.stage;
        D_80122EB4 = game_state.step + 1;
        main_state_set(MODE_CULTURE_STAGE);
        break;
    case 8:
        bustup_read_step(0x5FB7);
        break;
    case 9:
        func_80145844();
        D_80125CA0 = 0xC0;
        addr_init_bustup();
        game_state.current_girl = GIRL_YUKARI;
        bustup_show_checked();
        cd_read_sectors(0x10U, 0x801E8000, 0x4A73);
        break;
    case 10:
        normal_date_bggirl_fadein();
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            temp_v0_2 = girl_state_rank(5);
            switch (temp_v0_2) {
            case 0:
            case 1:
                dialogue_group = 0x62;
                break;
            case 2:
                dialogue_group = 0x63;
                break;
            case 3:
                dialogue_group = 0x64;
                break;
            }
            dialogue_group = (dialogue_group + (game_state.year * 3)) - 0x11D;
            func_801375D0();
        }
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        normal_date_bggirl_fadeout();
        break;
    case 14:
        dialogue_group = 0;
        main_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        func_80145530();
        break;
    default:
        game_reset_to_title();
        break;
    }
}


void func_801375D0(void) {
    dialogue_names_set(5);
    func_801322A4();
    dialogue_line = 0;
    dialogue_text_table = D_80155E90;
    dialogue_command_table = D_80155E94;
    dialogue_voice_table = D_80155E98;
    main_step_next();
}
