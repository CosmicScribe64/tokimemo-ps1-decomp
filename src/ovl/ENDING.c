#include "common.h"
#include "ovl/ENDING.h"

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80132000);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80132334);

void func_80132400(void) {
    func_80042878(0xC2);
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80132420);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80132B04);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80132D98);

void func_80132E10(void) {
    func_80044890(1, 0xBF98, 0xBF79, 0xCE6D, 0xCE33, 0xCE1E);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80132E70);

void func_80132EF0(void) {
    func_80044750(0x200);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80132F18);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80133030);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_801330E4);

void func_80133400(void) {
    func_8007ED84(0x4045);
    func_8004284C();
}

void func_80133428(void) {
    func_80046318(0x11, 0x801F0000, 0xAE07);
    func_80132000();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80133460);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_801334E4);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80133540);

void func_801335F4(void) {
    if (D_8013C3E0 == 0xC) {
        func_80083440(4);
    } else {
        func_80083440(0);
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_8013363C);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_801336A8);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80133738);

void func_801337A4(void) {
    RECT rect;
    rect.x = 0x140;
    rect.y = 0x80;
    rect.w = 0x180;
    rect.h = 0x80;
    func_8009C8E0(&rect, (void *)0x80180000);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_801337F0);

void func_80133870(void) {
    if (D_800E69DD == 0xE) {
        func_80062CD0(0x6B24);
    } else {
        func_80062CD0(0x5828);
    }
    func_8004284C();
}

void func_801338B8(void) {
    func_80043914(D_800CA130, 0x10, 1, 2, 1);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_801338F8);

void func_801339C0(void) {
    D_800E69DD = 0xC;
    func_800AE0F0(D_800CA188, D_8013BFB8);
    func_8004284C();
}

void func_80133A00(void) {
    D_800E69DD = 0xE;
    func_800AE0F0(D_800CA188, D_8013BFC0);
    func_8004284C();
}

void func_80133A40(void) {
    D_800E71DF = D_8013C3E0;
    func_8004284C();
}

void func_80133A6C(void) {
    if (D_800E71DF >= 0xDU) {
        func_80042908(7);
        D_800E7D34 |= 8;
        return;
    }
    func_8004284C();
    D_800E7D34 |= 4;
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80133AD0);

void func_80133B80(void) {
    D_800CA148 = 0;
    D_800CA14C = 0;
    func_80132000();
    D_800CA160 = D_8013C2B4;
    D_800CA164 = D_8013C2F8;
    D_800CA168 = D_8013C33C;
    func_8004284C();
}

void func_80133BE0(void) {
    func_8007ED84(0x4055);
    func_8004284C();
}

/* object boundary: alignment padding of the original link */
INCLUDE_ASM("src/ovl/pad", pad_ENDING_80133C08);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80133C10);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80133F1C);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80133F98);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_8013408C);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80134140);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_801341D8);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80134270);

void func_801349A8(void) {
    func_80046318(0x78, 0x80180000, 0x7B58);
    func_80133C10();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_801349E0);

void func_80134AA8(void) {
    if ((D_80120696 == 0) && (D_80120668 == 1) && (D_80120658 == 0x14)) {
        D_80120666 = 2;
        D_80120668 = 0;
        D_80120658 = 0;
        D_80120652 = 5;
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80134B18);

void func_80134BE8(void) {
    if (D_80120668 == 2) {
        if (D_80120658 == 0x2F) {
            func_8004284C();
        }
    }
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80134C2C);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80134D20);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80134DE0);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80134EF0);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80135000);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80135110);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80135220);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80135330);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80135440);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80135550);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80135660);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80135770);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80135880);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80135990);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80135AA0);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80135BB0);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80135CC0);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80135DD0);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80135ECC);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80135F4C);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80136BE0);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80136C5C);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80136CD0);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80136D60);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80136EE4);

void func_80137128(void) {
    func_80046318(0x3D, 0x80180000, 0x7B1B);
    func_80134D20();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80137160);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80137340);

void func_801374BC(void) {
    if (D_800E73A4 != 0) {
        func_80042878(0xC2);
        return;
    }
    func_80042808();
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_801374F8);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80137574);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_8013759C);

void func_801376FC(void) {
    D_80122CE4 = 0;
    D_8013C364 = 0;
    D_8013C360 = 0;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80137730);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80137AA8);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80137BB4);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80137E40);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80137F64);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_801380B0);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80138278);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_8013855C);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_801387B0);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80138A9C);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80138C70);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80138DC8);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80138FC8);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_801391E4);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80139498);

void func_801396A4(s16 arg0, u8 arg1) {
    D_8011F4DE = arg0;
    D_8011F4E0 = 0;
    D_8011F4D0 = 0;
    if (arg1 != 0xFF) {
        D_8011F4CA = arg1;
    }
}

/* object boundary: alignment padding of the original link */
INCLUDE_ASM("src/ovl/pad", pad_ENDING_801396D8);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_801396E0);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_801399B4);

void func_80139A58(void) {
    if (D_800E71DF == 0xD) {
        func_80042908(7);
        func_80042940(0xF);
        return;
    }
    func_80042908(8);
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80139AA0);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80139B48);

void func_80139D24(void) {
    func_80046318(9, 0x80180000, 0xAE18);
    func_8004284C();
}

void func_80139D54(void) {
    func_80048EB8(0);
    func_8006BD6C(0);
    func_80041584();
    func_8004E58C();
    func_8004E500(3);
    D_80122CE4 = 0;
    D_80122CD4 = 0;
    func_8004284C();
}

void func_80139DA8(void) {
    if (D_800E71DF != 0xD) {
        func_800462C8(9, 0x80162000, 0x497C);
    } else {
        func_800462C8(0xA, 0x80162000, 0x4985);
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80139E08);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80139EC4);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80139F3C);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_8013A004);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_8013A2E8);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_8013B510);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_8013B5E4);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_8013B640);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_8013B6A0);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_8013B760);

void func_8013B868(void) {
    func_80042908(6);
}

void func_8013B888(void) {
    func_80042908(8);
}

void func_8013B8A8(void) {
    if (D_800E683E != 0) {
        func_8004284C();
        return;
    }
    func_8007E5A0();
}

void func_8013B8E4(void) {
    func_80046318(0x45, 0x801A0000, 0x727A);
    func_8013B6A0();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_8013B91C);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_8013B9F0);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_8013BB9C);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_8013BC58);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_8013BDD8);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_8013BE64);
