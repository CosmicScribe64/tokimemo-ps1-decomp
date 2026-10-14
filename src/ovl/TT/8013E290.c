#include "common.h"
#include "ovl/TT.h"

void func_8013E290(void) {
    s32 *p;
    s32 ev;

    p = (s32 *)D_80158A68;
    EnterCriticalSection();
    ev = OpenEvent(0xF2000003, 2, 0x1000, func_8013EC1C);
    *p = ev;
    EnableEvent(ev);
    SetRCnt(0xF2000003, 1, 0x1000);
    StartRCnt(0xF2000003);
    ExitCriticalSection();
    func_800AE0B0("v_ev=%d\n", *p);
}

void func_8013E320(void) {
    func_800AE080(D_80158A64, 0x38);
    *(s32 *)(D_80158A68 + 8) = 0;
    func_800AE080(D_80158AA8, 0xA00);
    func_800AE080(D_80158A6C, 0x20);
    func_800AE080((u8 *)D_80158A70, 0x20);
    func_800AE080(D_80158A74, 0x74);
    func_800AE080(D_80158A78, 0x1EC);
    func_800AE080(D_80158A7C, 0x300);
    func_800AE080(D_80158A80, 0x3F0);
    func_800AE080(D_80158A84, 0x2C0);
    func_800AE080(D_80158A88, 0x2C0);
    func_800AE080(D_80158A8C, 0x424);
    func_800AE080(D_80158A90, 0xD80);
    func_800AE080(D_80158A94, 0x270);
    func_800AE080(D_80158A98, 0x1E00);
    func_800AE080(D_80158A9C, 0xF00);
    func_800AE080(D_80158AA0, 0x3400);
    func_800AE080(D_80158AA4, 0x3200);
}

void func_8013E454(void) {
    func_800AE080(D_80158A60, 0xA8);
    func_800AE080(D_80158AB4, 0x10170);
    func_8013E320();
}

/* NON_MATCHING: T-11540 (order_search: operand order 4 * x of the p[84] shift gave 101 -> 99 diff rows; store order of p[82..65] gives no gain), T-10710, 130 of 130 instructions with locals p/r/q for D_80158A60/D_80158A64/&D_800E7D10; differences are the ugen temp numbering (original skips t8 after the sll) and the constant 1 in t0 (uopt colour) instead of a ugen temp */
#ifdef NON_MATCHING
void func_8013E498(void) {
    u8 *p;
    u8 *r;
    u8 *q;

    p = D_80158A60;
    r = D_80158A64;
    q = (u8 *)&D_800E7D10;
    p[85] = 0;
    if (D_800E7D4C & 0x02000000) {
        p[83] = 0;
    } else {
        p[83] = 1;
    }
    p[82] = (u32)(*(s32 *)(q + 0x3C) * 8) >> 0x1D;
    p[84] = (u32)(4 * *(s32 *)(q + 0x3C)) >> 0x1F;
    p[53] = 0xFF;
    p[59] = 0xFF;
    p[65] = 0xFF;
    if (D_800E7D3C == 0) {
        *(u16 *)(p + 48) = 0x63C8;
        p[50] = 0x14;
        p[51] = 0x24;
        p[52] = 0x18;
        *(u16 *)(p + 54) = 0x71D8;
        p[56] = 0x17;
        p[57] = 0x24;
        p[58] = 0xA;
        *(u16 *)(p + 60) = 0x7FE8;
        p[62] = 0x16;
        p[63] = 0x24;
        p[64] = 0x12;
        p[68] = 7;
        p[69] = 6;
    } else {
        *(u16 *)(p + 48) = D_800E7D3C;
        p[50] = D_800E7D42;
        p[51] = D_800E7D43;
        p[52] = D_800E7D44;
        *(u16 *)(p + 54) = D_800E7D3E;
        p[56] = D_800E7D45;
        p[57] = D_800E7D46;
        p[58] = D_800E7D47;
        *(u16 *)(p + 60) = D_800E7D40;
        p[62] = D_800E7D48;
        p[63] = D_800E7D49;
        p[64] = D_800E7D4A;
        p[68] = D_800E7D4B;
        p[69] = *(s32 *)(q + 0x3C);
        p[67] = q[0x3D] & 1;
        if (p[83] == 0 && p[82] != 0) {
            p[73] = 0;
        } else {
            p[73] = (q[0x3D] >> 1) & 1;
        }
    }
    if (p[83] == 0 && p[82] != 0) {
        r[34] = 1;
    } else {
        r[34] = 0;
    }
    *(u8 **)(p + 76) = D_80150A50;
}
#else
INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013E290", func_8013E498);
#endif

void func_8013E6A0(void) {
    u8 *state;
    TtFrame80B8 *p;
    s32 x;
    s32 offset;

    p = (TtFrame80B8 *)D_80158AB4;
    state = D_80158A60;
    func_8009C5E0(0);
    func_8009F57C((s32)&p[0].draw_clip, 0, 0, 320, 240);
    func_8009F57C((s32)&p[1].draw_clip, 320, 0, 320, 240);
    if (state[73] != 0) {
        func_8009F638(&p[0].disp_rect, 352, 0, 256, 240);
        func_8009F638(&p[1].disp_rect, 32, 0, 256, 240);
    } else {
        func_8009F638(&p[0].disp_rect, 320, 0, 320, 240);
        func_8009F638(&p[1].disp_rect, 0, 0, 320, 240);
    }
    offset = 160;
    x = 32;
    do {
        p->draw_ofs[0] = offset;
        p->draw_clip.x = x;
        p->draw_clip.y = 8;
        p->draw_clip.w = 256;
        p->draw_clip.h = 232;
        p->unk_8060 = 0;
        p->unk_805E = 0;
        p->unk_805F = 1;
        p->unk_80B4 = 0;
        p->unk_80B5 = 0;
        x += 320;
        p->next = (u8 *)p + 8;
        p++;
        offset += 320;
    } while (x != 0x2A0);
}

void func_8013E824(void) {
    func_8004111C();
}
