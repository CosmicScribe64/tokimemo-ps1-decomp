#include "common.h"
#include "ovl/BUNKASAI.h"

typedef struct {
    void (*f[165])();
} FnTbl165; /* size 0x294 */
extern FnTbl165 D_80160810;

void func_80150930(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl165 tbl;

    tbl = D_80160810;
    idx = D_800E6280.unk_1109;
    tbl.f[idx]();
}

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80150930", func_801509A4);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80150930", func_80150B7C);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80150930", func_80150F20);

void func_80151064(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        cd_read_sectors(1U, 0x801E8000, 0x4B7F);
        func_8004284C();
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_8004E9F4(0);
            func_8006612C((s32) "天の声");
            func_8004E884(1);
            func_801323DC();
            D_800E6280.unk_F5F = 8;
            func_800847B8(8U);
            func_800AE0F0(D_800CA188, "　");
            D_800CA148 = 0;
            D_800CA14C = 0;
            D_800CA160 = (s32) D_801604F0;
            D_800CA164 = (s32) D_801604F4;
            D_800CA168 = (s32) D_801604F8;
            func_8004284C();
        }
        break;
    case 2:
        func_80085A60();
        break;
    case 3:
        func_8007E934();
        break;
    case 4:
        if (D_800E6280.unk_F74 == 1) {
            func_80048EB8(0);
            func_8006BD6C(0);
            func_80057390(0);
            main_state_set(0xD0);
        }
        break;
    default:
        func_80046500();
        break;
    }
}
