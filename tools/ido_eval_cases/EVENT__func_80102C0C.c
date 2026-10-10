#include "common.h"
#include "ovl/EVENT.h"

extern s8 D_800EAFA4;
extern s8 D_800EAFE8;

void func_80102C0C(void) {
    func_801024B0();
    func_80012D64(D_80122690, 0x11, 1, 2, 0);
    func_8004C46C(D_80122694, D_80122698, D_8012269C, D_801226A0, D_801226A4, D_801226A8);
    func_8004C6B0(D_80122684, D_80122688, D_80122680, (s32) D_8012268C);
    D_800EAFA4 = 8;
    D_800EAFE8 = 8;
    func_80011DFC();
}
