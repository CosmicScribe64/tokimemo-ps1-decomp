/* battle_asset_pointers.c: src/ovl/RPG_BAT/8014D380.c:4 switches on encounter mask rpg_encounter to assign sprite-sheet/frame/animation/palette asset pointers. Original object 0x8014D380-0x8014DF60 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/RPG_BAT.h"

void rpg_stage_asset_addresses(void) {
    switch (rpg_encounter) {
    case 0x10000:
        rpg_asset_sheet_2 = 0x8018C1F8;
        rpg_asset_sheet_3 = 0x801BF9A4;
        rpg_asset_sheet_1 = 0x801948E0;
        rpg_asset_sheet_0 = 0x801B5758;
        rpg_asset_frames_2 = 0x8018C204;
        rpg_asset_frames_3 = 0x80194598;
        rpg_asset_frames_1 = 0x801948EC;
        rpg_asset_frames_0 = 0x801B576C;
        rpg_asset_animation_2 = 0x8018C24C;
        rpg_asset_animation_3 = 0x8019460C;
        rpg_asset_animation_1 = 0x8019494C;
        rpg_asset_animation_0 = 0x801B5874;
        rpg_asset_palette_1 = 0x801B594C;
        rpg_asset_palette_3 = 0x801B794C;
        rpg_asset_palette_2 = 0x801B994C;
        rpg_asset_palette_4 = 0x801BB94C;
        rpg_asset_palette_0 = 0x801BD94C;
        break;
    case 0x20000:
        rpg_asset_sheet_2 = 0x801B781C;
        rpg_asset_sheet_3 = 0x801B7864;
        rpg_asset_sheet_1 = 0x8018C7E4;
        rpg_asset_sheet_0 = 0x801AD620;
        rpg_asset_frames_2 = 0x80184160;
        rpg_asset_frames_3 = 0x8018C4E0;
        rpg_asset_frames_1 = 0x8018C7F0;
        rpg_asset_frames_0 = 0x801AD634;
        rpg_asset_animation_2 = 0x80184190;
        rpg_asset_animation_3 = 0x8018C554;
        rpg_asset_animation_1 = 0x8018C844;
        rpg_asset_animation_0 = 0x801AD73C;
        rpg_asset_palette_1 = 0x801AD814;
        rpg_asset_palette_3 = 0x801AF814;
        rpg_asset_palette_2 = 0x801B1814;
        rpg_asset_palette_4 = 0x801B3814;
        rpg_asset_palette_0 = 0x801B5814;
        break;
    case 0x40000:
        rpg_asset_sheet_2 = 0x801B7864;
        rpg_asset_sheet_3 = 0x801B78AC;
        rpg_asset_sheet_1 = 0x8018C810;
        rpg_asset_sheet_0 = 0x801AD638;
        rpg_asset_frames_2 = 0x801841FC;
        rpg_asset_frames_3 = 0x8018C59C;
        rpg_asset_frames_1 = 0x8018C81C;
        rpg_asset_frames_0 = 0x801AD64C;
        rpg_asset_animation_2 = 0x8018423C;
        rpg_asset_animation_3 = 0x8018C610;
        rpg_asset_animation_1 = 0x8018C85C;
        rpg_asset_animation_0 = 0x801AD754;
        rpg_asset_palette_1 = 0x801AD82C;
        rpg_asset_palette_3 = 0x801AF82C;
        rpg_asset_palette_2 = 0x801B182C;
        rpg_asset_palette_4 = 0x801B382C;
        rpg_asset_palette_0 = 0x801B582C;
        break;
    case 0x80000000:
        rpg_asset_sheet_3 = 0x801CC360;
        rpg_asset_sheet_1 = 0x801A1234;
        rpg_asset_sheet_0 = 0x801C20A4;
        rpg_asset_frames_3 = 0x801A0C08;
        rpg_asset_frames_1 = 0x801A1240;
        rpg_asset_frames_0 = 0x801C20B8;
        rpg_asset_animation_3 = 0x801A0C7C;
        rpg_asset_animation_1 = 0x801A12B4;
        rpg_asset_animation_0 = 0x801C21C0;
        rpg_asset_palette_1 = 0x801C2298;
        rpg_asset_palette_4 = 0x801C8298;
        rpg_asset_palette_0 = 0x801CA298;
        rpg_asset_palette_3 = 0x801C6298;
        rpg_asset_sheet_5 = 0x80180790;
        rpg_asset_frames_5 = 0x801807A8;
        rpg_asset_animation_5 = 0x80180864;
        break;
    case 0x8000000:
        rpg_asset_sheet_3 = 0x801CA1CC;
        rpg_asset_sheet_1 = 0x80189030;
        rpg_asset_sheet_0 = 0x801BFFA0;
        rpg_asset_frames_3 = 0x8018833C;
        rpg_asset_frames_1 = 0x8018904C;
        rpg_asset_frames_0 = 0x801BFFB4;
        rpg_asset_animation_3 = 0x801883B0;
        rpg_asset_animation_1 = 0x80189174;
        rpg_asset_animation_0 = 0x801C00BC;
        rpg_asset_palette_1 = 0x801C0194;
        rpg_asset_palette_3 = 0x801C2194;
        rpg_asset_palette_2 = 0x801C4194;
        rpg_asset_palette_4 = 0x801C6194;
        rpg_asset_palette_0 = 0x801C8194;
        rpg_asset_palette_6 = 0x801A91D8;
        break;
    case 0x100000:
        rpg_asset_sheet_3 = 0x801B0BC8;
        rpg_asset_sheet_1 = 0x80188AC0;
        rpg_asset_frames_3 = 0x8018833C;
        rpg_asset_frames_1 = 0x80188ADC;
        rpg_asset_animation_3 = 0x801883B0;
        rpg_asset_animation_1 = 0x80188B50;
        rpg_asset_sheet_0 = 0x801C5DC8;
        rpg_asset_frames_0 = 0x801C5DDC;
        rpg_asset_animation_0 = 0x801C5EE4;
        rpg_asset_palette_1 = 0x801A6B90;
        rpg_asset_palette_3 = 0x801A8B90;
        rpg_asset_palette_2 = 0x801AAB90;
        rpg_asset_palette_4 = 0x801ACB90;
        rpg_asset_palette_0 = 0x801AEB90;
        rpg_asset_palette_6 = 0x801A4B90;
        break;
    }
    if (rpg_club_move_available == 1) {
        switch (rpg_player_club) {
        case 0:
            rpg_asset_sheet_4 = 0x801E86C0;
            rpg_asset_frames_4 = 0x801E86D0;
            rpg_asset_animation_4 = 0x801E8778;
            break;
        case 1:
            rpg_asset_sheet_4 = 0x801D82A0;
            rpg_asset_frames_4 = 0x801D82B0;
            rpg_asset_animation_4 = 0x801D82F4;
            break;
        case 2:
            rpg_asset_sheet_4 = 0x801D8868;
            rpg_asset_frames_4 = 0x801D8878;
            rpg_asset_animation_4 = 0x801D892C;
            break;
        case 3:
            rpg_asset_sheet_4 = 0x801EC528;
            rpg_asset_frames_4 = 0x801EC53C;
            rpg_asset_animation_4 = 0x801EC59C;
            rpg_asset_palette_5 = 0x801EC5CC;
            break;
        case 4:
            rpg_asset_sheet_4 = 0x801D8268;
            rpg_asset_frames_4 = 0x801D827C;
            rpg_asset_animation_4 = 0x801D82BC;
            rpg_asset_palette_5 = 0x801EC2E0;
            break;
        case 5:
            rpg_asset_sheet_4 = 0x801D8814;
            rpg_asset_frames_4 = 0x801D8824;
            rpg_asset_animation_4 = 0x801D88A8;
            break;
        case 6:
            rpg_asset_sheet_4 = 0x801E4308;
            rpg_asset_frames_4 = 0x801E4314;
            rpg_asset_animation_4 = 0x801E4364;
            break;
        case 7:
            rpg_asset_sheet_4 = 0x801E49B0;
            rpg_asset_frames_4 = 0x801E49BC;
            rpg_asset_animation_4 = 0x801E4A6C;
            break;
        case 8:
            rpg_asset_sheet_4 = 0x801EC58C;
            rpg_asset_frames_4 = 0x801EC5A0;
            rpg_asset_animation_4 = 0x801EC630;
            break;
        case 9:
            rpg_asset_sheet_4 = 0x801ED374;
            rpg_asset_frames_4 = 0x801ED388;
            rpg_asset_animation_4 = 0x801ED580;
            break;
        case 10:
            rpg_asset_sheet_4 = 0x801E43C8;
            rpg_asset_frames_4 = 0x801E43D4;
            rpg_asset_animation_4 = 0x801E4440;
            break;
        case 11: /* explicit label: the original jump table has 12 entries (T-10720) */
        default:
            rpg_asset_sheet_4 = 0x801D9370;
            rpg_asset_frames_4 = 0x801D9388;
            rpg_asset_animation_4 = 0x801D9550;
            break;
        }
    }
    switch (rpg_girl_id) {
    case 0:
        rpg_asset_sheet_5 = 0x801CB164;
        rpg_asset_frames_5 = 0x801C7114;
        rpg_asset_animation_5 = 0x801C7138;
        return;
    case 1:
        rpg_asset_sheet_5 = 0x801CF2CC;
        rpg_asset_frames_5 = 0x801CF250;
        rpg_asset_animation_5 = 0x801CF290;
        return;
    case 2:
        if (rpg_encounter != 0x80000000) {
            rpg_asset_sheet_5 = 0x801D776C;
            rpg_asset_frames_5 = 0x801D777C;
            rpg_asset_animation_5 = 0x801D7834;
            return;
        }
    default:
        return;
    case 3:
        rpg_asset_sheet_5 = 0x801D31F0;
        rpg_asset_frames_5 = 0x801D31FC;
        rpg_asset_animation_5 = 0x801D323C;
        return;
    case 4:
        rpg_asset_sheet_5 = 0x801CF380;
        rpg_asset_frames_5 = 0x801CF2EC;
        rpg_asset_animation_5 = 0x801CF34C;
        return;
    case 5:
        rpg_asset_sheet_5 = 0x801CF228;
        rpg_asset_frames_5 = 0x801CF1B8;
        rpg_asset_animation_5 = 0x801CF1F4;
        return;
    case 6:
        rpg_asset_sheet_5 = 0x801D32F4;
        rpg_asset_frames_5 = 0x801D3300;
        rpg_asset_animation_5 = 0x801D3358;
        return;
    case 7:
        rpg_asset_sheet_5 = 0x801D3348;
        rpg_asset_frames_5 = 0x801D32AC;
        rpg_asset_animation_5 = 0x801D3304;
        return;
    case 8:
        rpg_asset_sheet_5 = 0x801CF2B0;
        rpg_asset_frames_5 = 0x801C7230;
        rpg_asset_animation_5 = 0x801C7278;
        return;
    case 9:
        rpg_asset_sheet_5 = 0x801CB1FC;
        rpg_asset_frames_5 = 0x801CB18C;
        rpg_asset_animation_5 = 0x801CB1B8;
        return;
    case 10:
        rpg_asset_sheet_5 = 0x801CF2EC;
        rpg_asset_frames_5 = 0x801CF254;
        rpg_asset_animation_5 = 0x801CF2A4;
        return;
    case 13:
        rpg_asset_sheet_5 = 0x801D32C8;
        rpg_asset_frames_5 = 0x801D32D4;
        rpg_asset_animation_5 = 0x801D332C;
        break;
    case 14: /* explicit empty label: the original jump table has 15 entries (T-10720) */
        break;
    }
}
