#include "common.h"
#include "ovl/GYOZI.h"

void func_8013DB50(void) {
    func_8013DB70();
}

typedef struct {
    void (*f[42])();
} FnTbl42; /* size 0xA8 */
extern FnTbl42 D_80147FB0;

void func_8013DB70(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl42 tbl;

    tbl = D_80147FB0;
    idx = D_800F647A;
    tbl.f[idx]();
}

void func_8013DBE4(void) {
    func_8013A820();
    if (D_801474AC == 2) {
        D_8012E6B0 = 1;
        return;
    }
    D_8012E6B0 = 0;
}

/* bulk_m2c declarations (T-11190) */
extern u8 D_800F5788;
/* end bulk_m2c declarations */
void func_8013DC2C(void) {
    func_8004EDE0(1, 0);
    func_80054864(1);
    func_8004C670();
    func_8005493C(0);
    func_8004EDF4(1);
    func_80053EFC();
    func_8005ABC0();
    D_800F6412 = 0;
    D_800F6458 = 1;
    D_800F5788 |= 8;
    func_8009068C();
    D_800F53DA = 0x80;
    D_801317EB = 0;
    D_800C51C4 = 0;
    func_80089200();
    func_800744A0();
    func_80074950();
    func_8008FC10();
    D_800F5ACD = 0xA;
    D_800F62CF = 0xA;
    func_8008F618(0xA);
    func_8013A140();
    D_8014749C = D_80147450;
    D_801474A0 = D_80147474;
    D_801474A4 = D_80147498;
    func_8004DE1C();
}

INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/8013DB50.rodata", D_80145C40);

void func_8013DD38(void) {
    D_801474A8 = 0;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, &D_80145C40);
    func_8004DE1C();
}

void func_8013DD7C(void) {
    func_8008A0D4(0x3FC0);
    func_8004DE1C();
}

void func_8013DDA4(void) {
    if ((u8) D_800F5833 < 2U) {
        D_801474A8 += 1;
    }
    func_8004DE1C();
}

void func_8013DDE8(void) {
    if ((u8) D_800F5833 >= 2U) {
        D_801474A8 += 1;
    }
    func_8004DE1C();
}
