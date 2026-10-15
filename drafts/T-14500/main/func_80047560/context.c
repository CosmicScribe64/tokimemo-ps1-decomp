#include "common.h"
#include "game.h"

typedef struct TextureRecord44 {
    /* 0x00 */ u8 pad[0x34];
    /* 0x34 */ s32 *pages;
    /* 0x38 */ u8 pad38[0x0C];
} TextureRecord44; /* size 0x44; texture-page table view, T-10030 */

void func_80047550(void) {
    D_801255DC = 4;
}

/* bulk_m2c declarations (T-11190) */
extern s16 D_800E6294;
extern s16 D_800E6296;
extern s16 D_800E6298;
extern s16 D_800E629A;
/* end bulk_m2c declarations */
void func_80047560(void) {
    s16 temp_a2;
    s16 temp_a3;
    s32 temp_lo;
    s32 var_v1;
    u32 var_v0;
    u8 temp_t5;

    if (D_801255DC != 0) {
        var_v0 = 0;
        var_v1 = 0;
        if (D_801255DC != 0) {
            do {
                var_v1 += var_v0;
                var_v0 += 1;
            } while (var_v0 < (u8) D_801255DC);
        }
        temp_lo = (s32) (rsin(var_v1 * 4) / 4) / (s32) (D_801255DC + 0x100);
        temp_t5 = (D_801255DC + 4) & 0xFF;
        D_800E6294 = (s16) temp_lo + 0xA0;
        temp_a2 = (s16) temp_lo + 0xA0;
        D_800E6296 = temp_a2;
        D_800E6298 = (s16) temp_lo + 0x168;
        temp_a3 = (s16) temp_lo + 0x78;
        D_800E629A = temp_a3;
        D_801255E0 = temp_a2;
        D_801255E2 = temp_a3;
        D_801255DC = temp_t5;
        if (temp_t5 >= 0x81U) {
            D_801255DC = 0;
        }
    } else {
        D_801255E0 = 0xA0;
        D_801255E2 = 0x78;
        D_800E6294 = 0xA0;
        D_800E6296 = 0xA0;
        D_800E6298 = 0x168;
        D_800E629A = 0x78;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_800476C0);

void tpage_buf_clear_all(void) {
    s32 i;

    for (i = 0; i < 0x40; i++) {
        if ((i & 0xF) < 5) {
            D_800E6280.unk_1228[i] = 0;
        } else {
            D_800E6280.unk_1228[i] = -1;
        }
    }
}

void tpage_buf_clear(void) {
    s32 i;

    for (i = 0; i < 0x40; i++) {
        if ((i & 0xF) < 5) {
            D_800E6280.unk_1228[i] = 0;
        } else {
            D_800E6280.unk_1228[i] = -1;
        }
    }
    _sys_default_tpage_set();
}

void _sys_default_tpage_set(void) {
    s32 i;

    for (i = 0; i < 5; i++) {
        D_800E6280.unk_1228[i + 11] = 0x77;
        D_800E6280.unk_1228[i + 43] = 0x77;
        D_800E6280.unk_1228[i + 21] = 0x77;
        D_800E6280.unk_1228[i + 53] = 0x77;
    }
    D_800E6280.unk_1228[60] = *(s32 *)D_800C9730;
    D_800E6280.unk_1228[61] = D_800C9734;
    D_800E6280.unk_1228[62] = D_800C9738;
    D_800E6280.unk_1228[63] = D_800C973C;
    D_800E6280.unk_1228[31] = D_800C974C;
    D_800E6280.unk_1228[30] = D_800C9750;
    D_800E6280.unk_1228[29] = D_800C9754;
    D_800E6280.unk_1228[28] = D_800C9758;
    D_800E6280.unk_1228[27] = D_800C5FCC;
    D_800E6280.unk_1228[59] = D_800C5FD0;
}

/* NON_MATCHING: T-12010, 29 differing lines; narrow argument shuffle. */
#ifdef NON_MATCHING
void load_tpage_buf_lock(arg0, arg1, arg2)
s32 arg0;
s32 arg1;
u8 arg2;
{
    u32 idx;
    D_800E6280.unk_1228[arg0] = arg1;
    idx = arg0;
    if (arg0 < 0x20) {
        load_csr_tp(arg1, (u8)(idx & 0x1F), (s32)arg2, 0, 0, 0x80, 0x80);
    } else {
        load_csr_tp(arg1, arg0 & 0x1F, (s32)arg2, 0, 0x80, 0x80, 0x80);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/main/80047550", load_tpage_buf_lock);
#endif

s32 search_load_tpage_buf_lock(s32 arg0, u8 arg1) {
    GameState *var_v0;
    s32 var_v1;

    /* FAKE: shifted cursor keeps the target's base + 0x1228 addressing, T-9580. */
    var_v0 = (GameState *)((u8 *)&D_800E6280 + 0x14);
    for (var_v1 = 5; var_v1 < 0x40; var_v1++) {
        if (var_v0->unk_1228[0] == -1) {
            var_v0->unk_1228[0] = arg0;
            if (var_v1 < 0x20) {
                load_csr_tp(arg0, (u8)var_v1 & 0x1F, (s32)arg1, 0, 0, 0x80, 0x80);
                return var_v1;
            }
            load_csr_tp(arg0, (u8)var_v1 & 0x1F, (s32)arg1, 0, 0x80, 0x80, 0x80);
            return var_v1;
        }
        var_v0 = (GameState *)((u8 *)var_v0 + 4);
    }
    return -1;
}

s32 search_tpage_multi(s32 arg0, u8 arg1) {
    s32 j;
    s32 i;

    for (i = 0xF; i >= 5; i--) {
        for (j = 3; j >= 0; j--) {
            if (D_800E6280.unk_1128[i + j * 16] == 0x7FFE && D_800E6280.unk_1228[i + j * 16] == arg0) {
                return (j * 16 + i) & 0xFFFF;
            }
        }
    }
    for (i = 0; i < 0x40; i++) {
        if (arg0 == D_800E6280.unk_1128[i]) {
            return i & 0xFFFF;
        }
    }
    for (i = 0; i < 0x40; i++) {
        if (D_800E6280.unk_1128[i] == 0x7FFF) {
            D_800E6280.unk_1128[i] = arg0;
            if (i < 0x20) {
                load_csr_tp(arg0, i & 0xFF, arg1, 0, 0, 0x80, 0x80);
            } else {
                load_csr_tp(arg0, i & 0xFF, arg1, 0, 0x80, 0x80, 0x80);
            }
            return i & 0xFFFF;
        }
        if (i == 0x3F) {
            return 0x3F;
        }
    }
}

s32 search_tpage(s16 arg0, s32 arg1, u8 arg2) {
    s32 j;
    s32 i;
    TextureRecord44 *record;
    s16 index;

    if (arg0 == 0x7FFF) {
        for (i = 0; i < 64; i++) {
            if (D_800E6280.unk_1228[i] == -1) {
                D_800E6280.unk_1128[i] = 0x7FFF;
            } else {
                D_800E6280.unk_1128[i] = 0x7FFE;
            }
        }
        return 0;
    }
    record = &((TextureRecord44 *)D_8011ECD0)[arg1];
    index = (s16)(arg0 >> 6);
    for (i = 0xF; i >= 5; i--) {
        for (j = 3; j >= 0; j--) {
            if (D_800E6280.unk_1128[i + j * 16] == 0x7FFE &&
                D_800E6280.unk_1228[i + j * 16] == record->pages[index]) {
                return (j * 16 + i) & 0xFFFF;
            }
        }
    }
    for (i = 0; i < 0x40; i++) {
        if (D_800E6280.unk_1128[i] == record->pages[index]) {
            return i & 0xFFFF;
        }
    }
    for (i = 0; i < 0x40; i++) {
        if (D_800E6280.unk_1128[i] == 0x7FFF) {
            D_800E6280.unk_1128[i] = record->pages[index];
            if (i < 0x20) {
                load_csr_tp(record->pages[index], i & 0xFF, arg2, 0, 0, 0x80, 0x80);
            } else {
                load_csr_tp(record->pages[index], i & 0xFF, arg2, 0, 0x80, 0x80, 0x80);
            }
            return i & 0xFFFF;
        }
        if (i == 0x3F) {
            return 0x3F;
        }
    }
}

s32 search_tpage8(s16 arg0, s32 arg1, u8 arg2) {
    s32 j;
    s32 i;
    TextureRecord44 *record;
    s16 index;

    if (arg0 == 0x7FFF) {
        for (i = 0; i < 64; i++) {
            if (D_800E6280.unk_1228[i] == -1) {
                D_800E6280.unk_1128[i] = 0x7FFF;
            } else {
                D_800E6280.unk_1128[i] = 0x7FFE;
            }
        }
        return 0;
    }
    record = &((TextureRecord44 *)D_8011ECD0)[arg1];
    index = (s16)(arg0 >> 8);
    for (i = 0xF; i >= 5; i--) {
        for (j = 3; j >= 0; j--) {
            if (D_800E6280.unk_1128[i + j * 16] == 0x7FFE &&
                D_800E6280.unk_1228[i + j * 16] == record->pages[index]) {
                return (j * 16 + i) & 0xFFFF;
            }
        }
    }
    for (i = 0; i < 0x40; i++) {
        if (D_800E6280.unk_1128[i] == record->pages[index]) {
            return i & 0xFFFF;
        }
    }
    for (i = 0; i < 0x40; i++) {
        if (D_800E6280.unk_1128[i] == 0x7FFF) {
            D_800E6280.unk_1128[i] = record->pages[index];
            if (i < 0x20) {
                load_csr_tp(record->pages[index], i & 0xFF, arg2, 0, 0, 0x80, 0x80);
            } else {
                load_csr_tp(record->pages[index], i & 0xFF, arg2, 0, 0x80, 0x80, 0x80);
            }
            return i & 0xFFFF;
        }
        if (i == 0x3F) {
            return 0x3F;
        }
    }
}

void func_80048CF8(void) {
    s32 i;

    if (D_800E6280.unk_F80 & 2) {
        printf("PRINT_TPAGE_BUF\n");
        for (i = 0; i < 0x40; i++) {
            printf("[%d]%08x", i, D_800E6280.unk_1228[i]);
            if ((i & 3) == 3) {
                printf("\r");
            }
        }
    }
}

void func_80048DAC(s32 arg0) {
    if (arg0 != 0) {
        D_800E6280.unk_1112 = 1;
    } else {
        D_800E6280.unk_1112 = 0;
    }
}

void func_80048DD0(s32 count) {
    s32 i;

    printf("%d CLASS_CLEAR \n", count);
    if (count != 0) {
        for (i = 0; i < count && i < 160; i++) {
            D_8011ECD0[i * 0x44 + 3] = 0;
        }
    } else {
        for (i = 0; i < 160; i++) {
            D_8011ECD0[i * 0x44 + 3] = 0;
        }
    }
}

void func_80048E78(void) {
    s32 i;

    for (i = 0x60; i < 0xA0; i++) {
        func_80048F64(i);
    }
}

void func_80048EB8(s32 arg0) {
    s32 n;
    s32 i;
    s32 a;
    s32 b;
    s32 c;

    a = D_8011ECF6;
    b = D_8011ECFA;
    c = D_8011ECE8;
    if (arg0 == 0) {
        n = 0xA0;
    } else {
        n = (arg0 >= 0xA1) ? 0xA0 : arg0;
    }
    for (i = 1; i < n; i++) {
        func_80048F64(i);
    }
    D_8011ECF6 = a;
    D_8011ECFA = b;
    D_8011ECE8 = c;
}

void func_80048F64(s32 arg0) {
    u8 *p;

    if (arg0 < 0xA0) {
        p = D_8011ECD0 + arg0 * 0x44;
        p[0] = 0;
        *(s32 *)(p + 0x38) = 0;
        p[1] = 0;
        p[4] = 0;
        p[5] = 0;
        p[2] = 0;
        p[3] = 0;
        p[6] = 1;
        *(s16 *)(p + 8) = 0;
        *(s32 *)(p + 0xC) = (s32)sprite_frame_table;
        *(s32 *)(p + 0x10) = (s32)D_800C9A14;
        *(s32 *)(p + 0x34) = (s32)D_800C9730;
        *(s16 *)(p + 0x14) = 0;
        *(s16 *)(p + 0x16) = 0;
        *(s16 *)(p + 0x18) = 0;
        *(s16 *)(p + 0x1A) = 0x1000;
        *(s16 *)(p + 0x1C) = 0x1000;
        p[0x43] = 0;
        p[7] = 0x80;
        *(s32 *)(p + 0x20) = 0;
        *(s32 *)(p + 0x24) = 0;
        *(s32 *)(p + 0x28) = 0;
        *(s32 *)(p + 0x2C) = 0;
        *(s32 *)(p + 0x30) = 0;
        p[0x40] = 0;
        p[0x41] = 0;
        p[0x42] = 0;
    }
}

u8 func_8004901C(void) {
    return D_800E6280.unk_03A;
}

void func_8004902C(void) {
    D_800E6280.unk_03A = 0x80;
    D_800E6280.unk_03B = 0;
}

void func_80049044(void) {
    u8 *p;
    s32 v;

    p = &D_800E6280.unk_03A;
    if ((s8)D_800E6280.unk_03B != 0) {
        if ((s8)D_800E6280.unk_03B > 0) {
            v = *p;
            v += (s8)D_800E6280.unk_03B;
            if (v >= 0x100) {
                v = 0xFF;
            }
            *p = v;
        } else if ((s8)D_800E6280.unk_03B < 0) {
            v = *p;
            v += (s8)D_800E6280.unk_03B;
            if (v < 0) {
                v = 0;
            }
            *p = v;
        }
    }
}

void func_800490B4(u8 arg0) {
    D_800E6280.unk_03B = arg0;
}
