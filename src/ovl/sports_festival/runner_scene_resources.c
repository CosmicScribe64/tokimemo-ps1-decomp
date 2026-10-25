/* runner_scene_resources.c: ; src/ovl/sports_festival/runner_actor_display.c:129-133 calls this after runner-scene sector reads; it binds runner resource addresses. Original object 0x80132B40-0x80132B90 (config/objects/TAIIKU.txt). */
#include "common.h"
#include "ovl/sports_festival.h"

void sports_runner_resource_addresses_init(void) {
    D_80148EF0 = 0x801A19CC;
    D_80148EF4 = 0x8018F000;
    D_80148EF8 = 0x8019D000;
    D_80148EFC = 0x8019E190;
}
