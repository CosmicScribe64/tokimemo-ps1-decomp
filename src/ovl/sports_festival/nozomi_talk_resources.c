/* nozomi_talk_resources.c: ; src/ovl/TAIIKU/80141C00.c:76-82 calls it before setting Nozomi talk tables. Original object 0x80142140-0x80142180 (config/objects/TAIIKU.txt). */
#include "common.h"
#include "ovl/sports_festival.h"

void sports_nozomi_talk_resource_addresses_init(void) {
    D_8014A0D0 = 0x801E8294;
    D_8014A0D4 = 0x801E82C8;
    D_8014A0D8 = 0x801E8468;
}
