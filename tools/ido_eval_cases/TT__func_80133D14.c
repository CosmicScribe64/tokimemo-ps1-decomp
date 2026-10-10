#include "common.h"
#include "ovl/TT.h"

s32 func_800A0070();
s32 func_800A0140();
u8 *func_80133BA4();
s32 func_80133BE0();
void func_80133C1C();
extern s32 D_80158AA0;

void func_80133D14(s16 arg0, s16 arg1, s32 arg2, u16 arg3) {
    u8 *sp38;
    s32 sp34;
    s32 temp_v0_2;
    u8 *temp_v0;

    sp38 = D_80158A74;
    temp_v0 = func_80133BA4(D_80158AA0, D_80158AA0 + 0x3400);
    if (temp_v0 != 0) {
        *(s32 *)(temp_v0 + 0x20) = arg0 << 0x10;
        *(s32 *)(temp_v0 + 0x24) = arg1 << 0x10;
        temp_v0_2 = func_80133BE0(temp_v0, sp38);
        sp34 = temp_v0_2;
        *(s32 *)(temp_v0 + 0x28) = func_800A0140(temp_v0_2) * arg2;
        *(s32 *)(temp_v0 + 0x2C) = func_800A0070(sp34) * arg2;
        switch (arg3) {                             /* irregular */
        case 0xA0:
            func_80133C1C(temp_v0, 0xA0, 0x12);
            break;
        case 0xA8:
            func_80133C1C(temp_v0, 0xA8, 0x15);
            break;
        }
        *(s16 *)(temp_v0 + 0x4) = 0x40;
        *(s16 *)(temp_v0 + 0xA) = 0;
        *(s16 *)(temp_v0 + 0x8) = 0;
        *(s16 *)(temp_v0 + 0x6) = 0;
        *(u8 *)(temp_v0 + 0xF) = 0;
        *(u8 *)(temp_v0 + 0xE) = 0;
        *(u8 *)(temp_v0 + 0xD) = 0;
    }
}
