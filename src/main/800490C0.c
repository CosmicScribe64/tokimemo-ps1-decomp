#include "common.h"
#include "game.h"

void SetWorkBase(s32 arg0, s32 arg1) {
    D_800E6280.unk_01C[arg1].unk_08 = arg0;
    D_800E6280.unk_01C[arg1].unk_00 = 0x10C00;
    D_800E6280.unk_01C[arg1].unk_04 = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/800490C0", GetWorkBase);

INCLUDE_ASM("asm/nonmatchings/main/800490C0", sprite_set_gpu_poly_ft4);

void goto_tpage(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 work;
    u16 tpage;

    tpage = func_8009ECB0(arg4, arg3, arg0, arg1);
    work = GetWorkBase(0xC, D_8011ECA0);
    func_8009D294(work, 0, 0, tpage, 0);
    AddPrim(D_800E8CA0 + (D_8011ECA0 << 10) + arg2 * 4, work);
    safe_env(arg2);
}

void safe_env(s32 arg0) {
    u8 *p;

    p = (u8 *)GetWorkBase(0xC, D_8011ECA0);
    func_8009F0B8(p);
    *(s16 *)(p + 8) = 0;
    *(s16 *)(p + 0xA) = 0;
    AddPrim(D_800E8CA0 + (D_8011ECA0 << 10) + arg0 * 4, (s32)p);
}

void dtd_on_tpage(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 work;

    work = GetWorkBase(0xC, D_8011ECA0);
    func_8009D294(work, 0, 1, func_8009ECB0(arg4, arg3, arg0, arg1), 0);
    AddPrim(D_800E8CA0 + (D_8011ECA0 << 10) + arg2 * 4, work);
    safe_env(arg2);
}

void dtd_on(s32 arg0) {
    s32 work;

    work = GetWorkBase(0xC, D_8011ECA0);
    func_8009D294(work, 0, 1, 0, 0);
    AddPrim(D_800E8CA0 + (D_8011ECA0 << 10) + arg0 * 4, work);
    safe_env(arg0);
}

INCLUDE_ASM("asm/nonmatchings/main/800490C0", _sprite_set_light_effect1);

INCLUDE_ASM("asm/nonmatchings/main/800490C0", func_80049A40);
