/* candidate_scene_picture_resources.c: ; src/ovl/TAIIKU/80138950.c:139-143 calls it during candidate-scene setup. Original object 0x80141BB0-0x80141C00 (config/objects/TAIIKU.txt). */
#include "common.h"
#include "ovl/TAIIKU.h"

void sports_candidate_scene_picture_resource_addresses_init(void) {
    D_8014A0B0 = 0x801A1000;
    D_8014A0B4 = 0x8018F000;
    D_8014A0B8 = 0x801A2198;
    D_8014A0BC = 0x801A1008;
}
