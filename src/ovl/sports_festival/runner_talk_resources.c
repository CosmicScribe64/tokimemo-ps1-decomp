/* runner_talk_resources.c: ; src/ovl/sports_festival/festival_entry_and_competition_select.c:73-77 calls it immediately before runner talk-table setup. Original object 0x80132B90-0x80132BD0 (config/objects/TAIIKU.txt). */
#include "common.h"
#include "ovl/sports_festival.h"

void sports_runner_talk_resource_addresses_init(void) {
    D_80148F00 = 0x801F016C;
    D_80148F04 = 0x801F01A4;
    D_80148F08 = 0x801F0284;
}
