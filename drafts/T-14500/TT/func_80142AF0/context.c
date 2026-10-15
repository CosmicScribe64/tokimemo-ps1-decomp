#include "common.h"
#include "ovl/TT.h"

/* NON_MATCHING: T-11000, register-only (~60 lines, identical instruction stream incl. 4x unroll): original skips v1/a0, i in a1, 0x40/0x70/0x20 in a2/a3/t0; IDO uses v1, a0, a1, a2 (phantom a0, T-0018) */
#ifdef NON_MATCHING
void func_80142570(void) {
    u8 *p;
    s32 i;

    func_80133AD0(0x60C);
    p = D_80158A98;
    i = 0;
    do {
        i++;
        if (*p != 0 && *(u16 *)(p + 4) == 0x40) {
            *(u16 *)(p + 4) = 0x70;
            *(s16 *)(p + 0xA) = 0;
            *(s16 *)(p + 8) = 0;
            *(s16 *)(p + 6) = 0;
            p[0xF] = 0;
            p[0xE] = 0;
            p[0xD] = 0;
        }
        p += 0x78;
    } while (i < 0x40);
    p = D_80158A9C;
    for (i = 0; i < 0x20; i++) {
        if (*p != 0 && *(u16 *)(p + 4) == 0x40) {
            *(u16 *)(p + 4) = 0x70;
            *(s16 *)(p + 0xA) = 0;
            *(s16 *)(p + 8) = 0;
            *(s16 *)(p + 6) = 0;
            p[0xF] = 0;
            p[0xE] = 0;
            p[0xD] = 0;
        }
        p += 0x78;
    }
}
#else
INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142570", func_80142570);
#endif

void func_801426F0(void) {
    s32 i;
    u8 *p = D_80158A8C;
    u8 *r = D_80158AA4 + 0xC80;

    p[0] = 0;
    r[0xC8] = 0;
    r[0x64] = 0;
    r[0] = 0;
    r += 0x12C;
    for (i = 3; i < 0x1F; i++) {
        *r = 0;
        r += 0x64;
    }
}

void func_80142740(void) {
    u8 *p = D_80158A8C;
    s32 i;

    for (i = 0; i < 0x18; i++) {
        p[0x310 + i] = 0;
    }
}

void func_80142774(void) {
    u8 *p = D_80158A8C;
    s32 i;

    for (i = 0; i < 12; i++) {
        *(s32 *)(p + 0x218 + i * 4) = 0;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142570", func_801427A8);

void func_80142AF0(void *arg0, s32 arg1) {
    s16 temp_t1;
    s8 var_a2;
    u8 *temp_a0;
    u8 temp_t0;
    u8 temp_t9;
    u8 *temp_a2;
    u8 *temp_t0_2;

    temp_a2 = D_80158A8C + (arg1 * 0x14) + 0x38;
    if ((*D_80158A8C != 0) && (temp_a0 = &D_80158A8C[arg1], (*(u8 *)((unsigned char *)temp_a0 + (784)) != 0))) {
        temp_t0 = *(u8 *)((unsigned char *)temp_a2 + (8));
        if (temp_t0 & 0x80) {
            *(u8 *)((unsigned char *)temp_a2 + (8)) = (u8) (*(u8 *)((unsigned char *)temp_a2 + (8)) & 0xFF7F);
            *(s8 *)((unsigned char *)temp_a2 + (6)) = (s8) (*(s32 *)arg0);
            *(u8 *)((unsigned char *)temp_a2 + (9)) = (u8) *(s16 *)((unsigned char *)arg0 + (2));
            *(s8 *)((unsigned char *)(&D_80158A8C[arg1]) + (808)) = 0;
            return;
        }
        if (temp_t0 & 0x20) {
            *(u8 *)((unsigned char *)temp_a2 + (8)) = (u8) (*(u8 *)((unsigned char *)temp_a2 + (8)) & 0xFFDF);
            *(s8 *)((unsigned char *)(&D_80158A8C[arg1]) + (808)) = 0;
            return;
        }
        if (*(u8 *)((unsigned char *)temp_a2 + (9)) == 0) {
            var_a2 = *(u8 *)((unsigned char *)temp_a0 + (808)) + 2;
            temp_t0_2 = ((u8 *)arg0) + (var_a2 * 2);
            temp_t1 = (*(s32 *)temp_t0_2);
            if (temp_t1 == -1) {
                temp_t9 = *(u8 *)((unsigned char *)temp_a2 + (8)) | 1;
                *(u8 *)((unsigned char *)temp_a2 + (8)) = temp_t9;
                *(u8 *)((unsigned char *)temp_a2 + (8)) = (u8) (temp_t9 | 0x40);
                *(s8 *)((unsigned char *)temp_a2 + (6)) = (s8) *(s16 *)((unsigned char *)temp_t0_2 + (-4));
            } else if (temp_t1 == -2) {
                var_a2 = 0;
                *(s8 *)((unsigned char *)temp_a2 + (6)) = (s8) (*(s32 *)arg0);
                *(u8 *)((unsigned char *)temp_a2 + (9)) = (u8) *(s16 *)((unsigned char *)arg0 + (2));
            } else if (temp_t1 == -3) {
                *(s8 *)((unsigned char *)(&D_80158A8C[arg1]) + (784)) = 0;
                *(u8 *)((unsigned char *)temp_a2 + (8)) = (u8) (*(u8 *)((unsigned char *)temp_a2 + (8)) | 1);
            } else {
                *(s8 *)((unsigned char *)temp_a2 + (6)) = (s8) temp_t1;
                *(u8 *)((unsigned char *)temp_a2 + (9)) = (u8) *(s16 *)((unsigned char *)temp_t0_2 + (2));
            }
            *(s8 *)((unsigned char *)(&D_80158A8C[arg1]) + (808)) = var_a2;
            return;
        }
        *(u8 *)((unsigned char *)temp_a2 + (9)) = (u8) (*(u8 *)((unsigned char *)temp_a2 + (9)) - 1);
    }
}

void func_80142C70(void) {
    s32 i;
    u8 *base;

    base = D_80158A8C;
    if (base[0] != 0) {
        for (i = 0; i != 0x18; i++) {
            if (base[i + 0x310] != 0) {
                *(s16 *)(base + 0x14) = *(s16 *)(base + 0x22) + *(s16 *)(base + 0x248 + i * 2);
                *(s16 *)(base + 0x16) = *(s16 *)(base + 0x26) + *(s16 *)(base + 0x278 + i * 2);
                func_80147A18(base + 0x14, base + 0x38 + i * 0x14, *(u16 *)(base + 0x2A8 + i * 2));
            }
        }
        *(s16 *)(base + 0x14) = *(s16 *)(base + 0x22);
        *(s16 *)(base + 0x16) = *(s16 *)(base + 0x26);
    }
}
