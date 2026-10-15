#include "common.h"
#include "ovl/BUNKAKEN.h"

#ifdef NON_MATCHING
/* NON_MATCHING: T-10320, only the switch selector register ($v0 vs $v1) differs */
void func_801428D0(void) {
    D_80122EBC = 3;
    switch (D_80122EB8) {
    case 4:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            if (func_8008667C(3) == 4 && !D_800E6280.unk_1BC[3].unk_0C.f.b2) {
                D_80122EBC = 0xFF;
                func_80042908(5);
                return;
            }
            func_80042908(0x36);
            return;
        case 0x60:
            if (func_8008667C(3) == 4 && !D_800E6280.unk_1BC[3].unk_0C.f.b2) {
                D_80122EBC = 0xFF;
                func_80042908(5);
                return;
            }
            func_80042908(0x37);
            return;
        case 0x61:
            if (func_8008667C(3) == 4 && !D_800E6280.unk_1BC[3].unk_0C.f.b2) {
                D_80122EBC = 0xFF;
                func_80042908(5);
                return;
            }
            func_80042908(0x38);
            return;
        }
        break;
    case 5:
        func_80042908(0x39);
        return;
    default:
        func_80046500();
        return;
    }
}
#else
INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/801428D0", func_801428D0);
#endif

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/801428D0", func_80142A40);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/801428D0", func_80142FD4);

/* T-12230: existing CharFlags bitfields reproduce the target register choices. */
void func_80143494(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (D_800E6280.unk_110A) {
    case 0:
        dec_bg_cd_read(0x410F, 1);
        func_8004284C();
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80145844();
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x58DC);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            D_800E6280.unk_F5F = 3;
            func_80083474();
            cd_read_sectors(9U, 0x801E8000, 0x4B0A);
        }
        break;
    case 5:
        if ((func_8008667C(3) == 4) || (D_80122EA4 == 3)) {
            func_8004284C();
        } else {
            normal_date_girl_in();
        }
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            func_80143D68();
            temp_v0 = func_8008667C(3);
            switch (temp_v0) {
            case 0:
            case 1:
                D_800CA148 = 0x30;
                break;
            case 2:
                D_800CA148 = 0x34;
                break;
            case 3:
                D_800CA148 = 0x38;
                break;
            }
            if ((func_8008667C(3) == 4) || (D_80122EA4 == 3)) {
                D_800CA148 = 0x3C;
            }
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if ((func_8008667C(3) == 4) || (D_80122EA4 == 3)) {
            func_80042940(0xC);
        } else if (D_800E6280.unk_1BC[3].unk_0C.f.b2) {
            func_80042940(0xA);
        } else {
            D_800CA148 = 1;
            func_8004284C();
        }
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        temp_v0_2 = func_8008667C(3);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 0x31;
            break;
        case 2:
            D_800CA148 = 0x35;
            break;
        case 3:
            D_800CA148 = 0x39;
            break;
        }
        func_8004284C();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        if ((func_8008667C(3) == 4) || (D_80122EA4 == 3)) {
            normal_date_bg_fadeout();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        if ((func_8008667C(3) == 4) || (D_80122EA4 == 3)) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        if ((func_8008667C(3) == 4) || (D_80122EA4 == 3)) {
            normal_date_bg_fadeout();
        } else {
            normal_date_girl_out();
        }
        break;
    case 17:
        if ((func_8008667C(3) == 4) || (D_80122EA4 == 3)) {
            func_8004284C();
        } else {
            normal_date_bg_fadeout();
        }
        break;
    case 18:
        D_800CA148 = 0;
        func_8004284C();
        break;
    case 19:
        normal_date_speak();
        break;
    case 20:
        func_80145530();
        break;
    default:
        func_80046500();
        break;
    }
}

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/801428D0", func_801438F8);

void func_80143D68(void) {
    func_800847B8(3);
    func_801321D4();
    D_800CA14C = 0;
    D_800CA160 = D_80155E90;
    D_800CA164 = D_80155E94;
    D_800CA168 = D_80155E98;
    func_8004284C();
}
