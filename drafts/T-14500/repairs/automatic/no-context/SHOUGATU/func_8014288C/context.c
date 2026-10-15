#include "common.h"
#include "ovl/SHOUGATU.h"
#include "game.h"

typedef struct {
    void (*f[51])();
} FnTbl51; /* size 0xCC */
extern FnTbl51 D_801468A4;

void func_80141780(void) {
    D_801464B0 = 0x801DA680;
    D_801464B4 = 0x801DA68C;
    D_801464B8 = 0x801DA72C;
    /* address literals: the data belongs to another overlay; the original loads it with a separate base register */
    D_801464BC = *(s16 *)0x801DA754;
    D_801464C0 = 0x801B0000;
    D_801464C4 = 0x801DA000;
    D_801464C8 = 0x801B2000;
    D_801464CC = 0x801B6000;
    D_801464D0 = 0x801BA000;
    D_801464D4 = 0x801BE000;
    D_801464D8 = 0x801C2000;
    D_801464DC = 0x801C6000;
    D_801464E0 = 0x801D6000;
}

void func_80141850(void) {
    D_801464E4 = 0x801D2430;
    D_801464E8 = 0x801D2434;
    D_801464EC = 0x801D2498;
    /* address literal, see func_80141780 */
    D_801464F0 = *(s16 *)0x801D24B4;
    D_801464F4 = 0x801B0000;
    D_801464C4 = 0x801D2000;
    D_801464F8 = 0x801B2000;
    D_801464FC = 0x801B6000;
    D_80146500 = 0x801BA000;
    D_80146504 = 0x801BE000;
    D_80146508 = 0x801C2000;
    D_8014650C = 0x801C6000;
    D_801464E0 = 0x801CE000;
}

void func_80141920(void) {
    D_80146510 = (u8 *)0x801D22D0;
    D_80146514 = (u8 *)0x801D22D8;
    D_80146518 = (u8 *)0x801D233C;
    D_8014651C = *(s16 *)0x801D2358;
    D_80146520 = (u8 *)0x801B0000;
    D_80146524 = (u8 *)0x801B2000;
    D_80146528 = (u8 *)0x801B6000;
    D_8014652C = (u8 *)0x801BA000;
    D_80146530 = (u8 *)0x801BE000;
    D_80146534 = (u8 *)0x801C2000;
    D_80146538 = (u8 *)0x801C6000;
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80141780", func_801419D0);

void func_80141AC0(void) {
    D_800B5A60 = 1;
    func_8004284C();
}

void func_80141AE8(void) {
    u32 r;
    u8 flag;

    flag = 1;
    r = func_8004ECF0();
    if (r >= 2U) {
        D_800E6280.unk_1104.w += 1;
    }
    if (((u32)D_800E6280.unk_1104.w < 0x73U || (u32)D_800E6280.unk_1104.w % 60U != 6) && !((u8)D_80120652 & 1)) {
        flag = 0;
    }
    func_8013D5D8();
    if (flag == 0 && D_80122CD8 != 0) {
        D_80120652 = 0;
    }
}

void func_80141B98(void) {
    func_80044750(0x208);
    func_8004284C();
}

void func_80141BC0(void) {
    D_8014653C = get_k_speed();
    D_80122CFC = 0x72;
    k_speed_set(0x1E);
    k_disp_start(2);
    func_8004284C();
}

void func_80141C0C(void) {
    k_speed_set(D_8014653C);
    k_disp_start(1);
    func_8004284C();
}

void func_80141C44(void) {
    func_8004435C(0x280, 0, 0x40, 0x80, D_801464E0);
    D_800E6280.unk_1228[10] = 1;
    D_80120666 = 2;
    D_801206AA = 0;
    func_8004284C();
}

void func_80141CA0(void) {
    cd_read_sectors(0x55, 0x801B0000, 0x8065);
    func_80141780();
    func_8004284C();
}

void func_80141CD8(void) {
    D_801467FC = 1;
    if (D_800E6280.unk_F5F == 0) {
        func_80044750(0x206);
    }
    D_801217D0[4].unk_04 = -0x64;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80141780", func_80141D20);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80141780", func_801421D4);

void func_801424CC(void) {
    bg_read_sub2(0x475A);
    func_80085B3C(2, 6);
    func_8004284C();
}

void func_80142500(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl51 tbl;

    tbl = D_801468A4;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
    func_8014288C();
}

void func_8014257C(void) {
    func_80062CD0(0x5C8D);
    func_8004284C();
}

void func_801425A4(void) {
    func_80044750(0x207);
    func_8004284C();
}

void func_801425CC(void) {
    func_80083440(5);
    func_8004284C();
}

void func_801425F4(void) {
    cd_read_sectors(0x45, 0x801B0000, 0x8571);
    func_80141850();
    func_8004284C();
}

void func_8014262C(void) {
    D_801206DB &= 0x7F;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80141780", func_8014265C);

void func_80142784(void) {
    if (D_80122CDC != 0) {
        D_800E6280.unk_110A += 0x18;
        D_800E6280.unk_56C[0x34] = 2;
        return;
    }
    D_800CA150 = (u16)D_800CA150 + 2;
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_02 += 1;
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 += 2;
    func_80084D3C();
    func_8004284C();
}

void func_80142844(void) {
    func_8004435C(0x280, 0, 0x40, 0x80, D_801464E0);
    D_800E6280.unk_1228[10] = 1;
    func_8004284C();
}

/* bulk_m2c declarations (T-11190) */
extern s16 D_80121864;
extern s16 D_80146798;
extern s16 D_801467FC;
extern s32 D_800E7325;
extern s32 D_800E7328;
extern s32 D_801217D0;
extern s32 D_80122D04;
extern s32 D_8014653C;
extern s32 D_80146540;
extern s32 D_80146608;
extern s32 D_801466D0;
extern u32 D_80146800;
extern u8 D_800B593C;
s32 func_800AE0D0();
/* end bulk_m2c declarations */
void func_8014288C(void) {
    s16 *temp_s4;
    s16 *var_v1;
    s16 temp_a0;
    s16 temp_s2;
    s16 temp_v0;
    s16 temp_v0_4;
    s16 var_a0;
    s16 var_s2;
    s16 var_s2_2;
    s16 var_v1_2;
    s32 *temp_s0;
    s32 temp_at;
    s32 temp_hi;
    s32 temp_lo;
    s32 temp_s6;
    s32 temp_t7;
    s32 temp_v0_3;
    s32 temp_v1;
    s8 temp_t8;
    s8 temp_v0_2;
    u8 *temp_s0_2;
    u8 *temp_s1;
    u8 *temp_s3;
    u8 *temp_s5;

    var_a0 = D_801467FC;
    if (var_a0 == 0) {
        return;
    }
    if (D_80146800 == 0x1E) {
        D_80121864 = 0xB4;
        var_a0 += 1;
        D_80146798 = 1;
    } else if (D_80146800 == 0x5A) {
        var_a0 += 1;
        D_80121864 = 0x28;
    } else if (((u32) D_80146800 >= 0xC9U) && (((u32) D_80146800 % 20U) == 0) && (var_a0 < 0x32)) {
        D_801467FC = var_a0 + 1;
        temp_a0 = D_801467FC;
        temp_t8 = (func_800AE0D0(var_a0, D_80146800) % 2) * 8;
        *(s8 *)((unsigned char *)(&D_801217D0 + (temp_a0 * 0x24)) + (122)) = temp_t8;
        if ((temp_t8 & 0xFF) == 8) {
            *(&D_800E7328 + temp_a0) = 0xC;
            D_801467FC = temp_a0;
            var_a0 = D_801467FC;
            var_v1 = &(&D_80146798)[var_a0];
            *(s16 *)((unsigned char *)var_v1 + (-2)) = (s16) ((func_800AE0D0(temp_a0) % 2) + 3);
        } else {
            *(&D_800E7328 + temp_a0) = 9;
            D_801467FC = temp_a0;
            var_a0 = D_801467FC;
            var_v1 = &(&D_80146798)[var_a0];
            *(s16 *)((unsigned char *)var_v1 + (-2)) = (s16) ((func_800AE0D0(temp_a0) % 2) + 1);
        }
        temp_v0 = *(s16 *)((unsigned char *)var_v1 + (-2));
        *(&D_8014653C + (var_a0 * 4)) = 0x10000 / (s32) (((temp_v0 >= 3) * 2) + temp_v0 + 1);
    }
    if (D_80122D04 == 0) {
        D_801467FC = var_a0;
        var_v1_2 = var_a0;
    } else {
        D_801467FC = var_a0;
        var_v1_2 = 0xA;
        var_s2 = 0xE;
        do {
            temp_lo = var_s2 * 0x24;
            temp_s2 = var_s2 + 1;
            temp_at = temp_s2 < 0x36;
            var_s2 = temp_s2;
            temp_s0 = &D_801217D0 + temp_lo;
            *temp_s0 |= 0x80000000;
        } while (temp_at != 0);
    }
    temp_s6 = var_v1_2 + 4;
    var_s2_2 = 4;
    if (temp_s6 >= 5) {
        do {
            temp_s4 = &(&D_80146798)[var_s2_2];
            temp_v0_2 = D_800B593C & 0xFF;
            temp_s0_2 = &D_801217D0 + (var_s2_2 * 0x24);
            *(s8 *)((unsigned char *)temp_s0_2 + (21)) = temp_v0_2;
            *(s8 *)((unsigned char *)temp_s0_2 + (20)) = temp_v0_2;
            *(u8 *)((unsigned char *)temp_s0_2 + (22)) = (u8) D_800B593C;
            temp_v0_3 = var_s2_2 * 4;
            (*(s32 *)temp_s0_2) = (s32) ((*(s32 *)temp_s0_2) & 0x7FFFFFFF);
            temp_s5 = temp_v0_3 + &D_80146540;
            temp_s1 = temp_v0_3 + &D_80146608;
            temp_s3 = temp_v0_3 + &D_801466D0;
            if ((D_80146800 % (u32) *(s16 *)((unsigned char *)temp_s4 + (-8))) == 0) {
                *(s16 *)((unsigned char *)temp_s0_2 + (6)) = (s16) (*(s16 *)((unsigned char *)temp_s0_2 + (6)) + 1);
            }
            temp_v1 = *(s32 *)((unsigned char *)temp_s5 + (-16));
            temp_t7 = *(s32 *)((unsigned char *)temp_s1 + (-16)) + temp_v1;
            *(s32 *)((unsigned char *)temp_s1 + (-16)) = temp_t7;
            *(s32 *)((unsigned char *)temp_s5 + (-16)) = (s32) (temp_v1 + ((s32) (*(s32 *)((unsigned char *)temp_s3 + (-16)) - temp_t7) >> 8));
            *(s16 *)((unsigned char *)temp_s0_2 + (4)) = (s16) *(s16 *)((unsigned char *)temp_s1 + (-14));
            if (*(s16 *)((unsigned char *)temp_s0_2 + (6)) >= 0x29) {
                temp_hi = func_800AE0D0() % 320;
                *(s32 *)((unsigned char *)temp_s1 + (-16)) = 0;
                *(s16 *)((unsigned char *)temp_s0_2 + (6)) = -0x78;
                *(s16 *)((unsigned char *)temp_s0_2 + (4)) = (s16) (temp_hi - 0x9F);
                *(s16 *)((unsigned char *)temp_s1 + (-14)) = (s16) *(s16 *)((unsigned char *)temp_s0_2 + (4));
                *(s32 *)((unsigned char *)temp_s3 + (-16)) = (s32) *(s32 *)((unsigned char *)temp_s1 + (-16));
                *(u8 *)((unsigned char *)temp_s0_2 + (14)) = (u8) ((func_800AE0D0() % 2) * 8);
                if (D_80122D04 == 1) {
                    *(u8 *)((unsigned char *)temp_s0_2 + (14)) = 0U;
                }
                if (*(u8 *)((unsigned char *)temp_s0_2 + (14)) == 8) {
                    *(&D_800E7325 + var_s2_2) = 0xC;
                    *(s16 *)((unsigned char *)temp_s4 + (-8)) = (s16) ((func_800AE0D0() % 2) + 3);
                } else {
                    *(&D_800E7325 + var_s2_2) = 9;
                    *(s16 *)((unsigned char *)temp_s4 + (-8)) = (s16) ((func_800AE0D0() % 2) + 1);
                }
                temp_v0_4 = *(s16 *)((unsigned char *)temp_s4 + (-8));
                *(s32 *)((unsigned char *)temp_s5 + (-16)) = (s32) (0x10000 / (s32) (((temp_v0_4 >= 3) * 2) + temp_v0_4 + 1));
            }
            var_s2_2 += 1;
        } while (var_s2_2 < temp_s6);
    }
    D_80146800 += 1;
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80141780", func_80142DA0);

void func_8014306C(void) {
    bg_read_sub2(0x4752);
    func_80085B3C(2, 0x34);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80141780", func_801430A0);

void func_80143200(void) {
    func_80044750(0x205);
    func_8004284C();
}

void func_80143228(void) {
    func_80044750(0x504);
}

void func_80143248(void) {
    cd_read_sectors(0x45, 0x801B0000, 0x8BC9);
    func_80141920();
    func_8004284C();
}

void func_80143280(void) {
    D_801206F0 = 0;
    D_801206E0 = 0;
    D_801206DA = 3;
    func_80044750(0x503);
    func_8004284C();
}

void func_801432C4(void) {
    D_800CA150 = 0;
    D_800CA154 = 0;
    func_800847B8(D_800E6280.unk_F5F);
    func_8013C2E4();
    D_80145F34 = D_80145C5C;
    D_80145F38 = D_80145D98;
    D_80145F3C = D_80145ED4;
    func_8004284C();
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_02 += 1;
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 += 2;
    func_80084D3C();
    func_800AE0F0(D_800CA19C, "近所の公園");
}

void func_801433A4(void) {
    D_800E6280.unk_1228[26] = -1;
    D_800E6280.unk_1228[58] = -1;
    func_80141920();
    load_palette(D_80146520, 0x11, 1, 2, 0);
    func_80084E90((s32) D_80146524, (s32) D_80146528, (s32) D_8014652C, (s32) D_80146530, (s32) D_80146534,
                  (s32) D_80146538);
    func_800850D4((s32) D_80146514, (s32) D_80146518, (s32) D_80146510, D_8014651C);
    ((DisplayActor44 *) D_8011ECD0)[97].unk_04 = 8;
    ((DisplayActor44 *) D_8011ECD0)[96].unk_04 = 8;
    ((DisplayActor44 *) D_8011ECD0)[98] = ((DisplayActor44 *) D_8011ECD0)[96];
    ((DisplayActor44 *) D_8011ECD0)[98].unk_16 = 4;
    ((DisplayActor44 *) D_8011ECD0)[98].unk_18 = 0;
    ((DisplayActor44 *) D_8011ECD0)[98].unk_08 = 0;
    ((DisplayActor44 *) D_8011ECD0)[98].unk_02 = 0;
    func_8004284C();
}

void func_801434E0(void) {
    bg_read_sub2(0x42E2);
    func_8004284C();
}

void func_80143508(void) {
    func_8007ED84(0x47C3);
    func_80085B3C(2, 0x42);
    func_8004284C();
}
