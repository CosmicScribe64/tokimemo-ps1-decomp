#include "common.h"
#include "ovl/SHUGAKU.h"

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
    ShFnTbl38 tbl;

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
    cd_read_sectors(0x45, 0x801B0000, 0x8902);
    func_801344B0();
    func_8004284C();
}

/* FAKE: `* 0 + 8` folds to `li v1, 8` after uopt copy propagation, promoting D_80120698 to $v1 across the block (T-24104, cf. T-22021). */
void func_80134658(void) {
    func_801365F4();
    D_8013BFFC = D_8013C57C;
    D_8013C000 = D_8013C6B8;
    D_8013C004 = D_8013C7F4;
    func_801344B0();
    load_palette(D_8013C0D0, 0x11, 1, 2, 0);
    func_80084E90(D_8013C0D4, D_8013C0D8, D_8013C0DC, D_8013C0E0, D_8013C0E4, D_8013C0E8);
    func_800850D4(D_8013C0C4, D_8013C0C8, D_8013C0C0, D_8013C0CC);
    D_80120698 = D_80120698 * 0 + 8;
    D_80120654 = D_80120698;
    D_800CA2E4 = 0;
    D_800CA2E8 = 0;
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_02 += 1;
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 += 2;
    check_para_limit();
    func_800AE0F0(D_800CA19C, "ホテル・廊下");
    func_8004284C();
}

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
