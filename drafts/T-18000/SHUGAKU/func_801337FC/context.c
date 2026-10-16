#include "common.h"
#include "ovl/SHUGAKU.h"

void func_80133430(void) {
    D_8013BE70 = 0x801B00DC;
    D_8013BE74 = 0x801B02AC;
    D_8013BE78 = 0x801B03C8;
    D_8013BE7C = 0x801B04C4;
    D_8013BE80 = 0x801B0668;
    D_8013BE84 = 0x801B0808;
    D_8013BE88 = 0x801B09DC;
    D_8013BE8C = 0x801B0BE8;
    D_8013BE90 = 0x801B0D8C;
    D_8013BE94 = 0x801B0F5C;
    D_8013BE98 = 0x801B1164;
    D_8013BE9C = 0x801B12D8;
    D_8013BEA0 = 0x801B1454;
    D_8013BEA4 = 0x801B00F4;
    D_8013BEA8 = 0x801B02C8;
    D_8013BEAC = 0x801B03D0;
    D_8013BEB0 = 0x801B04DC;
    D_8013BEB4 = 0x801B0680;
    D_8013BEB8 = 0x801B0820;
    D_8013BEBC = 0x801B09F8;
    D_8013BEC0 = 0x801B0C00;
    D_8013BEC4 = 0x801B0DA4;
    D_8013BEC8 = 0x801B0F78;
    D_8013BECC = 0x801B1180;
    D_8013BED0 = 0x801B12E0;
    D_8013BED4 = 0x801B1470;
    D_8013BED8 = 0x801B01A8;
    D_8013BEDC = 0x801B0398;
    D_8013BEE0 = 0x801B03F0;
    D_8013BEE4 = 0x801B0590;
    D_8013BEE8 = 0x801B0734;
    D_8013BEEC = 0x801B08D4;
    D_8013BEF0 = 0x801B0AE4;
    D_8013BEF4 = 0x801B0CB4;
    D_8013BEF8 = 0x801B0E58;
    D_8013BEFC = 0x801B1064;
    D_8013BF00 = 0x801B126C;
    D_8013BF04 = 0x801B1338;
    D_8013BF08 = 0x801B155C;
}

void func_801336A4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    ShFnTbl43 tbl;

    tbl = D_8013BF20;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_80133720(void) {
    cd_read_sectors(3, 0x801B0000, 0xAF43);
    func_80133430();
    func_8004284C();
}

void func_80133758(void) {
    func_800847B8(D_800E6280.unk_F5F);
    func_80133430();
    func_80133A68();
    if (*(u8 *)&D_800CA2F0 != 2) {
        if (D_800B593C != 0) {
            D_800CA2F0 = 1;
        } else {
            D_800CA2F0 = 0;
        }
    }
    D_8013BF18 = 0;
    D_8013BF1C = 0;
    func_8004284C();
    if (*(u8 *)&D_800CA2F0 == 2) {
        D_800E6280.unk_110A += 2;
    }
}

s32 func_801337FC(void) {
    s16 var_s0;
    s16 var_s7;
    s32 temp_s2;
    s32 temp_s3;
    s32 temp_s4;
    s32 temp_v0;
    s32 temp_v1;
    u32 temp_a0;
    u32 temp_t2;
    u32 var_s5;
    u8 temp_a0_2;

    var_s7 = 0xFF;
    var_s5 = 4;
    var_s0 = 0;
    do {
        if ((D_800CA2B0[var_s0] != 0) && ((temp_v1 = get_g_zyotai_s(var_s0) & 0x7F, temp_a0 = temp_v1 & 0xFF, ((temp_a0 < var_s5) != 0)) || ((temp_a0 == var_s5) && (temp_a0 == var_s5) && (temp_s2 = get_h_yuukou(var_s0) & 0xFF, temp_s3 = get_h_tokimeki(var_s0) & 0xFF, temp_s4 = get_h_tokimeki(var_s7) & 0xFF, (((u32) (get_h_yuukou(var_s7) + temp_s4) < (u32) (temp_s3 + temp_s2)) != 0))))) {
            var_s7 = var_s0 & 0xFF;
            var_s5 = temp_v1 & 0xFF & 0xFF;
        }
        var_s0 += 1;
    } while (var_s0 < 0xA);
    temp_v0 = get_g_zyotai_s(var_s7);
    temp_t2 = temp_v0 & 0x7F;
    if ((var_s7 == 4) && (temp_t2 < 2U) && (D_800E6280.unk_721 == 5)) {
        if (D_800E6280.unk_56C[0x20] != 0) {
            D_800E6280.unk_71E = D_800E6280.unk_71E | 8;
        }
    }
    if ((var_s7 != 0xFF) && ((u32) (temp_v0 & 0x7F) < 2U)) {
        temp_a0_2 = var_s7 & 0xFF;
        if (!(D_800E6280.unk_71E & 8)) {
            D_800E6280.unk_F5F = temp_a0_2;
            func_800847B8(temp_a0_2);
            func_80133A68();
            if (D_800E6280.unk_720 != 6) {
                func_80085B3C(0xD, 0x26);
            } else {
                switch (D_800CA2CC) {
                case 0:
                    func_80085B3C(0xD, 0x27);
                    break;
                case 1:
                    func_80085B3C(0xD, 0x28);
                    break;
                default:
                    func_80085B3C(0xD, 0x29);
                    break;
                }
            }
            return func_8004284C();
        }
    }
    D_8013BF0C = D_8013BE9C;
    D_8013BF10 = D_8013BED0;
    D_8013BF14 = D_8013BF04;
    D_8013BF18 = 0;
    D_8013BF1C = 0;
    D_800E6280.unk_110A = D_800E6280.unk_110A + 0x20;
    return 0;
}

void func_80133A68(void) {
    switch (D_800E6280.unk_F5F) {
    case 0:
        D_8013BF0C = D_8013BE74;
        D_8013BF10 = D_8013BEA8;
        D_8013BF14 = D_8013BEDC;
        return;
    case 1:
        D_8013BF0C = D_8013BE88;
        D_8013BF10 = D_8013BEBC;
        D_8013BF14 = D_8013BEF0;
        return;
    case 2:
        D_8013BF0C = D_8013BE7C;
        D_8013BF10 = D_8013BEB0;
        D_8013BF14 = D_8013BEE4;
        return;
    case 3:
        D_8013BF0C = D_8013BE80;
        D_8013BF10 = D_8013BEB4;
        D_8013BF14 = D_8013BEE8;
        return;
    case 4:
        D_8013BF0C = D_8013BE98;
        D_8013BF10 = D_8013BECC;
        D_8013BF14 = D_8013BF00;
        return;
    case 5:
        D_8013BF0C = D_8013BE8C;
        D_8013BF10 = D_8013BEC0;
        D_8013BF14 = D_8013BEF4;
        return;
    case 6:
        D_8013BF0C = D_8013BE90;
        D_8013BF10 = D_8013BEC4;
        D_8013BF14 = D_8013BEF8;
        return;
    case 7:
        D_8013BF0C = D_8013BE84;
        D_8013BF10 = D_8013BEB8;
        D_8013BF14 = D_8013BEEC;
        return;
    case 8:
        D_8013BF0C = D_8013BE70;
        D_8013BF10 = D_8013BEA4;
        D_8013BF14 = D_8013BED8;
        return;
    case 9:
        D_8013BF0C = D_8013BE94;
        D_8013BF10 = D_8013BEC8;
        D_8013BF14 = D_8013BEFC;
        return;
    case 10:
        D_8013BF0C = D_8013BEA0;
        D_8013BF10 = D_8013BED4;
        D_8013BF14 = D_8013BF08;
        /* fallthrough */
    default:
        return;
    }
}

void func_80133CD8(void) {
    D_800CA134 = (u8 *) &D_8013BF18;
    D_800CA138 = (u8 *) &D_8013BF1C;
    D_800CA13C = D_8013BF0C;
    D_800CA140 = D_8013BF10;
    D_800CA144 = D_8013BF14;
    func_80082764(D_80122CDC, 1, 0);
}
