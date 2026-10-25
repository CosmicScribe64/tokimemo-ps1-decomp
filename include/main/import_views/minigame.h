#ifndef MAIN_MINIGAME_H
#define MAIN_MINIGAME_H

#include "common.h"
#include "libgte.h"

/* T-12040: halfword tested by the RPG battle command dispatcher. */
extern s16 D_80121428;
extern s32 D_800E7384; /* frame counter, GameState.mode_step_timer.w: kept as a separate symbol by keep lines with a proof (config/migrate_globals.txt, T-20820) */
extern u16 D_800E7D3C;
extern u16 D_800E7D3E;
extern u16 D_800E7D40;
extern u8 D_800E7D42;
extern u8 D_800E7D43;
extern u8 D_800E7D44;
extern u8 D_800E7D45;
extern u8 D_800E7D46;
extern u8 D_800E7D47;
extern u8 D_800E7D48;
extern u8 D_800E7D49;
extern u8 D_800E7D4A;
extern u8 D_800E7D4B;
extern s32 D_800E7D4C;
extern u8 D_8011ED1B;
extern u8 D_8011ED57;
extern s16 D_8011FDFA;
extern s16 D_8011FE3E;
extern s16 D_8011FE82;
extern s16 D_8011FEC6;
extern s16 D_8011FF0A;
extern s16 D_8011FF4E;
extern s16 D_8011FF92;
extern s16 D_8011FFD6;
extern s16 D_8012001A;
extern s16 D_8012005E;
extern s16 D_801200A2;
extern s16 D_801200E6;
extern s16 D_8012012A;
extern s16 D_8012016E;
extern s16 D_801201B2;
extern s16 D_801201F6;
extern s16 D_8012023A;
extern s16 D_8012027E;
extern s16 D_801202C2;
extern s16 D_80120306;
extern s16 D_8012034A;
extern s16 D_8012038E;
extern s16 D_801203D2;
extern s16 D_80120416;
extern s16 D_8012045A;
extern s16 D_8012049E;
extern s16 D_801204E2;
extern s16 D_80120526;
extern s16 D_8012056A;
extern s16 D_801205AE;
extern s16 D_801205F2;
extern s16 D_80120636;
extern s16 D_8012077A;
extern s8 D_80120DC1;
extern s8 D_80120E05;
extern u8 D_80120E0A;
extern s16 D_80120E1C;
extern u8 D_80120E4E;
extern s16 D_80120E60;
extern u8 D_80120E93;
extern s16 D_80120EE6;
extern s16 D_80120F2A;
extern s16 D_80120F3A;
extern s16 D_80120F3E;
extern u8 D_801210F2;
extern s16 D_8012114C;
extern u8 D_8012117A;
extern u8 D_801211BE;
extern s16 D_801211E6;
extern u8 D_80121202;
extern s16 D_8012121A;
extern u8 D_80121246;
extern s8 D_801212CB;
extern s8 D_80121311;
extern u8 D_80121316;
#ifndef MAIN_API_OVERRIDE_D_80121317
extern s8 D_80121317;
#endif
extern s8 D_80121353;
extern u8 D_8012135A;
extern s8 D_80121397;
extern s8 D_80121399;
extern u8 D_8012139A;
extern u8 D_8012139E;
extern s8 D_801213DD;
extern s8 D_80121421;
extern u8 D_80121422;
extern u8 D_80121423;
extern u8 D_80121426;
extern s16 D_80121438;
extern u8 D_80121467;
extern s8 D_8012146B;
extern s32 D_8012149C;
extern s8 D_801214A7;
extern u8 D_801214AB;
extern s8 D_801214AF;
extern s32 D_801214E0;
extern s8 D_801214EB;
extern u8 D_801214EF;
extern s8 D_801214F3;
extern s32 D_80121524;
extern s8 D_8012152F;
extern u8 D_80121532;
extern s8 D_80121575;
extern u8 D_801215BA;
extern s8 D_801215FD;
extern s8 D_801215FE;
extern s16 D_80121614;
extern s8 D_80121647;
extern s8 D_80121685;
extern s8 D_8012168B;
extern s32 D_8012749C;
extern s32 D_801274A0;
extern u8 D_80127610[];
extern u8 D_801277F0[];
extern MATRIX D_80129C40; /* bss matrix copied whole by TACO (T-16560) */
extern s8 D_80120E49;

#endif
