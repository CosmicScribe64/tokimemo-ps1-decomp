#ifndef SDK_LIBSND_H
#define SDK_LIBSND_H

#include "common.h"

extern s32 D_80094710;
extern u16 D_80094714;
extern u16 D_80094718;
extern u8 *D_8009471C;
extern u8 *D_80094720;
extern s32 D_80094724;
extern s32 D_80094728;
extern s32 D_8009472C;
extern s16 D_80094730;
extern s16 D_80094734;
extern s32 D_8009473C;
extern s32 D_80094740;
extern s32 D_80094744;
extern u8 D_80094748[];
extern u8 D_80094764[];
extern u8 D_8009476C[];
extern u8 D_80094784[];
extern u8 D_800947C4[];
extern u8 D_80094818[];
extern u8 D_8009483C[];
extern s16 D_80094868;
void func_8008B8C4();
void func_8008B904();
void func_8008BAA4();
void func_8008BE34();
void func_8008BE54();
void func_8008BEBC();
void func_8008BEE0();
#ifndef MAIN_API_OVERRIDE_func_8008BFB0
void func_8008BFB0();
#endif
s32 func_8008C1B0();
s32 func_8008C620();
#ifndef MAIN_API_OVERRIDE_SsVabTransCompleted
void SsVabTransCompleted();
#endif
#ifndef MAIN_API_OVERRIDE_func_8008D610
void func_8008D610();
#endif
void func_8008E280();
void func_8008E2E0();
void func_8008E310();
void func_8008E408();
void func_8008E6AC();
void func_8008E8B4();
void func_8008F54C();
#ifndef MAIN_API_OVERRIDE_func_8008F618
void func_8008F618();
#endif
void func_8008FB00();
void func_8008FC10();
void func_8008FD1C();
void func_8008FD68();
void func_8008FEB0();
void func_8008FED0();
void func_8008FEF0();
void func_8008FF5C();
#ifndef MAIN_API_OVERRIDE_func_8008FF60
void func_8008FF60();
#endif
void func_80090580(s16);
void func_8009068C();
void func_800907E0(s16);
void func_80090960();
void func_80090D20(void);
void func_80090D60();
void func_80090DB0();
void func_80090DF0();
void func_80090E30();
void func_80090E60(s16);

#endif
