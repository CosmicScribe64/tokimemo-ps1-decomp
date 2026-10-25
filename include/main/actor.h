#ifndef MAIN_ACTOR_H
#define MAIN_ACTOR_H

#include "common.h"

#ifndef MAIN_API_OVERRIDE_actor_pool
extern u8 actor_pool[]; /* 0x44-byte records; DATE uses entries 96..99 (T-10730). */
#endif
#ifndef MAIN_API_OVERRIDE_actor_pool_96
extern u8 actor_pool_96[];
#endif
void actor_draw_enable(s32 arg0);
void actor_clear_upper(void);
void actor_clear_range(s32 arg0);
void actor_clear(s32 arg0);

void actor_deactivate(s32 arg0);

extern u8 D_80120651;
extern u8 D_80120653;
extern u8 D_80120655;
extern s16 D_80120676;
extern s16 D_8012067A;
extern s32 D_80120688;
extern u8 D_80120695;
extern u8 D_80120697;
extern u8 D_80120699;
extern s16 D_801206BA;
extern s16 D_801206BE;
extern u8 D_801206D9;
extern u8 D_801206DB;
extern u8 D_801206DD;
extern s16 D_801206FE;
extern s16 D_80120702;
extern u8 D_8012071F;
extern s16 D_80120742;
extern s16 D_80120746;
extern u8 D_80120761;
extern u8 D_80120763;
extern u8 D_80120765;
extern s16 D_80120786;
extern s16 D_8012078A;
extern u8 D_801207A5;
extern u8 D_801207A7;
extern u8 D_801207A9;
extern s16 D_801207CA;
extern s16 D_801207CE;
extern s32 D_801207DC;
extern u8 D_801207E9;
extern u8 D_801207EB;
extern u8 D_801207ED;
extern s16 D_8012080E;
extern s16 D_80120812;
extern s32 D_80120820;
extern u8 D_8012082D;
extern u8 D_8012082F;
extern u8 D_80120831;
extern s16 D_80120852;
extern s16 D_80120856;
extern s32 D_80120864;
extern s16 D_80120896;
extern s16 D_8012089A;
extern s16 D_801208DE;
extern u8 D_801208FB;
extern s16 D_8012091E;
extern s16 D_80120922;
extern u8 D_8012093F;
extern s16 D_80120966;
extern s16 D_801209AA;
extern s16 D_801209EE;
extern s16 D_80120A32;
extern s16 D_80120A72;
extern s16 D_80120A76;
extern s16 D_80120AB6;
extern s16 D_80120ABA;
extern s16 D_80120AFA;
extern s16 D_80120AFE;
extern s16 D_80120B3E;
extern s16 D_80120B42;
extern u8 D_80120B5F;
extern s16 D_80120B82;
extern s16 D_80120B86;
extern u8 D_80120BA3;
extern s16 D_80120BC6;
extern s16 D_80120BCA;
extern u8 D_80120BE7;
extern s16 D_80120C0A;
extern s16 D_80120C0E;
extern u8 D_80120C2B;
extern s16 D_80120C4E;
extern s16 D_80120C52;
extern u8 D_80120C6F;
extern s16 D_80120C92;
extern s16 D_80120C96;
extern s16 D_80120CD6;
extern s16 D_80120CDA;
extern s16 D_80120D1A;
extern s16 D_80120D1E;
extern s16 D_80120D5E;
extern s16 D_80120D62;
extern s16 D_80120DA2;
extern s16 D_80120DA6;
extern u8 D_80120E07;
extern s16 D_80120EF6;
extern s16 D_80120EFA;

#endif
