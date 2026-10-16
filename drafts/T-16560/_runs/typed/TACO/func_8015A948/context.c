#include "common.h"
#include "ovl/TACO.h"

void func_8015A900(void) {
    D_801604C0 = 0;
    D_801604C4 += 1;
    D_801604D0 = 0;
}

void func_8015A928(void) {
    D_801604C8 = 0;
    D_801604CC += 1;
}

/* bulk_m2c declarations (T-11190) */
void func_8015AB80(s32 arg0);
void func_8015AB18(s32 arg0);
/* end bulk_m2c declarations */
void func_8015A948(s32 arg0, s32 arg1) {
    u32 temp_t6;

    temp_t6 = arg0 & 0xFF;
    switch (temp_t6) {
    case 0x1:
        func_80155230(arg1);
        return;
    case 0x2:
        func_801557E8(arg1);
        return;
    case 0x3:
        func_80155C64(arg1);
        return;
    case 0x4:
        func_80155E10(arg1);
        return;
    case 0x5:
        func_80155F98(arg1);
        return;
    case 0x6:
        func_80156244(arg1);
        return;
    case 0x7:
        func_80156924(arg1);
        return;
    case 0x10:
        func_801570A4(arg1);
        return;
    case 0x13:
        func_80157EA8(arg1);
        return;
    case 0x14:
        func_80157CD4(arg1);
        return;
    case 0x12:
        func_8015776C(arg1);
        return;
    case 0x11:
        func_801585D0(arg1);
        return;
    case 0x15:
        func_80158B84(arg1);
        return;
    case 0x16:
        func_80158F48(arg1);
        return;
    case 0x20:
        func_80159678(arg1);
        return;
    case 0x21:
        func_80159F60(arg1);
        return;
    case 0x22:
        func_8015A378(arg1);
        return;
    case 0x23:
        func_8015A680(arg1);
        return;
    case 0xFD:
        func_8015AB80(arg1);
        return;
    case 0xFE:
        func_8015AB18(arg1);
        return;
    case 0xFF:
        func_8015C718(arg1);
        /* fallthrough */
        return;
    }
}

void func_8015AB18(s32 arg0) {
    (arg0 + D_8015EDB4)->unk78 += 0x100;
    if ((arg0 + D_8015EDB4)->unk78 > 0x1000) {
        func_8015C208();
    }
}

void func_8015AB80(s32 arg0) {
    (arg0 + D_8015EDB4)->unk78 -= 0xFA;
    if ((arg0 + D_8015EDB4)->unk78 < -0x7530) {
        func_8015C208();
    }
}
