#include "common.h"
#include "ovl/ETC.h"

void func_80132000(void) {
    if ((D_800E6280.unk_F88 & 0x40) || ((D_800E6280.unk_10F8 & 0x2FF) == 0x2FF)) {
        func_80042808();
    }
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80132000", func_80132048);

void func_80132098(void) {
    switch (D_800E6280.unk_1109) {
    case 0:
        func_80132148();
        break;
    case 1:
        func_80132204();
        break;
    case 2:
        func_80132290();
        break;
    case 3:
        func_80132344();
        break;
    }
    func_80064F48();
    func_800646CC();
    func_80064DEC();
    func_80066C08(0);
    func_8006BA40();
    func_80066334();
}
void func_80132148(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80132198();
        break;
    case 1:
        func_80132000();
        break;
    }
}
void func_80132198(void) {
    s32 i;

    for (i = 0x60; i < 0xA0; i++) {
        func_80048F64(i);
    }
    func_8004E58C();
    func_8006612C("告白判定");
    func_800649D4();
    k_disp_start(1);
    func_8004284C();
}

void func_80132204(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80132254();
        break;
    case 1:
        func_80132000();
        break;
    }
}
void func_80132254(void) {
    func_8004E58C();
    func_8006612C("外井告白シーン");
    k_disp_start(1);
    func_8004284C();
}

void func_80132290(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_801322E0();
        break;
    case 1:
        func_80132000();
        break;
    }
}
void func_801322E0(void) {
    func_80044774(0);
    func_80046290(0x3000656, 0xB000E0C, 0xF);
    func_80044750(0x300);
    func_8004E58C();
    func_8006612C("女々しい野郎どもへ");
    k_disp_start(1);
    func_8004284C();
}

void func_80132344(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80132394();
        break;
    case 1:
        func_80132048(0x63);
        break;
    }
}
void func_80132394(void) {
    func_8004E58C();
    func_8006612C("机の中に手紙が");
    k_disp_start(1);
    func_8004284C();
}

void func_801323D0(void) {
    switch (D_800E6280.unk_1109) {
    case 0:
        func_80132460();
        break;
    case 1:
        func_80132558();
        break;
    case 2:
        func_801325E4();
        break;
    case 3:
        func_80132670();
        break;
    }
    func_80066C08(0);
    func_80066334();
}
void func_80132460(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_801324B0();
        break;
    case 1:
        func_80132000();
        break;
    }
}
void func_801324B0(void) {
    if (D_800E6280.unk_110D == 0) {
        func_80044890(0, 0, 0, 0xCE6D, 0xCE33, 0xCE1E);
        D_800E6280.unk_110D += 1;
    } else if (func_80044E8C() == 1) {
        func_80044750(0x201);
        func_80048EB8(0);
        func_8004E58C();
        func_8006612C("告白シーンデータリード");
        k_disp_start(1);
        func_8004284C();
    }
}

void func_80132558(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_801325A8();
        break;
    case 1:
        func_80132000();
        break;
    }
}
void func_801325A8(void) {
    func_8004E58C();
    func_8006612C("シルエット画面");
    k_disp_start(1);
    func_8004284C();
}

void func_801325E4(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80132634();
        break;
    case 1:
        func_80132000();
        break;
    }
}
void func_80132634(void) {
    func_8004E58C();
    func_8006612C("バストアップスクロール");
    k_disp_start(1);
    func_8004284C();
}

void func_80132670(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_801326C0();
        break;
    case 1:
        func_80132048(0xC1);
        break;
    }
}
void func_801326C0(void) {
    func_8004E58C();
    func_8006612C("告白シーン");
    k_disp_start(1);
    func_8004284C();
}

void func_801326FC(void) {
    switch (D_800E6280.unk_1109) {
    case 0:
        func_8013275C();
        break;
    case 1:
        func_801327F0();
        break;
    }
    func_80066334();
    func_80066C08(0);
}
void func_8013275C(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_801327AC();
        break;
    case 1:
        func_80132000();
        break;
    }
}
void func_801327AC(void) {
    func_80044750(0x200);
    func_8004E58C();
    func_8006612C("エピローグ用初期化");
    k_disp_start(1);
    func_8004284C();
}

void func_801327F0(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80132840();
        break;
    case 1:
        func_80132048(0xC2);
        break;
    }
}
void func_80132840(void) {
    func_8004E58C();
    func_8006612C("エピローグ");
    k_disp_start(1);
    func_8004284C();
}
