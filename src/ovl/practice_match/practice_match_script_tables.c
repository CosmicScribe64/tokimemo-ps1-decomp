/* practice_match_script_tables.c: ; src/ovl/RENSYU/80132000.c:3-7 binds three practice-match talk tables. Original object 0x80132000-0x80132040 (config/objects/RENSYU.txt). */
#include "ovl/practice_match.h"

void practice_intro_talk_pointer_setup(void) {
    D_801342B0 = 0x801E990C;
    D_801342B4 = 0x801E9C40;
    D_801342B8 = 0x801EB2D8;
}
