/* folk_dance_talk_resources.c: ; src/ovl/TAIIKU/80132000.c:119-126 calls it before setting folk-dance talk tables. Original object 0x80133BF0-0x80133C30 (config/objects/TAIIKU.txt). */
#include "common.h"
#include "ovl/sports_festival.h"

void sports_folk_dance_talk_resource_addresses_init(void) {
    D_801491B0 = 0x801E8564;
    D_801491B4 = 0x801E8694;
    D_801491B8 = 0x801E8BE0;
}
