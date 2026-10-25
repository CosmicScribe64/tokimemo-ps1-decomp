/* battle_palette_loading.c: src/ovl/RPG_BAT/80142F70.c:4 loads shared palettes then selects palette and texture uploads by rpg_encounter encounter mask. Original object 0x80142F70-0x80143830 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/rpg_battle.h"

void rpg_stage_palettes_load(void) {
    load_palette(rpg_asset_palette_0, 0x10, 1, 4, 0);
    load_palette(rpg_asset_palette_4, 0x1E, 1, 4, 0);
    load_palette(rpg_asset_palette_4, 0x1F, 1, 4, 0xF);
    switch (rpg_encounter) {                           /* switch 1; irregular */
    case 0x10000:                                   /* switch 1 */
        load_palette(rpg_asset_palette_0, 0x11, 1, 1, 1);
        load_palette(rpg_asset_palette_0, 0x17, 1, 3, 1);
        load_palette(rpg_asset_palette_2, 0x18, 1, 4, 0);
        load_palette(rpg_asset_palette_1, 0x13, 1, 3, 0);
        load_palette(rpg_asset_palette_1, 0x12, 1, 4, 0);
        break;
    case 0x20000:                                   /* switch 1 */
        load_palette(rpg_asset_palette_0, 0x12, 1, 1, 2);
        load_palette(rpg_asset_palette_0, 0x17, 1, 3, 2);
        load_palette(rpg_asset_palette_2, 0x18, 1, 4, 1);
        load_palette(rpg_asset_palette_1, 0x13, 1, 4, 1);
        break;
    case 0x40000:                                   /* switch 1 */
        load_palette(rpg_asset_palette_0, 0x13, 1, 1, 3);
        load_palette(rpg_asset_palette_0, 0x17, 1, 3, 3);
        load_palette(rpg_asset_palette_2, 0x18, 1, 4, 2);
        load_palette(rpg_asset_palette_1, 0x12, 1, 4, 2);
        break;
    case 0x80000000:                                /* switch 1 */
        load_palette(rpg_asset_palette_0, 0x16, 1, 1, 6);
        load_palette(rpg_asset_palette_0, 0x17, 1, 3, 6);
        load_palette(rpg_asset_palette_1, 0x12, 1, 4, 5);
        break;
    case 0x8000000:                                 /* switch 1 */
        load_palette(rpg_asset_palette_0, 0x11, 1, 1, 1);
        load_palette(rpg_asset_palette_0, 0x17, 1, 3, 1);
        load_palette(rpg_asset_palette_1, 0x12, 1, 4, 4);
        break;
    case 0x100000:                                  /* switch 1 */
        load_palette(rpg_asset_palette_0, 0x15, 1, 1, 5);
        load_palette(rpg_asset_palette_0, 0x14, 1, 1, 4);
        load_palette(rpg_asset_palette_0, 0x17, 1, 3, 4);
        load_palette(rpg_asset_palette_1, 0x12, 1, 4, 3);
        load_palette(rpg_asset_palette_6, 0x13, 1, 1, 0);
        break;
    }
    if (rpg_club_move_available == 1) {
        switch (rpg_player_club) {                       /* switch 2 */
        case 0:                                     /* switch 2 */
            load_palette(rpg_asset_palette_4, 0x1D, 1, 4, 6);
            break;
        case 1:                                     /* switch 2 */
            load_palette(rpg_asset_palette_4, 0x1D, 1, 4, 7);
            break;
        case 2:                                     /* switch 2 */
            load_palette(rpg_asset_palette_4, 0x1D, 1, 4, 9);
            break;
        case 3:                                     /* switch 2 */
            load_palette(rpg_asset_palette_4, 0x1D, 1, 4, 8);
            break;
        case 4:                                     /* switch 2 */
            load_palette(rpg_asset_palette_4, 0x1D, 1, 4, 0xA);
            break;
        case 5:                                     /* switch 2 */
            load_palette(rpg_asset_palette_4, 0x1D, 1, 4, 0xB);
            break;
        case 6:                                     /* switch 2 */
            load_palette(rpg_asset_palette_4, 0x1D, 1, 4, 1);
            break;
        case 7:                                     /* switch 2 */
            load_palette(rpg_asset_palette_4, 0x1D, 1, 4, 2);
            break;
        case 8:                                     /* switch 2 */
            load_palette(rpg_asset_palette_4, 0x1D, 1, 4, 4);
            break;
        case 9:                                     /* switch 2 */
            load_palette(rpg_asset_palette_4, 0x1D, 1, 4, 5);
            break;
        case 10:                                    /* switch 2 */
            load_palette(rpg_asset_palette_4, 0x1D, 1, 4, 3);
            break;
        case 11:                                    /* switch 2 */
            load_palette(rpg_asset_palette_4, 0x1D, 1, 4, 0xC);
            break;
        }
    }
    switch (rpg_girl_id) {                           /* switch 3 */
    case 0:                                         /* switch 3 */
        load_palette(rpg_asset_palette_3, 0x1C, 1, 4, 0);
        break;
    case 1:                                         /* switch 3 */
        load_palette(rpg_asset_palette_3, 0x1C, 1, 4, 1);
        break;
    case 2:                                         /* switch 3 */
        if (rpg_encounter != 0x80000000) {
            load_palette(rpg_asset_palette_3, 0x1C, 1, 4, 2);
        } else {
            load_palette(rpg_asset_palette_3, 0x1C, 1, 4, 3);
        }
        break;
    case 3:                                         /* switch 3 */
        load_palette(rpg_asset_palette_3, 0x1C, 1, 4, 3);
        break;
    case 4:                                         /* switch 3 */
        load_palette(rpg_asset_palette_3, 0x1C, 1, 4, 4);
        break;
    case 5:                                         /* switch 3 */
        load_palette(rpg_asset_palette_3, 0x1C, 1, 4, 5);
        break;
    case 6:                                         /* switch 3 */
        load_palette(rpg_asset_palette_3, 0x1C, 1, 4, 6);
        break;
    case 7:                                         /* switch 3 */
        load_palette(rpg_asset_palette_3, 0x1C, 1, 4, 7);
        break;
    case 8:                                         /* switch 3 */
        load_palette(rpg_asset_palette_3, 0x1C, 1, 4, 8);
        break;
    case 9:                                         /* switch 3 */
        load_palette(rpg_asset_palette_3, 0x1C, 1, 4, 9);
        break;
    case 10:                                        /* switch 3 */
        load_palette(rpg_asset_palette_3, 0x1C, 1, 4, 0xA);
        break;
    case 13:                                        /* switch 3 */
        load_palette(rpg_asset_palette_3, 0x1C, 1, 4, 0xD);
        break;
    case 14:
        break;
    }
    mode_stage_next();
}
