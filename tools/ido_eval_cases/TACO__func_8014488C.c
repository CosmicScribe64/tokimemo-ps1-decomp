#include "common.h"
#include "ovl/TACO.h"

void func_800438DC();
void func_80058D20();
extern u8 D_800E7ABC;
extern s16 D_80128888;
extern s16 D_8012888A;
extern s16 D_8012888C;
extern s16 D_80128890;
extern s16 D_80128892;
extern s16 D_80128894;
extern s32 D_8015F82C;
extern s32 D_8015F830;

void func_8014488C(void) {
    D_800E62B6 = 8;
    D_800E62B8 = 0x10;
    D_800E62B7 = 8;
    func_800438DC(1, 1);
    func_80058D20();
    func_80146E3C();
    func_80146D60(-0xFA0);
    func_801471D0(0x8019C800);
    func_80144CC0(D_800E78BC);
    func_80144CC0(&D_800E7ABC);
    func_8009C674(0);
    func_80144D90(7);
    func_80147488(1, D_8015F82C, 0);
    D_80127090.unk0 = 0;
    D_80128888 = -0x5A;
    D_8012888C = 0;
    D_8012888A = 0x5A;
    func_80147488(2, D_8015F830, 0);
    D_801270A0.unk0 = 0;
    D_80128890 = -0x5A;
    D_80128894 = 0;
    D_80128892 = -0x5A;
    func_8004284C();
}
