#include "common.h"
#include "ovl/KANGEI.h"

typedef struct {
    void (*f[30])();
} FnTbl30; /* size 0x78 */
extern FnTbl30 D_80139B30;

typedef struct {
    void (*f[37])();
} FnTbl37; /* size 0x94 */
typedef struct {
    void (*f[33])();
} FnTbl33; /* size 0x84 */
extern FnTbl37 D_80139BEC;
extern FnTbl33 D_80139C80;

/* Bit 1 of the flag word of a Rec38 record (tested with sll 30 / bgez). */
typedef struct KangeiFlagBits {
    u32 pad0 : 1;
    u32 flag : 1;
    u32 flag2 : 1;
    u32 rest : 29;
} KangeiFlagBits;

void func_80134120(void) {
    if ((D_800E6280.unk_F5F != 0xFF) && ((KangeiFlagBits *)&D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C)->flag) {
        func_801341E8();
        return;
    }
    if ((D_800E6280.unk_F5F != 0xFF) && !((KangeiFlagBits *)&D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C)->flag && ((u8)D_800E6280.unk_03E < 0x61U) && (D_800E6280.unk_F5F != 0xA)) {
        func_80134DB0();
        return;
    }
    func_80042808();
}

void func_801341E8(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl30 tbl;

    tbl = D_80139B30;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x20);
}

void func_8013425C(void) {
    func_80044890(1, 0xBF98, 0xBF79, (&D_800B3688)[D_800E6280.unk_F5F], (&D_800B36C8)[D_800E6280.unk_F5F], (&D_800B3708)[D_800E6280.unk_F5F]);
    if (func_80044E8C() == 1) {
        func_80044750(0x200);
        func_8004284C();
    }
    func_8004284C();
}

void func_801342EC(void) {
    if (func_80044E8C() == 1) {
        func_80044750(0x200);
        func_8004284C();
    }
    if ((u32) D_800E6280.unk_1104.w++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E6280.unk_110A - 1) & 0xFF);
    }
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80134120", func_80134374);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80134120", func_8013454C);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80134120", func_80134724);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80134120", func_801349D4);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80134120", func_80134BC4);

void func_80134DB0(void) {
    s32 pad; /* FAKE: unused slot above tblA, the original frame has 8 more bytes (T-3330 idiom, real source unknown). T-8010 */
    FnTbl37 tblA;
    FnTbl33 tblB;

    tblA = D_80139BEC;
    tblB = D_80139C80;
    if ((u8)D_800E6280.unk_75D < 4) {
        tblA.f[D_800E6280.unk_110A](0x80);
        D_80139AC8 = 1;
        return;
    }
    tblB.f[D_800E6280.unk_110A](0x80);
}

void func_80134EA8(void) {
    s32 pad; /* FAKE: unused local above the saved selector (the original frame has the copy at sp+0x28), T-6050 */
    u32 sp;

    D_800E6280.unk_F5F = 0xE;
    sp = D_800E6280.unk_110A;
    func_80133EF8();
    if (sp != D_800E6280.unk_110A) {
        D_800E6280.unk_F5F = D_800E6280.unk_75D;
    }
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80134120", func_80134F00);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80134120", func_80135004);

void func_801350E0(void) {
    RECT rect;

    func_80083474();
    rect.x = 0x140;
    rect.y = 0x80;
    rect.w = 0x180;
    rect.h = 0x80;
    func_8009C884(&rect, (void *)0x80180000);
}

void func_8013512C(void) {
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 = 0x32;
    func_80044750(0x24);
    func_80135438();
}

void func_80135178(void) {
    if (D_800E6280.unk_F5F == 6) {
        don_wait();
        return;
    }
    func_8004284C();
}

void func_801351B8(void) {
    func_80044750(0x23);
    func_80044750(0x24);
    func_80044750(0x2E);
    if ((u8) D_800E6280.unk_F5F < 4U) {
        func_80044750(0x202);
    } else {
        func_80044750(0x201);
    }
    func_8004284C();
}

void func_80135220(void) {
    func_80044750(0x24);
    func_80044750(0x501);
    func_8004284C();
}

void func_80135250(void) {
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 = 0x46;
    D_80139ADC = 3;
    func_801349D4();
    func_8004284C();
}

void func_801352A4(void) {
    if ((u8)D_800E6280.unk_F5F < 4U) {
        func_8007ED84(0x42A9);
        func_800AE0F0(D_800CA19C, "ロビー");
    } else {
        func_8007ED84(0x4296);
        func_800AE0F0(D_800CA19C, "グランド");
    }
    func_8004284C();
}

INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/80134120.rodata", D_8013980C);

void func_80135314(void) {
    bg_read_sub2(0x42C6);
    func_80085B3C(0xE, 0xB);
    func_800AE0F0(D_800CA1DC, &D_8013980C);
    func_8004284C();
}

void func_8013535C(void) {
    bg_read_sub2(0x429F);
    func_80085B3C(0xE, 0xC);
    func_8004284C();
}

void func_80135390(void) {
    if ((D_80139AE0 == 2) && (D_800E6280.unk_F5F == 6)) {
        D_80139AE4 = 2;
    } else {
        D_80139AE4 = 0;
    }
    func_80133EF8();
    if ((D_80139AE0 == 3) && (D_800E6280.unk_1104.w++ == 0)) {
        if (D_800E6280.unk_F5F == 6) {
            func_80044750(0x503);
            func_80047550();
        }
    }
}

/* Bits 1 and 2 of the current girl's flag word; the index is read again after the first store (T-7010). */
void func_80135438(void) {
    ((KangeiFlagBits *)&D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C)->flag = 1;
    ((KangeiFlagBits *)&D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C)->flag2 = 1;
    func_80042808();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80134120", func_801354AC);

void func_80135570(void) {
    switch (D_800E6280.unk_F5F) {
    case 1:
        func_80062CD0(0x5F30);
        break;
    case 3:
        func_80062CD0(0x5A9E);
        break;
    default:
        func_8004284C();
        func_8004284C();
        break;
    }
    func_8004284C();
}
