#include "common.h"
#include "ovl/ENDING.h"

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80137340", func_80137340);

void func_801374BC(void) {
    if (D_800E6280.unk_1124 != 0) {
        func_80042878(0xC2);
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

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80137340", func_80137574);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80137340", func_8013759C);

void func_801376FC(void) {
    D_80122CE4 = 0;
    D_8013C364 = 0;
    D_8013C360 = 0;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80137340", func_80137730);

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

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80137340", func_801380B0);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80137340", func_80138278);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80137340", func_8013855C);

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

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80137340", func_80138FC8);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80137340", func_801391E4);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80137340", func_80139498);

void func_801396A4(s16 arg0, u8 arg1) {
    D_8011F4DE = arg0;
    D_8011F4E0 = 0;
    D_8011F4D0 = 0;
    if (arg1 != 0xFF) {
        D_8011F4CA = arg1;
    }
}
