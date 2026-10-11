#include "common.h"
#include "ovl/BUNKAKEN.h"

typedef struct {
    s32 w[6];
} Tbl6_80144D38;
extern Tbl6_80144D38 D_80155F70;

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80143DD0", func_80143DD0);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80143DD0", func_80144A70);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80143DD0", func_80144C5C);

void func_80144D38(void) {
    s32 i;
    s32 pad[2]; /* FAKE: unused local above the arrays reproduces the original frame 0xC0; real source unknown. T-9350 */
    s16 a[6];
    s16 b[6];
    s16 c[6];
    s16 d[6];
    Tbl6_80144D38 tbl;

    tbl = D_80155F70;
    func_8004E9F4(1);
    for (i = 0; i < 6; i++) {
        a[i] = (i & 1) * 0x78 - 0x80;
        b[i] = (i >> 1) * 0x10 + 0x32;
        func_8004E788(a[i], b[i], 0, tbl.w[i], 0);
        c[i] = 0x78;
        d[i] = 0x10;
    }
    D_80122D30 = 1;
    func_8004E884(7);
    func_8004F870(0, 6, a, b, c, d);
    func_8006BC28(0);
}

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80143DD0", func_80144E9C);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80143DD0.rodata", D_80154920);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80143DD0.rodata", D_80154930);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80143DD0.rodata", D_80154940);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80143DD0.rodata", D_80154950);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80143DD0.rodata", D_80154960);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80143DD0.rodata", D_80154970);
