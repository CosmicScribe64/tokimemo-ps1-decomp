#include "common.h"
#include "ovl/ENDING.h"

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80132000);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80132334);

void func_80132400(void) {
    func_80042878(0xC2);
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80132420);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80132B04);

void func_80132D98(void) {
    D_800CA134 = (u8 *)&D_8013C358;
    D_800CA138 = (u8 *)&D_8013C35C;
    D_800CA13C = D_8013C34C;
    D_800CA140 = D_8013C350;
    D_800CA144 = D_8013C354;
    func_80132B04(0xFF, 1, 1);
}

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
    bg_read_sub2(0x4045);
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

void func_801337F0(void) {
    if (D_800CA14C == 4) {
        func_80083440(4);
        load_palette(D_800CA130, 0x10, 1, 2, 1);
    }
    normal_date_speak();
    if (D_800CA14C == 7) {
        D_80122D20 = 1;
        return;
    }
    D_80122D20 = 0;
}

void func_80133870(void) {
    if (D_800E69DD == 0xE) {
        func_80062CD0(0x6B24);
    } else {
        func_80062CD0(0x5828);
    }
    func_8004284C();
}

void func_801338B8(void) {
    load_palette(D_800CA130, 0x10, 1, 2, 1);
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
    bg_read_sub2(0x4055);
    func_8004284C();
}

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

void func_80134DE0(void) {
    D_8013C620 = 0x801E6598;
    D_8013C624 = 0x801E65AC;
    D_8013C628 = 0x801E660C;
    D_8013C62C = *(s16 *)0x801E6638;
    D_8013C630 = 0x801A0000;
    D_8013C634 = 0x801A2000;
    D_8013C638 = 0x801A6000;
    D_8013C63C = 0x801CA000;
    D_8013C640 = 0x801CE000;
    D_8013C644 = 0x801AA000;
    D_8013C648 = 0x801AE000;
    D_8013C64C = 0x801B2000;
    D_8013C650 = 0x801B6000;
    D_8013C654 = 0x801BA000;
    D_8013C658 = 0x801BE000;
    D_8013C65C = 0x801C2000;
    D_8013C660 = 0x801C6000;
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80134EF0);

void func_80135000(void) {
    D_8013C6A8 = 0x801DE2F4;
    D_8013C6AC = 0x801DE300;
    D_8013C6B0 = 0x801DE34C;
    D_8013C6B4 = *(s16 *)0x801DE370;
    D_8013C6B8 = 0x801A0000;
    D_8013C6BC = 0x801A2000;
    D_8013C6C0 = 0x801A6000;
    D_8013C6C4 = 0x801CA000;
    D_8013C6C8 = 0x801CE000;
    D_8013C6CC = 0x801AA000;
    D_8013C6D0 = 0x801AE000;
    D_8013C6D4 = 0x801B2000;
    D_8013C6D8 = 0x801B6000;
    D_8013C6DC = 0x801BA000;
    D_8013C6E0 = 0x801BE000;
    D_8013C6E4 = 0x801C2000;
    D_8013C6E8 = 0x801C6000;
}

void func_80135110(void) {
    D_8013C6EC = 0x801E6484;
    D_8013C6F0 = 0x801E6498;
    D_8013C6F4 = 0x801E64F4;
    D_8013C6F8 = *(s16 *)0x801E6518;
    D_8013C6FC = 0x801A0000;
    D_8013C700 = 0x801A2000;
    D_8013C704 = 0x801A6000;
    D_8013C708 = 0x801CA000;
    D_8013C70C = 0x801CE000;
    D_8013C710 = 0x801AA000;
    D_8013C714 = 0x801AE000;
    D_8013C718 = 0x801B2000;
    D_8013C71C = 0x801B6000;
    D_8013C720 = 0x801BA000;
    D_8013C724 = 0x801BE000;
    D_8013C728 = 0x801C2000;
    D_8013C72C = 0x801C6000;
}

void func_80135220(void) {
    D_8013C730 = 0x801E64B4;
    D_8013C734 = 0x801E64C8;
    D_8013C738 = 0x801E6524;
    D_8013C73C = *(s16 *)0x801E6554;
    D_8013C740 = 0x801A0000;
    D_8013C744 = 0x801A2000;
    D_8013C748 = 0x801A6000;
    D_8013C74C = 0x801CA000;
    D_8013C750 = 0x801CE000;
    D_8013C754 = 0x801AA000;
    D_8013C758 = 0x801AE000;
    D_8013C75C = 0x801B2000;
    D_8013C760 = 0x801B6000;
    D_8013C764 = 0x801BA000;
    D_8013C768 = 0x801BE000;
    D_8013C76C = 0x801C2000;
    D_8013C770 = 0x801C6000;
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80135330);

void func_80135440(void) {
    D_8013C7B8 = 0x801EE6E8;
    D_8013C7BC = 0x801EE704;
    D_8013C7C0 = 0x801EE768;
    D_8013C7C4 = *(s16 *)0x801EE794;
    D_8013C7C8 = 0x801A0000;
    D_8013C7CC = 0x801A2000;
    D_8013C7D0 = 0x801A6000;
    D_8013C7D4 = 0x801CA000;
    D_8013C7D8 = 0x801CE000;
    D_8013C7DC = 0x801AA000;
    D_8013C7E0 = 0x801AE000;
    D_8013C7E4 = 0x801B2000;
    D_8013C7E8 = 0x801B6000;
    D_8013C7EC = 0x801BA000;
    D_8013C7F0 = 0x801BE000;
    D_8013C7F4 = 0x801C2000;
    D_8013C7F8 = 0x801C6000;
}

void func_80135550(void) {
    D_8013C7FC = 0x801EA624;
    D_8013C800 = 0x801EA63C;
    D_8013C804 = 0x801EA69C;
    D_8013C808 = *(s16 *)0x801EA6C8;
    D_8013C80C = 0x801A0000;
    D_8013C810 = 0x801A2000;
    D_8013C814 = 0x801A6000;
    D_8013C818 = 0x801CA000;
    D_8013C81C = 0x801CE000;
    D_8013C820 = 0x801AA000;
    D_8013C824 = 0x801AE000;
    D_8013C828 = 0x801B2000;
    D_8013C82C = 0x801B6000;
    D_8013C830 = 0x801BA000;
    D_8013C834 = 0x801BE000;
    D_8013C838 = 0x801C2000;
    D_8013C83C = 0x801C6000;
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80135660);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80135770);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80135880);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80135990);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80135AA0);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80135BB0);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80135CC0);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80135DD0);

void func_80135ECC(void) {
    func_80041584();
    func_80048EB8(0);
    tpage_buf_clear_all();
    D_800E7518 = 1;
    D_800E751C = 1;
    D_800E7520 = 1;
    k_speed_set(0x10);
    func_8004E58C();
    k_reset(1);
    func_8004E500(3);
    D_8013CA38 = 0;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80135F4C);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING", func_80136BE0);

void func_80136C5C(s16 arg0) {
    s16 i;
    u8 *p;

    for (i = 0; i < 0xC; i++) {
        p = (u8 *)&D_801217D0 + i * 0x24;
        *(s16 *)(p + 0x96) = *(s16 *)(p + 0x96) + arg0;
    }
    /* FAKE: D_8011F536 written as D_8011F4F2[0x22]; stops IDO hoisting the next load above this store (matching-notes, T-2100) */
    (&D_8011F4F2)[0x22] += arg0;
    D_8011F4F2 += arg0;
}

void func_80136CD0(s16 arg0) {
    s16 i;
    u8 *p;

    for (i = 0; i < 3; i++) {
        p = (u8 *)&D_801217D0 + i * 0x24;
        *(s16 *)(p + 0x246) = *(s16 *)(p + 0x246) + arg0;
    }
    for (i = 0; i < 0xE; i++) {
        p = D_8011ECD0 + i * 0x44;
        *(s16 *)(p + 0x19AA) = *(s16 *)(p + 0x19AA) + arg0;
    }
}

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
    normal_date_girl_out();
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
