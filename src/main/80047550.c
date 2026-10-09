#include "common.h"
#include "game.h"

void func_80047550(void) {
    D_801255DC = 4;
}

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_80047560);

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_800476C0);

INCLUDE_ASM("asm/nonmatchings/main/80047550", tpage_buf_clear_all);

INCLUDE_ASM("asm/nonmatchings/main/80047550", tpage_buf_clear);

INCLUDE_ASM("asm/nonmatchings/main/80047550", _sys_default_tpage_set);

INCLUDE_ASM("asm/nonmatchings/main/80047550", load_tpage_buf_lock);

INCLUDE_ASM("asm/nonmatchings/main/80047550", search_load_tpage_buf_lock);

INCLUDE_ASM("asm/nonmatchings/main/80047550", search_tpage_multi);

INCLUDE_ASM("asm/nonmatchings/main/80047550", search_tpage);

INCLUDE_ASM("asm/nonmatchings/main/80047550", search_tpage8);

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_80048CF8);

void func_80048DAC(s32 arg0) {
    if (arg0 != 0) {
        D_800E7392 = 1;
    } else {
        D_800E7392 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_80048DD0);

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_80048E78);

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_80048EB8);

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_80048F64);

u8 func_8004901C(void) {
    return D_800E62BA;
}

void func_8004902C(void) {
    D_800E62BA = 0x80;
    D_800E62BB = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_80049044);

void func_800490B4(u8 arg0) {
    D_800E62BB = arg0;
}

void SetWorkBase(s32 arg0, s32 arg1) {
    u8 *p = D_800E6280 + arg1 * 12;

    *(s32 *)(p + 0x24) = arg0;
    *(s32 *)(p + 0x1C) = 0x10C00;
    *(s32 *)(p + 0x20) = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/80047550", GetWorkBase);

INCLUDE_ASM("asm/nonmatchings/main/80047550", sprite_set_gpu_poly_ft4);

INCLUDE_ASM("asm/nonmatchings/main/80047550", goto_tpage);

INCLUDE_ASM("asm/nonmatchings/main/80047550", safe_env);

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

INCLUDE_ASM("asm/nonmatchings/main/80047550", _sprite_set_light_effect1);

INCLUDE_ASM("asm/nonmatchings/main/80047550", func_80049A40);
