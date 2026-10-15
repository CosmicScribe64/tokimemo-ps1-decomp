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

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E6480;
extern s32 D_800E64B8;
extern s32 D_800E64F0;
extern u16 D_800E6374;
extern u8 D_800E62BE;
extern u8 D_800E644A;
extern u8 D_800E738A;
/* end bulk_m2c declarations */
void func_80150B7C(void) {
    u32 temp_t2;

    switch (D_800E738A) {
    case 0:
        D_800CA148 = ((((u16) D_800E6374 >> 0xC) * 3) + D_800E62BE) - 0x4D;
        func_8004284C();
        return;
    case 1:
        normal_date_speak(2);
        return;
    case 2:
        D_80122EB8 = (s32) ((u16) D_800E6374 >> 0xC);
        temp_t2 = (u16) D_800E6374 >> 0xC;
        switch (temp_t2) {
        case 0:
            if (((u8) D_800E644A >> 4) == 0) {
                func_80042908(1);
                return;
            }
            if ((((u8) D_800E6280.unk_1BC[1].unk_0C.b[2] >> 4) == 0) && (((u32) (D_800E6280.unk_1BC[1].unk_0C.w << 0x1E) >> 0x1F) == 1)) {
                func_80042908(0x2A);
                return;
            }
            func_80042908(0xE);
            return;
        case 1:
            if (((u8) D_800E644A >> 4) == 1) {
                func_80042908(1);
                return;
            }
            if ((((u8) D_800E6280.unk_1BC[1].unk_0C.b[2] >> 4) == 1) && (((u32) (D_800E6480 << 0x1E) >> 0x1F) == 1)) {
                func_80042908(0x2A);
                return;
            }
            func_80042908(0xE);
            return;
        case 2:
            if (((u8) D_800E644A >> 4) == 2) {
                func_80042908(1);
                return;
            }
            if ((((u8) D_800E6280.unk_1BC[2].unk_0C.b[2] >> 4) == 2) && (((u32) (D_800E64B8 << 0x1E) >> 0x1F) == 1)) {
                func_80042908(0x3B);
                return;
            }
            func_80042908(0xE);
            return;
        case 3:
            if (((u8) D_800E644A >> 4) == 3) {
                func_80042908(1);
                return;
            }
            if ((((u8) D_800E6280.unk_1BC[2].unk_0C.b[2] >> 4) == 3) && (((u32) (D_800E6280.unk_1BC[2].unk_0C.w << 0x1E) >> 0x1F) == 1)) {
                func_80042908(0x3B);
                return;
            }
            func_80042908(0xE);
            return;
        case 4:
            if (((u8) D_800E644A >> 4) == 4) {
                func_80042908(1);
                return;
            }
            if ((((u8) D_800E6280.unk_1BC[3].unk_0C.b[2] >> 4) == 4) && (((u32) (D_800E64F0 << 0x1E) >> 0x1F) == 1)) {
                func_80042908(0x50);
                return;
            }
            func_80042908(0xE);
            return;
        case 5:
            if (((u8) D_800E644A >> 4) == 5) {
                func_80042908(1);
                return;
            }
            if ((((u8) D_800E6280.unk_1BC[3].unk_0C.b[2] >> 4) == 5) && (((u32) (D_800E6280.unk_1BC[3].unk_0C.w << 0x1E) >> 0x1F) == 1)) {
                func_80042908(0x50);
                return;
            }
            func_80042908(0xE);
            return;
        default:
            func_80046500();
            return;
        }
        break;
    default:
        func_80046500();
        return;
    }
}

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
