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

void func_80134270(void) {
    Rec24 *temp_v0;
    s16 temp_v1;
    s16 var_t6;
    s16 var_v1;
    s32 temp_at;
    u8 *temp_t7;

    func_8009C5E0(1);
    func_80133C10();
    load_palette(D_8013C5A0[0], 0x10, 1, 2, 0);
    load_palette(D_8013C5A0[0], 0x11, 1, 2, 1);
    LoadSquare(0x140U, 0x80U, 0x40U, 0x80U, D_8013C5A0[1]);
    LoadSquare(0x180U, 0x80U, 0x40U, 0x80U, D_8013C5A0[2]);
    LoadSquare(0x1C0U, 0x80U, 0x40U, 0x80U, D_8013C5A0[3]);
    LoadSquare(0x200U, 0x80U, 0x40U, 0x80U, D_8013C5A0[4]);
    LoadSquare(0x240U, 0x80U, 0x40U, 0x80U, D_8013C5A0[5]);
    LoadSquare(0x280U, 0x80U, 0x40U, 0x80U, D_8013C5A0[6]);
    D_800E6280.unk_1228[0x25] = 1;
    D_800E6280.unk_1228[0x26] = 1;
    D_800E6280.unk_1228[0x27] = 1;
    D_800E6280.unk_1228[0x28] = 1;
    D_800E6280.unk_1228[0x29] = 1;
    D_800E6280.unk_1228[0x2A] = 1;
    var_v1 = 0;
    do {
        temp_v0 = &D_801217D0[var_v1];
        temp_v0[4].unk_0C = (s16) (var_v1 + 5);
        temp_v0[4].unk_00 = 0x41000000;
        temp_v0[4].unk_08 = 0x80;
        temp_v0[4].unk_0A = 0x80;
        temp_v0[4].unk_0E = 0;
        temp_v0[4].unk_0F = 0x80;
        temp_v0[4].unk_10 = 0;
        temp_v0[4].unk_12 = 0x1F0;
        temp_v0[4].unk_14 = 0;
        temp_v0[4].unk_15 = 0;
        temp_v0[4].unk_16 = 0;
        temp_v0[4].unk_18 = 0;
        temp_v0[4].unk_1A = 0;
        temp_v0[4].unk_1C = 0x1000;
        temp_v0[4].unk_1E = 0x1000;
        temp_v0[4].unk_20 = 0;
        temp_t7 = (u8 *)&D_800E6280 + var_v1;
        temp_v1 = var_v1 + 1;
        temp_v0[4].unk_04 = (s16) (((var_v1 % 3) << 7) - 0x80);
        temp_at = temp_v1 < 6;
        temp_v0[4].unk_06 = (s16) (((var_v1 / 3) << 7) - 0x78);
        var_v1 = temp_v1;
        *(s8 *)((unsigned char *)temp_t7 + (4265)) = 0xB;
    } while (temp_at != 0);
    func_80048F64(0x60);
    ((DisplayActor44 *)D_8011ECD0)[96].unk_01 = 0xC;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_02 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_38 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_03 = 0x84;
    *(s32 *)&((DisplayActor44 *)D_8011ECD0)[96].unk_0C = (s32) D_8013C4C0.unk_38[0];
    *(s32 *)&((DisplayActor44 *)D_8011ECD0)[96].unk_10 = (s32) D_8013C4C0.unk_70[0];
    *(s32 *)&((DisplayActor44 *)D_8011ECD0)[96].unk_34 = (s32) D_8013C4C0.unk_00[0];
    ((DisplayActor44 *)D_8011ECD0)[96].unk_14 = (s16) D_8013C4C0.unk_A8[0].unk_00;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_06 = 1;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_43 = 0x11;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = -0x20;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = 0x28;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_07 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_05 = 1;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_04 = 0;
    ((DisplayActor44 *)D_8011ECD0)[97] = ((DisplayActor44 *)D_8011ECD0)[96];
    ((DisplayActor44 *)D_8011ECD0)[97].unk_03 = 4;
    ((DisplayActor44 *)D_8011ECD0)[97].unk_26 = 0x10;
    switch (D_800E6280.unk_F5F) {
    case 0:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C4C0.unk_38[1];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C4C0.unk_70[1];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4C0.unk_00[1];
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = (s16) D_8013C4C0.unk_A8[1].unk_00;
        break;
    case 1:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C4C0.unk_38[2];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C4C0.unk_70[2];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4C0.unk_00[2];
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = (s16) D_8013C4C0.unk_A8[2].unk_00;
        break;
    case 2:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C4C0.unk_38[3];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C4C0.unk_70[3];
        var_t6 = D_8013C4C0.unk_A8[3].unk_00;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4C0.unk_00[3];
block_17:
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = var_t6;
        break;
    case 3:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C4C0.unk_38[4];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C4C0.unk_70[4];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4C0.unk_00[4];
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = (s16) D_8013C4C0.unk_A8[4].unk_00;
        break;
    case 4:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C4C0.unk_38[5];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C4C0.unk_70[5];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4C0.unk_00[5];
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = (s16) D_8013C4C0.unk_A8[5].unk_00;
        break;
    case 5:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C4C0.unk_38[6];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C4C0.unk_70[6];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4C0.unk_00[6];
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = (s16) D_8013C4C0.unk_A8[6].unk_00;
        break;
    case 6:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C4C0.unk_38[7];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C4C0.unk_70[7];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4C0.unk_00[7];
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = (s16) D_8013C4C0.unk_A8[7].unk_00;
        break;
    case 7:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C4C0.unk_38[8];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C4C0.unk_70[8];
        var_t6 = D_8013C4C0.unk_A8[8].unk_00;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4C0.unk_00[8];
        goto block_17;
    case 8:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C4C0.unk_38[9];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C4C0.unk_70[9];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4C0.unk_00[9];
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = (s16) D_8013C4C0.unk_A8[9].unk_00;
        break;
    case 9:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C4C0.unk_38[0xA];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C4C0.unk_70[0xA];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4C0.unk_00[0xA];
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = (s16) D_8013C4C0.unk_A8[0xA].unk_00;
        break;
    case 10:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C4C0.unk_38[0xB];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C4C0.unk_70[0xB];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4C0.unk_00[0xB];
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = (s16) D_8013C4C0.unk_A8[0xB].unk_00;
        break;
    case 12:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C4C0.unk_38[0xC];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C4C0.unk_70[0xC];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4C0.unk_00[0xC];
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = (s16) D_8013C4C0.unk_A8[0xC].unk_00;
        break;
    default:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C4C0.unk_38[0xD];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C4C0.unk_70[0xD];
        var_t6 = D_8013C4C0.unk_A8[0xD].unk_00;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4C0.unk_00[0xD];
        goto block_17;
    }
    D_800E6280.unk_037 = 0;
    D_800E6280.unk_036 = 0;
    D_800E6280.unk_038 = 0;
    func_8004284C();
}

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

/* FAKE: explicit GameState byte views preserve the promoted colour register; shared base symbol D_80120650 lets as1 hoist the fade-alpha load across the second actor's store (T-24057, T-24108). */
void func_80134B18(void) {
    s16 i;

    (*(u8 *)&D_800E6280.unk_036) += 8;
    if ((*(u8 *)&D_800E6280.unk_036) == 0 || (*(u8 *)&D_800E6280.unk_036) == 7) {
        (*(u8 *)&D_800E6280.unk_036) = 0xFF;
    }
    (*(u8 *)&D_800E6280.unk_037) = (*(u8 *)&D_800E6280.unk_038) = (*(u8 *)&D_800E6280.unk_036);
    ((DisplayActor44 *)D_80120650)->unk_07 += 4;
    ((DisplayActor44 *)D_80120650)[1].unk_07 += 4;
    for (i = 4; i < 0xA; i++) {
        func_80084C98(i, ((DisplayActor44 *)D_80120650)->unk_07);
    }
    if (((DisplayActor44 *)D_80120650)->unk_07 >= 0x80) {
        ((DisplayActor44 *)D_80120650)->unk_02 = 5;
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

/* FAKE: explicit GameState byte views preserve the promoted colour register; shared base symbol D_80120650 lets as1 hoist the fade-alpha load across the second actor's store (T-24057, T-24108). */
void func_80134C2C(void) {
    s16 i;

    (*(u8 *)&D_800E6280.unk_036) -= 8;
    if ((*(u8 *)&D_800E6280.unk_036) == 7 || (*(u8 *)&D_800E6280.unk_036) == 0xF8) {
        (*(u8 *)&D_800E6280.unk_036) = 0;
    }
    (*(u8 *)&D_800E6280.unk_037) = (*(u8 *)&D_800E6280.unk_038) = (*(u8 *)&D_800E6280.unk_036);
    ((DisplayActor44 *)D_80120650)->unk_07 -= 4;
    ((DisplayActor44 *)D_80120650)[1].unk_07 -= 4;
    for (i = 4; i < 0xA; i++) {
        func_80084C98(i, ((DisplayActor44 *)D_80120650)->unk_07);
    }
    if (((DisplayActor44 *)D_80120650)->unk_07 < 4) {
        ((DisplayActor44 *)D_80120650)->unk_03 &= 0xFF7F;
        ((DisplayActor44 *)D_80120650)[1].unk_03 &= 0xFF7F;
        func_8004284C();
    }
}

