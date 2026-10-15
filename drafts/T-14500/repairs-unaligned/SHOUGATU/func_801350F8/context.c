#include "common.h"
#include "ovl/SHOUGATU.h"

void func_80134890(void) {
    D_80143DB0 = 0x801DABE0;
    D_80143DB4 = 0x801DB684;
    D_80143DB8 = 0x801DABE8;
    D_80143DBC = 0x801DB6A0;
    D_80143DC0 = 0x801DAC48;
    D_80143DC4 = 0x801DB6CC;
    D_80143DC8 = *(s16 *)0x801DB6E0;
    D_80143DCC = *(s16 *)0x801DB6E4;
    D_80143DD0 = 0x801C0000;
    D_80143DD4 = 0x801C2000;
}

typedef struct {
    void (*f[31])();
} FnTbl31; /* size 0x7C */
extern FnTbl31 D_80143DE0;

void func_80134930(void) {
    u16 idx; /* FAKE: u16 and the second call argument put idx in $a1 as the original does (permuter); real prototype unknown. T-8080 */
    FnTbl31 tbl;

    tbl = D_80143DE0;
    if (D_80143B20 != 0) {
        idx = D_800E6280.unk_110A;
        if ((void (*)())normal_date_girl_in == tbl.f[idx]) {
            tbl.f[idx] = func_8004284C;
        }
    }
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x40, idx);
    if (D_80143DDC != 0) {
        func_80085678(-0xA0, -0x78, 0x140, 0x64, 0xD, 0xE0FF, 0xFFFF);
        func_80085678(-0xA0, -0x14, 0x140, 0x3C, 0xD, 0x6060, 0xE0FF);
        func_8004955C(0xD);
    }
}

/* K&R return shape: failure returns zero; scene advance falls through. T-10030. */
s32 func_80134A54(void) {
    func_80134890();
    if (((s32 *)D_80143DB4)[0] != 0x801CC000 ||
        ((s32 *)D_80143DB4)[1] != 0x801CE000 ||
        ((s32 *)D_80143DB4)[2] != 0x801D0000 ||
        ((s32 *)D_80143DB4)[3] != 0x801D2000 ||
        ((s32 *)D_80143DB4)[4] != 0x801D4000 ||
        ((s32 *)D_80143DB4)[5] != 0x801D6000 ||
        ((s32 *)D_80143DB4)[6] != 0x801D8000 ||
        ((s32 *)D_80143DB0)[0] != 0x801C4000 ||
        ((s32 *)D_80143DB0)[1] != 0x801C8000 ||
        ((s32 *)D_80143DC4)[0] != 0x801DB634 ||
        ((s32 *)D_80143DC4)[1] != 0x801DB644 ||
        ((s32 *)D_80143DC4)[2] != 0x801DB654 ||
        ((s32 *)D_80143DC0)[0] != 0x801DAB84 ||
        ((s32 *)D_80143DC0)[1] != 0x801DAB8C ||
        ((s32 *)D_80143DC0)[2] != 0x801DABA0 ||
        ((s32 *)D_80143DC0)[3] != 0x801DABB4 ||
        ((s32 *)D_80143DC0)[4] != 0x801DABC0 ||
        ((s32 *)D_80143DC0)[5] != 0x801DABCC) {
        D_800E6280.unk_110A -= 2;
        return 0;
    }
    if (file_checksum_ok(0x37, 0x801C0000) == 0) {
        D_800E6280.unk_110A -= 2;
        return 0;
    }
    func_8004284C();
}

void func_80134C34(void) {
    if ((D_80143B20 == 0) && (D_800E6280.unk_75E[D_800E6280.unk_F5E - 1].unk_02 != 5)) {
        if (D_80143B24 != 0) {
            func_80085B3C(5, 5);
        } else {
            func_80085B3C(5, 2);
        }
    }
    bg_read_sub2(0x4200);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80134890", func_80134CB4);

void func_80134F10(void) {
    func_80044750(0x201);
    func_8004284C();
}

void func_80134F38(void) {
    func_80044750(0x200);
    func_8004284C();
}

void func_80134F60(void) {
    if ((D_800E6280.unk_F5F == 2) && (D_80143B20 == 0)) {
        func_8004284C();
    }
    func_8004284C();
}

void func_80134FAC(void) {
    if ((D_80143DD8 == 3) || (D_80143DD8 == 4)) {
        D_80143B00 += 2;
    } else if (D_80143DD8 != 0) {
        D_80143B00 += 1;
    }
    func_8004284C();
}

void func_80135014(void) {
    if (D_80143DD8 == 0) {
        D_80143B00 += 2;
    } else if ((D_80143DD8 != 3) && (D_80143DD8 != 4)) {
        D_80143B00 += 1;
    }
    func_8004284C();
}

void func_8013507C(void) {
    D_80120650[3] |= 0x80;
    D_80120650[0x46] = 1;
    D_80120650[0x47] |= 0x80;
    D_80120650[0x8A] = 1;
    D_80120650[0x8B] |= 0x80;
    D_80143DDC = 1;
    func_8004284C();
}

/* bulk_m2c declarations (T-11190) */
typedef struct { u8 b[35]; } UnalignedBytes35; /* size 0x23 */
extern UnalignedBytes35 D_80143E5C;
extern s16 D_8012067E;
extern s16 D_8012069C;
extern s32 D_800E7208;
extern s32 D_80120680;
extern u32 D_800E7384;
extern u8 D_800E62BC;
extern u8 D_800E62BE;
extern u8 D_800E738D;
s32 func_800AE130();
/* end bulk_m2c declarations */
void func_801350F8(void) {
    u8 sp84[] = "おみくじを引きます。";
    u8 sp6C[] = "うおりゃあああ〜・・・";
    u8 sp5C[] = "こ、これは！！";
    UnalignedBytes35 sp38;
    s16 var_v1;
    s32 var_at;
    s32 var_at_2;
    s32 var_at_3;
    u8 temp_t7_2;

    sp38 = D_80143E5C;
    switch (D_800E7384) {
    case 0:
        set_kanji_string(-0x68, 0x30, 0, (s32) &sp84, 0);
        D_80122CE4 = 1;
        D_8012067E = -4;
        D_800E7384 += 1;
        return;
    case 2:
        set_kanji_string(-0x68, 0x30, 0, (s32) &sp6C, 0);
        D_80122CE4 = 1;
        D_80143DDC = 0;
        D_8012071F |= 0x80;
        D_8012071E = 1;
        D_80120732 = 1;
        D_80120734 = 0;
        D_800E7384 += 1;
        return;
    case 3:
        D_80120674 += *(s32 *)((unsigned char *)D_8011ECD0 + (6572));
        if (*(s16 *)((unsigned char *)D_8011ECD0 + (6566)) < -0x6F) {
            D_80120732 = 0;
            *(s16 *)((unsigned char *)D_8011ECD0 + (6574)) = (s16) -*(s16 *)((unsigned char *)D_8011ECD0 + (6574));
            *(s16 *)((unsigned char *)D_8011ECD0 + (6574)) = (s16) (*(s16 *)((unsigned char *)D_8011ECD0 + (6574)) + 2);
            if (*(s16 *)((unsigned char *)D_8011ECD0 + (6574)) >= 0x15) {
                *(s16 *)((unsigned char *)D_8011ECD0 + (6574)) = 0x14;
            }
        } else if (*(s16 *)((unsigned char *)D_8011ECD0 + (6566)) >= 0x70) {
            D_80120732 = 1;
            *(s16 *)((unsigned char *)D_8011ECD0 + (6574)) = (s16) -*(s16 *)((unsigned char *)D_8011ECD0 + (6574));
            *(s16 *)((unsigned char *)D_8011ECD0 + (6574)) = (s16) (*(s16 *)((unsigned char *)D_8011ECD0 + (6574)) - 2);
            if (*(s16 *)((unsigned char *)D_8011ECD0 + (6574)) < -0x14) {
                *(s16 *)((unsigned char *)D_8011ECD0 + (6574)) = -0x14;
            }
        }
        var_v1 = *(s16 *)((unsigned char *)D_8011ECD0 + (6574));
        if ((var_v1 < -8) || (var_at = var_v1 < -0xC, ((var_v1 < 9) == 0))) {
            D_8012069C += 1;
            D_801206E0 += 1;
            var_v1 = D_8012067E;
            var_at = var_v1 < -0xC;
        }
        if ((var_at != 0) || (var_at_2 = var_v1 < -0x10, ((var_v1 < 0xD) == 0))) {
            D_8012069C += 2;
            D_801206E0 += 2;
            var_v1 = D_8012067E;
            var_at_2 = var_v1 < -0x10;
        }
        if ((var_at_2 != 0) || (var_v1 >= 0x11)) {
            D_8012069C += 4;
            D_801206E0 += 4;
        }
    case 1:
    case 7:
    case 9:
block_51:
        if (D_80122CE4 != 0) {
            if ((D_800E7208 & 0x400040) && (check_end_k() != 0) && (D_800E62BC == 2)) {
                k_reset(1);
                D_80122CE4 = 0;
                D_80122CF0 = 0;
                D_800CA2AC = 0;
                D_8011ED5B &= 0xFF7F;
                D_800E7384 += 1;
                return;
            }
            if ((D_800E7208 & 0x200020) && (check_end_k() != 0)) {
                k_reset(1);
                D_80122CE4 = 0;
                D_80122CF0 = 0;
                D_800CA2AC = 0;
                D_8011ED5B &= 0xFF7F;
                D_800E7384 += 1;
                return;
            }
            if (check_end_k() != 0) {
                D_8011ED5B |= 0x80;
                func_8006C934();
                return;
            }
            D_8011ED5B &= 0x7F;
        } else {
            return;
        }
        break;
    case 4:
        if (((*(s16 *)((unsigned char *)D_8011ECD0 + (6566)) >= 0) && (-*(s16 *)((unsigned char *)D_8011ECD0 + (6574)) >= *(s16 *)((unsigned char *)D_8011ECD0 + (6566)))) || ((*(s16 *)((unsigned char *)D_8011ECD0 + (6566)) <= 0) && (*(s16 *)((unsigned char *)D_8011ECD0 + (6566)) >= -*(s16 *)((unsigned char *)D_8011ECD0 + (6574))))) {
            *(s16 *)((unsigned char *)D_8011ECD0 + (6566)) = 0;
            D_80143DDC = 1;
            D_8012071F &= 0x7F;
            D_800E7384 += 1;
            return;
        }
        *(s32 *)((unsigned char *)D_8011ECD0 + (6564)) = (s32) (*(s32 *)((unsigned char *)D_8011ECD0 + (6564)) + *(s32 *)((unsigned char *)D_8011ECD0 + (6572)));
        var_at_3 = *(s16 *)((unsigned char *)D_8011ECD0 + (6566)) < 0x70;
        if (*(s16 *)((unsigned char *)D_8011ECD0 + (6566)) < -0x6F) {
            *(s16 *)((unsigned char *)D_8011ECD0 + (6574)) = 0xA;
            var_at_3 = *(s16 *)((unsigned char *)D_8011ECD0 + (6566)) < 0x70;
        }
        if (var_at_3 == 0) {
            *(s16 *)((unsigned char *)D_8011ECD0 + (6574)) = -0xA;
        }
        return;
    case 5:
        if (D_800E738D == 0) {
            *(s32 *)((unsigned char *)D_8011ECD0 + (6572)) = 0;
            D_80120680 = 0xFFFC0000;
            D_80120653 |= 0x10;
        }
        D_80120696 = 0;
        D_801206DA = 0;
        if (D_8012067A < -0xB4) {
            D_8012067A = -0xB4;
        }
        temp_t7_2 = (D_800E738D + 1) & 0xFF;
        if ((D_800E738D ^ 0x1E) == 0) {
            D_80120680 = 0;
            D_80120666 = (D_800E62BE == 0x61) + ((D_800E62BE == 0x62) * 4) + 1;
            D_80120652 = 5;
        }
        D_800E738D = temp_t7_2;
        if (temp_t7_2 == 0x4B) {
            D_80143DDC = 0;
            D_80120653 &= 0x7F;
            D_8012071F |= 0x80;
            D_8012071E = 9;
            D_80120732 = 2;
            D_80120734 = 0;
            func_80044750(0x500);
        }
        if ((u8) D_800E738D < 0x87U) {
            return;
        }
        func_801359FC();
        set_kanji_string(-0x68, 0x30, 0, (s32) &sp5C, 0);
        D_800E7384 += 1;
        return;
    case 6:
        if (((D_80120652 != 1) | 8) != 0) {
            D_80122CE4 = 1;
            D_800E7384 += 1;
        }
        return;
    case 8:
        D_80120653 |= 0x80;
        D_80120652 = 0x40;
        D_80120668 = D_80143DD8 + 0xD;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6566)) = -0xA0;
        D_8012067A = -0x78;
        D_80120680 = 0;
        if ((D_80143DD8 == 3) || (D_80143DD8 == 4)) {
            func_800AE130(&sp6C, "%sだ・・・", sp38.b + (D_80143DD8 * 5), &sp6C);
            D_8012071F |= 0x80;
            D_8012071E = 0x40;
            D_80120734 = 7;
        } else if (D_80143DD8 != 0) {
            func_800AE130(&sp6C, "おっ、%sだ。", sp38.b + (D_80143DD8 * 5), &sp6C);
            D_80120734 = 6;
            D_80143DDC = 1;
            D_8012071F &= 0x7F;
        } else {
            func_800AE130(&sp6C, "ラッキー！%sだ！！", sp38.b + (D_80143DD8 * 5), &sp6C);
            D_80120734 = 6;
            D_80143DDC = 1;
            D_8012071F &= 0x7F;
        }
        set_kanji_string(-0x68, 0x30, 0, (s32) &sp6C, 0);
        D_80122CE4 = 1;
        D_800E7384 += 1;
        return;
    default:
        func_8004284C();
        goto block_51;
    }
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80134890", func_801359FC);

void func_80135B10(void) {
    func_80048F64(0x60);
    func_80048F64(0x61);
    func_80048F64(0x62);
    func_80048F64(0x63);
    D_80143DDC = 0;
    func_8004284C();
}

void func_80135B54(void) {
    cd_read_sectors(0x37, 0x801C0000, 0xBD5E);
    func_8004284C();
}
