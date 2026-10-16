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

void func_801509A4(void) {
    BunkasaiGirlList list;
    u8 sp63;
    Rec38 *var_a0;
    s32 temp_v1;
    s32 var_v1;
    u8 var_a1;

    list = D_80160AA4;
    func_800AE120(D_800E6280.unk_10F4);
    var_a1 = 0;
    var_a0 = D_800E6280.unk_1BC;
    var_v1 = 0;
    do {
        if (((u32) (var_a0->unk_0C.w << 0x1E) >> 0x1F) == 1) {
            list.v[var_a1] = var_v1;
            var_a1 = (var_a1 + 1) & 0xFF;
        }
        var_v1 += 1;
        var_a0 += 1;
    } while (var_v1 != 0xB);
    sp63 = var_a1;
    temp_v1 = list.v[func_800AE0D0() % var_a1];
    D_80122EBC = temp_v1;
    switch (temp_v1) {
    case 0:
        func_80042908(0xF);
        return;
    case 1:
        func_80042908(0x1C);
        return;
    case 2:
        func_80042908(0x2E);
        return;
    case 3:
        func_80042908(0x41);
        return;
    case 4:
        func_80042908(0x55);
        return;
    case 5:
        func_80042908(0x61);
        return;
    case 6:
        func_80042908(0x6D);
        return;
    case 7:
        func_80042908(0x79);
        return;
    case 8:
        func_80042908(0x85);
        return;
    case 9:
        func_80042908(0x91);
        return;
    case 10:
        func_80042908(0x9A);
        return;
    default:
        func_80046500();
        return;
    }
}

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80150930", func_80150B7C);

void func_80150F20(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80048EB8(0);
        func_8006BD6C(0);
        func_80041168(0xFF);
        func_8004284C();
        return;
    case 1:
        func_80059E00();
        return;
    case 2:
        func_80059BC0();
        return;
    case 3:
        func_8004ADE4();
        func_8004284C();
        return;
    case 4:
        cd_read_sectors(0x95U, 0x801A0000, 0x9B94);
        func_8004284C();
        return;
    case 5:
        if (cd_read_sync_status() & 1) {
            func_80068938(D_800E6280.unk_03E, D_800E6280.unk_03F, 0);
            func_8004284C();
            return;
        }
        return;
    case 6:
        func_8006BC28(0);
        func_8006BD6C(0);
        D_800B5BC8 = 0xFE;
        func_80042908((s32) (*(u8 *)&D_80122EB0));
        func_80042940((s32) (*(u8 *)&D_80122EB4));
        return;
    default:
        func_80046500();
        break;
    }
}

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
