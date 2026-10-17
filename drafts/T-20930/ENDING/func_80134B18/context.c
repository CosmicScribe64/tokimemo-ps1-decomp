#include "common.h"
#include "ovl/ENDING.h"
#include "game.h"

void func_80133C10(void) {
    D_8013C4C0.unk_00[0] = 0x801B6550;
    D_8013C4C0.unk_00[1] = 0x801B6C6C;
    D_8013C4C0.unk_00[2] = 0x801B7364;
    D_8013C4C0.unk_00[3] = 0x801B7AB0;
    D_8013C4C0.unk_00[4] = 0x801B8200;
    D_8013C4C0.unk_00[5] = 0x801B8834;
    D_8013C4C0.unk_00[6] = 0x801B8EC4;
    D_8013C4C0.unk_00[7] = 0x801B9520;
    D_8013C4C0.unk_00[8] = 0x801B9B98;
    D_8013C4C0.unk_00[9] = 0x801BA088;
    D_8013C4C0.unk_00[10] = 0x801BA790;
    D_8013C4C0.unk_00[11] = 0x801BACB4;
    D_8013C4C0.unk_00[12] = 0x801BB3B0;
    D_8013C4C0.unk_00[13] = 0x801BB760;
    D_8013C4C0.unk_38[0] = 0x801B6554;
    D_8013C4C0.unk_38[1] = 0x801B6C70;
    D_8013C4C0.unk_38[2] = 0x801B7368;
    D_8013C4C0.unk_38[3] = 0x801B7AB4;
    D_8013C4C0.unk_38[4] = 0x801B8204;
    D_8013C4C0.unk_38[5] = 0x801B8838;
    D_8013C4C0.unk_38[6] = 0x801B8EC8;
    D_8013C4C0.unk_38[7] = 0x801B9524;
    D_8013C4C0.unk_38[8] = 0x801B9B9C;
    D_8013C4C0.unk_38[9] = 0x801BA08C;
    D_8013C4C0.unk_38[10] = 0x801BA794;
    D_8013C4C0.unk_38[11] = 0x801BACB8;
    D_8013C4C0.unk_38[12] = 0x801BB3B4;
    D_8013C4C0.unk_38[13] = 0x801BB764;
    D_8013C4C0.unk_70[0] = 0x801B65AC;
    D_8013C4C0.unk_70[1] = 0x801B6D14;
    D_8013C4C0.unk_70[2] = 0x801B7408;
    D_8013C4C0.unk_70[3] = 0x801B7B5C;
    D_8013C4C0.unk_70[4] = 0x801B82AC;
    D_8013C4C0.unk_70[5] = 0x801B88C4;
    D_8013C4C0.unk_70[6] = 0x801B8F60;
    D_8013C4C0.unk_70[7] = 0x801B95B4;
    D_8013C4C0.unk_70[8] = 0x801B9C30;
    D_8013C4C0.unk_70[9] = 0x801BA128;
    D_8013C4C0.unk_70[10] = 0x801BA834;
    D_8013C4C0.unk_70[11] = 0x801BAD58;
    D_8013C4C0.unk_70[12] = 0x801BB454;
    D_8013C4C0.unk_70[13] = 0x801BB7CC;
    D_8013C5A0[0] = 0x80180000;
    D_8013C5A0[1] = 0x80184000;
    D_8013C5A0[2] = 0x80188000;
    D_8013C5A0[3] = 0x8018C000;
    D_8013C5A0[4] = 0x80190000;
    D_8013C5A0[5] = 0x80194000;
    D_8013C5A0[6] = 0x80198000;
}

void func_80133F1C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    EdFnTbl13 tbl;

    tbl = D_8013C5BC;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

s32 func_80133F98(void) {
    D_800E6280.unk_1104.w += 1;
    if ((u32)D_800E6280.unk_1104.w < 2U) {
        func_80132000();
        func_800438DC(1, 0);
        func_80048DAC(1);
        func_800438F0(1);
        func_80048390();
        func_80041584();
        func_80048EB8(0);
        func_8006BD6C(0);
        func_8004E58C();
        func_8008585C();
        D_800E6280.unk_03A = 0x80;
        D_800B593C = 0;
        D_800B5940 = 0;
        return 0;
    }
    if (D_800E6280.unk_1104.w == 5) {
        func_8009C5E0(0);
    } else if ((u32)D_800E6280.unk_1104.w < 0xBU) {
        return 0;
    }
    func_80134140();
    func_8004284C();
}

s32 func_8013408C(void) {
    D_800E6280.unk_1104.w += 1;
    if ((u32)D_800E6280.unk_1104.w < 2U) {
        func_800438DC(1, 0);
        func_80048DAC(1);
        func_800438F0(1);
        func_80048390();
        func_80041584();
        func_80048EB8(0);
        func_8006BD6C(0);
        func_8004E58C();
        func_8004E9F4(1);
        return 0;
    }
    if ((u32)D_800E6280.unk_1104.w < 0xBU) {
        return 0;
    }
    func_801341D8();
    func_8004284C();
}

void func_80134140(void) {
    func_80097D90(0x100, 0xF0, 0, 0, 0);
    func_80098380();
    func_80098490(0, 0, 0, 0xF0);
    D_800E8C70 = 8;
    D_800E8C74 = D_800E8CA0;
    D_800E8C84 = 8;
    D_800E8C88 = D_800E90A0;
    func_80098530();
    InitGeom();
    D_800E6280.unk_000 = 0x100;
}

void func_801341D8(void) {
    func_80097D90(0x140, 0xF0, 0, 0, 0);
    func_80098380();
    func_80098490(0, 0, 0, 0xF0);
    D_800E8C70 = 8;
    D_800E8C74 = D_800E8CA0;
    D_800E8C84 = 8;
    D_800E8C88 = D_800E90A0;
    func_80098530();
    InitGeom();
    D_800E6280.unk_000 = 0x140;
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80133C10", func_80134270);

void func_801349A8(void) {
    cd_read_sectors(0x78, 0x80180000, 0x7B58);
    func_80133C10();
    func_8004284C();
}

void func_801349E0(void) {
    s16 t;
    s16 i;

    t = D_80120676 - 1;
    for (i = 0; i < 6; i++) {
        *(s16 *)((u8 *)D_801217D0 + i * 0x24 + 0x94) -= 1;
    }
    D_80120676 = t;
    if (t < -0x9E) {
        ((DisplayActor44 *)D_8011ECD0)[96].unk_02 = 1;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = 1;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = 0;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_08 = 0;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_03 |= 0x80;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_02 = 5;
        func_8004284C();
    }
}

void func_80134AA8(void) {
    if ((D_80120696 == 0) && (D_80120668 == 1) && (D_80120658 == 0x14)) {
        D_80120666 = 2;
        D_80120668 = 0;
        D_80120658 = 0;
        D_80120652 = 5;
        func_8004284C();
    }
}

/* NON_MATCHING: T-10520, only the register of the promoted D_80120657 differs ($v0 here, $a1 in the original: one range across the calls), 9 lines */
void func_80134B18(void) {
    s16 i;

    D_800E6280.unk_036 += 8;
    if (D_800E6280.unk_036 == 0 || D_800E6280.unk_036 == 7) {
        D_800E6280.unk_036 = 0xFF;
    }
    D_800E6280.unk_037 = D_800E6280.unk_038 = D_800E6280.unk_036;
    D_80120657 += 4;
    D_8012069B += 4;
    for (i = 4; i < 0xA; i++) {
        func_80084C98(i, D_80120657);
    }
    if (D_80120657 >= 0x80) {
        D_80120652 = 5;
        func_8004284C();
    }
}

void func_80134BE8(void) {
    if (D_80120668 == 2) {
        if (D_80120658 == 0x2F) {
            func_8004284C();
        }
    }
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80133C10", func_80134C2C);
