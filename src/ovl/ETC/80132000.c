#include "common.h"
#include "ovl/ETC.h"

void func_80132000(void) {
    if ((D_800E7208 & 0x40) || ((D_800E7378 & 0x2FF) == 0x2FF)) {
        func_80042808();
    }
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80132000", func_80132048);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80132000", func_80132098);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80132000", func_80132148);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80132000", func_80132198);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80132000", func_80132204);

void func_80132254(void) {
    func_8004E58C();
    func_8006612C("外井告白シーン");
    k_disp_start(1);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80132000", func_80132290);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80132000", func_801322E0);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80132000", func_80132344);

void func_80132394(void) {
    func_8004E58C();
    func_8006612C("机の中に手紙が");
    k_disp_start(1);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80132000", func_801323D0);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80132000", func_80132460);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80132000", func_801324B0);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80132000", func_80132558);

void func_801325A8(void) {
    func_8004E58C();
    func_8006612C("シルエット画面");
    k_disp_start(1);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80132000", func_801325E4);

void func_80132634(void) {
    func_8004E58C();
    func_8006612C("バストアップスクロール");
    k_disp_start(1);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80132000", func_80132670);

void func_801326C0(void) {
    func_8004E58C();
    func_8006612C("告白シーン");
    k_disp_start(1);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80132000", func_801326FC);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80132000", func_8013275C);

void func_801327AC(void) {
    func_80044750(0x200);
    func_8004E58C();
    func_8006612C("エピローグ用初期化");
    k_disp_start(1);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80132000", func_801327F0);

void func_80132840(void) {
    func_8004E58C();
    func_8006612C("エピローグ");
    k_disp_start(1);
    func_8004284C();
}
