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

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013E290", func_8013E498);

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
