/* candidate_scene_layer_resources.c: ; src/ovl/sports_festival/candidate_girl_scene.c:256-260 uses outputs in layer_tileset_init, srn_init and srn_vram_set. Original object 0x80133C30-0x80133C80 (config/objects/TAIIKU.txt). */
#include "common.h"
#include "ovl/sports_festival.h"

void sports_candidate_scene_layer_resource_addresses_init(void) {
    D_801491C0 = 0x801C3000;
    D_801491C4 = 0x801C300C;
    D_801491C8 = 0x801C479C;
    D_801491CC = 0x801B3000;
}
