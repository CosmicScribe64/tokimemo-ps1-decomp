#include "common.h"
#include "game.h"

void func_800789E0(void) {
    D_800B6D30 = 0;
    ovl_load_state = 0;
    ovl_load_sectors = 0;
    ovl_load_buf = 0;
    ovl_load_lba = 0;
}

void func_80078A0C(void) {
    u8 sel = ovl_load_state; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (sel) {
    case 0:
        func_80078C48();
        break;
    case 1:
        ovl_load_poll();
        break;
    case 2:
        ovl_load_verify();
        break;
    case 3:
        func_80079070();
        break;
    }
    func_80078A94();
}

void func_80078A94(void) {
    s32 pad; /* FAKE: unused local above the spill slot of n, the original has the spill at sp+0x28 (T-3330) */
    s32 n;

    n = 0;
    if (D_8011F113 & 0x80) {
        hizuke_show();
        n = 1;
    }
    if (D_8011ED17 & 0x80) {
        message_window_show();
        n += 1;
    }
    if (D_8011F553 & 0x80) {
        parameter_show();
        n += 1;
    }
    if (*D_8011F3FF & 0x80) {
        func_80067870();
        n += 1;
    }
    if (D_8011ECD3 & 0x80) {
        func_8006BA40();
        n += 1;
    }
    if (D_800E6280.unk_110E == 0x45) {
        cal_base_show();
    }
    if (n != 0) {
        func_80066C08(0);
        switch (D_800B6D30) {
        case 0:
            if (D_80122D44 & 1) {
                func_8006B900();
            }
            if (D_80122D44 & 2) {
                func_80066334();
            }
            func_800578F4(get_last_gamen_mode());
            break;
        case 1:
            func_80057710();
            func_800578F4(1);
            break;
        default:
            back_clear_switch(1);
            break;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/main/800789E0", func_80078C48);

void ovl_load_poll(void) {
    if (cd_read_sync_status() & 1) {
        ovl_load_state = 2;
    }
}

void ovl_load_verify(void) {
    if (file_checksum_ok(ovl_load_sectors, ovl_load_buf) != 0) {
        ovl_load_state = 3;
        func_80042458();
    } else {
        ovl_load_state = 0;
        ovl_load_lba = 0;
    }
}

/* bulk_m2c declarations (T-11190) */
extern s32 D_80132000;
extern s32 D_80132040;
extern s32 D_8013209C;
extern s32 D_80132140;
extern s32 D_80132214;
extern s32 D_80132244;
extern s32 D_80132274;
extern s32 D_801322D4;
extern s32 D_80132334;
extern s32 D_80132FF8;
extern s32 D_80133120;
extern s32 D_801335E8;
extern s32 D_80133A2C;
extern s32 D_80134040;
extern s32 D_80134C90;
extern s32 D_80137144;
extern s32 D_80137490;
extern s32 D_80137890;
extern s32 D_80137F64;
extern s32 D_8013882C;
extern s32 D_801389A0;
extern s32 D_80139150;
extern s32 D_801397D0;
extern s32 D_8013B484;
extern s32 D_8013B5B0;
extern s32 D_8013C780;
extern s32 D_8013D180;
extern s32 D_801407F0;
extern s32 D_80141280;
extern s32 D_8014231C;
extern s32 D_801426D0;
extern s32 D_80142700;
extern s32 D_80142974;
extern s32 D_801448D4;
extern s32 D_80144AD0;
extern s32 D_80144F70;
extern s32 D_80145A6C;
extern s32 D_8014636C;
extern s32 D_801465F8;
extern s32 D_801492C0;
extern s32 D_8014ABD0;
extern s32 D_8014AF28;
extern s32 D_8014C08C;
extern u8 D_800E7388;
extern u8 D_800E7389;
extern u8 D_800E738A;
extern u8 D_800E738B;
extern u8 D_800E738C;
extern u8 D_800E738D;
extern u8 D_800E738E;
/* end bulk_m2c declarations */
void func_80079070(void) {
    u8 sp32;
    u8 sp31;
    u8 sp30;
    u8 sp2F;

    switch ((u8) D_800E738E) {
    case 0x3A:
        D_80123110 = (u32) &D_80132000;
        break;
    case 0x82:
        D_80123110 = (u32) &D_80132000;
        break;
    case 0x76:
        D_80123110 = (u32) &D_80132140;
        break;
    case 0x75:
        D_80123110 = (u32) &D_80139150;
        break;
    case 0x74:
        D_80123110 = (u32) &D_80133A2C;
        break;
    case 0x80:
        D_80123110 = (u32) &D_80137890;
        break;
    case 0x81:
        D_80123110 = (u32) &D_8013B5B0;
        break;
    case 0x72:
        D_80123110 = (u32) (&D_80141280);
        break;
    case 0x60:
        D_80123110 = (u32) &D_80132000;
        break;
    case 0x5F:
        D_80123110 = (u32) &D_80132FF8;
        break;
    case 0x73:
        D_80123110 = (u32) &D_80132040;
        break;
    case 0x70:
        D_80123110 = (u32) &D_80142700;
        break;
    case 0x71:
        D_80123110 = (u32) &D_80144F70;
        break;
    case 0x11:
        D_80123110 = (u32) &D_80144AD0;
        break;
    case 0x14:
        D_80123110 = (u32) &D_8013209C;
        break;
    case 0x15:
        D_80123110 = (u32) &D_8014AF28;
        break;
    case 0x13:
        D_80123110 = (u32) &D_8014231C;
        break;
    case 0x43:
        D_80123110 = (u32) (&D_801397D0);
        break;
    case 0x45:
        D_80123110 = (u32) &D_80132000;
        break;
    case 0x63:
        D_80123110 = (u32) &D_80132334;
        break;
    case 0xC4:
        D_80123110 = (u32) &D_80145A6C;
        break;
    case 0x37:
        D_80123110 = (u32) &D_801492C0;
        break;
    case 0x36:
        D_80123110 = (u32) &D_8014ABD0;
        break;
    case 0xC2:
        D_80123110 = (u32) &D_80142974;
        break;
    case 0x91:
        D_80123110 = (u32) &D_801407F0;
        break;
    case 0xC3:
        D_80123110 = (u32) &D_8014636C;
        break;
    case 0x35:
        D_80123110 = (u32) &D_801465F8;
        break;
    case 0x5E:
        D_80123110 = (u32) (&D_8014C08C);
        break;
    case 0x50:
        D_80123110 = (u32) &D_801448D4;
        break;
    case 0x5C:
        D_80123110 = (u32) &D_80137F64;
        break;
    case 0x58:
        D_80123110 = (u32) &D_80132214;
        break;
    case 0x62:
        D_80123110 = (u32) &D_80132000;
        break;
    case 0x61:
        D_80123110 = (u32) &D_80133120;
        break;
    case 0x59:
        D_80123110 = (u32) &D_80132244;
        break;
    case 0x56:
        D_80123110 = (u32) &D_80134040;
        break;
    case 0x57:
        D_80123110 = (u32) &D_80134C90;
        break;
    case 0x5D:
        D_80123110 = (u32) &D_8013882C;
        break;
    case 0x5A:
        D_80123110 = (u32) (&D_801426D0);
        break;
    case 0x51:
        D_80123110 = (u32) &D_80137490;
        break;
    case 0x54:
        D_80123110 = (u32) &D_80132274;
        break;
    case 0x52:
        D_80123110 = (u32) &D_8013D180;
        break;
    case 0x53:
        D_80123110 = (u32) &D_801322D4;
        break;
    case 0x55:
        D_80123110 = (u32) &D_8013B484;
        break;
    case 0x5B:
        D_80123110 = (u32) &D_80137144;
        break;
    case 0x20:
        D_80123110 = (u32) &D_801335E8;
        break;
    case 0x21:
        D_80123110 = (u32) &D_801389A0;
        break;
    case 0x22:
        D_80123110 = (u32) &D_8013C780;
        break;
    default:
        D_80123110 = (u32) (&D_80132000);
        break;
    }
    ovl_load_state = 0;
    D_800E7388 = D_800E738E;
    sp32 = D_800E738A;
    sp31 = D_800E738D;
    sp2F = D_800E738C;
    sp30 = D_800E738B;
    func_80042908((s32) D_800E7389);
    func_80042940((s32) sp32);
    D_800E738D = sp31;
    D_800E738C = sp2F;
    D_800E738B = sp30;
}

s32 func_80079524(void) {
    if (D_800E6280.unk_1108 == 0xD0 || D_800E6280.unk_1108 == 0xD1) {
        return 1;
    }
    return 0;
}

s32 func_80079554(void) {
    if (D_800E6280.unk_1108 == 0x90) {
        return 1;
    }
    return 0;
}

s32 func_80079578(void) {
    if (D_800E6280.unk_1108 == 0x21) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/main/800789E0", func_8007959C);

INCLUDE_ASM("asm/nonmatchings/main/800789E0", func_80079800);
