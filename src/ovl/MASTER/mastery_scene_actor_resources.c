/* mastery_scene_actor_resources.c: ; src/ovl/MASTER/club_master_scene_and_tables.c:1015-1019 binds actor frame/sequence addresses for its scene. Original object 0x80138F70-0x80138FC0 (config/objects/MASTER.txt). */
#include "ovl/MASTER.h"

void master_set_companion_actor_assets_c(void) {
    D_8013C740 = 0x8019DCFC;
    D_8013C744 = 0x8019DD00;
    D_8013C748 = 0x8019DDC0;
    D_8013C74C = *(s16 *)0x8019DDF8;
}
