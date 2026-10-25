/* nozomi_scene_picture_resources.c: ; src/ovl/TAIIKU/80141C00.c:120-125 calls it before loading Nozomi scene pictures. Original object 0x80145210-0x801452D0 (config/objects/TAIIKU.txt). */
#include "common.h"
#include "ovl/TAIIKU.h"

void sports_nozomi_scene_picture_resource_addresses_init(void) {
    D_8014A390 = 0x801B523C;
    D_8014A394 = 0x801B518C;
    D_8014A398 = 0x801B51DC;
    D_8014A39C = *(s16 *)0x801B5240;
    D_8014A3A0 = 0x80197000;
    D_8014A3A4 = 0x80199000;
    D_8014A3A8 = 0x8019D000;
    D_8014A3AC = 0x801A1000;
    D_8014A3B0 = 0x801A5000;
    D_8014A3B4 = 0x801A9000;
    D_8014A3B8 = 0x801AD000;
}
