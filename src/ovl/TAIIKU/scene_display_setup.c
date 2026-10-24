/* scene_display_setup.c: ; src/ovl/TAIIKU/801488F0.c:5-9 sets ambient light, camera parameters and scene display state. Original object 0x801488F0-0x80148940 (config/objects/TAIIKU.txt). */
#include "common.h"
#include "ovl/TAIIKU.h"

/* Built with PsyQ gcc 2.8.1 (config/toolchains.txt, T-9200). */
void func_801488F0(s16 *p) {
    func_800A13AC(p[0]);
    func_800A13B8(*(s32 *)(p + 2));
    func_800A12F4(*((u8 *)p + 8), *((u8 *)p + 9), *((u8 *)p + 10));
}
