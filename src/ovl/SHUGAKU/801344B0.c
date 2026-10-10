#include "common.h"
#include "ovl/SHUGAKU.h"

typedef struct {
    void (*f[38])();
} FnTbl38; /* size 0x98 */
extern FnTbl38 D_8013C0EC;

void func_801344B0(void) {
    D_8013C0C0 = (u8 *)0x801D22B8;
    D_8013C0C4 = (u8 *)0x801D22C0;
    D_8013C0C8 = (u8 *)0x801D2310;
    D_8013C0CC = *(s16 *)0x801D2328;
    D_8013C0D0 = (u8 *)0x801B0000;
    D_8013C0D4 = (u8 *)0x801B2000;
    D_8013C0D8 = (u8 *)0x801B6000;
    D_8013C0DC = (u8 *)0x801BA000;
    D_8013C0E0 = (u8 *)0x801BE000;
    D_8013C0E4 = (u8 *)0x801C2000;
    D_8013C0E8 = (u8 *)0x801C6000;
}

void func_80134560(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl38 tbl;

    tbl = D_8013C0EC;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_801345E8(void) {
    func_80044750(0x24);
    func_80044750(0x206);
    func_80044750(0x504);
    func_8004284C();
}

void func_80134620(void) {
    func_80046318(0x45, 0x801B0000, 0x8902);
    func_801344B0();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/801344B0", func_80134658);

void func_801347C8(void) {
    bg_read_sub2(0x4285);
    func_8004284C();
}

void func_801347F0(void) {
    bg_read_sub2(0x4774);
    func_80085B3C(2, 0x27);
    func_8004284C();
}

void func_80134824(void) {
    s32 pad; /* FAKE: unused local above `unused`, puts it at sp+0x2B as in the original; real source unknown. T-4010 */
    u8 unused; /* read uninitialised: the original passes the stack byte */

    func_80051B48(D_800E6280.unk_F5F);
    func_800634FC(unused);
    func_80048F64(0x60);
    func_80048F64(0x61);
    func_80048F64(0x62);
    func_8004284C();
}
