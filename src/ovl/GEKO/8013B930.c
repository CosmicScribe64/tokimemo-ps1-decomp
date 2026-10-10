#include "common.h"
#include "ovl/GEKO.h"

void func_8013B930(void) {
    bg_read_sub2(0x46A3);
    func_8004284C();
}

void func_8013B958(void) {
    bg_read_sub2(0x42D8);
    func_8004284C();
}

void func_8013B980(void) {
    D_80146780 = 0x801B0000;
    D_80146784 = 0x801B2000;
    D_80146788 = 0x801B6000;
    D_8014678C = 0x801BA000;
    D_80146790 = 0x801BE000;
    D_80146794 = 0x801C2000;
    D_80146798 = 0x801C6000;
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013B930", func_8013B9F0);

void func_8013BA6C(void) {
    if (D_800E6280.unk_1109 == 0) {
        func_8013BBE0();
        return;
    }
    func_80046500();
}

void func_8013BAA8(void) {
    if (D_800E6280.unk_1109 == 0) {
        func_8013BD04();
        return;
    }
    func_80046500();
}

void func_8013BAE4(void) {
    if (D_800E6280.unk_1109 == 0) {
        func_8013BE58();
        return;
    }
    func_80046500();
}

void func_8013BB20(void) {
    func_8004500C(0, 0x200);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013B930", func_8013BB4C);

typedef struct {
    void (*f[58])();
} FnTbl58; /* size 0xE8 */
extern FnTbl58 D_8014679C;

void func_8013BBE0(void) {
    s32 idx; /* unused: its stack slot sits above tbl (T-3330) */
    FnTbl58 tbl;

    tbl = D_8014679C;
    if (D_800E6280.unk_110A == 0x1B) {
        func_80044750(0xCF);
    }
    tbl.f[D_800E6280.unk_110A](0x80);
}

void func_8013BC74(void) {
    s32 cur; /* declared before prev: its stack slot sits above prev (T-3330) */
    s32 prev;

    prev = D_800E6280.unk_110A;
    func_8007C8A4();
    cur = D_800E6280.unk_110A;
    if (prev != cur) {
        D_800E6280.unk_110A = cur - 1;
        func_8007EC68();
    }
}

void func_8013BCBC(void) {
    s32 cur; /* declared before prev: its stack slot sits above prev (T-3330) */
    s32 prev;

    prev = D_800E6280.unk_110A;
    func_8007E81C();
    cur = D_800E6280.unk_110A;
    if (prev != cur) {
        D_800E6280.unk_110A = cur - 1;
        func_8007EC9C();
    }
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013B930", func_8013BD04);

void func_8013BD78(void) {
    func_80046290(0, 0, 0xC);
    func_80044750(0x300);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013B930", func_8013BDB0);

void func_8013BDF8(void) {
    func_80044750(0x500);
    func_8004284C();
}

void func_8013BE20(void) {
    func_80046318(0x35, 0x801B0000, 0x8265);
    func_8013B980();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013B930", func_8013BE58);

s32 func_8013BEE0(void) {
    func_80138AF8();
    if ((u16) D_800CA154 == 9 && check_end_k() != 0) {
        if (*(u8 *) &D_800E6280.unk_03C == 0 || (*(u8 *) &D_800E6280.unk_03C != 0 && func_80046094() == 9)) {
            D_80122CE4 = 0;
            k_reset(1);
            D_80122CF0 = 0;
            D_8011ED5B &= 0xFF7F;
            D_800CA2AC = 0;
            D_800CA150 = (u16) D_800CA150 + 1;
            D_800CA154 = 0;
            func_8004284C();
        }
    }
}

void func_8013BFA4(void) {
    func_80044750(0x24);
    func_80044750(0x503);
    func_8004284C();
}

void func_8013BFD4(void) {
    if (D_80122CDC != 0) {
        D_800CA150 = (u16) D_800CA150 + 5;
    }
    func_8004284C();
}

void func_8013C014(void) {
    func_8004284C();
    if (D_80122CDC != 0) {
        D_800E6280.unk_110A += 0x14;
        D_800E6280.unk_56C[0x4F] = 1;
        return;
    }
    D_800E6280.unk_56C[0x4F] = 2;
}

void func_8013C070(void) {
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013B930", func_8013C090);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013B930", func_8013C200);

void func_8013C330(void) {
    func_8013B980();
    load_palette(D_80146780, 0x11, 1, 2, 0);
    func_80084E90(D_80146784, D_80146788, D_8014678C, D_80146790, D_80146794, D_80146798);
    func_800850D4(0, 0, 0, 0);
    func_8004284C();
}

void func_8013C3C4(void) {
    D_80146274 = D_80145F1C;
    D_80146278 = D_80146058;
    D_8014627C = D_80146194;
    D_800CA368 = 0x51;
    func_8004284C();
}

void func_8013C41C(void) {
    bg_read_sub2(0x4045);
    func_8004284C();
}

void func_8013C444(void) {
    bg_read_sub2(0x4943);
    func_8004284C();
}

void func_8013C46C(void) {
    bg_read_sub2(0x403B);
    func_8004284C();
}

void func_8013C494(void) {
    func_8007ED84(0x403B);
    func_8004284C();
}
