#include "common.h"
#include "ovl/SHOUGATU.h"

/* Bit 2 of the girl record flag byte. T-9720. */
typedef struct ShougatuFlag4 {
    /* 0x00 */ u8 pad : 2;
    /* 0x00 */ u8 set : 1;
    /* 0x00 */ u8 rest : 5;
} ShougatuFlag4; /* size 0x01 */

void func_8013A190(void) {
    if (D_80144E14 == 0) {
        func_8013A1CC();
        return;
    }
    func_8013A27C();
}

void func_8013A1CC(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl26 tbl;

    tbl = D_801455E0;
    idx = D_800E6280.unk_110A;
    tbl.f[idx]();
}

void func_8013A254(void) {
    func_80044750(0x502);
    func_8004284C();
}

void func_8013A27C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl19 tbl;

    tbl = D_80145648;
    idx = D_800E6280.unk_110A;
    tbl.f[idx]();
}

void func_8013A2F8(void) {
    func_800438DC(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    func_800438F0(1);
    func_80048390();
    func_8004E58C();
    D_800E6280.unk_10A2 = 0;
    D_800E6280.unk_10E8 = 1;
    func_8008585C();
    D_800E6280.unk_03A = 0x80;
    D_800B593C = 0;
    D_800B5940 = 0;
    func_8007C740();
    func_800649D4();
    func_80064E84();
    func_80084E4C();
    ((ShougatuFlag4 *) (&D_800E6280.unk_1BC[5].unk_0C.b[0]))->set = 1;
    /* FAKE: the dead triple test preserves the original argument/store scheduling.
     * IDO removes all of its reads and tests. T-9720. */
    if ((!D_80122EBC) && (!D_80122EBC) && (!D_80122EBC)) {}
    func_800847B8(*((u8 *) (&D_800E6280.unk_F5F)) = 5);
    func_80136F90();
    D_80144DFC = D_80144DA4;
    D_80144E00 = D_80144DC8;
    D_80144E04 = D_80144DEC;
    func_8004284C();
}

void func_8013A3FC(void) {
    D_80144E08 = 0;
    D_80144E0C = 0;
    func_800AE0F0(D_800CA19C, "グランド");
    func_8004284C();
}

void func_8013A440(void) {
    D_80144E08 = 5;
    D_80144E0C = 0;
    func_800AE0F0(D_800CA19C, "グランド");
    func_8004284C();
}

void func_8013A488(void) {
    bg_read_sub2(0x4097);
    func_8004284C();
}

void func_8013A4B0(void) {
    if (D_800E6280.unk_F5F == 5) {
        func_80062CD0(0x5FE4);
    } else {
        func_80062CD0(0x552B);
    }
    func_8004284C();
}
