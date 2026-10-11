#define MAIN_API_OVERRIDE_D_80122CD0 /* switched as u32 (main_api.h: s32), T-6030 */
#include "common.h"
#include "ovl/GEKO.h"

typedef struct {
    void (*f[50])();
} FnTbl50; /* size 0xC8 */
extern FnTbl50 D_80146920;

typedef struct {
    void (*f[39])();
} FnTbl39; /* size 0x9C */
extern FnTbl39 D_80146884;

extern u32 D_80122CD0;

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

void func_8013B9F0(void) {
    switch (D_80122CD0) {
    case 1:
        func_8013BA6C();
        return;
    case 2:
        func_8013BAA8();
        return;
    case 3:
        func_8013BAE4();
        return;
    default:
        func_80046500();
        return;
    }
}

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

s32 func_8013BB4C(void) {
    func_80044750(0xCF);
    if ((u32)D_800E6280.unk_1104.w++ < 0x40U) {
        return 0;
    }
    D_800B3D60 = 0;
    func_80044890(0, 0, 0, 0xD989, 0xD949, 0xD941);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

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

void func_8013BD04(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl39 tbl;

    tbl = D_80146884;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_8013BD78(void) {
    func_80046290(0, 0, 0xC);
    func_80044750(0x300);
    func_8004284C();
}

s32 func_8013BDB0(void) {
    if ((u32)D_800E6280.unk_1104.w++ >= 0x11U) {
        func_8004E9F4(1);
        func_8004284C();
    }
}

void func_8013BDF8(void) {
    func_80044750(0x500);
    func_8004284C();
}

void func_8013BE20(void) {
    func_80046318(0x35, 0x801B0000, 0x8265);
    func_8013B980();
    func_8004284C();
}

void func_8013BE58(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl50 tbl;

    tbl = D_80146920;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

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

void func_8013C090(void) {
    /* Chain over two GameState bytes, written as element stores: IDO then keeps the one
     * li v0,12 for both, as the original (T-9210). */
    (&D_800E6280.unk_F5F)[0] = (&D_800E6280.unk_75D)[0] = 0xC;
    func_800438DC(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    func_800438F0(1);
    func_80048390();
    func_8004E58C();
    func_8006612C("");
    D_8011ED9F = 0;
    D_8011EDE3 = 0;
    D_800CA150 = 0;
    D_800CA154 = 0;
    func_8007C740();
    func_80137990();
    func_800649D4();
    func_80064E84();
    func_80084E4C();
    D_800B593C = 0;
    func_800847B8(D_800E6280.unk_F5F);
    func_8008585C();
    func_80137990();
    D_80146274 = D_80145F14;
    D_80146278 = D_80146050;
    D_8014627C = D_8014618C;
    func_800AE0F0(D_800CA19C, "廊下");
    func_800AE0F0(D_800CA1DC, "廃工場前");
    if (D_80122CD0 == 1) {
        D_800E6280.unk_11C.unk_02 += 5;
    }
    func_80084D3C();
    D_800CA368 = 0x4F;
    func_8004284C();
}

void func_8013C200(void) {
    /* Same chain as func_8013C090 (T-9210). */
    (&D_800E6280.unk_F5F)[0] = (&D_800E6280.unk_75D)[0] = 0xC;
    func_800438DC(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    func_800438F0(1);
    func_80048390();
    func_8004E58C();
    func_8006612C("");
    D_8011ED9F = 0;
    D_8011EDE3 = 0;
    D_800CA150 = 0;
    D_800CA154 = 0;
    func_8007C740();
    func_80137990();
    func_800649D4();
    func_80064E84();
    func_80084E4C();
    D_800B593C = 0;
    func_800847B8(D_800E6280.unk_F5F);
    func_8008585C();
    func_80137990();
    D_80146274 = D_80145F18;
    D_80146278 = D_80146054;
    D_8014627C = D_80146190;
    func_800AE0F0(D_800CA19C, "廊下");
    D_800CA368 = 0x50;
    func_8004284C();
}

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
