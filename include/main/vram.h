#ifndef MAIN_VRAM_H
#define MAIN_VRAM_H

#include "common.h"

void vram_clear_framebuffers(void);
void vram_clear_blocks(s32 arg0);
void csr_load_vram(s32 arg0);
void palette_load_vram(s32 arg0);
s32 load_palette();
s32 load_csr_tp();
void LoadSquare(u16 arg0, u16 arg1, u16 arg2, u16 arg3, void *arg4);

#endif
