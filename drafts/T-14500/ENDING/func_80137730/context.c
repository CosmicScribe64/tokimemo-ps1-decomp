#include "common.h"
#include "ovl/ENDING.h"

s32 func_80137340(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl6 tbl;

    tbl = D_8013CAA4;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
    if (D_800E6280.unk_110A >= 3 || D_800E6280.unk_1109 >= 6) {
        return 0;
    }
    func_80132F18();
    func_8004EC24();
    if (tbl.f[D_800E6280.unk_110A] == func_80132D98) {
        func_80137730();
    }
    _sprite_set_box_shade_tarao(-0xA0, -0x78, 0x140, 0xF0, 0xE, 0xC0C0FF, 0x4080FF);
    dtd_on(0xE);
    func_80136D60();
    func_80137AA8();
    if (D_800E6280.unk_10F8 % 240 == 0 && D_80122D0C == 0) {
        func_80137574();
    }
}

void func_801374BC(void) {
    if (D_800E6280.unk_1124 != 0) {
        main_state_set(0xC2);
        return;
    }
    func_80042808();
}

void func_801374F8(void) {
    _sprite_set_box_shade_tarao(-0xA0, -0x78, 0x140, 0xF0, 0xE, 0xC0C0FF, 0x4080FF);
    dtd_on(0xE);
    if (D_800E6280.unk_1104.w++ >= 0x81U) {
        func_8004284C();
    }
}

s32 func_80137574(void)
{
  u8 *byte;
  s16 promoted;
  u8 value;
  promoted = D_8011F4CA;
  value = promoted;
  byte = &value;
  if (((*byte) & 1) == 0)
  {
    value |= 3;
    D_8011F4CA = *byte;
  }
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80137340", func_8013759C);

void func_801376FC(void) {
    D_80122CE4 = 0;
    D_8013C364 = 0;
    D_8013C360 = 0;
    func_8004284C();
}

/* bulk_m2c declarations (T-11190) */
extern s16 D_80125C80;
extern s16 D_80125C82;
extern s16 D_80125C84;
extern s16 D_80125C86;
extern s32 D_800B5A68;
extern s32 D_800E7200;
extern s32 D_800E7378;
extern u8 D_800E71DF;
/* end bulk_m2c declarations */
void func_80137730(void) {
    s16 *temp_s2;
    s16 *temp_v0;
    s16 temp_v0_2;
    s16 var_s0;
    s16 var_s0_2;
    s16 var_s0_3;
    s16 var_s0_4;
    s16 var_s0_5;
    s16 var_v1;
    s16 var_v1_2;
    s16 var_v1_3;
    s32 var_a0;
    s32 var_a2;
    s32 var_at;
    s32 var_at_2;
    u8 *temp_a1;
    u8 *var_t0;
    u8 temp_v1;
    u8 var_v0;
    u8 *temp_v0_3;

    temp_v0 = get_ksys();
    temp_s2 = temp_v0;
    if (D_8013C35C <= 0) {

    } else {
        temp_v1 = *(u8 *)((unsigned char *)(*(s32 *)(D_8013C350 + (D_8013C358 * 4)) + D_8013C35C) + (-1));
        if ((temp_v1 == 0) || (temp_v1 == 6)) {
            var_s0 = 1;
            if (*temp_v0 > 0) {
                do {
                    *(s8 *)((unsigned char *)get_k_work(var_s0) + (7)) = 1;
                    var_s0 += 1;
                } while (*temp_s2 >= var_s0);
            }
            D_8011F524 = 0;
        } else {
            var_v1 = 0;
            if (func_800460EC() & 4) {
                var_v1 = getCDlevel();
            }
            (&D_80125C80)[D_800E7378 & 3] = var_v1;
            var_v1_2 = D_80125C80;
            var_a0 = 0;
            if (var_v1_2 < D_80125C82) {
                var_v1_2 = D_80125C82;
                var_a0 = 1;
            }
            var_s0_2 = 0;
            if (var_v1_2 < D_80125C84) {
                var_v1_2 = D_80125C84;
                var_a0 = 2;
            }
            var_a2 = 0;
            var_v1_3 = 0;
            if (var_v1_2 < D_80125C86) {
                var_a0 = 3;
            }
            do {
                if (var_s0_2 != var_a0) {
                    temp_v0_2 = (&D_80125C80)[var_s0_2];
                    if (var_v1_3 < temp_v0_2) {
                        var_v1_3 = temp_v0_2;
                    }
                }
                var_s0_2 += 1;
            } while (var_s0_2 < 4);
            temp_a1 = (D_800E71DF * 3) + &D_800B5A68;
            var_s0_3 = 0;
            var_t0 = temp_a1;
            do {
                var_at = (u8) *var_t0 < (u32) var_v1_3;
                if (var_s0_3 == 0) {
                    var_at = (u32) var_v1_3 > 8U;
                }
                if (var_at != 0) {
                    var_a2 = var_s0_3 + 1;
                }
                var_s0_3 += 1;
                var_t0 = temp_a1 + var_s0_3;
            } while (var_s0_3 < 3);
            var_v0 = 0;
            if (D_800E71DF == 0) {
                var_at_2 = var_a2 < 4;
                if (D_8011F522 == 8) {
                    var_v0 = 1;
                    goto block_28;
                }
            } else {
block_28:
                var_at_2 = var_a2 < 4;
            }
            if (var_at_2 == 0) {
                var_a2 = 3;
            }
            D_800B5A60 = var_v0;
            if (var_a2 != 0) {
                D_800B5A60 = var_v0;
                if (var_v0 != 0) {
                    var_a2 = 3 - var_a2;
                }
            }
            if ((D_8011F524 != 0) && (D_800E7378 & 1)) {

            } else if (var_v1_3 == 0) {
                D_8011F524 = 0;
            } else if (var_a2 == 0) {
                D_8011F524 = 0;
            } else if (var_a2 == 1) {
                D_8011F524 = 1;
            } else if (var_a2 == 2) {
                D_8011F524 = 2;
            } else if (var_a2 >= 3) {
                D_8011F524 = 3;
            }
            if (D_800E7200 != 0) {
                var_s0_4 = 1;
                if (*temp_s2 > 0) {
                    do {
                        temp_v0_3 = get_k_work(var_s0_4);
                        if (*(s16 *)((unsigned char *)temp_v0_3 + (2)) >= 0x48) {
                            *(s8 *)((unsigned char *)temp_v0_3 + (7)) = 1;
                        }
                        var_s0_4 += 1;
                    } while (*temp_s2 >= var_s0_4);
                }
            } else {
                var_s0_5 = 1;
                if (*temp_s2 > 0) {
                    do {
                        *(s8 *)((unsigned char *)get_k_work(var_s0_5) + (7)) = 0;
                        var_s0_5 += 1;
                    } while (*temp_s2 >= var_s0_5);
                }
            }
        }
    }
}

void func_80137AA8(void) {
    switch (D_800E6280.unk_F5F) {
    case 0:
        func_80137BB4();
        return;
    case 1:
        func_80137E40();
        return;
    case 2:
        func_80137F64();
        return;
    case 3:
        func_801380B0();
        return;
    case 4:
        func_80138278();
        return;
    case 5:
        func_8013855C();
        return;
    case 6:
        func_801387B0();
        return;
    case 7:
        func_80138A9C();
        return;
    case 8:
        func_80138C70();
        return;
    case 9:
        func_80138DC8();
        return;
    case 10:
        func_80138FC8();
        return;
    case 12:
        func_801391E4();
        return;
    default:
        func_80139498();
        return;
    }
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80137340", func_80137BB4);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80137340", func_80137E40);

/* D_8011F4D0/F4DE/F4E0/F522 are fields +0x18/+0x26/+0x28 of the 0x44-byte records 30 and 31 of the
 * D_8011ECD0 table; read through the table they compare in the original's operand order (T-9020). */
void func_80137F64(void) {
    if (D_8013C35C == 1) {
        if (D_8013C360 == 0) {
            *(s16 *)&D_8011ECD0[0x852] = 5;
            func_801396A4(0, 0xFF);
            D_80122D0C = 0;
        }
    } else if (D_8013C35C == 0x17) {
        if (D_8013C360 == 0) {
        } else if (D_8013C360 == 1) {
            *(s16 *)&D_8011ECD0[0x852] = 6;
            func_801396A4(1, 5);
            D_80122D0C = 1;
        } else if (*(s16 *)&D_8011ECD0[0x80E] == 1 && *(s16 *)&D_8011ECD0[0x810] == 3) {
            func_801396A4(2, 1);
        }
    } else if (D_8013C35C == 0x18) {
        if (D_8013C360 == 0) {
            func_801396A4(3, 0);
            D_80122D0C = 0;
        }
    } else if (D_8013C35C == 0x1B && D_8013C360 == 0) {
        func_801396A4(4, 5);
        D_80122D0C = 1;
    }
    D_8013C360 += 1;
}

void func_801380B0(void) {
    if (D_8013C35C == 1) {
        if (D_8013C360 == 0) {
            *(s16 *)&D_8011ECD0[0x852] = 4;
            func_801396A4(0, 0xFF);
            D_80122D0C = 0;
        }
    } else if (D_8013C35C == 0xD) {
        if (D_8013C360 == 1) {
            *(s16 *)&D_8011ECD0[0x852] = 5;
            func_801396A4(1, 1);
            D_80122D0C = 1;
        }
    } else if (D_8013C35C == 0xE) {
        if (D_8013C360 == 1) {
            *(s16 *)&D_8011ECD0[0x852] = 4;
            func_801396A4(0, 0);
            D_80122D0C = 0;
        }
    } else if (D_8013C35C == 0x12) {
        if (D_8013C360 == 1) {
            *(s16 *)&D_8011ECD0[0x852] = 5;
            func_801396A4(1, 1);
            D_80122D0C = 1;
        }
    } else if (D_8013C35C == 0x15) {
        if (D_8013C360 == 1) {
            *(s16 *)&D_8011ECD0[0x852] = 4;
            func_801396A4(0, 0);
            D_80122D0C = 0;
        }
    } else if (D_8013C35C == 0x17) {
        if (D_8013C360 == 0) {
            *(s16 *)&D_8011ECD0[0x852] = 6;
            func_801396A4(2, 1);
            D_80122D0C = 1;
        }
    } else if (D_8013C35C == 0x19 && D_8013C360 == 1) {
        func_801396A4(3, 0);
        D_80122D0C = 0;
    }
    D_8013C360 += 1;
}

void func_80138278(void) {
    u32 remainder;

    if (D_8013C35C == 1) {
        if (D_8013C360 == 0) {
            *(s16 *)&D_8011ECD0[0x852] = 6;
            func_801396A4(0, 0xFFU);
            D_80122D0C = 0;
        }
    } else if (D_8013C35C == 8) {
        if (D_8013C360 == 0) {
            func_801396A4(1, 0xFFU);
        }
    } else if (D_8013C35C == 9) {
        if (D_8013C360 == 0) {
            func_801396A4(0, 0xFFU);
        }
    } else if (D_8013C35C == 0xA) {
        if (D_8013C360 == 0xF0) {
            *(s16 *)&D_8011ECD0[0x852] = 7;
            func_801396A4(1, 0xFFU);
        }
    } else if (D_8013C35C == 0xC) {
        if (D_8013C360 == 0) {
            func_801396A4(0, 0xFFU);
        }
    } else if (D_8013C35C == 0xF) {
        if (D_8013C360 == 0) {
            *(s16 *)&D_8011ECD0[0x852] = 6;
            func_801396A4(1, 0xFFU);
        }
    } else if (D_8013C35C == 0x11) {
        if (D_8013C360 == 1) {
            *(s16 *)&D_8011ECD0[0x852] = 8;
            func_801396A4(2, 1U);
            D_80122D0C = 1;
        }
        if (D_8013C360 == 0x8C) {
            func_801396A4(4, 5U);
        }
    } else if (D_8013C35C == 0x12) {
        if (D_8013C360 == 0) {
            func_801396A4(2, 1U);
            D_80122D0C = 0;
        }
    } else if ((D_800E6280.unk_110A == 2) && (D_800E6280.unk_1104.w == 0)) {
        func_801396A4(5, 5U);
        D_80122D0C = 1;
    }
    if ((D_8013C35C >= 0x12) && (D_8011F4DE < 4)) {
        /* FAKE: this dead test puts the dividend in v1 and removes one
         * temporary from the remainder chunk (T-9410). */
        if (D_800E6280.unk_10F8) {
        }
        remainder = D_800E6280.unk_10F8 % 240;
        if (remainder == 0) {
            D_8011F4DE = 3;
            D_8011F4E0 = 0;
            D_8011F4D0 = 0;
        }
        if (remainder >= 8U) {
            D_8011F4DE = 2;
            D_8011F4E0 = 0;
            D_8011F4D0 = 0;
            D_8011F4CA = 1;
        }
    }
    D_8013C360 += 1;
}

void func_8013855C(void) {
    u32 remainder;
    s16 one; /* FAKE: narrow constant keeps the compare operands in order (T-9410). */
    one = 1;
    if (D_8013C35C == 1) {
        if (D_8013C360 == 0) {
            *(s16 *)&D_8011ECD0[0x852] = 7;
            func_801396A4(0, 0xFF);
            D_80122D0C = 0;
        }
    } else if (D_8013C35C == 0xB) {
        if (D_8013C360 == 0xFA) {
            func_801396A4(1, 0xFF);
        }
    } else if (D_8013C35C == 0xD) {
        if (D_8013C360 == 1) {
            *(s16 *)&D_8011ECD0[0x852] = 8;
            func_801396A4(2, 5);
            D_80122D0C = 1;
        } else if (D_8013C360 == 0x64) {
            func_801396A4(3, 5);
        } else if (D_8011F4DE == 3 && one == D_8011F4E0 && one == D_8011F4D0) {
            func_801396A4(4, 1);
            D_80122D0C = 0;
        }
    } else if (D_8013C35C == 0x11) {
        if (D_8013C360 == 0x172) {
            func_801396A4(6, 5);
            D_80122D0C = 1;
        }
    }
    if (D_8013C35C >= 0xD) {
        /* FAKE: dead test selects v1 and removes a remainder temporary (T-9410). */
        if (D_800E6280.unk_10F8) {
        }
        remainder = D_800E6280.unk_10F8 % 240;
        if (remainder == 0) {
            if (D_8011F4DE == 4) {
                D_8011F4DE = 5;
                D_8011F4E0 = 0;
                D_8011F4D0 = 0;
            }
        }
        if ((remainder >= 0xCU) && (D_8011F4DE == 5)) {
            D_8011F4DE = 4;
            D_8011F4E0 = 0;
            D_8011F4CA = 1;
            D_8011F4D0 = 0;
        }
    }
    D_8013C360 += 1;
    if (D_8013C35C == 9 && D_8013C360 != 0) {
        func_8004EBEC(0xE);
    }
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80137340", func_801387B0);

void func_80138A9C(void) {
    if (D_8013C35C == 1) {
        if (D_8013C360 == 0) {
            *(s16 *)&D_8011ECD0[0x852] = 7;
            func_801396A4(0, 0xFF);
            D_80122D0C = 0;
        }
    } else if (D_8013C35C == 6) {
        if (D_8013C360 == 1) {
            func_801396A4(1, 5);
            D_80122D0C = 1;
        }
    } else if (D_8013C35C == 8) {
        if (D_8013C360 == 1) {
            func_801396A4(2, 5);
        }
    } else if (D_8013C35C == 9) {
        if (D_8013C360 == 0) {
            func_801396A4(0, 0);
            D_80122D0C = 0;
        }
    } else if (D_8013C35C == 0x18) {
        if (D_8013C360 == 0) {
            func_801396A4(3, 5);
            D_80122D0C = 1;
        } else if (*(s16 *)&D_8011ECD0[0x80E] == 3 && *(s16 *)&D_8011ECD0[0x810] == 2 && *(s16 *)&D_8011ECD0[0x800] == 1) {
            func_801396A4(4, 0);
            D_80122D0C = 0;
        }
    } else if (D_8013C35C == 0x1C) {
        *(s16 *)&D_8011ECD0[0x852] = 8;
        if (D_8013C360 == 0) {
            func_801396A4(5, 0xFF);
        }
    } else if (D_8013C35C == 0x1E && D_8013C360 == 0x16D) {
        func_801396A4(6, 5);
        D_80122D0C = 1;
    }
    D_8013C360 += 1;
}

void func_80138C70(void) {
    if (D_8013C35C == 1) {
        if (D_8013C360 == 0) {
            *(s16 *)&D_8011ECD0[0x852] = 5;
            func_801396A4(0, 0xFF);
            D_80122D0C = 0;
        }
    } else if (D_8013C35C == 7) {
        if (D_8013C360 == 0) {
            *(s16 *)&D_8011ECD0[0x852] = 6;
        }
    } else if (D_8013C35C == 9) {
        if (D_8013C360 == 0) {
            *(s16 *)&D_8011ECD0[0x852] = 5;
            func_801396A4(1, 5);
            D_80122D0C = 1;
        }
    } else if (D_8013C35C == 0xB) {
        if (D_8013C360 == 0) {
            func_801396A4(2, 0);
            D_80122D0C = 0;
        }
    } else if (D_8013C35C == 0xF) {
        if (D_8013C360 == 0) {
            *(s16 *)&D_8011ECD0[0x852] = 7;
            func_801396A4(3, 5);
            D_80122D0C = 1;
        } else if (D_8013C360 == 0x3C) {
            func_801396A4(4, 0);
            D_80122D0C = 0;
        }
    }
    D_8013C360 += 1;
}

void func_80138DC8(void) {
    if (D_8013C35C == 1) {
        if (D_8013C360 == 0) {
            *(s16 *)&D_8011ECD0[0x852] = 8;
            func_801396A4(0, 0xFF);
            D_80122D0C = 0;
        }
    } else if (D_8013C35C == 6) {
        if (D_8013C360 == 0) {
            func_801396A4(1, 5);
            D_80122D0C = 1;
        }
    } else if (D_8013C35C == 8) {
        if (D_8013C360 == 0) {
            func_801396A4(0, 0);
            D_80122D0C = 0;
        }
    } else if (D_8013C35C == 0xB) {
        if (D_8013C360 == 0) {
            func_801396A4(2, 5);
            D_80122D0C = 1;
        } else if (*(s16 *)&D_8011ECD0[0x80E] == 2 && *(s16 *)&D_8011ECD0[0x810] == 2 && *(s16 *)&D_8011ECD0[0x800] == 1) {
            func_801396A4(3, 0);
            D_80122D0C = 0;
        }
    } else if (D_8013C35C == 0xD) {
        if (D_8013C360 == 0) {
            func_801396A4(4, 5);
            D_80122D0C = 1;
        }
    } else if (D_8013C35C == 0xF) {
        if (D_8013C360 == 0) {
            func_801396A4(5, 5);
        }
        if (D_8013C360 == 0xD2) {
            *(s16 *)&D_8011ECD0[0x852] = 9;
            func_801396A4(6, 0);
            D_80122D0C = 0;
        }
    } else if (D_8013C35C == 0x14 && D_8013C360 == 0xC8) {
        func_801396A4(4, 5);
        D_80122D0C = 1;
    }
    D_8013C360 += 1;
}

void func_80138FC8(void) {
    u32 remainder;

    if (D_8013C35C == 1) {
        if (D_8013C360 == 0) {
            *(s16 *)&D_8011ECD0[0x852] = 6;
            func_801396A4(0, 0xFFU);
            D_80122D0C = 0;
        }
    } else if (D_8013C35C == 9) {
        if (D_8013C360 == 0xDC) {
            func_801396A4(1, 1U);
            D_80122D0C = 1;
        }
    } else if (D_8013C35C == 0xC) {
        if (D_8013C360 == 0) {
            *(s16 *)&D_8011ECD0[0x852] = 7;
            func_801396A4(2, 1U);
            D_80122D0C = 0;
        }
    } else if (D_8013C35C == 0x10) {
        if (D_8013C360 == 1) {
            func_801396A4(4, 5U);
            D_80122D0C = 1;
        } else if (D_8013C360 == 0x5A) {
            func_801396A4(2, 1U);
            D_80122D0C = 0;
        }
    } else if ((D_8013C35C == 0x14) && (D_8013C360 == 0xC8)) {
        func_801396A4(5, 5U);
        D_80122D0C = 1;
    }
    if (D_8013C35C >= 0xC) {
        /* FAKE: dead test selects v1 and removes a remainder temporary (T-9410). */
        if (D_800E6280.unk_10F8) {
        }
        remainder = D_800E6280.unk_10F8 % 240;
        if (remainder == 0) {
            if (D_8011F4DE == 2) {
                D_8011F4DE = 3;
                D_8011F4E0 = 0;
                D_8011F4D0 = 0;
            }
        }
        if ((remainder >= 0xBU) && (D_8011F4DE == 3)) {
            D_8011F4DE = 2;
            D_8011F4E0 = 0;
            D_8011F4D0 = 0;
            D_8011F4CA = 1;
        }
    }
    D_8013C360 += 1;
}

void func_801391E4(void) {
    u32 remainder;
    if (D_8013C35C == 1) {
        if (D_8013C360 == 0) {
            *(s16 *)&D_8011ECD0[0x852] = 4;
            func_801396A4(0, 0xFF);
            D_80122D0C = 0;
        }
    } else if (D_8013C35C == 19) {
        if (D_8013C360 == 170) {
            func_801396A4(1, 1);
        }
    } else if (D_8013C35C == 23) {
        if (D_8013C360 == 0) {
            func_801396A4(0, 0);
        }
    } else if (D_8013C35C == 32) {
        if (D_8013C360 == 0) {
            func_801396A4(2, 1);
        }
    } else if (D_8013C35C == 34) {
        if (D_8013C360 == 0) {
            *(s16 *)&D_8011ECD0[0x852] = 5;
        }
    } else if (D_8013C35C == 37) {
        if (D_8013C360 == 0) {
            *(s16 *)&D_8011ECD0[0x852] = 4;
            func_801396A4(3, 0);
        }
    } else if (D_8013C35C == 42) {
        if (D_8013C360 == 0) {
            func_801396A4(0, 0);
        }
    } else if (D_8013C35C == 44) {
        if (D_8013C360 == 0) {
            *(s16 *)&D_8011ECD0[0x852] = 5;
            func_801396A4(2, 1);
        }
    }
    if ((D_8013C35C >= 0x13 && D_8013C35C < 0x17) || (D_8013C35C >= 0x22 && D_8013C35C < 0x25) || D_8013C35C >= 0x2C) {
        /* FAKE: dead test selects v1 and removes a remainder temporary (T-9410). */
        if (D_800E6280.unk_10F8) {
        }
        remainder = D_800E6280.unk_10F8 % 240;
        if (remainder == 0) {
            if (D_8011F4DE == 2) {
                D_8011F4DE = 3;
                D_8011F4E0 = 0;
                D_8011F4D0 = 0;
            }
        }
        if ((remainder >= 0xAU) && (D_8011F4DE == 3)) {
            D_8011F4DE = 2;
            D_8011F4E0 = 0;
            D_8011F4D0 = 0;
            D_8011F4CA = 1;
        }
    }
    if (D_800E6280.unk_110A == 2) {
        D_8011F4DE = 2;
        D_8011F4CA = 1;
    }
    D_8013C360 += 1;
}

void func_80139498(void) {
    if (D_8013C35C == 1) {
        if (D_8013C360 == 0) {
            *(s16 *)&D_8011ECD0[0x852] = 5;
            func_801396A4(0, 0xFF);
            D_80122D0C = 0;
            D_8013CAA0 = 0;
        }
    } else if (D_8013C35C == 7) {
        if (D_8013C360 == 0) {
            func_801396A4(1, 0);
            D_80122D0C = 1;
        }
    } else if (D_8013C35C == 8) {
        if (D_8013C360 == 0xC8) {
            func_801396A4(0, 5);
            D_80122D0C = 0;
        }
    } else if (D_8013C35C == 0xB) {
        if (D_8013CAA0 == 0 && (u8)D_8013C364 == 0xA) {
            D_8013CAA0 = 1;
            D_8013C364 = 0;
        }
        if ((u8)D_8013C364 == 0x1E && D_8011F4DE == 0 && D_8013CAA0 == 1) {
            func_801396A4(2, 0);
            *(s16 *)&D_8011ECD0[0x852] = 6;
            D_80122D0C = 1;
        }
    } else if (D_8013C35C == 0xE) {
        if (D_8013C360 == 0) {
            *(s16 *)&D_8011ECD0[0x852] = 7;
            func_801396A4(3, 1);
        } else if (D_8013C360 == 0x8C) {
            func_801396A4(4, 5);
        }
    } else if (D_8013C35C == 0xF) {
        if (D_8013C360 == 0) {
            func_801396A4(3, 1);
        }
    } else if (D_8013C35C == 0x10 && D_8013C360 == 0) {
        func_801396A4(4, 5);
    }
    D_8013C360 += 1;
}

void func_801396A4(s16 arg0, u8 arg1) {
    D_8011F4DE = arg0;
    D_8011F4E0 = 0;
    D_8011F4D0 = 0;
    if (arg1 != 0xFF) {
        D_8011F4CA = arg1;
    }
}
